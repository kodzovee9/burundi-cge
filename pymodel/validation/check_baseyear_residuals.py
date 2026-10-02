"""Base-year transcription check: evaluate every model equation residual at
the calibrated 2019 point. A correct port yields residuals ~0 everywhere,
because base-year calibration is by construction a solution of the model.

Run: python3 validation/check_baseyear_residuals.py
"""

import sys
import os
from collections import defaultdict

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from gemcore.database import load_database, bdi2019_data2
from gemcore.calibration import calibrate
from gemcore.state import build_state, Closure
from gemcore.model import residuals

DATA = os.path.join(os.path.dirname(__file__), "..", "..",
                    "model", "user-files", "bdi2019", "bdi2019-data.xlsx")


def main():
    db = load_database(DATA, data2_hook=bdi2019_data2)
    cal = calibrate(db)
    tmin = cal.db.sets["tmin"][0]
    cal._solve_t = tmin

    V = build_state(cal, tmin)
    clo = Closure(cal, dcal01=True)          # calibration-pass form of PRODFN
    R = residuals(V, cal, tmin, {}, clo)

    # aggregate by equation family
    by_fam = defaultdict(lambda: [0.0, None, 0])
    worst = []
    for key, val in R.items():
        if val is None:
            continue
        fam = key[0]
        a = abs(val)
        rec = by_fam[fam]
        rec[2] += 1
        if a > rec[0]:
            rec[0] = a
            rec[1] = key
        worst.append((a, key))

    print(f"Equations evaluated: {sum(r[2] for r in by_fam.values())} "
          f"across {len(by_fam)} families\n")
    print(f"{'family':<22}{'count':>6}{'max|resid|':>16}   worst index")
    print("-" * 80)
    tol_fail = 0
    for fam in sorted(by_fam, key=lambda f: -by_fam[f][0]):
        mx, key, n = by_fam[fam]
        flag = "  <-- FAIL" if mx > 1e-6 else ""
        if mx > 1e-6:
            tol_fail += 1
        print(f"{fam:<22}{n:>6}{mx:>16.3e}   {key}{flag}")

    worst.sort(reverse=True)
    print("\nTop 15 individual residuals:")
    for a, key in worst[:15]:
        print(f"  {a:>14.3e}   {key}")

    globalmax = max((r[0] for r in by_fam.values()), default=0.0)
    print(f"\nGLOBAL max |residual| = {globalmax:.3e}")
    print("RESULT:", "PASS (transcription verified)" if globalmax < 1e-6
          else f"{tol_fail} equation families above 1e-6")


if __name__ == "__main__":
    main()
