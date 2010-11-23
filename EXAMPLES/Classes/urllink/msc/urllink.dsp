# Microsoft Developer Studio Project File - Name="urllink" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=urllink - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "urllink.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "urllink.mak" CFG="urllink - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "urllink - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "urllink - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "urllink - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /W3 /GR /GX /O2 /X /I "c:\owl\include" /I "d:\msdev\vc\include" /I "c:\owl\include\stl" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_OWLDLL" /YX"owl/pch.h" /FD /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.li advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib mowl60f.lib /nologo /subsystem:windows /machine:I386 /libpath:"c:\owl\lib"

!ELSEIF  "$(CFG)" == "urllink - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /MDd /W3 /Gm /GR /GX /Zi /Od /X /I "c:\owl\include" /I "d:\msdev\vc\include" /I "c:\owl\include\stl" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_OWLDLL" /YX"owl/pch.h" /FD /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib msvcirt.lib msvcrt.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"c:\owl\lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "urllink - Win32 Release"
# Name "urllink - Win32 Debug"
# Begin Source File

SOURCE=.\applsdi.ico
# End Source File
# Begin Source File

SOURCE=.\urllnkap.cpp
# End Source File
# Begin Source File

SOURCE=.\UrllnkAp.h
# End Source File
# Begin Source File

SOURCE=.\urllnkap.rc

!IF  "$(CFG)" == "urllink - Win32 Release"

!ELSEIF  "$(CFG)" == "urllink - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UrllnkDC.cpp
# End Source File
# Begin Source File

SOURCE=.\UrllnkDC.h
# End Source File
# End Target
# End Project
