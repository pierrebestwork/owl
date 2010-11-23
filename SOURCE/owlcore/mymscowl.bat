rem
rem ObjectWindows
rem
:@echo off

call "d:\Program Files\Microsoft Visual Studio\VC98\Bin\VCVARS32.BAT"

:===========================================================================================
: For MS VC 5.0 added VERSION=5

:echo Compile with MS VC 5.0 OWL6V.DLL 
:d:\msdev\vc\bin\nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 VERSION=5 MBCS=0

:echo Compile with MS VC 5.0 OWL6DV.DLL
:d:\msdev\vc\bin\nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 VERSION=5 MBCS=0

:echo Compile with MS VC 5.0 OWL6DV.DLL and BoundceChecker
:d:\msdev\vc\bin\nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 VERSION=5 MBCS=0 USE_BC=1

:===========================================================================================
: For MS VC 6.0

echo Compile with MS VC 6.0 OWL6V.DLL STD RTL
nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6

echo Compile with MS VC 6.0 OWL6DV.DLL STD RTL
nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6

:------------------------------------------------------------------------------------------
echo Compile with MS VC 6.0 OWL6VU.DLL STD RTL + UNICODE
nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 UNICODE=1 MBCS=0 VERSION=6

echo Compile with MS VC 6.0 OWL6DVU.DLL STD RTL + UNICODE
nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 UNICODE=1 MBCS=0 VERSION=6
:------------------------------------------------------------------------------------------

:------------------------------------------------------------------------------------------
:echo Compile with MS VC 6.0 static OWL6V.LIB STD RTL
:nmake -fmscmake RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6

:echo Compile with MS VC 6.0 static OWL6VD.LIB STD RTL
:nmake -fmscmake RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6
:------------------------------------------------------------------------------------------

:------------------------------------------------------------------------------------------
:echo Compile with MS VC 6.0 OWL6VU.LIB STD RTL + UNICODE
:nmake -fmscmake RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 UNICODE=1 MBCS=0 VERSION=6

:echo Compile with MS VC 6.0 static OWL6DVU.LIB STD RTL + UNICODE
:nmake -fmscmake RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 UNICODE=1 MBCS=0 VERSION=6 VERSION=6
:===========================================================================================

:===========================================================================================
: For MSVC.NET 7.0

:echo Compile with MS VC 7.0 OWL6V.DLL STD RTL
:d:\PROGRA~1\MICROS~1.NET\VC7\bin\nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=7

