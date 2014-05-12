@echo off

echo.
echo ===============================================
echo  OWLExt Build Script for Microsoft Visual C++
echo ===============================================
echo.

rem If the compiler is configured then try to identify it.
rem Otherwise search for a compiler environment to set up.
rem Try the newest known compiler first.
rem If all fails, quit with a message.

echo ---
echo --- Identifying the compiler
echo ---
echo.

if not defined VCINSTALLDIR goto try10
if not exist "%VCINSTALLDIR%\bin\cl.exe" goto error

cl 2>&1 | find "Version 16"
if "%errorlevel%"=="0" set COMPVER=10 & goto build

cl 2>&1 | find "Version 15"
if "%errorlevel%"=="0" set COMPVER=9 & goto build

cl 2>&1 | find "Version 14"
if "%errorlevel%"=="0" set COMPVER=8 & goto build

cl 2>&1 | find "Version 13"
if "%errorlevel%"=="0" set COMPVER=7 & goto build

goto error

:try10

if not exist "%VS100COMNTOOLS%VSVARS32.BAT" goto try9
call "%VS100COMNTOOLS%VSVARS32.BAT"
set COMPVER=10
goto build

:try9

if not exist "%VS90COMNTOOLS%VSVARS32.BAT" goto try8
call "%VS90COMNTOOLS%VSVARS32.BAT"
set COMPVER=9
goto build

:try8

if not exist "%VS80COMNTOOLS%VSVARS32.BAT" goto try71
call "%VS80COMNTOOLS%VSVARS32.BAT"
set COMPVER=8
goto build

:try71

if not exist "%VS71COMNTOOLS%VSVARS32.BAT" goto error
call "%VS71COMNTOOLS%VSVARS32.BAT"
set COMPVER=7
goto build

:error

echo ---
echo --- Error: Unable to locate a supported compiler.
echo --- See the script source for supported versions.
echo ---
goto exit

:build

echo COMPVER=%COMPVER%
echo.
set MAKE=nmake -fvc.mak
set COMMON=COMPVER=%COMPVER% %1 %2 %3 %4 %5 %6 %7 %8 %9
rem set DEBUGS=DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN"
set DEBUGS=DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__WARN"

rem Non-Unicode variants

rem echo ---
rem echo --- Building the OWLExt Release DLL
rem echo ---
rem %MAKE% DLL=1 %COMMON% 
rem echo.

rem echo ---
rem echo --- Building the OWLExt Debug DLL
rem echo ---
rem %MAKE% DLL=1 %DEBUGS% %COMMON% 
rem echo.

echo ---
echo --- Building the OWLExt Static Release Library
echo ---
%MAKE% %COMMON%
echo.

echo ---
echo --- Building the OWLExt Static Debug Library
echo ---
%MAKE% %DEBUGS% %COMMON%
echo.

rem Unicode variants

rem echo ---
rem echo --- Building the OWLExt Unicode Release DLL
rem echo ---
rem %MAKE% UNICODE=1 DLL=1 %COMMON% 
rem echo.

rem echo ---
rem echo --- Building the OWLExt Unicode Debug DLL
rem echo ---
rem %MAKE% UNICODE=1 DLL=1 %DEBUGS% %COMMON% 
rem echo.

rem echo ---
rem echo --- Building the OWLExt Unicode Static Release Library
rem echo ---
rem %MAKE% UNICODE=1 %COMMON%
rem echo.

rem echo ---
rem echo --- Building the OWLExt Unicode Static Debug Library
rem echo ---
rem %MAKE% UNICODE=1 %DEBUGS% %COMMON%
rem echo.

rem Report outputs

echo ---
echo --- Listing the target outputs after successful build
echo ---
echo.
dir ..\..\lib\*.lib ..\..\bin\*.dll

:exit

rem Clear local symbols.

set COMPVER=
set MAKE=
set COMMON=
set DEBUGS=
