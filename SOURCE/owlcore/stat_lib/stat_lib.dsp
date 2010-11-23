# Microsoft Developer Studio Project File - Name="stat_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=stat_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "stat_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "stat_lib.mak" CFG="stat_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "stat_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "stat_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "stat_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_OWLPCH" /D "_X86_" /Yu"owl/pch.h" /FD /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\include" /i "..\..\..\include\owl" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\stat_lib.lib ..\..\..\lib\owlv.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "stat_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_OWLPCH" /D "_X86_" /FR /Yu"owl/pch.h" /Fd"Debug/stat_lib.pdb" /FD /GF /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\..\include" /i "..\..\..\include\owl" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\stat_lib.lib ..\..\..\lib\owlvd.lib  copy debug\stat_lib.pdb ..\..\..\lib\owlvd.pdb
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "stat_lib - Win32 Release"
# Name "stat_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\animctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\appdict.cpp
# End Source File
# Begin Source File

SOURCE=..\applicat.cpp
# End Source File
# Begin Source File

SOURCE=..\bardescr.cpp
# End Source File
# Begin Source File

SOURCE=..\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\bitmapga.cpp
# End Source File
# Begin Source File

SOURCE=..\bitset.cpp
# End Source File
# Begin Source File

SOURCE=..\brush.cpp
# End Source File
# Begin Source File

SOURCE=..\btntextg.cpp
# End Source File
# Begin Source File

SOURCE=..\button.cpp
# End Source File
# Begin Source File

SOURCE=..\buttonga.cpp
# End Source File
# Begin Source File

SOURCE=..\castable.cpp
# End Source File
# Begin Source File

SOURCE=..\celarray.cpp
# End Source File
# Begin Source File

SOURCE=..\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=..\checklst.cpp
# End Source File
# Begin Source File

SOURCE=..\checks.cpp
# SUBTRACT CPP /Yu
# End Source File
# Begin Source File

SOURCE=..\chooseco.cpp
# End Source File
# Begin Source File

SOURCE=..\choosefo.cpp
# End Source File
# Begin Source File

SOURCE=..\clipboar.cpp
# End Source File
# Begin Source File

SOURCE=..\clipview.cpp
# End Source File
# Begin Source File

SOURCE=..\cmdline.cpp
# End Source File
# Begin Source File

SOURCE=..\colmnhdr.cpp
# End Source File
# Begin Source File

SOURCE=..\color.cpp
# End Source File
# Begin Source File

SOURCE=..\combobex.cpp
# End Source File
# Begin Source File

SOURCE=..\combobox.cpp
# End Source File
# Begin Source File

SOURCE=..\commctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\commdial.cpp
# End Source File
# Begin Source File

SOURCE=..\commview.cpp
# End Source File
# Begin Source File

SOURCE=..\compat.cpp
# End Source File
# Begin Source File

SOURCE=..\configfl.cpp
# End Source File
# Begin Source File

SOURCE=..\control.cpp
# End Source File
# Begin Source File

SOURCE=..\controlb.cpp
# End Source File
# Begin Source File

SOURCE=..\controlg.cpp
# End Source File
# Begin Source File

SOURCE=..\createdc.cpp
# End Source File
# Begin Source File

SOURCE=..\cstring.cpp
# End Source File
# Begin Source File

SOURCE=..\cursor.cpp
# End Source File
# Begin Source File

SOURCE=..\date.cpp
# End Source File
# Begin Source File

SOURCE=..\dateio.cpp
# End Source File
# Begin Source File

SOURCE=..\datep.cpp
# End Source File
# Begin Source File

SOURCE=..\datetime.cpp
# End Source File
# Begin Source File

SOURCE=..\dc.cpp
# End Source File
# Begin Source File

SOURCE=..\decframe.cpp
# End Source File
# Begin Source File

SOURCE=..\decmdifr.cpp
# End Source File
# Begin Source File

SOURCE=..\diaginit.cpp
# End Source File
# Begin Source File

SOURCE=..\dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\dib.cpp
# End Source File
# Begin Source File

