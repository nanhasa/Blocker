@echo off
where doxygen.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\..\Docs\Doxyfile (
		cd ..\..\Docs
		doxygen Doxyfile
		cd ..\Source\BlockerSolution
	)
)