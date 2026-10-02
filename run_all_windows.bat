@echo off
rem Re-runs every simulation and checks the findings (about 15-30 minutes).
rem Results: pymodel\reports\REPRODUCTION-CHECK.md
cd /d "%~dp0"
if not exist .venv\Scripts\python.exe (echo Run setup_windows.bat first. & pause & exit /b 1)
cd pymodel
..\.venv\Scripts\python runs\reproduce_all.py %*
echo.
echo Open pymodel\reports\REPRODUCTION-CHECK.md to see the results.
pause
