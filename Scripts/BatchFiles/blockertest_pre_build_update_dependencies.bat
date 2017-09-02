@echo off
REM checks first that python is in path
REM then checks if dependencyUpdater file exists from solution file perspective
REM then runs the file from solution file perspective 
where python.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\ObjDependencyUpdater\dependencyUpdater.py (
		python %~dp0..\ObjDependencyUpdater\dependencyUpdater.py BlockerTest ..\..\Test\BlockerTest ..\..\Temp\BlockerWin32Debug ..\..\Temp\BlockerWin32Release
	)
)
