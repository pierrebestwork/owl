#-----------------------------------------------------------------------------
# gc.mak - makefile for building OWLNext
# Created by Kenneth Haley (khaley@bigfoot.com) 1999
#
#$Revision: 1.1.2.1 $
#$Author: jogybl $
#$Date: 2010-03-24 11:54:28 $
#
# OWL specific defines (parameters):
#
#   COMPVER              compiler version (currently not used)
#   OWLVER               OWLNext version number (optional)
#
#
#   NO_EMULATED_CTRL     Builds library using only system implementation of
#                        common controls
#   NO_OWL1_COMPAT       Builds library without API exposed for backward
#                        compatibility with v1.0 of ObjectWindows.
#   NO_OWL2_COMPAT       Builds library without API exposed for backward
#                        compatibility with v2.x of ObjectWindows.
#                        NOTE: NO_OWL2_COMPAT implies NO_OWL1_COMPAT
#   NO_OWL5_COMPAT       Builds library without API exposed for backward
#                        compatibility with v5.x of ObjectWindows.
#                        NOTE: NO_OWL5_COMPAT implies NO_OWL2_COMPAT and
#                        NO_OWL1_COMPAT
#   NO_OBJ_STREAMING     Builds library without special code to allow
#                        some OWL objects to be persisted in a stream
#
# Defines (parameters):
#   MT                   Build multi-thread version (Win32 only)
#   UNICODE              Build unicode version (Win32 only)
#   DLL                  Build a DLL. (no default)
#   NAMESPACE    	       Builds the library within a namespace
#                         (ClassLib, OWL, OCF, OwlExt)
#   DIAGS                Build the diagnostic version of the target.
#   DEBUG                Build the debug version of the target.
#   ALIGN=<1|2|4|8>      Specifies the alignment
#   BI_DBCS_SUPPORT      builds library with DBCS support
#   BI_MSG_JAPAN         builds library with japaneze support
#
# The name of the targetfile (and the directory for compiler output)
# is constructed using the following format:
#   TARGETSFX = $(VERSION)$(COMP)$(DBG)$(MODEL)$(UC)
#   TARGETNAME = $(BASENAME)$(TARGETSFX)
#
# Where:
#   BASENAME             Base name of the target (bidp, owlp, ocfp, owlx)
#   VERSION              Version number (default=630)
#   MODEL=<f|t>		 Model/target platform
#                        f=32-bit static/dynamic library
#			 t=32-bit static/dynamic multhi threaded library
#   UC=<u|>              u=Unicode                      (Win32 only)
#                        <nothing>=ANSI
#   DBG=<d|>             d=Debug/diagnostic build
#                        <nothing>=Release build
#   COMP=<,v,g>          Borland=none, MS VC++ = v, GNU = g
#-----------------------------------------------------------------------------

ifdef OWLNAME
  BASENAME = $(OWLNAME)
endif

ifndef BASENAME
  BASENAME = owl
endif

ifndef OWLVER
  VERSION  = 630
else
  VERSION  = OWLVER
endif

ifndef COMPNAME
#currentyl unused as 2010-02
#  COMPNAME = g
endif


#-----------------------------------------------------------------------------
# Tools
#-----------------------------------------------------------------------------

# This can prodece 'CreateProcess fail' in Windows, don't bother.
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  PLATFORM=Linux
  GNUROOT=
  SDKDIR=/usr/include/wine/windows

else ifeq ($(UNAME), Solaris)
  PLATFORM=Solaris
  GNUROOT=/usr/bin/
  SDKDIR=/usr/include/wine/

else ifeq ($(UNAME), Darwin)
  PLATFORM=Osx
  GNUROOT=/usr/bin/
  SDKDIR=/usr/include/wine/

else ifeq ($(UNAME), HP-UX)
  PLATFORM=HP-UX
  GNUROOT=/usr/bin/
  SDKDIR=/usr/include/wine

else
  PLATFORM=Windows
  GNUROOT=c:\mingw\bin\
  SDKDIR=c:\mingw\include

