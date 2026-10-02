#!/bin/bash
# One-time setup on a Mac: creates a private Python environment in this folder
# (.venv) and installs the packages the model needs. Double-click to run, or
# run `bash setup_mac.command` in Terminal. Safe to run again.
cd "$(dirname "$0")"
PY=""
for cand in python3.13 python3.12 python3.11 python3; do
  if command -v "$cand" >/dev/null 2>&1; then
    v=$("$cand" -c 'import sys; print("%d.%d" % sys.version_info[:2])' 2>/dev/null)
    case "$v" in 3.11|3.12|3.13) PY="$cand"; break ;; esac
  fi
done
if [ -z "$PY" ]; then
  echo "Python 3.11, 3.12 or 3.13 was not found."
  echo "Install Python 3.13 from https://www.python.org/downloads/ and run this again."
  read -r -p "Press Return to close." _; exit 1
fi
echo "Using $PY ($("$PY" --version))"
"$PY" -m venv .venv || { echo "Could not create the environment."; read -r -p "Press Return to close." _; exit 1; }
.venv/bin/python -m pip install --upgrade pip >/dev/null
.venv/bin/python -m pip install -r pymodel/requirements.txt || { echo "Package installation failed (see above)."; read -r -p "Press Return to close." _; exit 1; }
echo
echo "Setup complete. Next: double-click run_all_mac.command (or run_quick_mac.command for a 5-minute check)."
read -r -p "Press Return to close." _
