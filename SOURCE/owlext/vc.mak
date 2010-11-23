#
# vc.mak - Makefile for building OWLExt with Microsoft Visual C++
#
# Usage: nmake -fvc.mak clean        (removes all intermediary files)
#    or: nmake -fvc.mak [options]    (builds one library variant (see below))
#
# Note that an NMAKE CLEAN should be performed before building a new variant.
# Options are one of each of:
#
#   "COMPVER=x"         (required)
#           This option indicates the version of Visual C++.
#           7=Visual C++ 2003, 8=Visual C++ 2005, 9=Visual C++ 2008
#
#   "DLL=1"             (defaults to 0)
#           If this item is 0, then a normal library is generated.
#           If this item is 1, a dynamic link library and a import library 
#           are generated.
#
#   "DEBUG=1"           (defaults to 0)
#           If this item is 1, debugging support is compiled into
#           the library.  If this item is 0, then debugging support
#           is disabled.  Debug support does not include CodeView information.
#
#   "CODEVIEW=1"        (defaults to 1 for DEBUG=1, 0 for DEBUG=0)
#           If this item is 1 CodeView information is compiled into
#           the library.  You must use the /DEBUG:FULL and /DEBUGTYPE:cv link
#           options when linking your executable. A value of 0 indicates that
#           no CodeView information is to be generated.
#
#   "MT=0"              (defaults to 1)
#           To build a non-multithreaded library instead of the default
#           (which enables multitheading and uses the multithread
#           C-runtimes) you can use MT=0.
#
#   "BROWSE=1"          (defaults to 0)
#           Set this option to build the browse database for the MFC
#           library.  By setting BROWSE=1, both the .SBRs and the .BSC
#           files will be built along with the .OBJ and .LIB files that
#           are part of the normal build process.
#
#   "BROWSEONLY=1"      (defaults to 0)
#           Set this option to build the browse files without re-building
#           the MFC library itself.  Note: This option is used internally
#           when BROWSE=1 is selected.
#
#   "INCREMENTAL=1"     (defaults to 0)
#           This option enables incremental/minimal compilation and
#           incremental linking.
#
#   "MBCS=1"            (defaults to 0)
#           To build an MBCS library instead of the default (SBCS)
#           you can use MBCS=1.  This creates a slightly larger
#           library, but the code will work in far-east markets.
#           This option has no effect when UNICODE=1.
#
#   "NO_PCH=1"
#           Set this item to override the default use of precompiled headers.
#
#   "NO_PDB=1"
#           Set this item to override the default use of PDB files.
#
#   "OPT="              (no default value)
#           This allows additional compiler options to be added to the build.
#           If more than one switch is desired, put double-quotes around the
#           whole OPT= argument, e.g., "OPT=/J /W3".
#
#   "OBJ=.\obj"         (default specific to variant)       
#           This optional specification specifies where temporary OBJ files
#           are stored during the build process.  The directory is created or
#           removed as necessary.
#
#   "PLATFORM=INTEL"    (defaults depends on host)
#           This option chooses the appropriate tools and sources for the
#           different platforms supporting the Win32 API. Currently INTEL,
#           MIPS, ALPHA, PPC, M68K, and MPPC are supported; more will
#           be added as they become available.  This option must be set for
#           Mac targets since they are built on an Intel host.
#


#-------------------------------------------------
# Set the path to the OWLNext root folder.

!ifndef OWLROOT
  OWLROOT = ..\..
!endif


#-------------------------------------------------
# Parse the OWLNext version number from sources.

!if [parse_version.bat]
!  error Unable to parse OWLNext version number.
!endif

!include $(TEMP)\owlver.tmp

!if "$(OWLVER)" == ""
!  error OWLVER is undefined (parsing failed).
!endif


#############################################################################
# Start of DLL specifics, Part 1 (former msowldll.mak)
#############################################################################

!if "$(DLL)" == "1"

!ifndef LIBNAME
LIBNAME=owlext
!endif

!ifdef TARGETDIR
OUTDIR = $(TARGETDIR)
!endif

!ifdef TARGETLIBDIR
LIBDIR = $(TARGETLIBDIR)
!endif