#reserved for possible future use
#  paths := $(shell echo %path%)
#  paths := $(subst ;, ,$(paths))
#  binpath := $(strip $(foreach path,$(paths),$(if $(wildcard $(path)/gcc.exe),$(path))))
#  binpath := $(subst \,/,$(binpath))
#  binpath := $(patsubst %/bin,%,$(binpath))
#  libpath := $(binpath)/lib

endif


ifeq ($(PLATFORM), Windows)
  # Replace slash by backslash
  #SLASH := $(subst /,\,$(SLASH)) It won't work, above is used the expresion $(SLASH)% wich will result in \%
  SLASH=\\

  # mingw32 settings
 CFLAGS+= -mno-cygwin
else
  SLASH=/
  CFLAGS += -DWINELIB
endif


RC=rc.exe
WINDRES=$(GNUROOT)windres.exe
GCC=$(GNUROOT)gcc
LD=$(GNUROOT)ld
AR=$(GNUROOT)ar
ASM=$(GNUROOT)as
CPP=$(GNUROOT)c++
STRIP=$(GNUROOT)strip


#------------------------------------------------------------------------------


#Greg Chicare report problems with vtable-thunks, remove it
#CFLAGS = -fvtable-thunks

# current additional options
#NO_OBJ_STREAMING = 1  # ??? can we enable this ?????
NO_OWL5_COMPAT    = 1  # this flag eliminates all drawbacks and old wrong functions,
                       # still highly compatible with old codebase
NAMESPACE         = 1
NO_WIN32S_SUPPORT = 1

# Compile as DLL
DLL               = 1

#DEBUG            = 1

CFLAGS+= -DBI_NOINCLUDE_NS_OWL -D_WIN32_IE=0x0501 -DBI_SUPPRESS_OLE -DOWL_NATIVECTRL_ALWAYS
ifdef DEBUG
CFLAGS+= -D_DEBUG	-D__DEBUG=2 -D_TRACE=1 -D_WARN=1
endif

#location of win32 headers
RCW32INC = $(SDKDIR)
LINKLIBDIR = -L /usr/lib/wine -L /d/gcc/lib/gcc-lib/mingw32/3.1
LFLAGS = -shared --subsystem windows --verbose

#mingw32 settings
#CFLAGS+= -mno-cygwin -I /d/gcc/include
LINKLIBDIR += -L /d/gcc/lib

CFLAGS+= -Wall

#CDEFINES =

DEFFILE  = owlp.def
RESFILE  = owl.res
WINLIBS = -lgdi32 -luser32 -lkernel32 -ladvapi32 -lshell32
RTLLIBS = -lmingw32 -lgcc -lmoldname -lmingwex -lmsvcrt -lstdc++
LINKLIBS = $(RTLLIBS) $(WINLIBS) $(RTLLIBS) $(WINLIBS)

ifdef DLL
DLLCDEFS       = -D_BUILDOWLDLL
LIBDLL_DEFS    = -D_OWLDLL
LIBDLL_UNDEFS  = -U_BUILDOWLDLL
endif

ifndef CXX
CXX = c++
endif


#-----------------------------------------------------------------------------
# Various OWL settings
#-----------------------------------------------------------------------------

#
# Emulate Common Ctrls or not
#
ifdef NO_EMULATED_CTRL
  CDEFINES += -DOWL_NATIVECTRL_ALWAYS
endif

#
# Build OWL5 compatible version?
#
ifdef NO_OWL5_COMPAT
 ifndef NO_OWL2_COMPAT            # No OWL5 Compat => No OWL2 Compat
   NO_OWL2_COMPAT = 1
 endif
 CDEFINES += -DNO_OWL5_COMPAT
endif

#
# Build OWL2 compatible version?
#
ifdef NO_OWL2_COMPAT
 ifndef NO_OWL1_COMPAT            # No OWL2 Compat => No OWL1 Compat
   NO_OWL1_COMPAT = 1
 endif
 CDEFINES += -DNO_OWL2_COMPAT
endif

#
# Build OWL1 compatible version?
#
ifdef NO_OWL1_COMPAT
  CDEFINES += -DNO_OWL1_COMPAT
