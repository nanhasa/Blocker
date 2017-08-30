@echo off
where python.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\CodeCommenter\codeCommenter.py (
		python %~dp0..\CodeCommenter\codeCommenter.py ..\CodeCommenter\ ..\..\Source\
	)
)