!ifndef OUTDIR
OUTDIR=$(OWLROOT)\bin
!endif

!ifndef LIBDIR
LIBDIR=$(OWLROOT)\lib
!endif

!ifndef REGEN
REGEN=0
!endif

!ifndef DEBUG
DEBUG=0
!endif

!ifndef BI_STD_RTL
BI_STD_RTL=1
!endif

!if "$(DEBUG)" != "0"
CRTDLL=$(CRTDLL) msvcrtd.lib 
!if "$(BI_STD_RTL)" == "1"
CRTDLL=$(CRTDLL) msvcprtd.lib
!else
CRTDLL=$(CRTDLL) msvcirtd.lib
!endif
!else
CRTDLL=$(CRTDLL) msvcrt.lib 
!if "$(BI_STD_RTL)" == "1"
CRTDLL=$(CRTDLL) msvcprt.lib
!else
CRTDLL=$(CRTDLL) msvcirt.lib
!endif
!endif

TARGET=w
RCFLAGS=$(RCFLAGS) /r /DWIN32
!if "$(DEBUG)" != "0"
RCFLAGS=$(RCFLAGS) /D_DEBUG
!endif

!if "$(UNICODE)" == "1"
TARGTYPE=u
RCDEFINES=$(RCDEFINES) /D_UNICODE
!else
!if "$(VERSION)" != "5"
LFLAGS=$(LFLAGS) /filealign:4096
!endif
!endif

!if "$(DEBUG)" != "0"
# Debug DLL build
TARGTYPE=$(TARGTYPE)d
RCDEFINES=$(RCDEFINES) /D_DEBUG
LFLAGS=$(LFLAGS)
!ifndef MONOLITHIC
MONOLITHIC=0
!endif
!else
# Release DLL build
RCDEFINES=$(RCDEFINES)
LFLAGS=$(LFLAGS)
!ifndef MONOLITHIC
MONOLITHIC=1
!endif
!endif

!if "$(MONOLITHIC)" == "1"
TARGDEFS=$(TARGDEFS) #/D_OWL_OLE_IMPL /D_OWL_DB_IMPL /D_OWL_NET_IMPL
RCDEFINES=$(RCDEFINES) /D_OWL_MONOLITHIC
!endif

!endif

#############################################################################
# End of DLL specifics, Part 1 (former msowldll.mak)
#############################################################################


!ifndef LIBDIR
LIBDIR=$(OWLROOT)\lib
!endif

!ifdef OWLEXTNAME
BASENAME = $(OWLEXTNAME)
!endif

!ifndef BASENAME
BASENAME = owlext
!endif

!if "$(DLL)" == "1"
DLL_OPT=/D_DLL /DBUILD_OWLEXTDLL /D_OWLDLL
!else
LIB_OPT=/DBUILD_OWLEXTLIB
!endif

CL_OPT=/D_OWLEXTPCH /D_OWLPCH
CPPFLAGS=/D_OWLEXTPCH /D_OWLPCH
PCH_HEADER="owlext/pch.h"


#############################################################################
# Start of former vc.inc
#############################################################################

#-------------------------------------------------
# Define defaults if not defined

!ifndef OUTDIR
OUTDIR=$(OWLROOT)\bin
!endif

# Default to Intel platform
!ifndef PLATFORM
PLATFORM=INTEL
!endif

# Default to DEBUG mode
!ifndef DEBUG
DEBUG=0
!endif

# Default to NOT DLL
!ifndef DLL
DLL=0
!endif

# Default Codeview Info
!ifndef CODEVIEW
!if "$(DEBUG)" == "1"
CODEVIEW=1
!else
CODEVIEW=0
!endif
!endif

!ifndef OWLSECTION
OWLSECTION=0
!endif

# BROWSEONLY is default 0 and implies BROWSE=1 if BROWSEONLY=1
!ifndef BROWSEONLY
BROWSEONLY=0
!endif

!if "$(BROWSEONLY)" != "0"
!undef BROWSE
BROWSE=1
!endif

# Default to no BROWSE info
!ifndef BROWSE
BROWSE=0
!endif