endif

#
# Build without persistent/stream support?
#
ifdef NO_OBJ_STREAMING
  CDEFINES += -DBI_NO_OBJ_STREAMING
endif

#-----------------------------------------------------------------------------
# The make target
#-----------------------------------------------------------------------------
Make: Vars Setup All

Vars:
	@echo OBJROOT $(OBJROOT)
	@echo OBJDIR $(OBJDIR)
	@echo VERSION $(VERSION)
	@echo MODEL $(MODEL)
	@echo KEY $(KEY)
	@echo COMP $(COMP)
	@echo FULLTARGET $(FULLTARGET)
	@echo LIBTARGET $(LIBTARGET)
	@echo LIBTARGET_CONCOL $(LIBTARGET_CONCOL)
	@echo RESFILE $(RESFILE)
	@echo DEFFILE $(DEFFILE)
	@echo ALIGN $(ALIGN)
	@echo CFLAGS $(CFLAGS)
	@echo CDEFINES $(CDEFINES)

	@echo PLATFORM $(PLATFORM)


#-----------------------------------------------------------------------------
# Check parameters
#-----------------------------------------------------------------------------

ifndef WIN64
  WIN32 = 1
endif

#--------------------------------------------------------------------
# Options which are memory model independent
#--------------------------------------------------------------------
#ifdef NAMESPACE
#  CDEFINES += -DBI_NAMESPACE
#else
#  CDEFINES += -DBI_NO_NAMESPACE
#endif
ifndef NAMESPACE
  CDEFINES += -DBI_NO_NAMESPACE
endif

ifdef BI_STD_STRING
  CDEFINES += -DBI_STD_STRING
endif

ifdef UNICODE
  CDEFINES += -DUNICODE
endif

ifdef DEBUG
  __DEBUG=2
endif
ifdef DIAGS         # default diags use precondition, check, trace & warn
  __DEBUG=2
  __TRACE=1
  __WARN=1
endif

#
# Build up compiler diag switches based on settings
#
ifdef __DEBUG
  CDEFINES += -D__DEBUG=$(__DEBUG)
endif

ifdef __TRACE
  CDEFINES += -D__TRACE
endif

ifdef __WARN
  CDEFINES += -D__WARN
endif

#--------------------------------------------------------------------
# Options which are compiler independent
#--------------------------------------------------------------------

ifdef DLL
	ifdef DLLCDEFS
    CDEFINES += $(DLLCDEFS)
	endif
endif

#--------------------------------------------------------------------
# Options for DBCS
#--------------------------------------------------------------------
ifdef BI_DBCS_SUPPORT
  CDEFINES += -DBI_DBCS_SUPPORT
endif

ifdef BI_MSG_JAPAN
  CDEFINES += -DBI_MSG_LANGUAGE=0x0411
endif


#
# Figure out default data alignment based on target
#
ifndef ALIGN
ifdef WIN32
ALIGN = 8
endif
ifdef WIN64
ALIGN = 8
endif
ifdef LINUX
ALIGN = 8
endif
ifdef OSX
ALIGN = 8
endif
endif

#-----------------------------------------------------------------------------
# Build target name
#-----------------------------------------------------------------------------
ifdef MT
  MODEL=t
else
  MODEL=s
endif

ifdef UNICODE
  UC=u
endif

ifndef COMP
  COMP=g
endif


KEY = w$(MODEL)

ifdef DLL
	# 32bit flat or multithread DLL import library
  KEY:=$(KEY)i
endif

#ifdef USERDLL
# Static or import library for use by dlls
#  KEY:=$(KEY)u
#endif

ifdef DEBUG
  DBG=d
else
ifdef DIAGS
  DBG=d
endif
endif

TARGETSFX = $(VERSION)$(COMP)$(DBG)$(MODEL)$(UC)
TARGETNAME = $(BASENAME)$(TARGETSFX)

#-----------------------------------------------------------------------------
# Directory names
#-----------------------------------------------------------------------------

ifndef OWLROOT
	OWLROOT = ../..
endif