SOURCE=..\dibdc.cpp
# End Source File
# Begin Source File

SOURCE=..\dibitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\dispatch.cpp
# End Source File
# Begin Source File

SOURCE=..\docking.cpp
# End Source File
# Begin Source File

SOURCE=..\docmanag.cpp
# End Source File
# Begin Source File

SOURCE=..\doctpl.cpp
# End Source File
# Begin Source File

SOURCE=..\document.cpp
# End Source File
# Begin Source File

SOURCE=..\draglist.cpp
# End Source File
# Begin Source File

SOURCE=..\drawitem.cpp
# End Source File
# Begin Source File

SOURCE=..\edit.cpp
# End Source File
# Begin Source File

SOURCE=..\editfile.cpp
# End Source File
# Begin Source File

SOURCE=..\editsear.cpp
# End Source File
# Begin Source File

SOURCE=..\editview.cpp
# End Source File
# Begin Source File

SOURCE=..\eventhan.cpp
# End Source File
# Begin Source File

SOURCE=..\exbase.cpp
# End Source File
# Begin Source File

SOURCE=..\except.cpp
# End Source File
# Begin Source File

SOURCE=..\file.cpp
# End Source File
# Begin Source File

SOURCE=..\filedoc.cpp
# End Source File
# Begin Source File

SOURCE=..\filename.cpp
# End Source File
# Begin Source File

SOURCE=..\filtval.cpp
# End Source File
# Begin Source File

SOURCE=..\findrepl.cpp
# End Source File
# Begin Source File

SOURCE=..\flatctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\floatfra.cpp
# End Source File
# Begin Source File

SOURCE=..\font.cpp
# End Source File
# Begin Source File

SOURCE=..\framewin.cpp
# End Source File
# Begin Source File

SOURCE=..\fscroll.cpp
# End Source File
# Begin Source File

SOURCE=..\functor.cpp
# End Source File
# Begin Source File

SOURCE=..\gadget.cpp
# End Source File
# Begin Source File

SOURCE=..\gadgetli.cpp
# End Source File
# Begin Source File

SOURCE=..\gadgetwi.cpp
# End Source File
# Begin Source File

SOURCE=..\gauge.cpp
# End Source File
# Begin Source File

SOURCE=..\gdibase.cpp
# End Source File
# Begin Source File

SOURCE=..\gdiobjec.cpp
# End Source File
# Begin Source File

SOURCE=..\geometrp.cpp
# End Source File
# Begin Source File

SOURCE=..\geometrs.cpp
# End Source File
# Begin Source File

SOURCE=..\geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\global.cpp
# End Source File
# Begin Source File

SOURCE=..\glyphbtn.cpp
# End Source File
# Begin Source File

SOURCE=..\groupbox.cpp
# End Source File
# Begin Source File

SOURCE=..\hlpmanag.cpp
# End Source File
# Begin Source File

SOURCE=..\hotkey.cpp
# End Source File
# Begin Source File

SOURCE=..\hslider.cpp
# End Source File
# Begin Source File

SOURCE=..\icon.cpp
# End Source File
# Begin Source File

SOURCE=..\imagelst.cpp
# End Source File
# Begin Source File

SOURCE=..\initdll.cpp
# End Source File
# Begin Source File

SOURCE=..\inputdia.cpp
# End Source File
# Begin Source File

SOURCE=..\ipaddres.cpp
# End Source File
# Begin Source File

SOURCE=..\layoutwi.cpp
# End Source File
# Begin Source File

SOURCE=..\libmain.cpp
# End Source File
# Begin Source File

SOURCE=..\listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\listview.cpp
# End Source File
# Begin Source File

SOURCE=..\listwind.cpp
# End Source File
# Begin Source File

SOURCE=..\locale.cpp
# End Source File
# Begin Source File

SOURCE=..\localeco.cpp
# End Source File
# Begin Source File

SOURCE=..\localeid.cpp
# End Source File
# Begin Source File

SOURCE=..\lookval.cpp
# End Source File
# Begin Source File

SOURCE=..\mailer.cpp
# End Source File
# Begin Source File

