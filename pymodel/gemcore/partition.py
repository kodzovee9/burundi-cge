"""Automatic square-partition of the GEM-Core system via bipartite matching
on the Jacobian sparsity pattern at the calibrated point.

Idea (equivalent to how a GAMS MCP is square by construction):
  1. Fix the economically-meaningful closure variables (numeraire, gov/si/row
     closure choices, capital & debt at tmin, scaling switches).
  2. Among the remaining variable entries and ALL equations, find a maximum
     bipartite matching where an edge (eq, var) exists iff d(eq)/d(var) != 0
     at the calibrated point.
  3. Matched variables are the FREE unknowns; unmatched variables are
     structurally exogenous (their base value, typically 0, is fixed);
     unmatched equations are redundant (Walras' law) and dropped.

The result is a provably square, generically non-singular subsystem, cached
so the perturbation sweep runs only once per (closure, period-structure).
"""

from __future__ import annotations

import numpy as np
from scipy.sparse import csr_matrix
from scipy.sparse.csgraph import maximum_bipartite_matching

from .model import residuals
from .closure import (declared_pairs, PREDETERMINED_EQS,
                      CLOSURE_PINNED_EQS)


def _entry_list(V):
    return [(n, i) for n, d in V.items() for i in d]


def jacobian_sparsity(cal, t, V, L, clo, cand_vars, eqkeys, rel=1e-6,
                      absp=1e-8, thresh=1e-9):
    """Return dict eq_index -> set(var_index) of nonzero Jacobian entries,
    probing only candidate variables `cand_vars` (list of (name, idx))."""
    cal._solve_t = t
    base = residuals(V, cal, t, L, clo)
    b = np.array([base[k] for k in eqkeys], dtype=float)
    eq_of = {k: r for r, k in enumerate(eqkeys)}
    cols = {}   # var col -> list of eq rows touched
    for cvar in cand_vars:
        n, i = cvar
        x0 = V[n][i]
        h = rel * abs(x0) + absp
        V[n][i] = x0 + h
        pert = residuals(V, cal, t, L, clo)
        V[n][i] = x0
        p = np.array([pert[k] for k in eqkeys], dtype=float)
        d = np.abs(p - b)
        rows = np.nonzero(d > thresh * (1 + np.abs(b)))[0]
        cols[cvar] = rows
    return cols, b