# Default to no INCREMENTAL build
!ifndef DEVBUILD
DEVBUILD=0
!endif

!if "$(DEBUG)" != "0"
!ifndef INCREMENTAL
INCREMENTAL=$(DEVBUILD)
!endif
!endif

!ifndef INCREMENTAL
INCREMENTAL=0
!endif

# Default to no MBCS build
!ifndef MBCS
MBCS=0
!endif

# Default to multithreading support
!ifndef MT
MT=1
!endif

!ifndef BI_STD_RTL
BI_STD_RTL=1
!endif

# Normalize cases of parameters, or error check
BASE=w


#-------------------------------------------------
# Parse options


# DEBUG OPTIONS
#
!if "$(DEBUG)" != "0"

DEBUGSUF=d
DEBDEFS=$(DEBDEFS) /D_DEBUG
DEBOPTS=/Od

!endif


# NON-DEBUG OPTIONS
#
!if "$(DEBUG)" == "0"

DEBUGSUF=
DEBDEFS=

!if "$(PLATFORM)" == "INTEL"
DEBOPTS=/O1 /Gy
!endif
!endif


# PLATFORM options
#

!if "$(PLATFORM)" == "INTEL"
CL_MODEL=/D_X86_
!endif

!if "$(CL_MODEL)" == ""
!error PLATFORM must be INTEL
!endif

#
# TYPE = Library Type Designator
#       c = normal C library
#       d = DLL library
TYPE=c
DEXT=


! if "$(MT)" == "1"
MODEL=t
! else
MODEL=s
! endif

!if "$(UNICODE)" == "1"
UC=u
! endif



#! if $d(USERDLL)
#    KEY = $(KEY)u                 # Static or import library for use by dlls
#! endif

!if "$(DEBUG)" != "0"
DBG=d
!endif

KEY = $(DBG)$(MODEL)$(UC)

DELIM = -

!ifndef COMPVER
! error COMPVER not defined. Can't build.
!endif

COMP=v$(COMPVER)

TARGETNAME = $(BASENAME)$(DELIM)$(OWLVER)$(DELIM)$(COMP)$(DELIM)$(KEY)
TEMPTARGETNAME = $(BASENAME)$(COMP)$(KEY)


#-------------------------------------------------
#
# Object File Directory
#
!if "$(OBJ)" == ""
D=$(TEMPTARGETNAME)                      # subdirectory specific to variant
!else
D=$(OBJ)                                 # User specified directory
!endif


#-------------------------------------------------
#
# DLL Variant
#
!if "$(DLL)" == "1"

TYPE=e
!if "$(OBJ)" == ""
D=$(TEMPTARGETNAME)i
!endif # if $(OBJ)
TARGOPTS=$(TARGOPTS) /MD /GF
!if "$(MT)" != "0"
TARGOPTS=$(TARGOPTS) /D_MT
!endif
DLL_OPT=$(DLL_OPT)  /D_DLL

!else   

!if "$(MT)" != "0"
!if "$(DEBUG)" != "0"
TARGOPTS=$(TARGOPTS) /MTd
!endif
!if "$(DEBUG)" == "0"
TARGOPTS=$(TARGOPTS) /MT
!endif
!endif

!endif # DLL

!ifdef OBJROOT
OBJDIR    = $(OBJROOT)
!endif

!if "$(OBJDIR)" == ""
OBJDIR=$(OWLROOT)\Obj
!endif

!if "$(OBJDIR)" != ""
D=$(OBJDIR)\$D
!endif
 


!if "$(UNICODE)" == "1"
TARGDEFS=$(TARGDEFS) /D_UNICODE
!else
!if "$(MBCS)" == "1"
TARGDEFS=$(TARGDEFS) /D_MBCS
!endif
!endif

GOAL=$(TARGETNAME)

#-------------------------------------------------
#
# CODEVIEW options
#
!if "$(CODEVIEW)" == "1"
!if "$(NO_PDB)" == "1"
CVOPTS=$(CVOPTS) /Z7
!if "$(PROFLIB)" != ""
!error Can't build for profiling without PDB files.
!endif
!else
CVOPTS= $(CVOPTS) /Zi
!if "$(PROFLIB)" != ""
CVOPTS=$(CVOPTS) /Gh
!endif
!if "$(DLL)" == "1"
PDBOPTS=/Fd$(OUTDIR)\$(GOAL).pdb
!else
PDBOPTS=/Fd$(LIBDIR)\$(GOAL).pdb
!endif
!endif
!endif



