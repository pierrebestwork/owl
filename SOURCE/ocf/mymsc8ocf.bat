rem
rem ObjectWindows
rem
:@echo off


:===========================================================================================
: For MSVC.NET 8.0

call "C:\Program Files\Microsoft Visual Studio 8\VC\bin\"VCVARS32.BAT

: No Debug LIB
:echo Compile with MS VC 8.0 OCF6V.LIB with STD RTL and STL STRING
:"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmsocflib.mak RCDEFINES=/i..\..\include;..\..\include\owl;..\..\include\ocf LIBNAME=ocf OWLVER=6 DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 _OWLDLL=1

: Debug LIB
:echo Compile with MS VC 8.0 OCF6DV.LIB with STD RTL and STL STRING
:"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmsocflib.mak RCDEFINES=/i..\..\include;..\..\include\owl;..\..\include\ocf LIBNAME=ocf OWLVER=6 DEBUG=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0 VERSION=6 _OWLDLL=1

: No Debug Multithreaded DLL
echo Compile with MS VC 8.0 OCF6V.DLL with STD RTL and STL STRING
"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmsocfdll.mak RCDEFINES=/i..\..\include;..\..\include\owl;..\..\include\ocf LIBNAME=ocf OWLVER=6 DEBUG=0 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0

: Debug Multithreaded DLL
echo Compile with MS VC 8.0 OCF6DV.DLL  with STD RTL and STL STRING
"C:\Program Files\Microsoft Visual Studio 8\VC\bin\"nmake -fmsocfdll.mak RCDEFINES=/i..\..\include;..\..\include\owl;..\..\include\ocf LIBNAME=ocf OWLVER=6 DEBUG=1 DEBDEFS="/D__DEBUG=2 /D__TRACE /D__WARN" CODEVIEW=1 PLATFORM=INTEL WIN32=1 BI_STD_RTL=1 MBCS=0