SOURCE=..\main.cpp
# End Source File
# Begin Source File

SOURCE=..\mci.cpp
# End Source File
# Begin Source File

SOURCE=..\mciwav.cpp
# End Source File
# Begin Source File

SOURCE=..\mdichild.cpp
# End Source File
# Begin Source File

SOURCE=..\mdiclien.cpp
# End Source File
# Begin Source File

SOURCE=..\mdiframe.cpp
# End Source File
# Begin Source File

SOURCE=..\memory.cpp
# End Source File
# Begin Source File

SOURCE=..\memorydc.cpp
# End Source File
# Begin Source File

SOURCE=..\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\menudesc.cpp
# End Source File
# Begin Source File

SOURCE=..\menugadg.cpp
# End Source File
# Begin Source File

SOURCE=..\messageb.cpp
# End Source File
# Begin Source File

SOURCE=..\metafile.cpp
# End Source File
# Begin Source File

SOURCE=..\metafldc.cpp
# End Source File
# Begin Source File

SOURCE=..\modegad.cpp
# End Source File
# Begin Source File

SOURCE=..\module.cpp
# End Source File
# Begin Source File

SOURCE=..\moduleob.cpp
# End Source File
# Begin Source File

SOURCE=..\modversi.cpp
# End Source File
# Begin Source File

SOURCE=..\monthcal.cpp
# End Source File
# Begin Source File

SOURCE=..\msgthred.cpp
# End Source File
# Begin Source File

SOURCE=..\notetab.cpp
# End Source File
# Begin Source File

SOURCE=..\objstrm.cpp
# End Source File
# Begin Source File

SOURCE=..\opensave.cpp
# End Source File
# Begin Source File

SOURCE=..\owl.cpp
# End Source File
# Begin Source File

SOURCE=..\owlmain.cpp
# End Source File
# Begin Source File

SOURCE=..\pager.cpp
# End Source File
# Begin Source File

SOURCE=..\paintdc.cpp
# End Source File
# Begin Source File

SOURCE=..\palette.cpp
# End Source File
# Begin Source File

SOURCE=..\panespli.cpp
# End Source File
# Begin Source File

SOURCE=..\pen.cpp
# End Source File
# Begin Source File

SOURCE=..\picker.cpp
# End Source File
# Begin Source File

SOURCE=..\picklist.cpp
# End Source File
# Begin Source File

SOURCE=..\pictval.cpp
# End Source File
# Begin Source File

SOURCE=..\pictwind.cpp
# End Source File
# Begin Source File

SOURCE=..\popupgad.cpp
# End Source File
# Begin Source File

SOURCE=..\preview.cpp
# End Source File
# Begin Source File

SOURCE=..\prevwin.cpp
# End Source File
# Begin Source File

SOURCE=..\printdc.cpp
# End Source File
# Begin Source File

SOURCE=..\printdia.cpp
# End Source File
# Begin Source File

SOURCE=..\printer.cpp
# End Source File
# Begin Source File

SOURCE=..\printout.cpp
# End Source File
# Begin Source File

SOURCE=..\profile.cpp
# End Source File
# Begin Source File

SOURCE=..\propsht.cpp
# End Source File
# Begin Source File

SOURCE=..\radiobut.cpp
# End Source File
# Begin Source File

SOURCE=..\rangeval.cpp
# End Source File
# Begin Source File

SOURCE=..\rcntfile.cpp
# End Source File
# Begin Source File

SOURCE=..\regheap.cpp
# End Source File
# Begin Source File

SOURCE=..\region.cpp
# End Source File
# Begin Source File

SOURCE=..\registry.cpp
# End Source File
# Begin Source File

SOURCE=..\reglink.cpp
# End Source File
# Begin Source File

SOURCE=..\resource.cpp
# End Source File
# Begin Source File

SOURCE=..\richedit.cpp
# End Source File
# Begin Source File

SOURCE=..\richedpr.cpp
# End Source File
# Begin Source File

SOURCE=..\richedv.cpp
# End Source File
# Begin Source File

