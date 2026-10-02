"""BRB (Banque de la Republique du Burundi) public import statistics.

Transcribed by hand from BRB publications (PDFs kept in `<repo>/raw-brb/`):

  * Rapport annuel, Exercice 2022, Annexe 11 "Structures des importations
    (en MBIF et en Tonnes)", 2018-2022 (source OBR), and Tableau 18
    "Balance des paiements (en MBIF)", 2018-2022.
    https://www.brb.bi/sites/default/files/2024-09/Rapport%20annuel%202022.pdf
  * Indicateurs de conjoncture, Decembre 2023, section IV.2: 2023 annual
    values by group, tonnages by use class only.
    https://www.brb.bi/sites/default/files/2024-11/Indicateur%20%20de%20D%C3%A9cembre%202023.pdf
  * Rapports de politique monetaire, T2-2024 and T4-2024, Annexe 9
    "Structure des importations en valeur (MBIF) et en volume (tonnes)":
    quarters T2-2023, T4-2023 and all four quarters of 2024.
    https://www.brb.bi/sites/default/files/2025-03/Rapport%20de%20Politique%20Mon%C3%A9taire%20%20du%202%C3%A8me%20Trimestre%202024.pdf
    https://www.brb.bi/sites/default/files/2025-03/MPC%20Q4-2024.pdf

Values in millions of BIF (CIF), quantities in tonnes. BRB publishes no
breakdown of imports by financing source; the balance of payments is the
nearest thing.

2023 group tonnages are not published. They are estimated as the 2023
annual value divided by the average unit value of the two published 2023
quarters (T2, T4) -- `estimated_2023_tonnes`.
"""

from __future__ import annotations

YEARS = ["2018", "2019", "2020", "2021", "2022"]

# annual, 2018-2022: key -> (label, values MBIF, tonnes)
ANNUAL = {
    "production_total": ("I. Biens de production",
        [629446.0, 739588.9, 750547.0, 933269.0, 1335033.0],
        [686636, 854148, 873078, 965489, 986202]),
    "metallurgy": ("10. Metallurgie", [82689.4, 91848.7, 119438.0, 178967.0, 155630.0],
        [42222, 43466, 57423, 83365, 61147]),
    "agri_inputs": ("11. Agriculture et elevage (fertiliser mainly)",
        [71873.1, 62155.7, 85020.0, 77740.0, 179024.0], [67638, 61704, 93602, 65593, 100959]),
    "food_inputs": ("12. Alimentation", [85689.9, 109486.9, 104347.0, 124461.0, 145821.0],
        [128071, 156605, 148871, 142075, 131702]),
    "construction": ("14. Construction", [49998.7, 100828.4, 90978.0, 113734.0, 113713.0],
        [198450, 314995, 278541, 378140, 358059]),
    "chemicals": ("180. Chimiques", [23403.8, 26074.9, 34762.0, 51409.0, 47604.0],
        [8708, 11467, 11713, 10620, 13362]),
    "mineral_oils": ("182. Huiles minerales (fuel)",
        [274147.6, 296812.2, 263208.0, 329495.0, 601720.0], [221211, 232319, 238848, 247847, 283910]),
    "equipment_total": ("II. Biens d'equipement", [275997.0, 278900.0, 364785.0, 387420.0, 414265.0],
        [40096, 50071, 55127, 57618, 57282]),
    "consumption_total": ("III. Biens de consommation",
        [509218.9, 619938.9, 626575.4, 714574.5, 814927.0], [249962, 239647, 247526, 308881, 294635]),
    "food_consumer": ("310. Alimentaires", [159271.9, 159449.9, 174058.0, 195999.0, 210959.0],
        [165638, 140639, 139718, 176723, 165623]),
    "pharma": ("311. Pharmaceutiques", [107981.7, 117435.0, 123812.0, 140848.0, 124670.0],
        [4638, 6107, 5256, 13787, 5059]),
}

# 2023 annual values (Indicateurs de conjoncture, Dec 2023); tonnages by class
VALUE_2023 = {"production_total": 1517400.0, "equipment_total": 559100.0,
              "consumption_total": 880900.0, "mineral_oils": 638660.7,
              "agri_inputs": 269326.2, "food_inputs": 205175.6,
              "construction": 142645.2, "metallurgy": 122862.3,
              "food_consumer": 258102.9, "pharma": 105091.8}
TONNES_2023_CLASS = {"production_total": 975635, "equipment_total": 60690,
                     "consumption_total": 315010}

