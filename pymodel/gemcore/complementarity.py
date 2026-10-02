"""Solve a path while enforcing the model's two inequality closures.

GEM-Core is an MCP; this port solves equalities. Two of the model's
complementarities cannot be left to the equality solver, because it will
happily return a "converged" solution on the wrong branch:

  * **Import quotas** (`EQ_QMCONST`, mod.gms 2608): either the quota binds
    with a positive rent, or it is slack -- imports below the ceiling, rent
    zero. Solving the binding branch as an equality can return a NEGATIVE
    rent, which is the model saying the quota should be slack. It did exactly
    that for chemicals in the 2019-24 backcast (rent −0.085 in a solve that
    reported convergence).
  * **The idle-resource closure** for a natural resource: either demand at
    the reservation rent is below the endowment and the surplus is idle
    (`UERAT > 0`, rent fixed), or the endowment binds and the rent rises to
    ration it (`UERAT = 0`, rent free). Solving the idle branch alone can
    return `UERAT < 0` -- over-employment -- which means nothing.

`sweep_solve` runs the path, inspects the solution, moves any cell found on
the wrong branch, and re-solves, until every cell is consistent. The branch
sets live on `cal` (`quota_slack`, `idle_cells`) because that is where the
closure (`solver.base_closure_fixed`) and the state builder read them.
"""

from __future__ import annotations

from .dynamics import _solve_path


def sweep_solve(cal, clo, periods, verbose=False, warm=None, idle_factors=(),
                max_sweeps=6, tol=1e-9, log=print):
    """Solve `periods` with `_solve_path`, sweeping the two complementarities.

    `cal.quota_slack` is taken as the starting set of slack quota cells (a
    scenario may declare some); `cal.idle_cells` is reset so every
    (factor, period) for `idle_factors` starts on the idle branch. Returns
    the solution dict; the final branch sets are left on `cal` and a summary
    is available through `sweep_report(cal, periods)`.
    """
    cal.quota_slack = set(getattr(cal, "quota_slack", set()))
    cal.idle_factors = set(idle_factors or ())
    cal.idle_cells = {(f, t) for f in cal.idle_factors for t in periods}
    CMBAR = cal.db.sets["cmbar"]
    sols = None
    for sweep in range(max_sweeps):
        sols = _solve_path(cal, clo, periods, verbose, warm=warm)
        newly = {(c, t) for t in periods for c in CMBAR
                 if (c, t) not in cal.quota_slack
                 and sols[t]["PRQMBAR"].get(c, 0.0) < -tol}
        from .fuel import quota_ceiling
        over = {(c, t) for (c, t) in cal.quota_slack
                if t in sols and (c, t) in cal.qmbar0
                and sols[t]["QM"][c] > quota_ceiling(cal, sols[t], c, t) * (1 + 1e-6)}
        overuse = {(f, t) for (f, t) in cal.idle_cells
                   if t in sols and sols[t]["UERAT"].get(f, 0.0) < -tol}
        # informal fuel (gemcore/fuel.py): QMI >= 0 _|_ PM <= informal price
        on_inf, off_inf = set(), set()
        fcfg = getattr(cal, "fuel", None)
        if fcfg and fcfg.informal:
            from .fuel import informal_gap
            fu = fcfg.fuel
            active = getattr(cal, "inf_active", set())
            for t in periods:
                if t not in sols:
                    continue
                if (fu, t) in active:
                    if sols[t]["QMI"].get(fu, 0.0) < -1e-9 * cal.QM00[fu]:
                        off_inf.add((fu, t))
                elif informal_gap(cal, sols[t], t) > 1e-7 * sols[t]["PM"][fu]:
                    on_inf.add((fu, t))
            if on_inf:
                log(f"  sweep {sweep}: pump shortage deep enough for informal "
                    f"fuel in {sorted(t for _, t in on_inf)}")
            if off_inf:
                log(f"  sweep {sweep}: informal fuel negative -> off in "
                    f"{sorted(t for _, t in off_inf)}")
            cal.inf_active = (active | on_inf) - off_inf
        if over:
            log(f"  sweep {sweep}: {len(over)} slack quota cell(s) import above "
                f"the ceiling -> re-bind: {sorted(over)[:6]}")
            cal.quota_slack -= over
        if newly:
            log(f"  sweep {sweep}: {len(newly)} quota cell(s) with negative rent "
                f"-> slack, e.g. {sorted(newly)[:6]}")
            cal.quota_slack |= newly
        if overuse:
            log(f"  sweep {sweep}: {len(overuse)} idle cell(s) over-employed -> "
                f"full employment: {sorted(overuse)[:8]}")
            cal.idle_cells -= overuse
        if not (newly or over or overuse or on_inf or off_inf):
            break
    else:
        log(f"  !! complementarity sweep did not settle in {max_sweeps} passes")
    return sols


def sweep_report(cal, periods, through=None):
    """Which cells ended on which branch, as a small dict for printing."""
    through = through or periods[-1]
    CMBAR = cal.db.sets["cmbar"]
    slack = {c: sorted(t for (cc, t) in getattr(cal, "quota_slack", ())
                       if cc == c and t <= through) for c in CMBAR}
    idle = {f: sorted(t for (ff, t) in getattr(cal, "idle_cells", ())
                      if ff == f and t <= through)
            for f in getattr(cal, "idle_factors", ())}
    return {"quota slack (rent 0, imports below ceiling)":
            {c: ts for c, ts in slack.items() if ts},
            "idle branch active (rent at reservation, UERAT free)":
            {f: ts for f, ts in idle.items()}}
