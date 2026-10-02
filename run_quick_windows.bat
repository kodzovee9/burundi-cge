@echo off
rem The core checks only (about 5-10 minutes). Results: pymodel\reports\REPRODUCTION-CHECK.md
cd /d "%~dp0"
if not exist .venv\Scripts\python.exe (echo Run setup_windows.bat first. & pause & exit /b 1)
cd pymodel
..\.venv\Scripts\python runs\reproduce_all.py --quick
echo.
echo Open pymodel\reports\REPRODUCTION-CHECK.md to see the results.
pause
