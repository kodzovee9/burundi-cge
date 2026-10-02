#!/bin/bash
# The core checks only (about 5-10 minutes). Results: pymodel/reports/REPRODUCTION-CHECK.md
cd "$(dirname "$0")"
if [ ! -x .venv/bin/python ]; then echo "Run setup_mac.command first."; read -r -p "Press Return to close." _; exit 1; fi
cd pymodel && ../.venv/bin/python runs/reproduce_all.py --quick
echo
echo "Open pymodel/reports/REPRODUCTION-CHECK.md to see the results."
read -r -p "Press Return to close." _
