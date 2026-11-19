@echo off
:: Clean Build VCC 

set VCC_CONFIG=Release

:: Synchronize window title, About dialog, EXE metadata and INI version.
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\SetBuildVersion.ps1"
if errorlevel 1 exit /b 1
if exist "%VSINSTALLDIR%\MSBuild\Current\Bin\MSBuild.exe" (
  msbuild vcc.sln /t:Clean /p:Configuration=%VCC_CONFIG% /p:Platform=x86
  set _CL_=
  call Build.bat
  msbuild vcc.sln /t:Clean /p:Configuration=%VCC_CONFIG% /p:Platform=x86
  set _CL_=/DUSE_LOGGING
  call Build.bat
) else (
  echo Build must be run from Developer Command Prompt for VS 2022
)
