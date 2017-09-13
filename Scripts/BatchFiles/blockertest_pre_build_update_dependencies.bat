@echo off
REM checks first that python is in path
REM then checks if dependencyUpdater file exists from solution file perspective
REM then runs the file from solution file perspective 
where python.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\PyCppUtility\ObjDependencyUpdater\dependencyUpdater.py (
		python %~dp0..\PyCppUtility\ObjDependencyUpdater\dependencyUpdater.py BlockerTest %~dp0..\..\Test\BlockerTest %~dp0..\..\Temp\BlockerWin32Debug %~dp0..\..\Temp\BlockerWin32Release
	)
)
