:: Windows Installer for Simple 2D
::   This script will install includes, libraries, and DLLs for
::   building Simple 2D apps.

@echo off

echo.
echo Simple 2D and its dependencies will be installed to:
echo   %%LOCALAPPDATA%%\simple2d
echo   Resolving to %LOCALAPPDATA%\simple2d
echo.

set INPUT=
set /p INPUT=Continue? (y/n) %=%
if /i "%INPUT%"=="y" goto yes
echo.
echo Quitting...
timeout 3
exit /b 0

:yes
echo. & echo Copying... & echo.
set S2D_DIR=%LOCALAPPDATA%\simple2d
if not exist %S2D_DIR% mkdir %S2D_DIR%
xcopy /e /y simple2d\* %S2D_DIR%

echo.
echo Simple 2D installed successfully!
echo Remember to add %%LOCALAPPDATA%%\simple2d to your %%PATH%%
echo.
pause
