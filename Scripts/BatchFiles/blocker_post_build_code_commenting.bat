@echo off
where python.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\..\Scripts\CodeCommenter\codeCommenter.py (
		python %~dp0..\..\Scripts\CodeCommenter\codeCommenter.py %~dp0..\..\Scripts\CodeCommenter\ %~dp0..\
	)
)
