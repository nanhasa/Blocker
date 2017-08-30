@echo off
where python.exe >nul 2>nul
if %errorlevel% EQU 0 (
	if exist %~dp0..\ObjDependencyUpdater\dependencyUpdater.py (
		python %~dp0..\ObjDependencyUpdater\dependencyUpdater.py BlockerTest ..\..\Test\BlockerTest ..\..\Temp\BlockerWin32Debug ..\..\Temp\BlockerWin32Release
	)
)