#-------------------------------------------------
#
# INCREMENTAL options
#
!if "$(INCREMENTAL)" == "1"
INCROPTS=/Gm
!else
INCROPTS=/Gm-
!endif


#-------------------------------------------------
# COMPILER OPTIONS
#
!if "$(PLATFORM)" == "INTEL"
CL_OPT= /W3 /Zl /EHsc /GR $(INCROPTS) $(DEBOPTS) $(CVOPTS) $(TARGOPTS) /I$(OWLROOT)\include /I.\ /DBI_NOINCLUDE_NS_OWL /D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NON_CONFORMING_SWPRINTFS /D_SCL_SECURE_NO_WARNINGS
!endif

!if "$(BI_STD_RTL)" == "1"
#CL_OPT=$(CL_OPT) /Za
!endif

!if "$(BROWSE)" != "0"
CL_OPT=/FR$D\ $(CL_OPT)
!endif

!if "$(BROWSEONLY)" != "0"
CL_OPT=/Zs $(CL_OPT)
!else
CL_OPT=/Fo$D\ $(CL_OPT)
!endif

DEFS=$(DEFS) $(DEBDEFS) $(TARGDEFS)

#-------------------------------------------------
# Standard tools
!if "$(USE_BC)" == "1"
MSCL=d:\msdev\bchecker\nmcl.exe
MSLINK=d:\msdev\bchecker\nmlink.exe
MSRC=rc
MSLIB=lib
MSML=ml
CRTDLL =$(CRTDLL) d:\msdev\bchecker\BCINTERF.LIB  
!else
MSCL=cl
MSLINK=link
MSRC=rc
MSLIB=lib
MSML=ml
!endif

#-------------------------------------------------
# Set CPPFLAGS for use with .cpp.obj and .c.obj rules
# Define rule for use with OBJ directory
# C++ uses a PCH file

CPPFLAGS=$(CPPFLAGS) $(CL_MODEL) $(CL_OPT) $(PDBOPTS) $(DEFS) $(OPT) $(DLL_OPT) $(LIB_OPT)

!ifndef PCH_FILE_NAME
PCH_FILE_NAME=owlextpch
!endif

!ifndef NO_PCH
!ifndef PCH_FILE
PCH_FILE=$D\$(PCH_FILE_NAME)
!if "$(BROWSE)" != "0"
PCH_FILE=$(PCH_FILE)b
!endif    # "$(BROWSE)" != "0"
PCH_FILE=$(PCH_FILE).pch
!endif    #PCH_FILE
!ifndef PCH_CPP
PCH_CPP=owlextpch
!endif  # NO_PCH

NOHDRFLAGS=$(CL_MODEL) $(CL_OPT) $(PDBOPTS) $(DEFS) $(OPT) $(DLL_OPT) $(LIB_OPT) /DNOHDR
CPPFLAGS=$(CPPFLAGS) /Yu$(PCH_HEADER) /Fp$(PCH_FILE)
!else
PCH_FILE=
!endif

.SUFFIXES:: .cpp

!if "$(BROWSE)" != "0"
.cpp{$D}.obj:
  $(MSCL) @<<
$(CPPFLAGS) /c $<
<<
  copy /b $*.sbr+pchmark.bin $*.sbr >NUL
!else
.cpp{$D}.obj::
  $(MSCL) @<<
$(CPPFLAGS) /c $<
<<
!endif

.cpp{$D}.sbr:
  $(MSCL) @<<
$(CPPFLAGS) /c $<
<<
  copy /b $*.sbr+pchmark.bin $*.sbr >NUL


#############################################################################
# End of former vc.inc
#############################################################################


#-------------------------------------------------
# Library Components

