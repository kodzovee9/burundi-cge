"""Re-run every simulation and check the results against the documented findings.

    python runs/reproduce_all.py              # everything (about 15-30 minutes)
    python runs/reproduce_all.py --quick      # the core checks only (about 5 minutes)
    python runs/reproduce_all.py --workers 2  # fewer runs at a time (slower machines)

What it does, in order:

1. checks that the model reproduces the 2019 base year exactly;
2. runs every simulation, several at a time, each writing its usual report
   in `reports/` and its screen output in `reports/logs/<job>.log`;
3. checks that every run converged;
4. compares the key findings with the values in `runs/expected_results.json`
   (each with a tolerance), and writes `reports/REPRODUCTION-CHECK.md`.

It needs nothing beyond the packages in requirements.txt. The 2019-24
backcast reads INSBU's national accounts: the files in `../raw-insbu/` if
there, else their extract `data/insbu-aggregates-2016-2024.csv`; without
either the backcast checks are skipped and the report says so.
"""

from __future__ import annotations

import argparse
import concurrent.futures as cf
import csv
import json
import os
import re
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
LOGS = os.path.join(ROOT, "reports", "logs")
EXPECTED = os.path.join(HERE, "expected_results.json")
REPORT = os.path.join(ROOT, "reports", "REPRODUCTION-CHECK.md")
INSBU = os.path.join(os.path.dirname(ROOT), "raw-insbu")
# replication-mode runs compare with the reference application's GAMS results,
# kept in data/gams-reference.json for internal use (not in the public repository)
GAMS_REF = os.path.join(ROOT, "data", "gams-reference.json")
GAMS_JOBS = {"scenarios-gams-replication", "detail-gams-replication", "audit-gams-replication"}

# (job name, script and arguments, part of --quick?, needs INSBU?)
JOBS = [
    ("scenarios", ["runs/all_scenarios.py"], True, False),
    ("scenarios-gams-replication", ["runs/all_scenarios.py", "--gams-replication"], True, False),
    ("detail-gams-replication", ["runs/validate_detail.py", "--gams-replication"], False, False),
    ("passthrough", ["runs/uni_passthrough.py"], False, False),
    ("audit", ["validation/check_solutions.py"], False, False),
    ("audit-gams-replication", ["validation/check_solutions.py", "--gams-replication"], False, False),
    ("reform-omega-0", ["runs/reform_boost.py", "--rent-cost", "0"], False, False),
    ("reform-omega-0.25", ["runs/reform_boost.py", "--rent-cost", "0.25"], True, False),
    ("reform-omega-0.5", ["runs/reform_boost.py", "--rent-cost", "0.5"], False, False),
    ("backcast", ["runs/backcast.py"], True, True),
    ("backcast-fuel-fx", ["runs/backcast.py", "--fuel-quota", "fx", "--tag", "fuel-fx"], False, True),
    ("backcast-chem-fx", ["runs/backcast.py", "--chem-quota", "fx", "--tag", "chem-fx"], False, True),
    ("backcast-fx-both", ["runs/backcast.py", "--fuel-quota", "fx", "--chem-quota", "fx",
                          "--tag", "fx-both"], False, True),
    ("backcast-premium-flat", ["runs/backcast.py", "--premium", "flat", "--tag", "premium-flat"], False, True),
    ("backcast-quotas-frozen", ["runs/backcast.py", "--quotas", "data", "--tag", "quotas-frozen"], False, True),
    ("backcast-pt-0.75", ["runs/backcast.py", "--passthrough", "0.75", "--tag", "pt-0.75"], False, True),
    ("backcast-pt-0.5", ["runs/backcast.py", "--passthrough", "0.5", "--tag", "pt-0.5"], False, True),
    ("backcast-pt-0.25", ["runs/backcast.py", "--passthrough", "0.25", "--tag", "pt-0.25"], False, True),
    ("backcast-govcon-share", ["runs/backcast.py", "--govcon", "insbu", "--tag", "govcon-share"], False, True),
    ("backcast-stocks-model", ["runs/backcast.py", "--stocks", "model", "--tag", "stocks-model"], False, True),
    ("backcast-mining-pinned", ["runs/backcast.py", "--sector-targets", "--target-groups", "min",
                                "--pin-gdp", "--tag", "mining-pinned"], False, True),
]

# a log containing any of these did not finish properly
BAD = ["NOT CONVERGED", "did not settle", "Traceback", "NOT EXACT", "FAIL"]


def run(name, args):
    t0 = time.time()
    log = os.path.join(LOGS, f"{name}.log")
    with open(log, "w") as fh:
        proc = subprocess.run([sys.executable] + args, cwd=ROOT, stdout=fh,
                              stderr=subprocess.STDOUT)
    text = open(log, errors="replace").read()
    problems = [b for b in BAD if b in text]
    if proc.returncode:
        problems.append(f"exit code {proc.returncode}")
    return name, time.time() - t0, problems


def insbu_present():
    sys.path.insert(0, ROOT)
    from gemcore.insbu import source
    return source() is not None          # INSBU's files, or their extract


# ---- reading values back from the reports ----------------------------------

