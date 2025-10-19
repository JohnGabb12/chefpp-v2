@echo off
REM Initializes MSVC environment and builds the project

REM Find Visual Studio installation
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"

if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
        set "VSINSTALL=%%i"
    )
)

if not defined VSINSTALL (
    REM Fallback to common paths
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
        set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
        set "VSDEVCMD=C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
        set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
    )
) else (
    set "VSDEVCMD=%VSINSTALL%\Common7\Tools\VsDevCmd.bat"
)

if not defined VSDEVCMD (
    echo ERROR: Could not locate VsDevCmd.bat
    echo Please install Visual Studio with C++ workload
    exit /b 1
)

echo Initializing MSVC environment...
call "%VSDEVCMD%" -arch=x64 -host_arch=x64 >nul 2>&1
if errorlevel 1 (
    echo ERROR: Failed to initialize MSVC environment
    exit /b 1
)

cd /d "%~dp0.."

set "CONFIG=%1"
if "%CONFIG%"=="" set "CONFIG=Release"

echo.
echo Building Chef++ (%CONFIG%)...
echo.

if /i "%CONFIG%"=="Debug" (
    cl.exe /std:c++17 /W4 /EHsc /Od /MDd /Zi /I"%CD%" /Fe:"chefpp.exe" "main.cpp" /link /INCREMENTAL:NO
) else (
    cl.exe /std:c++17 /W4 /EHsc /O2 /MD /I"%CD%" /Fe:"chefpp.exe" "main.cpp" /link /INCREMENTAL:NO
)

if errorlevel 1 (
    echo.
    echo Build FAILED
    exit /b 1
)

echo.
echo Build succeeded! Executable: %CD%\chefpp.exe
exit /b 0
