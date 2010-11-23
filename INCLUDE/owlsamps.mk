# Common include for building OWL Sample programs
#
#  typical usage
#       PROJ=foo
#       OBJS=foo.obj bar.obj ...
#       !INCLUDE ..\..\SAMPLE_.MAK
#
#  ROOT specifies the location of the msdev\samples\mfc directory,
#  relative to the project directory. Because the MFC tutorial samples
#  have an intermediate STEP<n> subdirectory, they use
#       ROOT=..\..\..
#  instead of the default
#       ROOT=..\..
#
# NOTE: do not include 'stdafx.obj' in the OBJS list - the correctly
#    built version will be included for you
#
# Options to NMAKE:
#     "PLATFORM=?"
#       This option chooses the appropriate tools and sources for the
#       different platforms support by Windows/NT.  Currently INTEL,
#       MIPS, ALPHA, PPC are supported. The default is chosen based on
#       the host environment.
#     "DEBUG=0"     use release (default debug)
#     "CODEVIEW=1"  include codeview info (even for release builds)
#         "_OWLDLL=1"    to use shared DLL version of OWL
#         "USRDLL=1"    to build a DLL that uses static OWL
#     "UNICODE=1"   to build UNICODE enabled applications
#                   (not all samples support UNICODE)
#     "NO_PCH=1"    do not use precompiled headers (defaults to use pch)
#     "COFF=1"      include COFF symbols

!ifndef PROJ
!ERROR You forgot to define 'PROJ' symbol!!
!endif


ROOT=.
!ifndef ROOT
!endif

!ifndef OBJS
!ERROR You forgot to define 'OBJS' symbol!!
!endif

!ifndef DEBUG
DEBUG=1
!endif

!ifndef OWLDLL
OWLDLL=0
!endif

!ifndef UNICODE
UNICODE=0
!endif

!ifndef USRDLL
USRDLL=0
!endif

!if "$(USRDLL)" != "0"
OWLDLL=0
!endif

!ifndef PLATFORM
!ifndef PROCESSOR_ARCHITECTURE
PROCESSOR_ARCHITECTURE=x86
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "x86"
PLATFORM=INTEL
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "ALPHA"
PLATFORM=ALPHA
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "MIPS"
PLATFORM=MIPS
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "PPC"
PLATFORM=PPC
!endif
!endif

!ifndef USES_OLE
USES_OLE=0
!endif

!ifndef USES_DB
USES_DB=0
!endif

!ifndef CONSOLE
CONSOLE=0
!endif

!ifndef MBCS
MBCS=1
!endif

!ifndef NO_PCH
NO_PCH=0
!endif

OWLDEFS=$(OWLDEFS) /D_OWLPCH

BASE=W

!if "$(UNICODE)" == "0"
!if "$(OWLDLL)" == "0"
!if "$(USRDLL)" != "1"
OWLPCH=owlpch
!else
OWLPCH=stdusr
!endif
!else
OWLPCH=stddll
!endif
!endif

!if "$(UNICODE)" == "1"
!if "$(OWLDLL)" == "0"
!if "$(USRDLL)" != "1"
OWLPCH=uniowl
!else
OWLPCH=uniusr
!endif
!else
OWLPCH=unidll
!endif
!endif

!if "$(DEBUG)" == "1"
OWLPCH=$(OWLPCH)d
!if "$(COFF)" != "1"
!ifndef CODEVIEW
CODEVIEW=1
!endif
!endif
!endif

!if "$(CODEVIEW)" == "1"
OWLPCH=$(OWLPCH)v
!endif

!if "$(DEBUG)" == "1"
DEBUG_SUFFIX=d
!endif

!if "$(DEBUG)" != "0"
DEBUGFLAGS=/Od
OWLDEFS=$(OWLDEFS) /D_DEBUG 

!endif

!if "$(DEBUG)" == "0"
!if "$(PLATFORM)" == "INTEL"
DEBUGFLAGS=/O1 /Gy
!endif
!if "$(PLATFORM)" == "MIPS"
DEBUGFLAGS=/O1 /Gy
!endif
!if "$(PLATFORM)" == "ALPHA"
DEBUGFLAGS=/O1 /Gy
!endif
!if "$(PLATFORM)" == "PPC"
DEBUGFLAGS=/O1 /Gy
!endif
!endif # DEBUG == 0

!if "$(CODEVIEW)" == "1" || "$(COFF)" == "1"
DEBUGFLAGS=$(DEBUGFLAGS) /Z7
!endif

!if "$(UNICODE)" == "1"
DLL_SUFFIX=u
!endif

!if "$(OWLDLL)" == "1"
OWLFLAGS=$(OWLFLAGS) /MD$(DEBUG_SUFFIX)
OWLDEFS=$(OWLDEFS) /D_OWLDLL
!endif # OWLDLL == 1

!if "$(USRDLL)" == "1"
OWLDEFS=$(OWLDEFS) /D_OWLDLL /D_WINDLL
!endif

!if "$(OWLDLL)" == "0"
!if "$(MD)" == "1"
OWLFLAGS=$(OWLFLAGS) /MD$(DEBUG_SUFFIX)
!elseif "$(MT)" == "0"
OWLFLAGS=$(OWLFLAGS) /ML$(DEBUG_SUFFIX)
!else
OWLFLAGS=$(OWLFLAGS) /MT$(DEBUG_SUFFIX)
!endif
!endif

!if "$(UNICODE)" == "1"
OWLDEFS=$(OWLDEFS) /D_UNICODE
!else
!if "$(MBCS)" == "1"
OWLDEFS=$(OWLDEFS) /D_MBCS
!endif
!endif