def build_partition(cal, t, V, L, clo, closure_fixed, diag=None):
    """Return (free, fixed, kept_eqs, all_eqkeys).

    The declared pairing in `closure.py` decides which variable each equation
    determines; a maximum bipartite matching on the numeric Jacobian fills in
    the rows the declaration deliberately leaves open and guarantees the result
    is square and structurally non-singular. Pass a dict as `diag` to receive
    counts of what the declaration covered and what the matcher had to invent.
    """
    import casadi as ca
    cal._solve_t = t
    if diag is None:
        diag = {}
    all_entries = _entry_list(V)
    entry_set = set(all_entries)
    closure_fixed = {e for e in closure_fixed if e in entry_set}

    def match_on(cand_vars):
        """Return (free, kept_eqs, eqkeys) from a matching on the Jacobian
        sparsity with ONLY `cand_vars` symbolic and everything else the
        float value it has in V (so fixed-to-zero cancellations are seen)."""
        n_var = len(cand_vars)
        x = ca.SX.sym("x", n_var)
        Vsym = {nm: dict(d) for nm, d in V.items()}
        for j, (nm, idx) in enumerate(cand_vars):
            Vsym[nm][idx] = x[j]
        R = residuals(Vsym, cal, t, L, clo)
        eqkeys = [k for k in R if R[k] is not None]
        g = ca.vertcat(*[R[k] for k in eqkeys])
        rows_sp, cols_sp = ca.jacobian(g, x).sparsity().get_triplet()
        graph = csr_matrix(([1] * len(rows_sp),
                            (list(rows_sp), list(cols_sp))),
                           shape=(len(eqkeys), n_var))
        col_match = maximum_bipartite_matching(graph, perm_type="row")
        matched_cols = [j for j in range(n_var) if col_match[j] >= 0]
        matched_rows = {int(col_match[j]) for j in matched_cols}
        free = [cand_vars[j] for j in matched_cols]
        kept = [eqkeys[r] for r in sorted(matched_rows)]
        return free, kept, eqkeys

    def match_numeric(cand_vars):
        """Like match_on but builds the bipartite graph from the NUMERIC
        Jacobian at the current point V, so edges that are structurally
        present but numerically zero (e.g. an export-price column for a good
        with zero exports) are excluded. This removes the exact-null columns/
        rows that make the direct linear solve singular."""
        import numpy as np
        n_var = len(cand_vars)
        x = ca.SX.sym("x", n_var)
        Vsym = {nm: dict(d) for nm, d in V.items()}
        for j, (nm, idx) in enumerate(cand_vars):
            Vsym[nm][idx] = x[j]
        R = residuals(Vsym, cal, t, L, clo)
        eqkeys = [k for k in R if R[k] is not None]
        g = ca.vertcat(*[R[k] for k in eqkeys])
        Jf = ca.Function("J", [x], [ca.jacobian(g, x)])
        xk = np.array([V[nm][idx] for (nm, idx) in cand_vars], dtype=float)
        J = Jf(xk)
        rows_sp, cols_sp = J.sparsity().get_triplet()
        Jv = np.array(J.nonzeros())
        scale = max(abs(Jv).max(), 1.0) if len(Jv) else 1.0
        keep = abs(Jv) > 1e-11 * scale
        ri = [rows_sp[i] for i in range(len(Jv)) if keep[i]]
        ci = [cols_sp[i] for i in range(len(Jv)) if keep[i]]
        n_eq = len(eqkeys)

        # A maximum matching is not unique, and the unmatched columns are the
        # ones we end up fixing at their (merely guessed) start value, so WHICH
        # maximum matching we pick is an economic choice, not a detail.
        #
        # Two priorities decide it. First, a column whose Jacobian entries are
        # all numerically zero carries no information and is the only kind that
        # is harmless to fix, so weak columns are offered last. Second, among
        # strong columns, the fewer equations a column appears in the less
        # freedom the matcher has to place it later: a degree-one column such
        # as WFAVG (which occurs only in its own WFAVGDEF) or NFFINSSCAL (only
        # in NGOVNETFORFIN) MUST take that row, because fixing it would turn
        # its definition into a spurious extra restriction on the rest of the
        # block. Kuhn's algorithm never un-matches a column once matched, so
        # inserting columns in ascending-degree order guarantees those forced
        # pairs survive while the result stays a maximum matching.
        colnorm = np.zeros(n_var)
        degree = np.zeros(n_var, dtype=int)
        adj = [[] for _ in range(n_var)]
        Jk = Jv[keep]
        for i in range(len(ri)):
            colnorm[ci[i]] = max(colnorm[ci[i]], abs(Jk[i]))
            degree[ci[i]] += 1
            adj[ci[i]].append(ri[i])
        strong = [j for j in range(n_var) if colnorm[j] > 1e-9 * scale]
        weak = [j for j in range(n_var) if colnorm[j] <= 1e-9 * scale]
        order = (sorted(strong, key=lambda j: (degree[j], j))
                 + sorted(weak, key=lambda j: (degree[j], j)))

        row_col = [-1] * n_eq          # row -> column matched to it
        col_row = [-1] * n_var

        # Seed with the DECLARED pairing from closure.py. Which variable an
        # equation determines is a modelling statement, so it is written down
        # rather than discovered; the matcher only fills the remainder. Seeding
        # is safe: Kuhn's algorithm never leaves a matched column unmatched, and
        # a seeded column can only be moved to another row by an augmenting
        # path, which keeps the row count the same.
        colof = {e: j for j, e in enumerate(cand_vars)}
        declared, _unpaired, _rep = declared_pairs(
            eqkeys, V, set(closure_fixed), t)
        # A declared pair is only usable if the variable actually appears in
        # that equation with a non-zero derivative. Seeding a non-edge would
        # put a structural zero on the diagonal and make the solve singular, so
        # check adjacency and let anything else fall through to the matcher.
        edges = set(zip(ri, ci))
        seeded = 0
        for r, k in enumerate(eqkeys):
            e = declared.get(k)
            if e is None:
                continue
            j = colof.get(e)
            if j is None or col_row[j] != -1 or colnorm[j] <= 1e-9 * scale:
                continue
            if (r, j) not in edges:
                _rep["not_an_edge"] = _rep.get("not_an_edge", 0) + 1
                diag.setdefault("not_an_edge_list", []).append((k, e))
                continue
            row_col[r] = j
            col_row[j] = r
            seeded += 1

        # The forced-pair argument runs the other way too. A row with a single
        # strong column has to take it -- EQ_REXRDEF (REXR = EXR/DPI) has both
        # REXR and DPI pinned by the closure, so EXR is its only unknown -- but
        # such a row is invisible to a column-driven search: no other column
        # can ever reach it, so if its column gets matched elsewhere first the
        # row is simply dropped. Claim those pairs up front. Nothing later
        # unmatches them, because an augmenting path could only displace the
        # column through that same row, and no other column touches it.
        rowcols = [[] for _ in range(n_eq)]
        for i in range(len(ri)):
            if colnorm[ci[i]] > 1e-9 * scale:
                rowcols[ri[i]].append(ci[i])
        for r in range(n_eq):
            if row_col[r] != -1 or len(rowcols[r]) != 1:
                continue
            j = rowcols[r][0]
            if col_row[j] == -1:
                row_col[r] = j
                col_row[j] = r
        diag["seeded"] = seeded
        diag["declared"] = len(declared)

        def augment(j0):
            """Kuhn's augmenting search for column j0. Iterative, because the
            alternating paths through a CGE Jacobian run deeper than Python's
            recursion limit. Each stack frame is [column, next adjacency index,
            row this column is currently descending through]."""
            seen = bytearray(n_eq)
            stack = [[j0, 0, -1]]
            while stack:
                frame = stack[-1]
                j, k = frame[0], frame[1]
                if k >= len(adj[j]):
                    stack.pop()
                    continue
                frame[1] = k + 1
                r = adj[j][k]
                if seen[r]:
                    continue
                seen[r] = 1
                frame[2] = r
                if row_col[r] == -1:
                    for cj, _, cr in reversed(stack):
                        row_col[cr] = cj
                        col_row[cj] = cr
                    return True
                stack.append([row_col[r], 0, -1])
            return False

        for j in order:
            if col_row[j] == -1:
                augment(j)

        matched_cols = [j for j in range(n_var) if col_row[j] >= 0]
        matched_rows = {col_row[j] for j in matched_cols}

        # Record where the declaration and the matcher disagreed. A row the
        # matcher had to pair against something other than its declared
        # variable, or could not pair at all, is where a closure statement is
        # still missing -- that is the list to read when an out-year fails.
        improvised, orphans = [], []
        for r, k in enumerate(eqkeys):
            j = row_col[r]
            if j < 0:
                if (k[0] not in PREDETERMINED_EQS
                        and k[0] not in CLOSURE_PINNED_EQS):
                    orphans.append(k)
                continue
            want = declared.get(k)
            if want is not None and cand_vars[j] != want:
                improvised.append((k, want, cand_vars[j]))
        diag["improvised"] = improvised
        diag["orphan_eqs"] = orphans
        diag["reject_reasons"] = _rep
        diag["strong_unpaired"] = [cand_vars[j] for j in range(n_var)
                                   if col_row[j] < 0
                                   and colnorm[j] > 1e-9 * scale]
        return ([cand_vars[j] for j in sorted(matched_cols)],
                [eqkeys[r] for r in sorted(matched_rows)], eqkeys)

    # Strength-aware numeric matching from the start: strong columns are
    # matched with priority (an unmatched strong column would be fixed at a
    # guess value and poison the system); only numerically-null columns may
    # end up fixed. Iterate to a fixed point.
    cand = [e for e in all_entries if e not in closure_fixed]
    free, kept_eqs, eqkeys = match_numeric(cand)
    for _ in range(6):
        if len(free) == len(cand):
            break
        cand = free
        free, kept_eqs, eqkeys = match_numeric(cand)

    freeset = set(free)
    fixed = closure_fixed | {e for e in all_entries
                             if e not in freeset and e not in closure_fixed}
    return free, fixed, kept_eqs, eqkeys
