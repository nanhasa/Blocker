@echo off
REM checks first that python is in path
REM then checks if codeCommenter file exists from solution file perspective
REM then runs the file from solution file perspective 
where python.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\PyCppUtility\CodeCommenter\codeCommenter.py (
		python %~dp0..\PyCppUtility\CodeCommenter\codeCommenter.py %~dp0..\PyCppUtility\CodeCommenter\ %~dp0..\..\Source\
	)
)
