@echo off
REM checks first that doxygen is in path
REM then checks if Doxyfile file exists from solution file perspective
REM then moves to that directory
REM then runs the file from solution file perspective 
REM then moves back to solution directory
where doxygen.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\..\Docs\Doxyfile (
		cd ..\..\Docs
		doxygen Doxyfile
		cd ..\Source\BlockerSolution
	)
)