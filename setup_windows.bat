@echo off
rem One-time setup on Windows: creates a private Python environment in this
rem folder (.venv) and installs the packages the model needs. Double-click to run.
cd /d "%~dp0"
set PY=
for %%v in (3.13 3.12 3.11) do (
  if not defined PY (
    py -%%v --version >nul 2>&1 && set PY=py -%%v
  )
)
if not defined PY (
  echo Python 3.11, 3.12 or 3.13 was not found.
  echo Install Python 3.13 from https://www.python.org/downloads/ ^(tick "Add python.exe to PATH"^) and run this again.
  pause
  exit /b 1
)
echo Using %PY%
%PY% -m venv .venv || (echo Could not create the environment. & pause & exit /b 1)
.venv\Scripts\python -m pip install --upgrade pip >nul
.venv\Scripts\python -m pip install -r pymodel\requirements.txt || (echo Package installation failed ^(see above^). & pause & exit /b 1)
echo.
echo Setup complete. Next: double-click run_all_windows.bat ^(or run_quick_windows.bat for a short check^).
pause