SOURCE=..\rolldial.cpp
# End Source File
# Begin Source File

SOURCE=..\scrollba.cpp
# End Source File
# Begin Source File

SOURCE=..\scroller.cpp
# End Source File
# Begin Source File

SOURCE=..\serialze.cpp
# End Source File
# Begin Source File

SOURCE=..\shellitm.cpp
# End Source File
# Begin Source File

SOURCE=..\shelwapi.cpp
# End Source File
# Begin Source File

SOURCE=..\slider.cpp
# End Source File
# Begin Source File

SOURCE=..\splashwi.cpp
# End Source File
# Begin Source File

SOURCE=..\static.cpp
# End Source File
# Begin Source File

SOURCE=..\statusba.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"owl/pch.h" /Yc""owl/pch.h""
# End Source File
# Begin Source File

SOURCE=..\swindow.cpp
# End Source File
# Begin Source File

SOURCE=..\system.cpp
# End Source File
# Begin Source File

SOURCE=..\tabbed.cpp
# End Source File
# Begin Source File

SOURCE=..\tabctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\textgadg.cpp
# End Source File
# Begin Source File

SOURCE=..\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\time.cpp
# End Source File
# Begin Source File

SOURCE=..\timegadg.cpp
# End Source File
# Begin Source File

SOURCE=..\timeio.cpp
# End Source File
# Begin Source File

SOURCE=..\timep.cpp
# End Source File
# Begin Source File

SOURCE=..\tinycapt.cpp
# End Source File
# Begin Source File

SOURCE=..\tmplinst.cpp
# End Source File
# Begin Source File

SOURCE=..\toolbox.cpp
# End Source File
# Begin Source File

SOURCE=..\tooltip.cpp
# End Source File
# Begin Source File

SOURCE=..\tracewnd.cpp
# End Source File
# Begin Source File

SOURCE=..\trayicon.cpp
# End Source File
# Begin Source File

SOURCE=..\treewind.cpp
# End Source File
# Begin Source File

SOURCE=..\uiborder.cpp
# End Source File
# Begin Source File

SOURCE=..\uiface.cpp
# End Source File
# Begin Source File

SOURCE=..\uihandle.cpp
# End Source File
# Begin Source File

SOURCE=..\uimetric.cpp
# End Source File
# Begin Source File

SOURCE=..\uipart.cpp
# End Source File
# Begin Source File

SOURCE=..\updown.cpp
# End Source File
# Begin Source File

SOURCE=..\urllink.cpp
# End Source File
# Begin Source File

SOURCE=..\ustring.cpp
# End Source File
# Begin Source File

SOURCE=..\validate.cpp
# End Source File
# Begin Source File

SOURCE=..\version.cpp
# End Source File
# Begin Source File

SOURCE=..\version.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\view.cpp
# End Source File
# Begin Source File

SOURCE=..\vslider.cpp
# End Source File
# Begin Source File

SOURCE=..\window.cpp
# End Source File
# Begin Source File

SOURCE=..\windowdc.cpp
# End Source File
# Begin Source File

SOURCE=..\wing.cpp
# End Source File
# Begin Source File

SOURCE=..\winmain.cpp
# End Source File
# Begin Source File

SOURCE=..\winsock.cpp
# End Source File
# Begin Source File

SOURCE=..\wskaddr.cpp
# End Source File
# Begin Source File

SOURCE=..\wskerr.cpp
# End Source File
# Begin Source File

SOURCE=..\wskhostm.cpp
# End Source File
# Begin Source File

SOURCE=..\wskservm.cpp
# End Source File
# Begin Source File

SOURCE=..\wsksock.cpp
# End Source File
# Begin Source File

SOURCE=..\wsksockd.cpp
# End Source File
# Begin Source File

SOURCE=..\wsksockm.cpp
# End Source File
# Begin Source File

SOURCE=..\wsyscls.cpp
# End Source File
# Begin Source File

SOURCE=..\wsysclsp.cpp
# End Source File
# Begin Source File

SOURCE=..\wsysclss.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=..\owl.rc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Asm Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\intel\dibblt32.obj
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