!if "$(PLATFORM)" == "INTEL"
OWLDEFS=$(OWLDEFS) /D_X86_
CPP=cl
CFLAGS=/GR /GX /c /W3 $(DEBUGFLAGS) $(OWLFLAGS) $(OWLDEFS)
!endif


CPPMAIN_FLAGS=$(CFLAGS)

!if "$(NO_PCH)" == "1"
CPPFLAGS=$(CPPMAIN_FLAGS)
!else
PCHDIR=.
CPPFLAGS=$(CPPMAIN_FLAGS) /Yuowlpch.h /Fp$(PCHDIR)\$(OWLPCH).pch
!endif

!if "$(COFF)" == "1"
NO_PDB=1
!if "$(CODEVIEW)" != "1"
LINKDEBUG=/incremental:no /debug:full /debugtype:coff
!else
LINKDEBUG=/incremental:no /debug:full /debugtype:both
!endif
!endif

!if "$(COFF)" != "1"
!if "$(CODEVIEW)" == "1"
LINKDEBUG=/incremental:no /debug:full /debugtype:cv
!else
LINKDEBUG=/incremental:no /debug:none
!endif
!endif

!if "$(NO_PDB)" == "1"
LINKDEBUG=$(LINKDEBUG) /pdb:none
!endif

!if "$(PLATFORM)" == "INTEL"
LINKCMD=link $(LINKDEBUG)
!endif

# link flags - must be specified after $(LINKCMD)
#
# conflags : creating a character based console application
# guiflags : creating a GUI based "Windows" application

CONFLAGS=/subsystem:console
GUIFLAGS=/subsystem:windows

!if "$(UNICODE)" == "1"
CONFLAGS=$(CONFLAGS) /entry:wmainCRTStartup
GUIFLAGS=$(GUIFLAGS) /entry:wWinMainCRTStartup
!endif

!ifndef PROJRESFILE
PROJRESFILE=$(PROJ).res
!endif

!ifndef RESFILE
RESFILE=$(PROJRESFILE)
!endif

.SUFFIXES:
.SUFFIXES: .c .cpp .rcm .rc

.cpp.obj:
	$(CPP) @<<
$(CPPFLAGS) $*.cpp
<<

.c.obj:
	$(CPP) @<<
$(CFLAGS) $(CVARS) $*.c
<<

.rc.res:
	rc /r /i.\ /i$(OWLROOT)\include /i$(OWLROOT)\include\owl $(OWLDEFS) $<

#############################################################################

!if "$(NO_PCH)" == "0"
LINK_OBJS=$(OBJS) $(PCHDIR)\$(OWLPCH).obj
!else
LINK_OBJS=$(OBJS)
!endif

#
# Build CONSOLE Win32 application
#
!if "$(CONSOLE)" == "1"

$(PROJ).exe: $(LINK_OBJS)
	$(LINKCMD) @<<
$(CONFLAGS) /out:$(PROJ).exe /map:$(PROJ).map
$(LINK_OBJS) $(EXTRA_LIBS)
<<

!endif  # CONSOLE=1

#
# Build Win32 application
#
!if "$(CONSOLE)" == "0"

EXTRA_LIBS=gdi32.lib user32.lib kernel32.lib shell32.lib advapi32.lib $(EXTRA_LIBS) 

!if "$(USRDLL)" == "1"
$(PROJ).dll: $(LINK_OBJS) $(PROJRESFILE)
	$(LINKCMD) @<<
$(GUIFLAGS) /out:$(PROJ).dll /map:$(PROJ).map
/dll /def:$(PROJ).def
$(LINK_OBJS) $(RESFILE) $(EXTRA_LIBS)
<<

$(PROJ).res:  resource.h
$(PROJ).rsc:  resource.h
!endif

!if "$(SIMPLE_APP)" != "1"
$(PROJ).exe: $(LINK_OBJS) $(PROJRESFILE)
	$(LINKCMD) @<<
$(GUIFLAGS) /out:$(PROJ).exe /map:$(PROJ).map
$(LINK_OBJS) $(RESFILE) $(EXTRA_LIBS)
<<

$(PROJ).res:  resource.h
$(PROJ).rsc:  resource.h
!endif

!if "$(SIMPLE_APP)" == "1"

$(PROJ).exe: $(LINK_OBJS)
	$(LINKCMD) @<<
$(GUIFLAGS) /out:$(PROJ).exe /map:$(PROJ).map
$(LINK_OBJS) $(EXTRA_LIBS)
<<

!endif

!if "$(NO_PCH)" == "0"
$(PCHDIR)\$(OWLPCH).obj $(PCHDIR)\$(OWLPCH).pch: owlpch.h owlpch.cpp
	echo "BUILDING SHARED PCH and PCT files"
	$(CPP) @<<
$(CPPMAIN_FLAGS) /Ycowlpch.h /Fp$(PCHDIR)\$(OWLPCH).pch /Fo$(PCHDIR)\$(OWLPCH).obj /c $(ROOT)\owlpch.cpp
<<

$(OBJS): $(PCHDIR)\$(OWLPCH).pch
!endif

!endif  # CONSOLE=0

clean::
	if exist $(PROJ).exe erase $(PROJ).exe
	if exist *.aps erase *.aps
	if exist *.pch erase *.pch
	if exist *.map erase *.map
	if exist *.obj erase *.obj
	if exist *.exp erase *.exp
	if exist *.pdb erase *.pdb
	if exist *.map erase *.map
	if exist *.lib erase *.lib
	if exist *.res erase *.res
	if exist *.rsc erase *.rsc
	if exist *.pef erase *.pef

#############################################################################