def value(spec):
    path = os.path.join(ROOT, spec["file"])
    kind = spec["source"]
    if kind == "csv":                      # row = first column, col = header
        with open(path, newline="") as fh:
            for row in csv.DictReader(fh):
                if row[next(iter(row))] == spec["row"]:
                    return float(row[spec["col"]])
        raise KeyError(spec["row"])
    if kind == "json":                     # nested keys
        v = json.load(open(path))
        for k in spec["path"]:
            v = v[k]
        return float(v)
    if kind == "backcast":                 # indicator, year, model|actual
        with open(path, newline="") as fh:
            for row in csv.DictReader(fh):
                if row["indicator"] == spec["indicator"] and row["year"] == spec["year"]:
                    return float(row[spec["field"]])
        raise KeyError(spec["indicator"])
    if kind == "backcast-avg":             # average over years
        vals = []
        with open(path, newline="") as fh:
            for row in csv.DictReader(fh):
                if row["indicator"] == spec["indicator"] and row["year"] in spec["years"] \
                        and row[spec["field"]] not in ("", "None"):
                    vals.append(float(row[spec["field"]]))
        return sum(vals) / len(vals)
    if kind == "regex":                    # first number captured in a text file
        m = re.search(spec["pattern"], open(path, errors="replace").read())
        return float(m.group(1))
    raise ValueError(kind)


def check(spec):
    try:
        got = value(spec)
    except Exception as exc:               # missing file or row: the run did not produce it
        return None, f"not found ({exc.__class__.__name__})"
    if "max" in spec:
        ok = got <= spec["max"]
    else:
        ok = abs(got - spec["expected"]) <= spec["tol"]
    return got, "PASS" if ok else "FAIL"


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--quick", action="store_true", help="core checks only")
    ap.add_argument("--workers", type=int, default=max(1, min(6, (os.cpu_count() or 2) - 1)),
                    help="simulations run at the same time (default: up to 6)")
    ap.add_argument("--check-only", action="store_true",
                    help="skip the runs; only compare the existing reports")
    a = ap.parse_args()
    os.makedirs(LOGS, exist_ok=True)
    t0 = time.time()
    have_insbu = insbu_present()
    have_gams = os.path.exists(GAMS_REF)
    status = {}

    if not a.check_only:
        print("1/3  checking the 2019 base year ...", flush=True)
        name, secs, problems = run("baseyear", ["validation/check_baseyear_residuals.py"])
        status[name] = (secs, problems)
        if problems:
            print(f"     FAILED ({', '.join(problems)}); see reports/logs/baseyear.log")
            sys.exit(1)
        print(f"     ok ({secs:.0f} s)", flush=True)

        jobs = [(n, args) for n, args, quick, needs in JOBS
                if (quick or not a.quick) and (have_insbu or not needs)
                and (have_gams or n not in GAMS_JOBS)]
        print(f"2/3  running {len(jobs)} simulations, {a.workers} at a time ...", flush=True)
        if not have_insbu:
            print("     (no INSBU data, files or extract: backcasts skipped)")
        if not have_gams:
            print("     (no reference-application results: replication-mode runs skipped)")
        with cf.ThreadPoolExecutor(max_workers=a.workers) as ex:
            futs = [ex.submit(run, n, args) for n, args in jobs]
            for f in cf.as_completed(futs):
                name, secs, problems = f.result()
                status[name] = (secs, problems)
                flag = "ok" if not problems else "PROBLEM: " + ", ".join(problems)
                print(f"     {name:<30} {secs:6.0f} s   {flag}", flush=True)
        if any(n.startswith("reform-") for n, _ in jobs):
            run("reform-collect", ["runs/reform_boost.py", "--collect"])

    print("3/3  comparing with the expected findings ...", flush=True)
    specs = json.load(open(EXPECTED))
    rows, n_pass, n_fail, n_skip = [], 0, 0, 0
    for spec in specs:
        if spec.get("needs_insbu") and not have_insbu:
            rows.append((spec, None, "skipped (no INSBU data)")); n_skip += 1
            continue
        if spec.get("needs_gams") and not have_gams:
            rows.append((spec, None, "skipped (no reference-application results)")); n_skip += 1
            continue
        if a.quick and not spec.get("quick"):
            continue
        got, verdict = check(spec)
        rows.append((spec, got, verdict))
        if verdict == "PASS":
            n_pass += 1
        else:
            n_fail += 1

    md = ["# Reproduction check", "",
          f"Run on {time.strftime('%Y-%m-%d %H:%M')}, {time.time() - t0:.0f} s in all. "
          f"{n_pass} findings reproduced, {n_fail} not, {n_skip} skipped."
          + ("" if have_insbu else " No INSBU data (files or extract) was found, so the backcast was not run."), ""]
    if status:
        md += ["## Runs", "", "| run | seconds | status |", "| --- | ---: | --- |"]
        for n, (secs, problems) in status.items():
            md.append(f"| {n} | {secs:.0f} | {'converged' if not problems else ', '.join(problems)} |")
        md.append("")
    md += ["## Findings", "", "| finding | expected | reproduced | result | where |",
           "| --- | ---: | ---: | --- | --- |"]
    for spec, got, verdict in rows:
        exp = (f"≤ {spec['max']:g}" if "max" in spec else f"{spec['expected']:g} ± {spec['tol']:g}")
        gv = "–" if got is None else (f"{got:.3g}" if "max" in spec else f"{got:.3f}")
        md.append(f"| {spec['finding']} | {exp} | {gv} | {verdict} | `{spec['file']}` |")
    open(REPORT, "w").write("\n".join(md) + "\n")
    print(f"\n{n_pass} findings reproduced, {n_fail} not, {n_skip} skipped.")
    print(f"Report: {os.path.relpath(REPORT, os.getcwd())}")
    sys.exit(0 if n_fail == 0 and all(not p for _, p in status.values()) else 1)


if __name__ == "__main__":
    main()
