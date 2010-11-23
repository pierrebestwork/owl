rem
rem ObjectWindows
rem
:@echo off

:===========================================================================================
: For MSVC.NET 8.0

call "C:\Program Files\Microsoft Visual Studio 8\VC\bin\"VCVARS32.BAT

: No Debug
echo Compile with MS VC 8.0 OWL6V.DLL STD RTL
"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 ALIGN=4

: Debug Multithreaded
echo Compile with MS VC 8.0 OWL6DV.DLL STD RTL
"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 ALIGN=4

echo Compile with MS VC 6.0 static OWL6V.LIB STD RTL
"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmscmake RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 ALIGN=4

echo Compile with MS VC 6.0 static OWL6VD.LIB STD RTL
"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmscmake RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 ALIGN=4