OBJECT=\
$D\owlextpch.obj\
$D\bmpmenu.obj\
$D\browse.obj\
$D\busy.obj\
$D\butappdlg.obj\
$D\calcedit.obj\
$D\caledit.obj\
$D\colorhls.obj\
$D\colpick.obj\
$D\comboexp.obj\
$D\ctxhelpm.obj\
$D\dirdialg.obj\
$D\dlgtoolb.obj\
$D\dockingex.obj\
$D\editlist.obj\
$D\enumwnd.obj\
$D\gadgctrl.obj\
$D\gadgdesc.obj\
$D\gadgetex.obj\
$D\gaugegad.obj\
$D\harborex.obj\
$D\hlinkctl.obj\
$D\keyhook.obj\
$D\menubtn.obj\
$D\mousehk.obj\
$D\msghook.obj\
$D\multiwin.obj\
$D\notebook.obj\
$D\owlext.obj\
$D\pickedit.obj\
$D\pictdeco.obj\
$D\pictdlg.obj\
$D\popedit.obj\
$D\progress.obj\
$D\simplapp.obj\
$D\staticbm.obj\
$D\tabwin.obj\
$D\thredprg.obj\
$D\timer.obj\
$D\tipdialg.obj\
$D\validate.obj\
$D\toolcustdlg.obj

# without precompiled header
NO_HDR_OBJ=\
$D\version.obj

NO_HDR_CPP=\
version.obj


CPP_OBJS= $(OBJECT) $(NO_HDR_OBJ) $(OBJSECTION) $(OBJS_DLL) 


OBJS=$(CPP_OBJS)  

!if "$(DLL)" != "1"
OBJS=$(OBJS) $(STATICLINK_OBJS)
!endif

#-------------------------------------------------
# Goals to build

GOALS=create.dir
!if "$(BROWSEONLY)" == "0"
!if "$(DLL)" != "1"
GOALS=$(GOALS) $(LIBDIR)\$(GOAL).lib
!else
GOALS=$(GOALS) $(LIBDIR)\$(GOAL)i.lib
!endif
!endif
!if "$(BROWSE)" != "0"
GOALS=$(GOALS) $(GOAL).bsc
!endif

goal: $(GOALS)

create.dir:
	@-if not exist $D\*.* mkdir $D

clean:
	-if exist $D\*.obj erase $D\*.obj
	-if exist $D\*.pch erase $D\*.pch
	-if exist $D\*.res erase $D\*.res
	-if exist $D\*.rsc erase $D\*.rsc
	-if exist $D\*.map erase $D\*.map
	-if exist $D\*.exp erase $D\*.exp
	-if exist $D\*.lib erase $D\*.lib
	-if not exist $D\*.* rmdir $D
	-if exist $(LIBDIR)\$(GOAL).pdb del $(LIBDIR)\$(GOAL).pdb
	-if exist $(LIBDIR)\$(GOAL).idb del $(LIBDIR)\$(GOAL).idb
	-if exist $(LIBDIR)\$(GOAL).rep del $(LIBDIR)\$(GOAL).rep
	-if exist $(LIBDIR)\$(GOAL)i.pdb del $(LIBDIR)\$(GOAL)i.pdb
	-if exist $(LIBDIR)\$(GOAL)i.idb del $(LIBDIR)\$(GOAL)i.idb
	-if exist $(LIBDIR)\$(GOAL)i.rep del $(LIBDIR)\$(GOAL)i.rep
	-if exist $(OUTDIR)\$(GOAL).pdb del $(OUTDIR)\$(GOAL).pdb
	-if exist $(OUTDIR)\$(GOAL).idb del $(OUTDIR)\$(GOAL).idb
	-if exist $(OUTDIR)\$(GOAL).rep del $(OUTDIR)\$(GOAL).rep

#-------------------------------------------------
# Precompiled header file

!ifndef NO_PCH

!if "$(DEBUG)" == "1"
HDRS =$(OWLROOT)\include\owl\*.h
!else
HDRS =$(OWLROOT)\include\owl\*.h
!endif

PCH_TARGETS=$D\$(PCH_CPP).obj $(PCH_FILE)
!if "$(BROWSEONLY)" != "0"
PCH_TARGETS=$(PCH_TARGETS) $D\$(PCH_CPP).sbr
!endif