# quarterly (Annexe 9 of the T2-2024 and T4-2024 policy reports): (V, Q)
QUARTERLY = {
    "T2-2023": {"mineral_oils": (132137.9, 61600.5), "agri_inputs": (63758.0, 30802.1),
                "chemicals": (13077.6, 3050.9)},
    "T4-2023": {"mineral_oils": (202455.0, 66742.7), "agri_inputs": (94650.2, 51609.2),
                "chemicals": (14437.0, 2979.7)},
    "T1-2024": {"mineral_oils": (126884.9, 44593.1), "agri_inputs": (42238.6, 23826.9),
                "chemicals": (6954.6, 3187.9), "total": (730870.5, 320696.0)},
    "T2-2024": {"mineral_oils": (128942.8, 47866.1), "agri_inputs": (49106.0, 27869.2),
                "chemicals": (11736.7, 3598.2), "total": (748993.6, 315236.9)},
    "T3-2024": {"mineral_oils": (134353.1, 50453.6), "agri_inputs": (46699.4, 31602.5),
                "chemicals": (9233.7, 3067.8), "total": (737850.2, 367013.1)},
    "T4-2024": {"mineral_oils": (84550.3, 32759.6), "agri_inputs": (58343.4, 45473.8),
                "chemicals": (9519.0, 4607.8), "total": (707399.7, 334171.7)},
}

# balance of payments, MBIF, 2018-2022 (Rapport annuel 2022, Tableau 18)
BOP = {
    "imports_fob": [1215977.9, 1402027.9, 1502675.8, 1736567.0, 2178085.5],
    "secondary_income_credit": [505964.2, 707118.0, 879329.1, 1045883.4, 1048439.1],
    "capital_account_credit": [211888.2, 254136.5, 260191.2, 265881.5, 341724.0],
    "other_investment_liabilities": [335550.0, 590546.6, 536727.0, 965979.3, 764016.5],
    "reserve_assets": [-53754.8, 81397.2, -42003.9, 355095.8, -187528.4],
    "current_account": [-693850.5, -724857.0, -664255.0, -785210.1, -1265654.2],
}


def estimated_2023_tonnes(key):
    """2023 annual tonnes for a group: annual value over the mean unit value
    of the two published 2023 quarters. None if not estimable."""
    q = [QUARTERLY[p][key] for p in ("T2-2023", "T4-2023") if key in QUARTERLY[p]]
    if not q:
        return None
    unit = sum(v for v, _ in q) / sum(t for _, t in q)
    if key in VALUE_2023:
        return VALUE_2023[key] / unit
    # no 2023 annual value (chemicals): twice the two known quarters
    return 2.0 * sum(t for _, t in q)


def tonnes(key):
    """Annual tonnes 2018-2024 for a group ({year: tonnes}); 2023 estimated,
    2024 the sum of the four published quarters."""
    out = {y: float(q) for y, q in zip(YEARS, ANNUAL[key][2])}
    est = estimated_2023_tonnes(key)
    if est is not None:
        out["2023"] = est
    q24 = [QUARTERLY[p][key][1] for p in ("T1-2024", "T2-2024", "T3-2024", "T4-2024")
           if key in QUARTERLY[p]]
    if len(q24) == 4:
        out["2024"] = sum(q24)
    return out


def volume_index(keys, base="2019"):
    """Laspeyres volume index over groups, weights = base-year values."""
    w = {k: ANNUAL[k][1][YEARS.index(base)] for k in keys}
    q = {k: tonnes(k) for k in keys}
    years = sorted(set.intersection(*(set(q[k]) for k in keys)))
    return {y: sum(w[k] * q[k][y] / q[k][base] for k in keys) / sum(w.values())
            for y in years if y >= base}


# model commodities under an import quota -> the BRB groups that measure them
QUOTA_GROUPS = {
    "c-refpet": ("mineral_oils",),
    "c-chemplast": ("agri_inputs", "chemicals"),   # fertiliser + chemicals
}


if __name__ == "__main__":
    for c, keys in QUOTA_GROUPS.items():
        idx = volume_index(keys)
        print(f"{c:12} volume index (2019 = 1):", {y: round(v, 3) for y, v in idx.items()})
    for k in ("mineral_oils", "agri_inputs", "chemicals"):
        print(f"{k:12} tonnes:", {y: round(v) for y, v in tonnes(k).items()})
