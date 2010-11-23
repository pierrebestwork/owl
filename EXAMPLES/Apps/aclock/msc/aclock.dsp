# Microsoft Developer Studio Project File - Name="aclock" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=aclock - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aclock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aclock.mak" CFG="aclock - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aclock - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "aclock - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "aclock - Win32 Debug Static " (based on "Win32 (x86) Application")
!MESSAGE "aclock - Win32 Release Static" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aclock - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_OWLDLL" /D "_OWLPCH" /Yu"owl/pch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "aclock - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /D "_DEBUG" /D "_OWLDLL" /D "_OWLPCH" /D "WIN32" /D "_WINDOWS" /YX"owl/pch.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "aclock - Win32 Debug Static "

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SDebug"
# PROP BASE Intermediate_Dir "SDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SDebug"
# PROP Intermediate_Dir "SDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /D "_DEBUG" /D "_OWLDLL" /D "_OWLPCH" /D "WIN32" /D "_WINDOWS" /YX"owl/pch.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GR /GX /ZI /Od /D "_DEBUG" /D "_OWLPCH" /D "WIN32" /D "_WINDOWS" /Yu"owlpch.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib winmm.lib shell32.lib uuid.lib /nologo /subsystem:windows /verbose /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "aclock - Win32 Release Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SRelease"
# PROP BASE Intermediate_Dir "SRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SRelease"
# PROP Intermediate_Dir "SRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_OWLDLL" /D "_OWLPCH" /YX"owl/pch.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_OWLPCH" /Yu"owl/pch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "aclock - Win32 Release"
# Name "aclock - Win32 Debug"
# Name "aclock - Win32 Debug Static "
# Name "aclock - Win32 Release Static"
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ACLOCK.ICO
# End Source File
# Begin Source File

SOURCE=..\CUCKOO1.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO2.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO3.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO4.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO5.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO6.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO7.BMP
# End Source File
# Begin Source File

SOURCE=..\CUCKOO8.BMP
# End Source File
# Begin Source File

SOURCE=..\FACE_BMP.BMP
# End Source File
# End Group
# Begin Group "CPP Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ACLOCK.CPP
# ADD CPP /Yu"owl/pch.h"
# End Source File
# Begin Source File

SOURCE=.\aclock.RC
# End Source File
# Begin Source File

SOURCE=..\owlpch.cpp
# ADD CPP /Yc"owl/pch.h"
# End Source File
# End Group
# Begin Group "H Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ACLOCK.H
# End Source File
# End Group
# End Target
# End Project