ifndef TARGETDIR
    TARGETDIR = $(OWLROOT)$(SLASH)bin
endif

ifndef SRCROOT
  ifeq ($(PLATFORM), Windows)
    SRCROOT   = $(OWLROOT)\source
  else
    SRCROOT   = $(OWLROOT)/source
  endif
endif

ifndef INCDIR
  ifeq ($(PLATFORM), Windows)
	INCDIR    = $(OWLROOT)\include
  else
	INCDIR    = $(OWLROOT)/include
  endif
endif

ifndef RCINCDIR
  ifeq ($(PLATFORM), Windows)
     RCINCDIR  = $(OWLROOT)\include
  else
     RCINCDIR  = $(OWLROOT)/include
  endif
endif

ifndef LIBDIR
  ifeq ($(PLATFORM), Windows)
	LIBDIR    = $(OWLROOT)\lib
  else
	LIBDIR    = $(OWLROOT)/lib
  endif
endif

ifndef OBJROOT
  ifeq ($(PLATFORM), Windows)
	OBJROOT   = $(OWLROOT)\obj
  else
	OBJROOT   = $(OWLROOT)/obj
  endif
endif

ifndef OBJDIR
  ifeq ($(PLATFORM), Windows)
	OBJDIR    = $(OBJROOT)\$(BASENAME)$(COMP)$(DBG)$(KEY)$(UC)
  else
	OBJDIR    = $(OBJROOT)/$(BASENAME)$(COMP)$(DBG)$(KEY)$(UC)
  endif
endif

ifndef FULLTARGET
  ifeq ($(PLATFORM), Windows)
	FULLTARGET 		= $(TARGETDIR)\$(TARGETNAME)
  else
	FULLTARGET 		= $(TARGETDIR)/$(TARGETNAME)
  endif
endif


#-----------------------------------------------------------------------------
# Create full pathnames for DEFFILE and RESFILE
#-----------------------------------------------------------------------------
ifdef DEFFILE
	DEFFILE := $(OBJDIR)$(SLASH)$(DEFFILE)
endif

ifdef RESFILE
	RESFILE := $(OBJDIR)$(SLASH)$(RESFILE)
endif

#-----------------------------------------------------------------------------
# Compiler specific data
#-----------------------------------------------------------------------------

#INCDIR=
#LIBDIR=

ifdef DEBUG
  CFLAGS += -O0 -ggdb
else
  CFLAGS += -O3 -s
endif

#CFLAGS += -mwindows
CDEFINES += -D_OWLPCH

MCC=$(GCC) -c $(CFLAGS) $(CDEFINES) -I$(INCDIR) -I$(SDKDIR)

ifndef DEBUG
$(OBJDIR)$(SLASH)%.obj : %.cpp
	$(MCC) $< -o $@
else
MCC2=$(MCC:-O0=-O1)

$(OBJDIR)$(SLASH)%.obj : %.cpp
	-$(MCC) $< -o $@
	if ! test -e $@ ;\
	then \
	$(MCC2) $< -o $@;\
	fi
endif

$(OBJDIR)$(SLASH)%.obj : GNU$(SLASH)%.s
	$(MCC) $< -o $@

#-----------------------------------------------------------------------------
# Create output directory
#-----------------------------------------------------------------------------
Setup: CreateDirs

ifeq ($(PLATFORM), Windows)
CreateDirs:
	@-if not exist $(TARGETDIR)\NUL mkdir $(TARGETDIR)
	@-if not exist $(OBJROOT)\NUL mkdir $(OBJROOT)
	@-if not exist $(OBJDIR)\NUL mkdir $(OBJDIR)
#  @-if not exist $(TARGETLIBDIR)\NUL mkdir $(TARGETLIBDIR)
	@-if not exist $(LIBDIR)\NUL mkdir $(LIBDIR)
else
CreateDirs:
	if ! test -e $(TARGETDIR);then mkdir $(TARGETDIR);fi
	if ! test -e $(OBJROOT);then mkdir $(OBJROOT);fi
	if ! test -e $(OBJDIR);then mkdir $(OBJDIR);fi
	if ! test -e $(LIBDIR);then mkdir $(LIBDIR);fi