$(PCH_TARGETS):: msc\$(PCH_CPP).cpp $(HDRS)
	$(MSCL) @<<
/Yc /Fp$(PCH_FILE) $(CL_MODEL) $(CL_OPT) $(DLL_OPT) $(LIB_OPT) $(PDBOPTS) $(DEFS) $(OPT) /D_OWLPCH /D_OWLEXTPCH /c msc\$(PCH_CPP).cpp
<<
!if "$(BROWSE)" != "0"
	copy /b $D\$(PCH_CPP).sbr+pchmark.bin $D\$(PCH_CPP).sbr>NUL
!endif

!if "$(BROWSEONLY)" == "1"
$D\$(PCH_CPP).sbr:: msc\$(PCH_CPP).cpp $(PCH_FILE)
!endif

!endif # NO_PCH

#-------------------------------------------------

$(NO_HDR_OBJ): $(NO_HDR_CPP)
	$(MSCL) @<<
$(NOHDRFLAGS) /c $(NO_HDR_CPP)
<<
!if "$(BROWSE)" != "0"
	copy /b $*.sbr+pchmark.bin $*.sbr >NUL
!endif

#-------------------------------------------------
# Build the library from the up-to-date objs

SBRS=$(CPP_OBJS:.obj=.sbr)

!if "$(BROWSEONLY)" != "0"

# Build final browse database
$(GOAL).bsc: $(PCH_TARGETS) $(SBRS)
	bscmake /n /Iu /El /o$@ @<<
$(SBRS)
<<

!else #BROWSEONLY

!if "$(DLL)" != "1"

# Build final library
$(LIBDIR)\$(GOAL).lib: $(PCH_TARGETS) $(OBJS) $(NO_HDR_OBJ) $(STATICLINK_OBJS)
	@-if exist $@ erase $@
	@$(MSLIB) /out:$@ @<<
$(OBJS)
<<

# Recurse to build browse database
$(GOAL).bsc: $(PCH_TARGETS) $(SBRS)
	$(MAKE) /f bc.mak @<<
BROWSEONLY=1 PLATFORM=$(PLATFORM) DEBUG=$(DEBUG) CODEVIEW=$(CODEVIEW) \
DLL=$(DLL) NO_PCH=$(NO_PCH) OBJ=$(OBJ) OPT=$(OPT)
<<
!endif # DLL

!endif #!BROWSEONLY


#############################################################################
# Start of DLL specifics, Part 2 (former msowldll.mak)
#############################################################################

!if "$(DLL)" == "1"

# OPT:noref keeps unreferenced functions (ie. no dead-code elimination)
!if "$(REGEN)" == "0"
#LFLAGS=$(LFLAGS) /opt:ref
LFLAGS=$(LFLAGS) /opt:ref /opt:icf,32
!else
LFLAGS=$(LFLAGS) /opt:noref
!endif

DEFFILE=$(PLATFORM)\$(LIBNAME).def

!if "$(DEBUGTYPE)" == ""
#DEBUGTYPE=cv,fixup
DEBUGTYPE=cv
!endif

!if "$(CODEVIEW)" != "0"
!if "$(REGEN)" != "1"
LFLAGS=$(LFLAGS) /debug /debugtype:$(DEBUGTYPE)
!else
LFLAGS=$(LFLAGS)
!endif
!if "$(NO_PDB)" != "1" && "$(REGEN)" != "1"
LFLAGS=$(LFLAGS) /pdb:$(OUTDIR)\$(GOAL).pdb
!else
LFLAGS=$(LFLAGS) /pdb:none
!endif
!else
LFLAGS=$(LFLAGS)
!if "$(INCREMENTAL)" != "1"
LFLAGS=$(LFLAGS) /incremental:no
!endif
!endif

!ifdef RELEASE # Release VERSION info
RCDEFINES=$(RCDEFINES) /DRELEASE
LFLAGS=$(LFLAGS) /release
!endif

LFLAGS=$(LFLAGS) /dll

