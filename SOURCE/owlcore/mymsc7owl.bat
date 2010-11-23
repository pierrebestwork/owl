rem
rem ObjectWindows
rem
:@echo off

:===========================================================================================
: For MSVC.NET 7.0

call "D:\Program Files\Microsoft Visual Studio .NET 2003\vc7\bin\"VCVARS32.BAT

: No Debug
echo Compile with MS VC 7.0 OWL6V.DLL STD RTL
"D:\Program Files\Microsoft Visual Studio .NET 2003\vc7\bin\"nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 ALIGN=4

: Debug Multithreaded
echo Compile with MS VC 7.0 OWL6DV.DLL STD RTL
"D:\Program Files\Microsoft Visual Studio .NET 2003\vc7\bin\"nmake -fmsowldll.mak RCDEFINES=/i..\..\include;..\..\include\owl DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 ALIGN=4