endif


#-----------------------------------------------------------------------------
#
# Build the various file lists needed for dependency checking,
# and LIBing.
#
#   OBJS is the main list, conditionalized for the various targets
#       and options.
#
#   OBJS_LIB are the objs that only go in libs & implib, not the dll
#
#   OBJS_DLL are the objs that only go in the dll, not the libs
#
#   DEPOBJS is the list of object files for dependency checking
#
#   LIBOBJS is the list of object files for building the library
#
#-----------------------------------------------------------------------------

#
# Common, unsectioned objs go into static libs and DLLs
#
OBJS =\
 appdict.obj\
 bitmap.obj\
 bitmapga.obj\
 bitset.obj\
 btntextg.obj\
 buttonga.obj\
 castable.obj\
 celarray.obj\
 checks.obj\
 checklst.obj\
 chooseco.obj\
 choosefo.obj\
 clipboar.obj\
 clipview.obj\
 cmdline.obj\
 colmnhdr.obj\
 color.obj\
 commctrl.obj\
 commdial.obj\
 configfl.obj\
 controlb.obj\
 controlg.obj\
 createdc.obj\
 cstring.obj\
 cursor.obj\
 date.obj\
 dateio.obj\
 datep.obj\
 dc.obj\
 diaginit.obj\
 dibdc.obj\
 dispatch.obj\
 eventhan.obj\
 exbase.obj\
 except.obj\
 flatctrl.obj\
 font.obj\
 functor.obj\
 gadget.obj\
 gadgetli.obj\
 gadgetwi.obj\
 gauge.obj\
 gdibase.obj\
 gdiobjec.obj\
 geometrp.obj\
 geometrs.obj\
 geometry.obj\
 hotkey.obj\
 icon.obj\
 imagelst.obj\
 layoutwi.obj\
 locale.obj\
 localeco.obj\
 localeid.obj\
 mailer.obj\
 mci.obj\
 mciwav.obj\
 memory.obj\
 memorydc.obj\
 menu.obj\
 menugadg.obj\
 messageb.obj\
 metafile.obj\
 metafldc.obj\
 modegad.obj\
 modversi.obj\
 msgthred.obj\
 profile.obj\
 notetab.obj\
 objstrm.obj\
 owl.obj\
 paintdc.obj\
 palette.obj\
 panespli.obj\
 pen.obj\
 picklist.obj\
 pictwind.obj\
 popupgad.obj\
 preview.obj\
 prevwin.obj\
 printdc.obj\
 propsht.obj\
 psview.obj\
 rcntfile.obj\
 resource.obj\
 regheap.obj\
 region.obj\
 registry.obj\
 reglink.obj\
 rolldial.obj\
 serialze.obj\
 splashwi.obj\
 statusba.obj\
 swindow.obj\
 system.obj\
 tabctrl.obj\
 tabbed.obj\
 textgadg.obj\
 theme.obj\
 time.obj\
 timegadg.obj\
 timeio.obj\
 timep.obj\
 toolbox.obj\
 tooltip.obj\
 tracewnd.obj\
 uiborder.obj\
 uiface.obj\
 uihandle.obj\
 uimetric.obj\
 uipart.obj\
 updown.obj\
 urllink.obj\
 ustring.obj\
 version.obj\
 window.obj\
 windowdc.obj\
 wing.obj\
 winsock.obj\
 wskaddr.obj\
 wskerr.obj\
 wskhostm.obj\
 wsksockm.obj\
 wskservm.obj\
 wsksock.obj\
 wsksockd.obj\
 wsyscls.obj\
 wsysclsp.obj\
 wsysclss.obj

ifndef NO_OWL1_COMPAT
ifndef NO_OWL2_COMPAT
OBJS +=\
 compat.obj\
 moduleob.obj
endif
endif