!if "$(ORDER)" == "1"
!if exist($(PLATFORM)\$(GOAL).prf)
DEFS=$(DEFS) /D_OWL_FUNCTION_ORDER
LFLAGS=$(LFLAGS) /order:@$(PLATFORM)\$(GOAL).prf
!endif
!endif

!if "$(OBJ)" == ""
TMP_LIB=$(D)\_$(LIBNAME).lib
!else
TMP_LIB=$(OBJ)\_$(LIBNAME).lib
!endif



dll_goal: create2.dir \
	$(OUTDIR)\$(GOAL).dll $(LIBDIR)\$(GOAL)i.lib


create2.dir:
	@-if not exist $D\*.* mkdir $D

#-------------------------------------------------
# more flags and switches

LFLAGS=$(LFLAGS) /version:6.21
!if "$(UNICODE)" == "1"
LFLAGS=$(LFLAGS) /base:0x4F800000
!else
LFLAGS=$(LFLAGS) /base:0x4F400000
!endif
LIBS=$(LIBS) $(CRTDLL) kernel32.lib gdi32.lib user32.lib uuid.lib \
shell32.lib version.lib advapi32.lib \
ole32.lib oleaut32.lib $(PROFLIB)

#-------------------------------------------------
STATICLINK_OBJS=\
$(STATICLINK_OBJS)\
$D\owlextpchs.obj


CPPFLAGS_STATIC=\
	$(CL_MODEL) $(CL_OPT) $(DEFS) $(DEBDEF) /I$(OWLROOT)\include /DBUILD_OWLEXTDLL /D_OWLEXTDLL /D_OWLEXTPCH /D_OWLDLL /D_OWLPCH $(OPT) $(EH) /Gy /Fd$(LIBDIR)\$(GOAL)i.pdb

!if "$(DEBUG)" == "1"
CPPFLAGS_STATIC=$(CPPFLAGS_STATIC) /D_DEBUG
!endif

!if "$(UNICODE)" == "1"
CPPFLAGS_STATIC=$(CPPFLAGS_STATIC) /D_UNICODE
!endif

PCH_TARGET=$D\owlextpchs.obj

$D\owlextpchs.obj $D\owlextpchs.pch: msc\owlextpchs.cpp 
	$(MSCL) @<<
/c $(CPPFLAGS_STATIC) /Yc /Fp$D\owlextpchs.pch msc\owlextpchs.cpp
<<

#-------------------------------------------------
# Build target

$D\$(LIBNAME).res: owlext.rc
	$(MSRC) $(RCFLAGS) $(RCDEFINES) /i$(OWLROOT)\include;$(OWLROOT)\include\owl;$(OWLROOT)\include\owlext /fo $D\$(LIBNAME).res owlext.rc

DLL_OBJS=$(OBJECT) $(OBJSECTION) $(OBJS_DLL) $(OWLASM) $(NO_HDR_OBJ)

!if "$(MONOLITHIC)" == "1"
DLL_OBJS=$(DLL_OBJS) $(SOCKETS) $(OLEDLL) $(DAO) $(DB)
!endif

DLL_OBJS=$(DLL_OBJS)

DLL_RESOURCES=$D\$(LIBNAME).res


$(OUTDIR)\$(GOAL).dll $(TMP_LIB): $(DLL_OBJS) $(DEFFILE) $(DLL_RESOURCES)
	$(MSLINK) @<<
$(LFLAGS) /LIBPATH:$(LIBDIR)
$(LIBS)
$(DLL_OBJS)
$(DLL_RESOURCES)
/def:$(DEFFILE)
/out:$(OUTDIR)\$(GOAL).dll
/pdb:$(OUTDIR)\$(GOAL).pdb
/map:$D\$(LIBNAME).map
/implib:$(TMP_LIB)
!if "$(VERSION)" != "5"
/ignore:4037
/ignore:4065
/ignore:4199
/filealign:4096
!endif
<<

$(LIBDIR)\$(GOAL)i.lib: $(STATICLINK_OBJS) $(TMP_LIB)
	$(MSLIB) /out:$@ $(STATICLINK_OBJS) $(TMP_LIB)

!endif

#############################################################################
# End of DLL specifics, Part 2 (former msowldll.mak)
#############################################################################
