@echo off

cmd /c call build.cmd mvs16 Win32 Release
if errorlevel 1 goto :error

goto :eof

:error
exit /b 1