#
# Add in these objs when not sectioning
#
#else
OBJS += \
 module.obj\
 applicat.obj\
 brush.obj\
 button.obj\
 checkbox.obj\
 combobox.obj\
 control.obj\
 decframe.obj\
 decmdifr.obj\
 dialog.obj\
 dib.obj\
 dibitmap.obj\
 docking.obj\
 docmanag.obj\
 doctpl.obj\
 document.obj\
 edit.obj\
 editfile.obj\
 editsear.obj\
 editview.obj\
 file.obj\
 filedoc.obj\
 filename.obj\
 filtval.obj\
 findrepl.obj\
 floatfra.obj\
 framewin.obj\
 glyphbtn.obj\
 groupbox.obj\
 hslider.obj\
 inputdia.obj\
 listbox.obj\
 listview.obj\
 lookval.obj\
 mdichild.obj\
 mdiclien.obj\
 mdiframe.obj\
 memcbox.obj\
 menudesc.obj\
 opensave.obj\
 picker.obj\
 pictval.obj\
 printdia.obj\
 printer.obj\
 printout.obj\
 radiobut.obj\
 rangeval.obj\
 scrollba.obj\
 scroller.obj\
 slider.obj\
 static.obj\
 tinycapt.obj\
 validate.obj\
 view.obj\
 vslider.obj
#endif

#
# Common, unsectioned objs go into static libs and DLLs
#
# PFXcoolbar.objSFX\

OBJS +=\
 combobex.obj\
 commview.obj\
 bardescr.obj\
 datetime.obj\
 draglist.obj\
 drawitem.obj\
 hlpmanag.obj\
 ipaddres.obj\
 listwind.obj\
 monthcal.obj\
 pager.obj\
 richedit.obj\
 richedv.obj\
 richedpr.obj\
 shellitm.obj\
 shelwapi.obj\
 thread.obj\
 trayicon.obj\
 treewind.obj

#  OBJS += dibblt32.obj _128bs.obj _64bs.obj
OBJS += dibblt32.obj _128bs.obj



#
# Add in these objs when not sectioning
#
#else  # OWLSECTION

OBJS += \
 animctrl.obj\
 fscroll.obj

#endif # OWLSECTION

OBJS += \
 unix.obj\
 unixxcpt.obj

#
# Objs that go into static libs and import libs, but not in the owl dll itself
# These also get built differently when in the import library. Note that
# classes in the modules are declared _USERCLASS instead of _OWLCLASS
#
OBJS_LIB +=\
 global.obj

#
# Lib for use by DLLs
#
OBJS_LIB +=\
 libmain.obj\
 owlmain.obj\
 initdll.obj

#
# Lib for use in EXEs
#
OBJS_LIB += winmain.obj
OBJS_LIB += main.obj

#
# Objs that go into the Owl DLL only
#
#ifdef DLL
#OBJS_DLL =\
# PFXtmplinst.objSFX
#endif

#
#add object directory prefix
#
OBJS:=$(addprefix $(OBJDIR)$(SLASH),$(OBJS))
OBJS_LIB:=$(addprefix $(OBJDIR)$(SLASH),$(OBJS_LIB))

ifdef DLL
DLLTARGET = $(FULLTARGET).dll.so
LIBTARGET = $(OWLROOT)$(SLASH)lib$(SLASH)lib$(BASENAME)$(DBG)w$(MODEL)$(UC)i.a
LIBTARGET_CONCOL = $(OWLROOT)$(SLASH)lib$(SLASH)lib$(BASENAME)$(DBG)w$(MODEL)$(UC)ic.a
CDEFINES += -D_DLL
else
LIBTARGET = $(OWLROOT)$(SLASH)lib$(SLASH)lib$(BASENAME)$(DBG)w$(MODEL)$(UC).a
LIBTARGET_CONSOL = $(OWLROOT)$(SLASH)lib$(SLASH)lib$(BASENAME)$(DBG)w$(MODEL)$(UC)c.a
endif

RCDEF =/d__GNUC__ /dWIN32
ifndef DLL
RCDEF += /dMAKELIB
endif
RCTEMP= $(RESFILE:.res=.rso)

$(RESFILE): owl.rc
#	$(RC) /i$(RCW32INC) /i$(INCDIR) /i$(INCDIR)/owl $(RCDEF) /fo$(RCTEMP) $<
	$(WINDRES) -I res -i $(RCTEMP) -O coff -o $@
	rm $(RCTEMP)

#$(RESFILE): owl.rc
#	$(WINDRES) -O coff -i $< --include-dir=$(RCW32INC) --include-dir=$(INCDIR) --include-dir=$(INCDIR)/owl -o $@


ifdef DLL
All: $(DLLTARGET) $(LIBTARGET) $(LIBTARGET_CONSOL)
else
All: $(LIBTARGET) $(LIBTARGET_CONSOL)
endif
#--export-all
#DLLWRAP_FLAGS = --output-def $(DEFFILE) \
#	--implib $(LIBTARGET) \
#	--driver-name $(CXX) \
#	--target mingw32
MCC4=$(MCC:-c=)


#$(DLLTARGET): $(OBJS) $(OBJS_LIB) $(OBJS_DLL) $(RESFILE) #$(DEFFILE)
#	$(LD) -o $(DLLTARGET) --out-implib $(LIBTARGET) $(LINKLIBDIR) $(LINKLIBS) $(LFLAGS) $(OBJS) $(OBJS_DLL) $(RESFILE)
#	$(AR) rs $(LIBTARGET) $(OBJS_LIB)

$(DLLTARGET): $(OBJS) $(OBJS_LIB) $(OBJS_DLL) $(RESFILE) #$(DEFFILE)
	$(MCC4) -shared -Wl,--out-implib,$(LIBTARGET) -o $(DLLTARGET) $(OBJS) $(OBJS_DLL) $(RESFILE)
	$(AR) rs $(LIBTARGET) $(OBJS_LIB)
	$(STRIP) $(DLLTARGET)


#$(DLLTARGET): $(OBJS) $(OBJS_LIB) $(OBJS_DLL) $(RESFILE) #$(DEFFILE)
#	$(MCC4) -shared -Wl,--out-implib,$(LIBTARGET) -o $(DLLTARGET) $(OBJS) $(OBJS_DLL) $(RESFILE)

#$(LIBTARGET): $(OBJS) $(OBJS_LIB)
#	$(AR) rs $(LIBTARGET) $(OBJS_LIB)

MCC5=$(MCC:-D_DLL=)
MCC5:=$(MCC5:-D_BUILDOWLDLL=)

$(OBJDIR)$(SLASH)global.obj : global.cpp
	$(MCC5) $(LIBDLL_DEFS) $(LIBDLL_UNDEFS) -o $(OBJDIR)$(SLASH)global.obj global.cpp

$(OBJDIR)$(SLASH)libmain.obj : libmain.cpp
	$(MCC5) $(LIBDLL_DEFS) $(LIBDLL_UNDEFS) -o $(OBJDIR)$(SLASH)libmain.obj libmain.cpp

$(OBJDIR)$(SLASH)owlmain.obj : owlmain.cpp
	$(MCC5) $(LIBDLL_DEFS) $(LIBDLL_UNDEFS) -o $(OBJDIR)$(SLASH)owlmain.obj owlmain.cpp

$(OBJDIR)$(SLASH)initdll.obj : initdll.cpp
	$(MCC5) $(LIBDLL_DEFS) $(LIBDLL_UNDEFS) -D_DLL -o $(OBJDIR)$(SLASH)initdll.obj initdll.cpp

#else #making lib

#used for -frepo
#RPO=$(MCC:-c=)

#the ld command forces the generation of template instances for -frepo
#it will always fail
$(LIBTARGET): $(OBJS) $(OBJS_LIB)
#	@-$(RPO) -o /tmp/foo.tmp $(OBJS) $(OBJS_LIB)
	$(AR) rs $(LIBTARGET) $?

#endif

MCC3=$(MCC:-O2=-O0) -fno-omit-frame-pointer
MCC3:=$(MCC3:-O1=-O0)

$(OBJDIR)/unixxcpt.obj : unixxcpt.cpp
	$(MCC3) -o $(OBJDIR)/unixxcpt.obj unixxcpt.cpp

##################################################
