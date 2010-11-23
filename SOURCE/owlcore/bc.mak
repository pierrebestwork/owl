#-----------------------------------------------------------------------------
# bc.mak - makefile for building OWLNext
# Created by Mikael Lindgren (m@smtdata.se) 1998
#
#$Revision: 1.1.2.2 $
#$Author: jogybl $
#$Date: 2010-08-17 12:00:00 $
#
# OWL specific defines (parameters):
#
#   COMPVER              compiler version (required)
#   OWLVER               OWLNext version number (required)
#
#   -DCOMPILE_ASM        Compile assembler files (requires tasm)
#   -DNO_EMULATED_CTRL   Builds library using only system implementation of
#                        common controls
#   -DNO_OWL1_COMPAT     Builds library without API exposed for backward
#                        compatibility with v1.0 of ObjectWindows.
#   -DNO_OWL2_COMPAT     Bbuilds library without API exposed for backward
#                        compatibility with v2.x of ObjectWindows.
#                        NOTE: NO_OWL2_COMPAT implies NO_OWL1_COMPAT
#   -DNO_OWL5_COMPAT     Bbuilds library without API exposed for backward
#                        compatibility with v5.x of ObjectWindows.
#                        NOTE: NO_OWL5_COMPAT implies NO_OWL2_COMPAT and
#                        NO_OWL1_COMPAT
#   -DNO_OBJ_STREAMING   Builds library without special code to allow
#                        some OWL objects to be persisted in a stream
#
#   -DMT                 Build multi-thread version
#   -DUNICODE            Build unicode version
#   -DUSE_UNICOWS        Build unicode version with MSLU
#   -DDLL                Build a DLL. (no default)
#   -DNAMESPACE          Builds the library within a namespace
#                         (ClassLib, OWL, OCF, OwlExt)
#   -DDIAGS              Build the diagnostic version of the target.
#   -DDEBUG              Build the debug version of the target.
#   ALIGN=<1|2|4|8>      Specifies the alignment
#   BI_DBCS_SUPPORT      builds library with DBCS support
#   BI_MSG_JAPAN         builds library with japaneze support
#
# Borland C++ specific defines:
#
#   -DBCC32I             Uses bcc32i instead of bcc32 to compile
#   -DILINK              Uses ILINK32 instead of TLINK32 to link
#   CODEGUARD=<1|2>      Build the codeguard version of the target.
#                         1=basic data, 2=full checking
#

!if $d(OWLNAME) 
BASENAME = $(OWLNAME)
!endif

!if !$d(BASENAME)
BASENAME = owl
!endif

!if !$d(OWLVER)
! error OWLVER not defined. Can't build.
!endif

!if !$d(VERSION)
  VERSION = $(OWLVER)
!endif

!if !$d(COMPNAME)
COMPNAME = b
!endif

!if !$d(COMPVER)
! error COMPVER not defined. Can't build.
!endif

!if !$d(BCCVER)
BCCVER = $(COMPVER)
!endif

COMP = $(COMPNAME)$(COMPVER)

DEFFILE  = owlp.def
RESFILE  = owl.res
LINKLIBS =

CDEFINES       = -D_OWLPCH -DBI_NOINCLUDE_NS_OWL
PCHFILE		   = "owl\pch.h"
FAR_DATA_DLL16 = -DOWL_FARDATA

OBJ_SRC_DIR = owlcore\Obj\Borland

!if $d(DLL)
DLLCDEFS       = -D_BUILDOWLDLL
LIBDLL_DEFS    = -D_OWLDLL
LIBDLL_UNDEFS  = -U_BUILDOWLDLL
!endif

#-----------------------------------------------------------------------------
# Various OWL settings
#-----------------------------------------------------------------------------

#
# Emulate Common Ctrls or not
#
!if $d(NO_EMULATED_CTRL)
  CDEFINES = $(CDEFINES) -DOWL_NATIVECTRL_ALWAYS
!endif

#
# Build OWL5 compatible version?
#
!if $d(NO_OWL5_COMPAT)
! if !$d(NO_OWL2_COMPAT)            # No OWL5 Compat => No OWL2 Compat
     NO_OWL2_COMPAT = 1
! endif
    CDEFINES = $(CDEFINES) -DNO_OWL5_COMPAT
!else
  CDEFINES = $(CDEFINES) -DOWL5_COMPAT
  COMPAT = c5
!endif

#
# Build OWL2 compatible version?
#
!if $d(NO_OWL2_COMPAT)
! if !$d(NO_OWL1_COMPAT)            # No OWL2 Compat => No OWL1 Compat
     NO_OWL1_COMPAT = 1
! endif
    CDEFINES = $(CDEFINES) -DNO_OWL2_COMPAT
!else
  CDEFINES = $(CDEFINES) -DOWL2_COMPAT
  COMPAT = c2
!endif

#
# Build OWL1 compatible version?
#
!if $d(NO_OWL1_COMPAT)
  CDEFINES = $(CDEFINES) -DNO_OWL1_COMPAT
!else
  CDEFINES = $(CDEFINES) -DOWL1_COMPAT
  COMPAT = c1
!endif

#
# Build without persistent/stream support?
#
!if $d(NO_OBJ_STREAMING)
  CDEFINES = $(CDEFINES) -DBI_NO_OBJ_STREAMING
!endif

!if $d(BI_STD_RTL)
  CDEFINES = $(CDEFINES) -DBI_STD_RTL -DBI_STD_STRING -DBI_STD_IO -DBI_STD_EXCEPTIONS -DBI_STD_RTTI
!endif


#-----------------------------------------------------------------------------
# The make target
#-----------------------------------------------------------------------------
Make: Setup All


##############################################################################
# Start of former common.inc
##############################################################################

.autodepend
.cacheautodepend
.swap

#--------------------------------------------------------------------
# Configure options based on settings.
# Check for incompatible choices and warn user.
#--------------------------------------------------------------------

#
# Options which are memory model independent
#
!if $d(NAMESPACE)
  CDEFINES = $(CDEFINES) -DBI_NAMESPACE
!endif

#
# VCL compatibility implies use of namespace
#
!if $d(VCL)
!if $d(NONAMESPACE)
!error VCL compatibility implies use of namespace (i.e. don't define NONAMESPACE)
!endif
!endif

!if $d(BI_STD_STRING)
  CDEFINES = $(CDEFINES) -DBI_STD_STRING
!endif

#
# VCL compatibility implies -D_USE_OWL_CM_EXIT
#
CDEFINES = $(CDEFINES) -D_USE_OWL_CM_EXIT
!if $d(VCL)
CDEFINES = $(CDEFINES) -DVCLRTL
!endif

#
# Unicode
#
!if $d(UNICODE) && !$d(_UNICODE)
_UNICODE = 1
!endif
!if $d(_UNICODE) && !$d(UNICODE)
UNICODE = 1
!endif

!if $d(UNICODE)
  CDEFINES = $(CDEFINES) -DUNICODE -D_UNICODE
!endif

!if $d(UNICODE) && $d(USE_UNICOWS) 
  CDEFINES = $(CDEFINES) -DUSE_UNICOWS 
!endif

#
# Diagnostics
#
!if $d(DIAGS)         # default diags use precondition, check, trace & warn
  __DEBUG=2
  __TRACE=1
  __WARN=1
!endif

#
# Build up compiler diag switches based on settings
#
!if $d(__DEBUG)
  CDEFINES = $(CDEFINES) -D__DEBUG=$(__DEBUG)
!endif

!if $d(__TRACE)
  CDEFINES = $(CDEFINES) -D__TRACE
!endif

!if $d(__WARN)
  CDEFINES = $(CDEFINES) -D__WARN
!endif

#
# Options for DLL
#
!if $d(DLL) && $d(DLLCDEFS)
  CDEFINES = $(CDEFINES) $(DLLCDEFS)
!endif

#
# Options for DBCS
#
!if $d(BI_DBCS_SUPPORT)
  CFLAGS = $(CFLAGS) -DBI_DBCS_SUPPORT
!endif
!if $d(BI_MSG_JAPAN)
  CFLAGS = $(CFLAGS) -DBI_MSG_LANGUAGE=0x0411
!endif

#
# Figure out default data alignment based on target
#
!if !$d(ALIGN)
!  if $d(VCL)
     ALIGN = 8
!  else
!    if $(BCCVER) >= 6
       ALIGN = 8
!    else
       ALIGN = 1
!    endif
!  endif
!endif


#-----------------------------------------------------------------------------
# Build target name
#-----------------------------------------------------------------------------

!if $d(MT) 
!  if $d(VCL)
     MODEL = v
!  else
     MODEL = t
!  endif
!else
   MODEL = s
!endif

!if $d(UNICODE)
!  if $d(USE_UNICOWS) 
     UC = us
!  else
     UC = u
!  endif
!endif

!if $d(DEBUG) || $d(DIAGS)
  DBG = d
!endif

KEY = $(DBG)$(MODEL)$(UC)$(COMPAT)

DELIM = -

TARGETNAME = $(BASENAME)$(DELIM)$(VERSION)$(DELIM)$(COMP)$(DELIM)$(KEY)
TEMPTARGETNAME = $(BASENAME)$(COMP)$(KEY)


#-----------------------------------------------------------------------------
# Directory names
#-----------------------------------------------------------------------------

!if !$d(OWLROOT)
  OWLROOT = ..\..
!endif

!if !$d(TARGETDIR)
TARGETDIR = $(OWLROOT)\Bin
!endif

!if !$d(TARGETLIBDIR)
TARGETLIBDIR = $(OWLROOT)\Lib
!endif

!if !$d(SRCROOT)
SRCROOT   = $(OWLROOT)\Source
!endif
!if !$d(INCDIR)
INCDIR    = $(OWLROOT)\Include
!endif
!if !$d(RCINCDIR)
RCINCDIR  = $(OWLROOT)\Include
!endif
!if !$d(LIBDIR)
LIBDIR    = $(OWLROOT)\Lib
!endif
!if !$d(OBJROOT)
OBJROOT   = $(OWLROOT)\Obj
!endif
!if !$d(OBJDIR)
!if $d(DLL)
OBJDIR    = $(OBJROOT)\$(TEMPTARGETNAME)i
!else
OBJDIR    = $(OBJROOT)\$(TEMPTARGETNAME)
!endif
!endif

.PATH.obj = $(OBJDIR)
.PATH.res = $(OBJDIR)
.PATH.def = $(OBJDIR)
.PATH.lib = $(LIBDIR)


!if !$d(FULLTARGET)
FULLTARGET     = $(TARGETDIR)\$(TARGETNAME)
!endif


#-----------------------------------------------------------------------------
# Create full pathnames for DEFFILE and RESFILE
#-----------------------------------------------------------------------------

!if $d(DEFFILE)
  DEFFILE = $(OBJDIR)\$(DEFFILE)
!endif
!if $d(RESFILE)
  RESFILE = $(OBJDIR)\$(RESFILE)
!endif


#-----------------------------------------------------------------------------
# Borland specific directive (previously "bc5.inc" include file)
#-----------------------------------------------------------------------------

!if $(BCCVER) >= 3
  ILINK=1
!endif

!if $d(CODEGUARD)
  DBG = d
!endif

!if $d(VCL)
  MT = 1
!endif

!if $d(MT)
  RTL_MT = -WM
  RTLMT  = mt
!else
  RTL_MT = -WM-
!endif

#
# If building for VCL, use an RTL that begins "cp" instead of "cw"
#
!if $d(VCL)
  RTLPFX=cp
!else
  RTLPFX=cw
!endif

#--------------------------------------------------------------------
# dir & search paths, & precompiled headers
#--------------------------------------------------------------------

#
# BC tools BCBINDIR can be based on MAKEDIR if no BCROOT is defined
#
!if !$d(BCROOT) && !$d(BCBROOT)
! include $(MAKEDIR)\bcroot.inc
! if !$d(BCROOT) && !$d(BCBROOT)
    BCROOT = $(MAKEDIR)\..
! endif
!endif

#
# At least one of the roots is certainly defined now.
# If only one is defined, make them both the same.
#
!if !$d(BCROOT)
  BCROOT = $(BCBROOT)
!elif !$d(BCBROOT)
  BCBROOT = $(BCROOT)
!endif


#
# Where to find BC libraries and include files
#
! if $(BCCVER) >= 12
   INCDIR = $(INCDIR);$(BCROOT)\include\windows\crtl;$(BCROOT)\include\windows\sdk
! endif
!if $(BCCVER) >= 8
  INCDIR = $(INCDIR);$(BCROOT)\Include\dinkumware;$(BCROOT)\Include
!else
  INCDIR = $(INCDIR);$(BCROOT)\Include\Win32;$(BCROOT)\Include
!endif

!if $d(DEBUG)
LIBDIR   = $(LIBDIR);$(BCROOT)\Lib
! if $(BCCVER) >= 12
   LIBDIR   = $(LIBDIR);$(BCROOT)\lib\win32\debug
! else
   LIBDIR   = $(LIBDIR);$(BCROOT)\lib\debug
! endif
!else
LIBDIR   = $(LIBDIR);$(BCROOT)\Lib
! if $(BCCVER) >= 12
   LIBDIR   = $(LIBDIR);$(BCROOT)\lib\win32\release
! else
   LIBDIR   = $(LIBDIR);$(BCROOT)\lib\release
! endif
!endif

RCINCDIR = $(RCINCDIR);$(BCROOT)\Include
! if $(BCCVER) >= 12
  RCINCDIR = $(RCINCDIR);$(BCROOT)\include\windows\crtl;$(BCROOT)\include\windows\sdk
! endif
BCBINDIR = $(BCROOT)\bin


#
# Where to find VCL libraries, objs, and headers
#
!if $d(VCL)
! if !$d(VCLLIBDIR)
    VCLLIBDIR = $(BCBROOT)\lib
! endif
! if !$d(VCLOBJDIR)
    VCLOBJDIR = $(VCLLIBDIR)\obj
! endif
! if !$d(VCLINCDIR)
!  if $(BCCVER) >= 12
     VCLINCDIR = $(BCBROOT)\include\windows\vcl
!  else
     VCLINCDIR = $(BCBROOT)\include\vcl
!  endif
! endif
INCDIR = $(INCDIR);$(VCLINCDIR)
!endif


#-----------------------------------------------------------------------------
# Borland C++ 32-bit options
#-----------------------------------------------------------------------------

!if $d(BCC32I)
   CC     = $(BCBINDIR)\bcc32i      # Highly optimizing compiler
!else
   CC     = $(BCBINDIR)\bcc32       # Regular optimizing compiler
!endif

!if $d(DEBUG)
  RCC      = $(BCBINDIR)\brcc32 -w32 -DWIN32 -D_DEBUG # Resource compiler
  CPP      = $(BCBINDIR)\cpp32 -DWIN32 -D_DEBUG # Preprocessor
!else 
  RCC      = $(BCBINDIR)\brcc32 -w32 -DWIN32 # Resource compiler
  CPP      = $(BCBINDIR)\cpp32 -DWIN32 # Preprocessor
!endif

RLINK    = $(BCBINDIR)\brcc32      # Resource binder/linker
ASM      = $(BCBINDIR)\tasm32      # Assembler
  
!if $d(ILINK)
  LINK     = $(BCBINDIR)\ilink32     # Linker
!else
  LINK     = $(BCBINDIR)\tlink32     # Linker
!endif

LIB      = $(BCBINDIR)\tlib        # Librarian
IMPLIB   = $(BCBINDIR)\implib      # Import library generator
TDSTRIP  = $(BCBINDIR)\tdstrp32    # Debug info extractor

CFLAGS   = $(CFLAGS) -d            # duplicate strings merged
!if $d(DEBUG)
   CFLAGS = $(CFLAGS) -v -k -Od     # debug-info on, normal stack, no opt.
!else
!  if $(BCCVER) > 8
     CFLAGS = $(CFLAGS) -3 -k- -O2   # Optimizations -O2 , stdcall
!  else
     CFLAGS = $(CFLAGS) -3 -k- -Ogmpv   # Optimizations -O1gmpv , stdcall
!  endif
!endif

!if $d(CODEGUARD)                   # Enable compiler's CodeGuard support
   CFLAGS = $(CFLAGS) -vGd -vGt     # d=add data, t=check this
!  if $(CODEGUARD) == 2
     CFLAGS = $(CFLAGS) -vGc        # c=add calls
!  endif
!endif

CFGFILE  = bcc32.cfg               # Name of compiler's config file

!if $d(DLL)
!  if $d(DEBUG)
     LIBOPTS = /P64 #/C
!  else
     LIBOPTS = /P64 /0 #/C
!  endif
!else
!  if $d(DEBUG)
     LIBOPTS = /P4096 #/C
!  else
     LIBOPTS = /P1024 /0 #/C
!  endif
!endif

!if $d(DEBUG)
   LDBG = -v
!endif

!if $d(DEBUG)
   LIBOPTS = /P2048 #/C
!else
   LIBOPTS = /P256 /0 #/C
!endif

!if $d(DLL)
   CFLAGS = $(CFLAGS) -D_RTLDLL
!  if $(BCCVER) >= 6
     CFLAGS = $(CFLAGS) -WD
!  else
     CFLAGS = $(CFLAGS) -WDE
!  endif
   LFLAGS = $(LDBG) $(ILINKFLAG) -Tpd -c -aa -x -OS -V4.0
!  if $d(ILINK)
     LFLAGS = $(LFLAGS) -Gi
!  endif
!  if $d(IMAGEBASE)
!    if $d(ILINK)
       LFLAGS = $(LFLAGS) -b:$(IMAGEBASE)
!    else
       LFLAGS = $(LFLAGS) -B:$(IMAGEBASE)
!    endif
!  endif
   STDLIBS  = import32 ole2w32.lib $(RTLPFX)32$(RTLMT)i   # Libs for linking the DLL
!  if $d(CODEGUARD)
     STDLIBS  = cg32 $(STDLIBS)
!  endif
!  if $d(UNICODE)
!    if $d(USE_UNICOWS)
       STDLIBS  = unicows $(STDLIBS)
!    endif
     STARTUP  = c0d32w
!  else
     STARTUP  = c0d32
!  endif
!else
   CFLAGS = -W $(CFLAGS)
!endif

!if $d(ILINK)
   LFLAGS = $(LFLAGS) -Gn
!endif

!if $d(VCL)
!if $(BCCVER) >= 6
   STDLIBS = vcllink $(STDLIBS)
!else
   STDLIBS = vcl $(STDLIBS)
!endif
   CFLAGS  = -b- -Vx -Ve $(CFLAGS) -DVCLRTL
   LFLAGS  = $(LFLAGS) -j$(VCLOBJDIR)
!endif

!if $d(MT)
   CFLAGS = $(CFLAGS) -WM
!else
   CFLAGS = -WM- $(CFLAGS)
!endif

!if $d(VCL)
   CFLAGS = -a4 $(CFLAGS)
!elif $d(ALIGN)
   CFLAGS = -a$(ALIGN) $(CFLAGS)
!endif


#-----------------------------------------------------------------------------
# Common options
#-----------------------------------------------------------------------------

CFLAGS_WIN = -W $(CFLAGS_WIN)


#-----------------------------------------------------------------------------
# Setup
#-----------------------------------------------------------------------------

CC         = $(CC) +$(CFGFILE)
CCW        = $(CC) $(CFLAGS_WIN)
LINK       = $(LINK) $(LFLAGS) -L$(LIBDIR) $(STARTUP)
LIB        = $(LIB) $(LIBOPTS)
IMPLIB_I   = $(IMPLIB) -i -w          # Ignore WEP, no warnings

#
# Special variant of compiler,
# for objs that goes into the lib when building a dll
#
CC_LIBDLL = $(CC) -W
CC_LIBDLL = $(CC_LIBDLL) $(LIBDLL_DEFS) $(LIBDLL_UNDEFS) 

  CCNO     = $(CC)
  CCNO1    = $(CC) -o$< -DSECTION=1
  CCNO2    = $(CC) -o$< -DSECTION=2
  CCNO3    = $(CC) -o$< -DSECTION=3
  CCNO4    = $(CC) -o$< -DSECTION=4
  CCNO5    = $(CC) -o$< -DSECTION=5
  CCNO6    = $(CC) -o$< -DSECTION=6
  CCNO7    = $(CC) -o$< -DSECTION=7
  CCNO8    = $(CC) -o$< -DSECTION=8
  CCNO9    = $(CC) -o$< -DSECTION=9
  CCNO10   = $(CC) -o$< -DSECTION=10
  CCNO11   = $(CC) -o$< -DSECTION=11
  CCNO12   = $(CC) -o$< -DSECTION=12
  CCNO13   = $(CC) -o$< -DSECTION=13
  CCNO14   = $(CC) -o$< -DSECTION=14
  CCNO15   = $(CC) -o$< -DSECTION=15
  CCNO16   = $(CC) -o$< -DSECTION=16
  CCNO17   = $(CC) -o$< -DSECTION=17
  CCNO18   = $(CC) -o$< -DSECTION=18
  CCNO19   = $(CC) -o$< -DSECTION=19
  CCNO20   = $(CC) -o$< -DSECTION=20
  CCNO21   = $(CC) -o$< -DSECTION=21
  CCNO22   = $(CC) -o$< -DSECTION=22
  CCNO23   = $(CC) -o$< -DSECTION=23
  CCNO24   = $(CC) -o$< -DSECTION=24
  CCNO25   = $(CC) -o$< -DSECTION=25
  CCNO26   = $(CC) -o$< -DSECTION=26
  CCNO27   = $(CC) -o$< -DSECTION=27
  CCNO28   = $(CC) -o$< -DSECTION=28
  CCNO29   = $(CC) -o$< -DSECTION=29
  CCNO30   = $(CC) -o$< -DSECTION=30
  CCNO31   = $(CC) -o$< -DSECTION=31
  CCNO32   = $(CC) -o$< -DSECTION=32
  CCNO33   = $(CC) -o$< -DSECTION=33
  CCNO34   = $(CC) -o$< -DSECTION=34
  CCNO35   = $(CC) -o$< -DSECTION=35
  CCNO36   = $(CC) -o$< -DSECTION=36
  CCNO37   = $(CC) -o$< -DSECTION=37
  CCNO38   = $(CC) -o$< -DSECTION=38
  CCNO39   = $(CC) -o$< -DSECTION=39
  CCNO40   = $(CC) -o$< -DSECTION=40
  CCNO41   = $(CC) -o$< -DSECTION=41
  CCNO42   = $(CC) -o$< -DSECTION=42
  CCNO43   = $(CC) -o$< -DSECTION=43
  CCNO44   = $(CC) -o$< -DSECTION=44
  CCNO45   = $(CC) -o$< -DSECTION=45
  CCNO46   = $(CC) -o$< -DSECTION=46
  CCNO47   = $(CC) -o$< -DSECTION=47
  CCNO48   = $(CC) -o$< -DSECTION=48
  CCNO49   = $(CC) -o$< -DSECTION=49
  CCNO50   = $(CC) -o$< -DSECTION=50
  CCNO51   = $(CC) -o$< -DSECTION=51
  CCNO52   = $(CC) -o$< -DSECTION=52
  CCNO53   = $(CC) -o$< -DSECTION=53
  CCNO54   = $(CC) -o$< -DSECTION=54
  CCNO55   = $(CC) -o$< -DSECTION=55
  CCNO56   = $(CC) -o$< -DSECTION=56
  CCNO57   = $(CC) -o$< -DSECTION=57
  CCNO58   = $(CC) -o$< -DSECTION=58
  CCNO59   = $(CC) -o$< -DSECTION=59
  CCNO60   = $(CC) -o$< -DSECTION=60
  CCNO61   = $(CC) -o$< -DSECTION=61
  CCNO62   = $(CC) -o$< -DSECTION=62
  CCNO63   = $(CC) -o$< -DSECTION=63
  CCNO64   = $(CC) -o$< -DSECTION=64
  CCNO65   = $(CC) -o$< -DSECTION=65
  CCNO66   = $(CC) -o$< -DSECTION=66
  CCNO67   = $(CC) -o$< -DSECTION=67
  CCNO68   = $(CC) -o$< -DSECTION=68
  CCNO69   = $(CC) -o$< -DSECTION=69

#
# Special variants of compiler, when compiling owl modules into small sections
#
CCS1       = $(CC) -o$< -DSECTION=1
CCS2       = $(CC) -o$< -DSECTION=2
CCS3       = $(CC) -o$< -DSECTION=3
CCS4       = $(CC) -o$< -DSECTION=4
CCS5       = $(CC) -o$< -DSECTION=5
CCS6       = $(CC) -o$< -DSECTION=6
CCS7       = $(CC) -o$< -DSECTION=7
CCS8       = $(CC) -o$< -DSECTION=8
CCS9       = $(CC) -o$< -DSECTION=9
CCS10      = $(CC) -o$< -DSECTION=10
CCS11      = $(CC) -o$< -DSECTION=11
CCS12      = $(CC) -o$< -DSECTION=12
CCS13      = $(CC) -o$< -DSECTION=13
CCS14      = $(CC) -o$< -DSECTION=14
CCS15      = $(CC) -o$< -DSECTION=15
CCS16      = $(CC) -o$< -DSECTION=16
CCS17      = $(CC) -o$< -DSECTION=17
CCS18      = $(CC) -o$< -DSECTION=18
CCS19      = $(CC) -o$< -DSECTION=19
CCS20      = $(CC) -o$< -DSECTION=20
CCS21      = $(CC) -o$< -DSECTION=21
CCS22      = $(CC) -o$< -DSECTION=22
CCS23      = $(CC) -o$< -DSECTION=23
CCS24      = $(CC) -o$< -DSECTION=24
CCS25      = $(CC) -o$< -DSECTION=25
CCS26      = $(CC) -o$< -DSECTION=26
CCS27      = $(CC) -o$< -DSECTION=27
CCS28      = $(CC) -o$< -DSECTION=28
CCS29      = $(CC) -o$< -DSECTION=29
CCS30      = $(CC) -o$< -DSECTION=30
CCS31      = $(CC) -o$< -DSECTION=31
CCS32      = $(CC) -o$< -DSECTION=32
CCS33      = $(CC) -o$< -DSECTION=33
CCS34      = $(CC) -o$< -DSECTION=34
CCS35      = $(CC) -o$< -DSECTION=35
CCS36      = $(CC) -o$< -DSECTION=36
CCS37      = $(CC) -o$< -DSECTION=37
CCS38      = $(CC) -o$< -DSECTION=38
CCS39      = $(CC) -o$< -DSECTION=39
CCS40      = $(CC) -o$< -DSECTION=40
CCS41      = $(CC) -o$< -DSECTION=41
CCS42      = $(CC) -o$< -DSECTION=42
CCS43      = $(CC) -o$< -DSECTION=43
CCS44      = $(CC) -o$< -DSECTION=44
CCS45      = $(CC) -o$< -DSECTION=45
CCS46      = $(CC) -o$< -DSECTION=46
CCS47      = $(CC) -o$< -DSECTION=47
CCS48      = $(CC) -o$< -DSECTION=48
CCS49      = $(CC) -o$< -DSECTION=49
CCS50      = $(CC) -o$< -DSECTION=50
CCS51      = $(CC) -o$< -DSECTION=51
CCS52      = $(CC) -o$< -DSECTION=52
CCS53      = $(CC) -o$< -DSECTION=53
CCS54      = $(CC) -o$< -DSECTION=54
CCS55      = $(CC) -o$< -DSECTION=55
CCS56      = $(CC) -o$< -DSECTION=56
CCS57      = $(CC) -o$< -DSECTION=57
CCS58      = $(CC) -o$< -DSECTION=58
CCS59      = $(CC) -o$< -DSECTION=59
CCS60      = $(CC) -o$< -DSECTION=60
CCS61      = $(CC) -o$< -DSECTION=61
CCS62      = $(CC) -o$< -DSECTION=62
CCS63      = $(CC) -o$< -DSECTION=63
CCS64      = $(CC) -o$< -DSECTION=64
CCS65      = $(CC) -o$< -DSECTION=65
CCS66      = $(CC) -o$< -DSECTION=66
CCS67      = $(CC) -o$< -DSECTION=67
CCS68      = $(CC) -o$< -DSECTION=68
CCS69      = $(CC) -o$< -DSECTION=69

#
# Special variants of compiler, when compiling ocguid.cpp (in ocfp.lib)
#
CC_OCF   = $(CC) -H-


#!if $d(LINKOWLLIBS)
#LINKLIBS = $(TARGETLIBDIR)\$(OWLBASENAME)$(DELIM)$(VERSION)$(DELIM)$(COMP)$(DELIM)$(KEY)
#!endif

!if $d(LINKLIBS)
  TEMP_001 = $(LINKLIBS:TARGETSFX=$(TARGETSFX))
  USERLIBS = $(TEMP_001:;= )
!endif

LINKSFX = ,$(FULLTARGET).dll, nul,$(USERLIBS) $(STDLIBS),
!if $d(DEFFILE)
  LINKSFX = $(LINKSFX) $(DEFFILE)
!endif

LIBSFX =

LINKOBJPFX =
LINKOBJSFX =

LIBOBJPFX = +
LIBOBJSFX =


#--------------------------------------------------------------------
# Create compiler response file
#--------------------------------------------------------------------

CFGFILE = $(OBJDIR)\$(CFGFILE)
CPCH    = -H -H=$(OBJDIR)\pch.csm
!if $d(PCHFILE)
CPCH    = $(CPCH) -H$(PCHFILE)
!else
CPCH    = $(CPCH) -H"pch.h" 
!endif

!if $(BCCVER) != 8
CPCH    = $(CPCH) -Hc
!endif

InitCompiler:
  @copy &&|
-I$(INCDIR)
-c -w -g255 $(CFLAGS) $(CDBG) $(CDEFINES)
$(CPCH)
-n$(OBJDIR)
| $(CFGFILE) > NUL


#--------------------------------------------------------------------
# Rules
#--------------------------------------------------------------------

.rc.res:
  $(RCC) -i$(RCINCDIR) -fo$@ $.

.cpp.obj:
  $(CC) {$& }

.asm.obj:
  $(ASM) /mx $& $@


#-----------------------------------------------------------------------------
# Create output directory
#-----------------------------------------------------------------------------

Setup: CreateDirs InitCompiler

CreateDirs:
  @-if not exist $(TARGETDIR)\NUL md $(TARGETDIR)
  @-if not exist $(TARGETLIBDIR)\NUL md $(TARGETLIBDIR)
  @-if not exist $(OBJROOT)\NUL md $(OBJROOT)
  @-if not exist $(OBJDIR)\NUL md $(OBJDIR)

!if !$d(COMPILE_ASM) && $d(OBJ_SRC_DIR)
	@if not exist $(OBJDIR)\dibblt32.obj copy $(SRCROOT)\$(OBJ_SRC_DIR)\*.obj $(OBJDIR)
!endif		# COMPILE_ASM


##############################################################################
# End of former common.inc
##############################################################################


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

OBJS =$(OBJS)\
 PFXrichedit.objSFX\
 PFXcommctrl.objSFX\

#
# Common, unsectioned objs go into static libs and DLLs
#
OBJS =$(OBJS)\
 PFXwinsock.objSFX\
 PFXtrayicon.objSFX\
 PFXsystem.objSFX\
 PFXregistry.objSFX\
 PFXappdict.objSFX\
 PFXbitmap.objSFX\
 PFXbitmapga.objSFX\
 PFXbitset.objSFX\
 PFXbtntextg.objSFX\
 PFXbuttonga.objSFX\
 PFXcastable.objSFX\
 PFXcelarray.objSFX\
 PFXchecks.objSFX\
 PFXchecklst.objSFX\
 PFXchooseco.objSFX\
 PFXchoosefo.objSFX\
 PFXclipboar.objSFX\
 PFXclipview.objSFX\
 PFXcmdline.objSFX\
 PFXcolmnhdr.objSFX\
 PFXcolor.objSFX\
 PFXcommdial.objSFX\
 PFXconfigfl.objSFX\
 PFXcontrolb.objSFX\
 PFXcontrolg.objSFX\
 PFXcreatedc.objSFX\
 PFXcstring.objSFX\
 PFXcursor.objSFX\
 PFXdate.objSFX\
 PFXdateio.objSFX\
 PFXdatep.objSFX\
 PFXdc.objSFX\
 PFXdiaginit.objSFX\
 PFXdibdc.objSFX\
 PFXdispatch.objSFX\
 PFXdumpstack.objSFX\
 PFXeventhan.objSFX\
 PFXexbase.objSFX\
 PFXexcept.objSFX\
 PFXflatctrl.objSFX\
 PFXfont.objSFX\
 PFXfunctor.objSFX\
 PFXgadget.objSFX\
 PFXgadgetli.objSFX\
 PFXgadgetwi.objSFX\
 PFXgauge.objSFX\
 PFXgdibase.objSFX\
 PFXgdiobjec.objSFX\
 PFXgeometrp.objSFX\
 PFXgeometrs.objSFX\
 PFXgeometry.objSFX\
 PFXhotkey.objSFX\
 PFXicon.objSFX\
 PFXimagelst.objSFX\
 PFXlayoutwi.objSFX\
 PFXlocale.objSFX\
 PFXlocaleco.objSFX\
 PFXlocaleid.objSFX\
 PFXmailer.objSFX\
 PFXmci.objSFX\
 PFXmciwav.objSFX\
 PFXmemory.objSFX\
 PFXmemorydc.objSFX\
 PFXmenu.objSFX\
 PFXmenugadg.objSFX\
 PFXmessageb.objSFX\
 PFXmetafile.objSFX\
 PFXmetafldc.objSFX\
 PFXmodegad.objSFX\
 PFXmodversi.objSFX\
 PFXmsgthred.objSFX\
 PFXprofile.objSFX\
 PFXnotetab.objSFX\
 PFXobjstrm.objSFX\
 PFXowl.objSFX\
 PFXpaintdc.objSFX\
 PFXpalette.objSFX\
 PFXpanespli.objSFX\
 PFXpen.objSFX\
 PFXpicklist.objSFX\
 PFXpictwind.objSFX\
 PFXpopupgad.objSFX\
 PFXpreview.objSFX\
 PFXprevwin.objSFX\
 PFXprintdc.objSFX\
 PFXpropsht.objSFX\
 PFXpsview.objSFX\
 PFXrcntfile.objSFX\
 PFXresource.objSFX\
 PFXregheap.objSFX\
 PFXregion.objSFX\
 PFXreglink.objSFX\
 PFXrolldial.objSFX\
 PFXserialze.objSFX\
 PFXsplashwi.objSFX\
 PFXstatusba.objSFX\
 PFXswindow.objSFX\
 PFXtabctrl.objSFX\
 PFXtabbed.objSFX\
 PFXtextgadg.objSFX\
 PFXtheme.objSFX\
 PFXtime.objSFX\
 PFXtimegadg.objSFX\
 PFXtimeio.objSFX\
 PFXtimep.objSFX\
 PFXtoolbox.objSFX\
 PFXtooltip.objSFX\
 PFXtracewnd.objSFX\
 PFXuiborder.objSFX\
 PFXuiface.objSFX\
 PFXuihandle.objSFX\
 PFXuimetric.objSFX\
 PFXuipart.objSFX\
 PFXupdown.objSFX\
 PFXurllink.objSFX\
 PFXustring.objSFX\
 PFXversion.objSFX\
 PFXwindow.objSFX\
 PFXwindowdc.objSFX\
 PFXwing.objSFX\
 PFXwskaddr.objSFX\
 PFXwskerr.objSFX\
 PFXwskhostm.objSFX\
 PFXwsksockm.objSFX\
 PFXwskservm.objSFX\
 PFXwsksock.objSFX\
 PFXwsksockd.objSFX\
 PFXwsyscls.objSFX\
 PFXwsysclsp.objSFX\
 PFXwsysclss.objSFX\
 PFXstackwalker.obj

!if !$d(NO_OWL1_COMPAT) && !$d(NO_OWL2_COMPAT)
OBJS = $(OBJS) \
 PFXcompat.objSFX\
 PFXmoduleob.objSFX
!endif

#
# Add in these objs when sectioning
#
!if $d(OWLSECTION)
OBJS = $(OBJS)\
 PFXapplica1.objSFX\
 PFXapplica2.objSFX\
 PFXapplica3.objSFX\
 PFXapplica4.objSFX\
 PFXapplica5.objSFX\
 PFXapplica6.objSFX\
 PFXapplica7.objSFX\
 PFXapplica8.objSFX\
 PFXapplica9.objSFX\
 PFXapplic10.objSFX\
 PFXapplic11.objSFX\
 PFXapplic12.objSFX\
 PFXapplic13.objSFX\
 PFXapplic14.objSFX\
 PFXapplic15.objSFX\
 PFXapplic16.objSFX\
 PFXapplic17.objSFX\
 PFXapplic18.objSFX\
 PFXapplic19.objSFX\
 PFXapplic20.objSFX\
 PFXapplic21.objSFX\
 PFXapplic22.objSFX\
 PFXapplic23.objSFX\
 PFXapplic24.objSFX\
 PFXapplic25.objSFX\
 PFXapplic26.objSFX\
 PFXapplic27.objSFX\
 PFXapplic28.objSFX\
 PFXapplic29.objSFX\
 PFXapplic30.objSFX\
 PFXapplic31.objSFX\
 PFXapplic32.objSFX\
 PFXbrush1.objSFX\
 PFXbrush2.objSFX\
 PFXbrush3.objSFX\
 PFXbrush4.objSFX\
 PFXbrush5.objSFX\
 PFXbrush6.objSFX\
 PFXbrush7.objSFX\
 PFXbrush8.objSFX\
 PFXbrush9.objSFX\
 PFXbrus10.objSFX\
 PFXbrus11.objSFX\
 PFXbrus12.objSFX\
 PFXbrus13.objSFX\
 PFXbrus14.objSFX\
 PFXbrus15.objSFX\
 PFXbutton1.objSFX\
 PFXbutton2.objSFX\
 PFXbutton3.objSFX\
 PFXbutton4.objSFX\
 PFXbutton5.objSFX\
 PFXcheckbo1.objSFX\
 PFXcheckbo2.objSFX\
 PFXcheckbo3.objSFX\
 PFXcheckbo4.objSFX\
 PFXcheckbo5.objSFX\
 PFXcheckbo6.objSFX\
 PFXcheckbo7.objSFX\
 PFXcheckbo8.objSFX\
 PFXcombobo1.objSFX\
 PFXcombobo2.objSFX\
 PFXcombobo3.objSFX\
 PFXcombobo4.objSFX\
 PFXcombobo5.objSFX\
 PFXcombobo6.objSFX\
 PFXcombobo7.objSFX\
 PFXcombobo8.objSFX\
 PFXcombobo9.objSFX\
 PFXcombob10.objSFX\
 PFXcombob11.objSFX\
 PFXcombob12.objSFX\
 PFXcombob13.objSFX\
 PFXcombob14.objSFX\
 PFXcombob15.objSFX\
 PFXcombob16.objSFX\
 PFXcombob17.objSFX\
 PFXcombob18.objSFX\
 PFXcombob19.objSFX\
 PFXcontrol1.objSFX\
 PFXcontrol2.objSFX\
 PFXcontrol3.objSFX\
 PFXcontrol4.objSFX\
 PFXcontrol5.objSFX\
 PFXcontrol6.objSFX\
 PFXdecfram1.objSFX\
 PFXdecfram2.objSFX\
 PFXdecmdif1.objSFX\
 PFXdecmdif2.objSFX\
 PFXdialog1.objSFX\
 PFXdialog2.objSFX\
 PFXdialog3.objSFX\
 PFXdialog4.objSFX\
 PFXdialog5.objSFX\
 PFXdialog6.objSFX\
 PFXdialog7.objSFX\
 PFXdialog8.objSFX\
 PFXdialog9.objSFX\
 PFXdialo10.objSFX\
 PFXdialo11.objSFX\
 PFXdialo12.objSFX\
 PFXdialo13.objSFX\
 PFXdialo14.objSFX\
 PFXdib1.objSFX\
 PFXdib2.objSFX\
 PFXdib3.objSFX\
 PFXdib4.objSFX\
 PFXdib5.objSFX\
 PFXdib6.objSFX\
 PFXdib7.objSFX\
 PFXdib8.objSFX\
 PFXdib9.objSFX\
 PFXdi10.objSFX\
 PFXdi11.objSFX\
 PFXdi12.objSFX\
 PFXdi13.objSFX\
 PFXdi14.objSFX\
 PFXdi15.objSFX\
 PFXdi16.objSFX\
 PFXdi17.objSFX\
 PFXdi18.objSFX\
 PFXdi19.objSFX\
 PFXdi20.objSFX\
 PFXdi21.objSFX\
 PFXdi22.objSFX\
 PFXdi23.objSFX\
 PFXdi24.objSFX\
 PFXdi25.objSFX\
 PFXdi26.objSFX\
 PFXdi27.objSFX\
 PFXdi28.objSFX\
 PFXdi29.objSFX\
 PFXdi30.objSFX\
 PFXdi31.objSFX\
 PFXdi32.objSFX\
 PFXdibitma1.objSFX\
 PFXdibitma2.objSFX\
 PFXdibitma3.objSFX\
 PFXdibitma4.objSFX\
 PFXdibitma5.objSFX\
 PFXdibitma6.objSFX\
 PFXdibitma7.objSFX\
 PFXdibitma8.objSFX\
 PFXdibitma9.objSFX\
 PFXdibitm10.objSFX\
 PFXdibitm11.objSFX\
 PFXdibitm12.objSFX\
 PFXdocking1.objSFX\
 PFXdocking2.objSFX\
 PFXdocking3.objSFX\
 PFXdocking4.objSFX\
 PFXdocking5.objSFX\
 PFXdocmang1.objSFX\
 PFXdocmang2.objSFX\
 PFXdocmang3.objSFX\
 PFXdoctpl1.objSFX\
 PFXdoctpl2.objSFX\
 PFXdocumen1.objSFX\
 PFXdocumen2.objSFX\
 PFXedit1.objSFX\
 PFXedit2.objSFX\
 PFXeditfil1.objSFX\
 PFXeditfil2.objSFX\
 PFXeditsea1.objSFX\
 PFXeditsea2.objSFX\
 PFXeditvie1.objSFX\
 PFXeditvie2.objSFX\
 PFXfi01.objSFX\
 PFXfi02.objSFX\
 PFXfi03.objSFX\
 PFXfi04.objSFX\
 PFXfi05.objSFX\
 PFXfi06.objSFX\
 PFXfi07.objSFX\
 PFXfi08.objSFX\
 PFXfi09.objSFX\
 PFXfi10.objSFX\
 PFXfi11.objSFX\
 PFXfi12.objSFX\
 PFXfi13.objSFX\
 PFXfi14.objSFX\
 PFXfi15.objSFX\
 PFXfi16.objSFX\
 PFXfi17.objSFX\
 PFXfi18.objSFX\
 PFXfi19.objSFX\
 PFXfi20.objSFX\
 PFXfi21.objSFX\
 PFXfi22.objSFX\
 PFXfi23.objSFX\
 PFXfi24.objSFX\
 PFXfi25.objSFX\
 PFXfi26.objSFX\
 PFXfi27.objSFX\
 PFXfi28.objSFX\
 PFXfi29.objSFX\
 PFXfi30.objSFX\
 PFXfi31.objSFX\
 PFXfi32.objSFX\
 PFXfi33.objSFX\
 PFXfi34.objSFX\
 PFXfi35.objSFX\
 PFXfi36.objSFX\
 PFXfi37.objSFX\
 PFXfi38.objSFX\
 PFXfi39.objSFX\
 PFXfi40.objSFX\
 PFXfi41.objSFX\
 PFXfi42.objSFX\
 PFXfi43.objSFX\
 PFXfi44.objSFX\
 PFXfi45.objSFX\
 PFXfi46.objSFX\
 PFXfi47.objSFX\
 PFXfi48.objSFX\
 PFXfi49.objSFX\
 PFXfi50.objSFX\
 PFXfi51.objSFX\
 PFXfi52.objSFX\
 PFXfi53.objSFX\
 PFXfi54.objSFX\
 PFXfi55.objSFX\
 PFXfi56.objSFX\
 PFXfi57.objSFX\
 PFXfi58.objSFX\
 PFXfi59.objSFX\
 PFXfi60.objSFX\
 PFXfi61.objSFX\
 PFXfi62.objSFX\
 PFXfiledoc1.objSFX\
 PFXfiledoc2.objSFX\
 PFXfilenam1.objSFX\
 PFXfilenam2.objSFX\
 PFXfilenam3.objSFX\
 PFXfilenam4.objSFX\
 PFXfilenam5.objSFX\
 PFXfilenam6.objSFX\
 PFXfilenam7.objSFX\
 PFXfilenam8.objSFX\
 PFXfilenam9.objSFX\
 PFXfilena10.objSFX\
 PFXfilena11.objSFX\
 PFXfilena12.objSFX\
 PFXfilena13.objSFX\
 PFXfilena14.objSFX\
 PFXfilena15.objSFX\
 PFXfilena16.objSFX\
 PFXfilena17.objSFX\
 PFXfilena18.objSFX\
 PFXfilena19.objSFX\
 PFXfilena20.objSFX\
 PFXfilena21.objSFX\
 PFXfilena22.objSFX\
 PFXfilena23.objSFX\
 PFXfilena24.objSFX\
 PFXfilena25.objSFX\
 PFXfilena26.objSFX\
 PFXfilena27.objSFX\
 PFXfilena28.objSFX\
 PFXfilena29.objSFX\
 PFXfiltval1.objSFX\
 PFXfiltval2.objSFX\
 PFXfindrep1.objSFX\
 PFXfindrep2.objSFX\
 PFXfloatfr1.objSFX\
 PFXfloatfr2.objSFX\
 PFXframewi1.objSFX\
 PFXframewi2.objSFX\
 PFXframewi3.objSFX\
 PFXglyphbt1.objSFX\
 PFXglyphbt2.objSFX\
 PFXglyphbt3.objSFX\
 PFXglyphbt4.objSFX\
 PFXglyphbt5.objSFX\
 PFXglyphbt6.objSFX\
 PFXglyphbt7.objSFX\
 PFXglyphbt8.objSFX\
 PFXglyphbt9.objSFX\
 PFXglyphb10.objSFX\
 PFXglyphb11.objSFX\
 PFXglyphb12.objSFX\
 PFXglyphb13.objSFX\
 PFXglyphb14.objSFX\
 PFXglyphb15.objSFX\
 PFXglyphb16.objSFX\
 PFXglyphb17.objSFX\
 PFXgroupbo1.objSFX\
 PFXgroupbo2.objSFX\
 PFXhslider1.objSFX\
 PFXhslider2.objSFX\
 PFXinputdi1.objSFX\
 PFXinputdi2.objSFX\
 PFXlistbox1.objSFX\
 PFXlistbox2.objSFX\
 PFXlistvie1.objSFX\
 PFXlistvie2.objSFX\
 PFXlookval1.objSFX\
 PFXlookval2.objSFX\
 PFXmdichil1.objSFX\
 PFXmdichil2.objSFX\
 PFXmdiclie1.objSFX\
 PFXmdiclie2.objSFX\
 PFXmdifram1.objSFX\
 PFXmdifram2.objSFX\
 PFXmemcbox1.objSFX\
 PFXmemcbox2.objSFX\
 PFXmenudes1.objSFX\
 PFXmenudes2.objSFX\
 PFXmodule1.objSFX\
 PFXmodule2.objSFX\
 PFXmodule3.objSFX\
 PFXmodule4.objSFX\
 PFXopensav1.objSFX\
 PFXopensav2.objSFX\
 PFXpicker1.objSFX\
 PFXpicker2.objSFX\
 PFXpicker3.objSFX\
 PFXpicker4.objSFX\
 PFXpicker5.objSFX\
 PFXpicker6.objSFX\
 PFXpicker7.objSFX\
 PFXpictval1.objSFX\
 PFXpictval2.objSFX\
 PFXprintdi1.objSFX\
 PFXprintdi2.objSFX\
 PFXprinter1.objSFX\
 PFXprinter2.objSFX\
 PFXprintou1.objSFX\
 PFXprintou2.objSFX\
 PFXradiobu1.objSFX\
 PFXradiobu2.objSFX\
 PFXrangevl1.objSFX\
 PFXrangevl2.objSFX\
 PFXscrollb1.objSFX\
 PFXscrollb2.objSFX\
 PFXscrolle1.objSFX\
 PFXscrolle2.objSFX\
 PFXslider1.objSFX\
 PFXslider2.objSFX\
 PFXstatic1.objSFX\
 PFXstatic2.objSFX\
 PFXtinycap1.objSFX\
 PFXtinycap2.objSFX\
 PFXvalidat1.objSFX\
 PFXvalidat2.objSFX\
 PFXview1.objSFX\
 PFXview2.objSFX\
 PFXview3.objSFX\
 PFXvslider1.objSFX\
 PFXvslider2.objSFX

#
# Add in these objs when not sectioning
#
!else
OBJS = \
 PFXmodule.objSFX\
 $(OBJS)\
 PFXapplicat.objSFX\
 PFXbrush.objSFX\
 PFXbutton.objSFX\
 PFXcheckbox.objSFX\
 PFXcombobox.objSFX\
 PFXcontrol.objSFX\
 PFXdecframe.objSFX\
 PFXdecmdifr.objSFX\
 PFXdialog.objSFX\
 PFXdib.objSFX\
 PFXdibitmap.objSFX\
 PFXdocking.objSFX\
 PFXdocmanag.objSFX\
 PFXdoctpl.objSFX\
 PFXdocument.objSFX\
 PFXedit.objSFX\
 PFXeditfile.objSFX\
 PFXeditsear.objSFX\
 PFXeditview.objSFX\
 PFXfile.objSFX\
 PFXfiledoc.objSFX\
 PFXfilename.objSFX\
 PFXfiltval.objSFX\
 PFXfindrepl.objSFX\
 PFXfloatfra.objSFX\
 PFXframewin.objSFX\
 PFXglyphbtn.objSFX\
 PFXgroupbox.objSFX\
 PFXhslider.objSFX\
 PFXinputdia.objSFX\
 PFXlistbox.objSFX\
 PFXlistview.objSFX\
 PFXlookval.objSFX\
 PFXmdichild.objSFX\
 PFXmdiclien.objSFX\
 PFXmdiframe.objSFX\
 PFXmemcbox.objSFX\
 PFXmenudesc.objSFX\
 PFXopensave.objSFX\
 PFXpicker.objSFX\
 PFXpictval.objSFX\
 PFXprintdia.objSFX\
 PFXprinter.objSFX\
 PFXprintout.objSFX\
 PFXradiobut.objSFX\
 PFXrangeval.objSFX\
 PFXscrollba.objSFX\
 PFXscroller.objSFX\
 PFXslider.objSFX\
 PFXstatic.objSFX\
 PFXtinycapt.objSFX\
 PFXvalidate.objSFX\
 PFXview.objSFX\
 PFXvslider.objSFX
!endif

#
# Common, unsectioned objs go into static libs and DLLs
#

OBJS =$(OBJS)\
 PFXcombobex.objSFX\
 PFXcommview.objSFX\
 PFXbardescr.objSFX\
 PFXdatetime.objSFX\
 PFXdraglist.objSFX\
 PFXdrawitem.objSFX\
 PFXhlpmanag.objSFX\
 PFXipaddres.objSFX\
 PFXlistwind.objSFX\
 PFXmonthcal.objSFX\
 PFXpager.objSFX\
 PFXrichedv.objSFX\
 PFXrichedpr.objSFX\
 PFXshellitm.objSFX\
# PFXshelwapi.objSFX\
 PFXthread.objSFX\
# PFXtrayicon.objSFX\
 PFXtreewind.objSFX
! if $d(COMPILE_ASM)
    OBJS = $(OBJS) PFXdibblt32.objSFX PFX_128bs.objSFX PFX_64bs.objSFX
! else		# COMPILE_ASM
    ASM_OBJS = PFXdibblt32.objSFX PFX_128bs.objSFX PFX_64bs.objSFX
! endif		# COMPILE_ASM

#
# Add in these objs when sectioning
#
!if $d(OWLSECTION)
OBJS = $(OBJS)\
 PFXanimctr1.objSFX\
 PFXanimctr2.objSFX\
 PFXanimctr3.objSFX\
 PFXanimctr4.objSFX\
 PFXanimctr5.objSFX\
 PFXanimctr6.objSFX\
 PFXfscrol1.objSFX\
 PFXfscrol2.objSFX\
 PFXfscrol3.objSFX\
 PFXfscrol4.objSFX\
 PFXfscrol5.objSFX\
 PFXfscrol6.objSFX\
 PFXfscrol7.objSFX\
 PFXfscrol8.objSFX\
 PFXfscrol9.objSFX


#
# Add in these objs when not sectioning
#
!else  # OWLSECTION
OBJS = $(OBJS)\
 PFXanimctrl.objSFX\
 PFXfscroll.objSFX

!endif # OWLSECTION

#
# Objs that go into static libs and import libs, but not in the owl dll itself
# These also get built differently when in the import library. Note that
# classes in the modules are declared _USERCLASS instead of _OWLCLASS
#
OBJS_LIB =\
 PFXglobal.objSFX

OBJS_LIB = $(OBJS_LIB)

#
# Lib for use by DLLs
#
OBJS_LIB = $(OBJS_LIB)\
 PFXlibmain.objSFX\
 PFXowlmain.objSFX\
 PFXinitdll.objSFX

#
# Lib for use in EXEs
#
OBJS_LIB = $(OBJS_LIB)\
 PFXwinmain.objSFX

OBJS_LIB = $(OBJS_LIB)\
 PFXmain.objSFX

#
# Objs that go into the Owl DLL only
#
!if $d(DLL)
#OBJS_DLL =\
# PFXtmplinst.objSFX
!endif


#
# Put together obj groups into lists for different dependencies & tools
#

LIBOBJPFX_DIR = $(LIBOBJPFX)$(OBJDIR)\\
LINKOBJPFX_DIR = $(LINKOBJPFX)$(OBJDIR)\\

!if $d(DLL)
    LIBDEPOBJS_S  = $(OBJS_LIB:PFX=)
    LINKDEPOBJS_S = $(OBJS:PFX=) $(OBJS_DLL:PFX=)
    LIBOBJS_S     = $(OBJS_LIB:PFX=$(LIBOBJPFX_DIR))
    LINKOBJS_S    = $(OBJS:PFX=$(LINKOBJPFX_DIR))\
                                    $(OBJS_DLL:PFX=$(LINKOBJPFX_DIR))\
                 $(ASM_OBJS:PFX=$(LINKOBJPFX_DIR))

!else
    LIBDEPOBJS_S  = $(OBJS:PFX=) $(OBJS_LIB:PFX=)
    LIBOBJS_S     = $(OBJS:PFX=$(LIBOBJPFX_DIR))\
                                    $(OBJS_LIB:PFX=$(LIBOBJPFX_DIR))\
                                    $(ASM_OBJS:PFX=$(LIBOBJPFX_DIR))
!endif

LIBDEPOBJS  = $(LIBDEPOBJS_S:SFX=)
LINKDEPOBJS = $(LINKDEPOBJS_S:SFX=)
LIBOBJS     = $(LIBOBJS_S:SFX=$(LIBOBJSFX))
LINKOBJS    = $(LINKOBJS_S:SFX=$(LINKOBJSFX))


#--------------------------------------------------------------------
# Resource file
#--------------------------------------------------------------------


# VH, May 2006:
# The resource file (owl.res) needs preprocessing of "version.rc".
# Preprocessing is necessary because of the limitations of the 
# preprocessor inside the Borland Resource Compiler. The stand-alone
# preprocessor hasn't got these problems so it is used to preprocess. 
# Then "owl.rc" will include "version.i" instead (see "owl.rc").

version.i: version.rc $(OWLROOT)\include\owl\version.h
  @echo Preprocessing version.rc
  $(CPP) @&&|
  -DRC_INVOKED -I$(RCINCDIR) 
  -o$@ 
| version.rc

$(RESFILE): version.i
  $(RCC) @&&|
  -i$(RCINCDIR)
  -fo$@ 
| $&.rc


#--------------------------------------------------------------------
# Targets
#--------------------------------------------------------------------


!if $d(DLL)
TARGETLIBNAME = $(TARGETNAME)i.lib
!else
TARGETLIBNAME = $(TARGETNAME).lib
!endif

LIBTARGET = $(TARGETLIBDIR)\$(TARGETLIBNAME)


!if $d(DLL)

DLLTARGET = $(FULLTARGET).dll

All: ECHOING $(DLLTARGET) 


ECHOING :
  @echo DLLTARGET = $(DLLTARGET)
  @echo LIBTARGET = $(LIBTARGET)
  @echo BCCVER    = $(BCCVER)
  @echo OWLVER    = $(OWLVER)
  @echo CDEFINES  = $(CDEFINES)


#
# Link and implib
#
$(DLLTARGET): $(LINKDEPOBJS) $(LIBDEPOBJS) $(DEFFILE) $(RESFILE)
  @cd $(.PATH.obj)
        #echo $(LINK) $(LINKOBJS) $(LINKSFX),$(RESFILE)
    $(LINK) @&&|
$(LINKOBJS) $(LINKSFX),$(RESFILE)
|
! if !$d(ILINK)
        $(TDSTRIP) -s $(DLLTARGET)
        $(IMPLIB) $(LIBTARGET) $(DLLTARGET)
! else
!message $(LIBTARGET)
     -del $(LIBTARGET)
     copy $(DLLTARGET:.dll=.lib) $(LIBTARGET)
     @-del 	$(DLLTARGET:.dll=.lib)
! endif
    $(LIB) "$(LIBTARGET)" @&&|
$(LIBOBJS) $(LIBSFX) 
|
  @-del $(LIBTARGET:.lib=.bak)
    @cd $(.PATH.cpp)


#
# Win32 def-file
#
!if $d(USE_ORD32)
$(DEFFILE): $(.PATH.cpp)\owl32.ord
!else
$(DEFFILE):
!endif
    @copy &&|
LIBRARY     "$(TARGETNAME)" INITINSTANCE
DESCRIPTION 'ObjectWindows DLL'
EXETYPE     WINDOWS
DATA        MULTIPLE
EXPORTS
!if $d(USE_ORD32)
| + $(.PATH.cpp)\owl32.ord $(DEFFILE) > NUL
!else
| $@ > NUL
!endif


#
# Compile objs that aren't part of the Owl DLL as if they were user EXE or
# user DLL objs
#
$(LIBDEPOBJS): 
    $(CC_LIBDLL) {$&.cpp }


!else  # ! DLL

All: $(LIBTARGET)

$(LIBTARGET): $(LIBDEPOBJS)
    @if exist $(LIBTARGET)  del $(LIBTARGET)
    $(LIB) "$(LIBTARGET)" @&&|
$(LIBOBJS) $(LIBSFX)
|
    @-del $(FULLTARGET).bak

!endif # if/else DLL


animctr1.obj: animctrl.cpp
  $(CCS1) animctrl.cpp
animctr2.obj: animctrl.cpp
  $(CCS2) animctrl.cpp
animctr3.obj: animctrl.cpp
  $(CCS3) animctrl.cpp
animctr4.obj: animctrl.cpp
  $(CCS4) animctrl.cpp
animctr5.obj: animctrl.cpp
  $(CCS5) animctrl.cpp
animctr6.obj: animctrl.cpp
  $(CCS6) animctrl.cpp


applicat.obj: applicat.cpp
  $(CCNO) applicat.cpp

applica1.obj: applicat.cpp
  $(CCNO1) applicat.cpp
applica2.obj: applicat.cpp
  $(CCNO2) applicat.cpp
applica3.obj: applicat.cpp
  $(CCNO3) applicat.cpp
applica4.obj: applicat.cpp
  $(CCNO4) applicat.cpp
applica5.obj: applicat.cpp
  $(CCNO5) applicat.cpp
applica6.obj: applicat.cpp
  $(CCNO6) applicat.cpp
applica7.obj: applicat.cpp
  $(CCNO7) applicat.cpp
applica8.obj: applicat.cpp
  $(CCNO8) applicat.cpp
applica9.obj: applicat.cpp
  $(CCNO9) applicat.cpp
applic10.obj: applicat.cpp
  $(CCNO10) applicat.cpp
applic11.obj: applicat.cpp
  $(CCNO11) applicat.cpp
applic12.obj: applicat.cpp
  $(CCNO12) applicat.cpp
applic13.obj: applicat.cpp
  $(CCNO13) applicat.cpp
applic14.obj: applicat.cpp
  $(CCNO14) applicat.cpp
applic15.obj: applicat.cpp
  $(CCNO15) applicat.cpp
applic16.obj: applicat.cpp
  $(CCNO16) applicat.cpp
applic17.obj: applicat.cpp
  $(CCNO17) applicat.cpp
applic18.obj: applicat.cpp
  $(CCNO18) applicat.cpp
applic19.obj: applicat.cpp
  $(CCNO19) applicat.cpp
applic20.obj: applicat.cpp
  $(CCNO20) applicat.cpp
applic21.obj: applicat.cpp
  $(CCNO21) applicat.cpp
applic22.obj: applicat.cpp
  $(CCNO22) applicat.cpp
applic23.obj: applicat.cpp
  $(CCNO23) applicat.cpp
applic24.obj: applicat.cpp
  $(CCNO24) applicat.cpp
applic25.obj: applicat.cpp
  $(CCNO25) applicat.cpp
applic26.obj: applicat.cpp
  $(CCNO26) applicat.cpp
applic27.obj: applicat.cpp
  $(CCNO27) applicat.cpp
applic28.obj: applicat.cpp
  $(CCNO28) applicat.cpp
applic29.obj: applicat.cpp
  $(CCNO29) applicat.cpp
applic30.obj: applicat.cpp
  $(CCNO30) applicat.cpp
applic31.obj: applicat.cpp
  $(CCNO31) applicat.cpp
applic32.obj: applicat.cpp
  $(CCNO32) applicat.cpp


brush1.obj: brush.cpp
  $(CCS1) brush.cpp
brush2.obj: brush.cpp
  $(CCS2) brush.cpp
brush3.obj: brush.cpp
  $(CCS3) brush.cpp
brush4.obj: brush.cpp
  $(CCS4) brush.cpp
brush5.obj: brush.cpp
  $(CCS5) brush.cpp
brush6.obj: brush.cpp
  $(CCS6) brush.cpp
brush7.obj: brush.cpp
  $(CCS7) brush.cpp
brush8.obj: brush.cpp
  $(CCS8) brush.cpp
brush9.obj: brush.cpp
  $(CCS9) brush.cpp
brus10.obj: brush.cpp
  $(CCS10) brush.cpp
brus11.obj: brush.cpp
  $(CCS11) brush.cpp
brus12.obj: brush.cpp
  $(CCS12) brush.cpp
brus13.obj: brush.cpp
  $(CCS13) brush.cpp
brus14.obj: brush.cpp
  $(CCS14) brush.cpp
brus15.obj: brush.cpp
  $(CCS15) brush.cpp

button1.obj: button.cpp
  $(CCS1) button.cpp
button2.obj: button.cpp
  $(CCS2) button.cpp
button3.obj: button.cpp
  $(CCS3) button.cpp
button4.obj: button.cpp
  $(CCS4) button.cpp
button5.obj: button.cpp
  $(CCS5) button.cpp

checkbo1.obj: checkbox.cpp
  $(CCS1) checkbox.cpp
checkbo2.obj: checkbox.cpp
  $(CCS2) checkbox.cpp
checkbo3.obj: checkbox.cpp
  $(CCS3) checkbox.cpp
checkbo4.obj: checkbox.cpp
  $(CCS4) checkbox.cpp
checkbo5.obj: checkbox.cpp
  $(CCS5) checkbox.cpp
checkbo6.obj: checkbox.cpp
  $(CCS6) checkbox.cpp
checkbo7.obj: checkbox.cpp
  $(CCS7) checkbox.cpp
checkbo8.obj: checkbox.cpp
  $(CCS8) checkbox.cpp

combobo1.obj: combobox.cpp
  $(CCS1) combobox.cpp
combobo2.obj: combobox.cpp
  $(CCS2) combobox.cpp
combobo3.obj: combobox.cpp
  $(CCS3) combobox.cpp
combobo4.obj: combobox.cpp
  $(CCS4) combobox.cpp
combobo5.obj: combobox.cpp
  $(CCS5) combobox.cpp
combobo6.obj: combobox.cpp
  $(CCS6) combobox.cpp
combobo7.obj: combobox.cpp
  $(CCS7) combobox.cpp
combobo8.obj: combobox.cpp
  $(CCS8) combobox.cpp
combobo9.obj: combobox.cpp
  $(CCS9) combobox.cpp
combob10.obj: combobox.cpp
  $(CCS10) combobox.cpp
combob11.obj: combobox.cpp
  $(CCS11) combobox.cpp
combob12.obj: combobox.cpp
  $(CCS12) combobox.cpp
combob13.obj: combobox.cpp
  $(CCS13) combobox.cpp
combob14.obj: combobox.cpp
  $(CCS14) combobox.cpp
combob15.obj: combobox.cpp
  $(CCS15) combobox.cpp
combob16.obj: combobox.cpp
  $(CCS16) combobox.cpp
combob17.obj: combobox.cpp
  $(CCS17) combobox.cpp
combob18.obj: combobox.cpp
  $(CCS18) combobox.cpp
combob19.obj: combobox.cpp
  $(CCS19) combobox.cpp

control1.obj: control.cpp
  $(CCS1) control.cpp
control2.obj: control.cpp
  $(CCS2) control.cpp
control3.obj: control.cpp
  $(CCS3) control.cpp
control4.obj: control.cpp
  $(CCS4) control.cpp
control5.obj: control.cpp
  $(CCS5) control.cpp
control6.obj: control.cpp
  $(CCS6) control.cpp

decfram1.obj: decframe.cpp
  $(CCS1) decframe.cpp
decfram2.obj: decframe.cpp
  $(CCS2) decframe.cpp

decmdif1.obj: decmdifr.cpp
  $(CCS1) decmdifr.cpp
decmdif2.obj: decmdifr.cpp
  $(CCS2) decmdifr.cpp

dialog1.obj: dialog.cpp
  $(CCS1) dialog.cpp
dialog2.obj: dialog.cpp
  $(CCS2) dialog.cpp
dialog3.obj: dialog.cpp
  $(CCS3) dialog.cpp
dialog4.obj: dialog.cpp
  $(CCS4) dialog.cpp
dialog5.obj: dialog.cpp
  $(CCS5) dialog.cpp
dialog6.obj: dialog.cpp
  $(CCS6) dialog.cpp
dialog7.obj: dialog.cpp
  $(CCS7) dialog.cpp
dialog8.obj: dialog.cpp
  $(CCS8) dialog.cpp
dialog9.obj: dialog.cpp
  $(CCS9) dialog.cpp
dialo10.obj: dialog.cpp
  $(CCS10) dialog.cpp
dialo11.obj: dialog.cpp
  $(CCS11) dialog.cpp
dialo12.obj: dialog.cpp
  $(CCS12) dialog.cpp
dialo13.obj: dialog.cpp
  $(CCS13) dialog.cpp
dialo14.obj: dialog.cpp
  $(CCS14) dialog.cpp

dib1.obj: dib.cpp
  $(CCS1) dib.cpp
dib2.obj: dib.cpp
  $(CCS2) dib.cpp
dib3.obj: dib.cpp
  $(CCS3) dib.cpp
dib4.obj: dib.cpp
  $(CCS4) dib.cpp
dib5.obj: dib.cpp
  $(CCS5) dib.cpp
dib6.obj: dib.cpp
  $(CCS6) dib.cpp
dib7.obj: dib.cpp
  $(CCS7) dib.cpp
dib8.obj: dib.cpp
  $(CCS8) dib.cpp
dib9.obj: dib.cpp
  $(CCS9) dib.cpp
di10.obj:		dib.cpp
  $(CCS10)	dib.cpp
di11.obj:		dib.cpp
  $(CCS11)	dib.cpp
di12.obj:		dib.cpp
  $(CCS12)	dib.cpp
di13.obj:		dib.cpp
  $(CCS13)	dib.cpp
di14.obj:		dib.cpp
  $(CCS14)	dib.cpp
di15.obj:		dib.cpp
  $(CCS15)	dib.cpp
di16.obj:		dib.cpp
  $(CCS16)	dib.cpp
di17.obj:		dib.cpp
  $(CCS17)	dib.cpp
di18.obj:		dib.cpp
  $(CCS18)	dib.cpp
di19.obj:		dib.cpp
  $(CCS19)	dib.cpp
di20.obj:		dib.cpp
  $(CCS20)	dib.cpp
di21.obj:		dib.cpp
  $(CCS21)	dib.cpp
di22.obj:		dib.cpp
  $(CCS22)	dib.cpp
di23.obj:		dib.cpp
  $(CCS23)	dib.cpp
di24.obj:		dib.cpp
  $(CCS24)	dib.cpp
di25.obj:		dib.cpp
  $(CCS25)	dib.cpp
di26.obj:		dib.cpp
  $(CCS26)	dib.cpp
di27.obj:		dib.cpp
  $(CCS27)	dib.cpp
di28.obj:		dib.cpp
  $(CCS28)	dib.cpp
di29.obj:		dib.cpp
  $(CCS29)	dib.cpp
di30.obj:		dib.cpp
  $(CCS30)	dib.cpp
di31.obj:		dib.cpp
  $(CCS31)	dib.cpp
di32.obj:		dib.cpp
  $(CCS32)	dib.cpp

dibitma1.obj:	dibitmap.cpp
  $(CCS1)			dibitmap.cpp
dibitma2.obj:	dibitmap.cpp
  $(CCS2)			dibitmap.cpp
dibitma3.obj:	dibitmap.cpp
  $(CCS3)			dibitmap.cpp
dibitma4.obj:	dibitmap.cpp
  $(CCS4)			dibitmap.cpp
dibitma5.obj:	dibitmap.cpp
  $(CCS5)			dibitmap.cpp
dibitma6.obj:	dibitmap.cpp
  $(CCS6)			dibitmap.cpp
dibitma7.obj:	dibitmap.cpp
  $(CCS7)			dibitmap.cpp
dibitma8.obj:	dibitmap.cpp
  $(CCS8)			dibitmap.cpp
dibitma9.obj:	dibitmap.cpp
  $(CCS9)			dibitmap.cpp
dibitm10.obj:	dibitmap.cpp
  $(CCS10)		dibitmap.cpp
dibitm11.obj:	dibitmap.cpp
  $(CCS11)		dibitmap.cpp
dibitm12.obj:	dibitmap.cpp
  $(CCS12)		dibitmap.cpp

docking1.obj: docking.cpp
  $(CCS1) docking.cpp
docking2.obj: docking.cpp
  $(CCS2) docking.cpp
docking3.obj: docking.cpp
  $(CCS3) docking.cpp
docking4.obj: docking.cpp
  $(CCS4) docking.cpp
docking5.obj: docking.cpp
  $(CCS5) docking.cpp

docmang1.obj: docmanag.cpp
  $(CCS1) docmanag.cpp
docmang2.obj: docmanag.cpp
  $(CCS2) docmanag.cpp
docmang3.obj: docmanag.cpp
  $(CCS3) docmanag.cpp

doctpl1.obj: doctpl.cpp
  $(CCS1) doctpl.cpp
doctpl2.obj: doctpl.cpp
  $(CCS2) doctpl.cpp

documen1.obj: document.cpp
  $(CCS1) document.cpp
documen2.obj: document.cpp
    $(CCS2) document.cpp

edit1.obj: edit.cpp
  $(CCS1) edit.cpp
edit2.obj: edit.cpp
  $(CCS2) edit.cpp

editfil1.obj: editfile.cpp
  $(CCS1) editfile.cpp
editfil2.obj: editfile.cpp
  $(CCS2) editfile.cpp

editsea1.obj: editsear.cpp
  $(CCS1) editsear.cpp
editsea2.obj: editsear.cpp
  $(CCS2) editsear.cpp

editvie1.obj: editview.cpp
  $(CCS1) editview.cpp
editvie2.obj: editview.cpp
  $(CCS2) editview.cpp

fi01.obj:		file.cpp
  $(CCS1)		file.cpp
fi02.obj:		file.cpp
  $(CCS2)		file.cpp
fi03.obj:		file.cpp
  $(CCS3)		file.cpp
fi04.obj:		file.cpp
  $(CCS4)		file.cpp
fi05.obj:		file.cpp
  $(CCS5)		file.cpp
fi06.obj:		file.cpp
  $(CCS6)		file.cpp
fi07.obj:		file.cpp
  $(CCS7)		file.cpp
fi08.obj:		file.cpp
  $(CCS8)		file.cpp
fi09.obj:		file.cpp
  $(CCS9)		file.cpp
fi10.obj:		file.cpp
  $(CCS10)	file.cpp
fi11.obj:		file.cpp
  $(CCS11)	file.cpp
fi12.obj:		file.cpp
  $(CCS12)	file.cpp
fi13.obj:		file.cpp
  $(CCS13)	file.cpp
fi14.obj:		file.cpp
  $(CCS14)	file.cpp
fi15.obj:		file.cpp
  $(CCS15)	file.cpp
fi16.obj:		file.cpp
  $(CCS16)	file.cpp
fi17.obj:		file.cpp
  $(CCS17)	file.cpp
fi18.obj:		file.cpp
  $(CCS18)	file.cpp
fi19.obj:		file.cpp
  $(CCS19)	file.cpp
fi20.obj:		file.cpp
  $(CCS20)	file.cpp
fi21.obj:		file.cpp
  $(CCS21)	file.cpp
fi22.obj:		file.cpp
  $(CCS22)	file.cpp
fi23.obj:		file.cpp
  $(CCS23)	file.cpp
fi24.obj:		file.cpp
  $(CCS24)	file.cpp
fi25.obj:		file.cpp
  $(CCS25)	file.cpp
fi26.obj:		file.cpp
  $(CCS26)	file.cpp
fi27.obj:		file.cpp
  $(CCS27)	file.cpp
fi28.obj:		file.cpp
  $(CCS28)	file.cpp
fi29.obj:		file.cpp
  $(CCS29)	file.cpp
fi30.obj:		file.cpp
  $(CCS30)	file.cpp
fi31.obj:		file.cpp
  $(CCS31)	file.cpp
fi32.obj:		file.cpp
  $(CCS32)	file.cpp
fi33.obj:		file.cpp
  $(CCS33)	file.cpp
fi34.obj:		file.cpp
  $(CCS34)	file.cpp
fi35.obj:		file.cpp
  $(CCS35)	file.cpp
fi36.obj:		file.cpp
  $(CCS36)	file.cpp
fi37.obj:		file.cpp
  $(CCS37)	file.cpp
fi38.obj:		file.cpp
  $(CCS38)	file.cpp
fi39.obj:		file.cpp
  $(CCS39)	file.cpp
fi40.obj:		file.cpp
  $(CCS40)	file.cpp
fi41.obj:		file.cpp
  $(CCS41)	file.cpp
fi42.obj:		file.cpp
  $(CCS42)	file.cpp
fi43.obj:		file.cpp
  $(CCS43)	file.cpp
fi44.obj:		file.cpp
  $(CCS44)	file.cpp
fi45.obj:		file.cpp
  $(CCS45)	file.cpp
fi46.obj:		file.cpp
  $(CCS46)	file.cpp
fi47.obj:		file.cpp
  $(CCS47)	file.cpp
fi48.obj:		file.cpp
  $(CCS48)	file.cpp
fi49.obj:		file.cpp
  $(CCS49)	file.cpp
fi50.obj:		file.cpp
  $(CCS50)	file.cpp
fi51.obj:		file.cpp
  $(CCS51)	file.cpp
fi52.obj:		file.cpp
  $(CCS52)	file.cpp
fi53.obj:		file.cpp
  $(CCS53)	file.cpp
fi54.obj:		file.cpp
  $(CCS54)	file.cpp
fi55.obj:		file.cpp
  $(CCS55)	file.cpp
fi56.obj:		file.cpp
  $(CCS56)	file.cpp
fi57.obj:		file.cpp
  $(CCS57)	file.cpp
fi58.obj:		file.cpp
  $(CCS58)	file.cpp
fi59.obj:		file.cpp
  $(CCS59)	file.cpp
fi60.obj:		file.cpp
  $(CCS60)	file.cpp
fi61.obj:		file.cpp
  $(CCS61)	file.cpp
fi62.obj:		file.cpp
  $(CCS62)	file.cpp

filedoc1.obj: filedoc.cpp
  $(CCS1) filedoc.cpp
filedoc2.obj: filedoc.cpp
  $(CCS2) filedoc.cpp

filenam1.obj: filename.cpp
  $(CCS1)			filename.cpp
filenam2.obj: filename.cpp
  $(CCS2)			filename.cpp
filenam3.obj: filename.cpp
  $(CCS3)			filename.cpp
filenam4.obj: filename.cpp
  $(CCS4)			filename.cpp
filenam5.obj: filename.cpp
  $(CCS5)			filename.cpp
filenam6.obj: filename.cpp
  $(CCS6)			filename.cpp
filenam7.obj: filename.cpp
  $(CCS7)			filename.cpp
filenam8.obj: filename.cpp
  $(CCS8)			filename.cpp
filenam9.obj: filename.cpp
  $(CCS9)			filename.cpp
filena10.obj: filename.cpp
  $(CCS10)		filename.cpp
filena11.obj: filename.cpp
  $(CCS11)		filename.cpp
filena12.obj: filename.cpp
  $(CCS12)		filename.cpp
filena13.obj: filename.cpp
  $(CCS13)		filename.cpp
filena14.obj: filename.cpp
  $(CCS14)		filename.cpp
filena15.obj: filename.cpp
  $(CCS15)		filename.cpp
filena16.obj: filename.cpp
  $(CCS16)		filename.cpp
filena17.obj: filename.cpp
  $(CCS17)		filename.cpp
filena18.obj: filename.cpp
  $(CCS18)		filename.cpp
filena19.obj: filename.cpp
  $(CCS19)		filename.cpp
filena20.obj: filename.cpp
  $(CCS20)		filename.cpp
filena21.obj: filename.cpp
  $(CCS21)		filename.cpp
filena22.obj: filename.cpp
  $(CCS22)		filename.cpp
filena23.obj: filename.cpp
  $(CCS23)		filename.cpp
filena24.obj: filename.cpp
  $(CCS24)		filename.cpp
filena25.obj: filename.cpp
  $(CCS25)		filename.cpp
filena26.obj: filename.cpp
  $(CCS26)		filename.cpp
filena27.obj: filename.cpp
  $(CCS27)		filename.cpp
filena28.obj: filename.cpp
  $(CCS28)		filename.cpp
filena29.obj: filename.cpp
  $(CCS29)		filename.cpp

filtval1.obj: filtval.cpp
  $(CCS1) filtval.cpp
filtval2.obj: filtval.cpp
  $(CCS2) filtval.cpp

findrep1.obj: findrepl.cpp
  $(CCS1) findrepl.cpp
findrep2.obj: findrepl.cpp
  $(CCS2) findrepl.cpp

floatfr1.obj: floatfra.cpp
  $(CCS1) floatfra.cpp
floatfr2.obj: floatfra.cpp
  $(CCS2) floatfra.cpp

framewi1.obj: framewin.cpp
  $(CCS1) framewin.cpp
framewi2.obj: framewin.cpp
  $(CCS2) framewin.cpp
framewi3.obj: framewin.cpp
  $(CCS3) framewin.cpp

fscrol1.obj:	fscroll.cpp
  $(CCS1)			fscroll.cpp
fscrol2.obj:	fscroll.cpp
  $(CCS2)			fscroll.cpp
fscrol3.obj:	fscroll.cpp
  $(CCS3)			fscroll.cpp
fscrol4.obj:	fscroll.cpp
  $(CCS4)			fscroll.cpp
fscrol5.obj:	fscroll.cpp
  $(CCS5)			fscroll.cpp
fscrol6.obj:	fscroll.cpp
  $(CCS6)			fscroll.cpp
fscrol7.obj:	fscroll.cpp
  $(CCS7)			fscroll.cpp
fscrol8.obj:	fscroll.cpp
  $(CCS8)			fscroll.cpp
fscrol9.obj:	fscroll.cpp
  $(CCS9)			fscroll.cpp

glyphbt1.obj: glyphbtn.cpp
  $(CCS1)			glyphbtn.cpp
glyphbt2.obj: glyphbtn.cpp
  $(CCS2)			glyphbtn.cpp
glyphbt3.obj: glyphbtn.cpp
  $(CCS3)			glyphbtn.cpp
glyphbt4.obj: glyphbtn.cpp
  $(CCS4)			glyphbtn.cpp
glyphbt5.obj: glyphbtn.cpp
  $(CCS5)			glyphbtn.cpp
glyphbt6.obj: glyphbtn.cpp
  $(CCS6)			glyphbtn.cpp
glyphbt7.obj: glyphbtn.cpp
  $(CCS7)			glyphbtn.cpp
glyphbt8.obj: glyphbtn.cpp
  $(CCS8)			glyphbtn.cpp
glyphbt9.obj: glyphbtn.cpp
  $(CCS9)			glyphbtn.cpp
glyphb10.obj: glyphbtn.cpp
  $(CCS10)		glyphbtn.cpp
glyphb11.obj: glyphbtn.cpp
  $(CCS11)		glyphbtn.cpp
glyphb12.obj: glyphbtn.cpp
  $(CCS12)		glyphbtn.cpp
glyphb13.obj: glyphbtn.cpp
  $(CCS13)		glyphbtn.cpp
glyphb14.obj: glyphbtn.cpp
  $(CCS14)		glyphbtn.cpp
glyphb15.obj: glyphbtn.cpp
  $(CCS15)		glyphbtn.cpp
glyphb16.obj: glyphbtn.cpp
  $(CCS16)		glyphbtn.cpp
glyphb17.obj: glyphbtn.cpp
  $(CCS17)		glyphbtn.cpp
 
groupbo1.obj: groupbox.cpp
  $(CCS1) groupbox.cpp
groupbo2.obj: groupbox.cpp
  $(CCS2) groupbox.cpp

hslider1.obj: hslider.cpp
  $(CCS1) hslider.cpp
hslider2.obj: hslider.cpp
  $(CCS2) hslider.cpp

inputdi1.obj: inputdia.cpp
    $(CCS1) inputdia.cpp
inputdi2.obj: inputdia.cpp
  $(CCS2) inputdia.cpp

listbox1.obj: listbox.cpp
  $(CCS1) listbox.cpp
listbox2.obj: listbox.cpp
  $(CCS2) listbox.cpp

listvie1.obj: listview.cpp
  $(CCS1) listview.cpp
listvie2.obj: listview.cpp
  $(CCS2) listview.cpp

lookval1.obj: lookval.cpp
  $(CCS1) lookval.cpp
lookval2.obj: lookval.cpp
  $(CCS2) lookval.cpp

mdichil1.obj: mdichild.cpp
  $(CCS1) mdichild.cpp
mdichil2.obj: mdichild.cpp
    $(CCS2) mdichild.cpp

mdiclie1.obj: mdiclien.cpp
  $(CCS1) mdiclien.cpp
mdiclie2.obj: mdiclien.cpp
  $(CCS2) mdiclien.cpp

mdifram1.obj: mdiframe.cpp
  $(CCS1) mdiframe.cpp
mdifram2.obj: mdiframe.cpp
  $(CCS2) mdiframe.cpp

memcbox1.obj: memcbox.cpp
  $(CCS1) memcbox.cpp
memcbox2.obj: memcbox.cpp
  $(CCS2) memcbox.cpp

menudes1.obj: menudesc.cpp
  $(CCS1) menudesc.cpp
menudes2.obj: menudesc.cpp
  $(CCS2) menudesc.cpp

module1.obj: module.cpp
  $(CCS1) module.cpp
module2.obj: module.cpp
  $(CCS2) module.cpp
module3.obj: module.cpp
  $(CCS3) module.cpp
module4.obj: module.cpp
  $(CCS4) module.cpp

opensav1.obj: opensave.cpp
  $(CCS1) opensave.cpp
opensav2.obj: opensave.cpp
  $(CCS2) opensave.cpp

picker1.obj: picker.cpp
  $(CCS1) picker.cpp
picker2.obj: picker.cpp
  $(CCS2) picker.cpp
picker3.obj: picker.cpp
  $(CCS3) picker.cpp
picker4.obj: picker.cpp
  $(CCS4) picker.cpp
picker5.obj: picker.cpp
  $(CCS5) picker.cpp
picker6.obj: picker.cpp
  $(CCS6) picker.cpp
picker7.obj: picker.cpp
  $(CCS7) picker.cpp

pictval1.obj: pictval.cpp
  $(CCS1) pictval.cpp
pictval2.obj: pictval.cpp
  $(CCS2) pictval.cpp

printou1.obj: printout.cpp
  $(CCS1) printout.cpp
printou2.obj: printout.cpp
  $(CCS2) printout.cpp

printer1.obj: printer.cpp
  $(CCS1) printer.cpp
printer2.obj: printer.cpp
  $(CCS2) printer.cpp

printdi1.obj: printdia.cpp
  $(CCS1) printdia.cpp
printdi2.obj: printdia.cpp
  $(CCS2) printdia.cpp

radiobu1.obj: radiobut.cpp
  $(CCS1) radiobut.cpp
radiobu2.obj: radiobut.cpp
  $(CCS2) radiobut.cpp

rangevl1.obj: rangeval.cpp
  $(CCS1) rangeval.cpp
rangevl2.obj: rangeval.cpp
  $(CCS2) rangeval.cpp

scrolle1.obj: scroller.cpp
  $(CCS1) scroller.cpp
scrolle2.obj: scroller.cpp
  $(CCS2) scroller.cpp

scrollb1.obj: scrollba.cpp
  $(CCS1) scrollba.cpp
scrollb2.obj: scrollba.cpp
  $(CCS2) scrollba.cpp

slider1.obj: slider.cpp
  $(CCS1) slider.cpp
slider2.obj: slider.cpp
  $(CCS2) slider.cpp

static1.obj: static.cpp
  $(CCS1) static.cpp
static2.obj: static.cpp
  $(CCS2) static.cpp

tinycap1.obj: tinycapt.cpp
    $(CCS1) tinycapt.cpp
tinycap2.obj: tinycapt.cpp
  $(CCS2) tinycapt.cpp

validat1.obj: validate.cpp
  $(CCS1) validate.cpp
validat2.obj: validate.cpp
  $(CCS2) validate.cpp

view1.obj: view.cpp
  $(CCS1) view.cpp
view2.obj: view.cpp
  $(CCS2) view.cpp
view3.obj: view.cpp
  $(CCS3) view.cpp

vslider1.obj: vslider.cpp
  $(CCS1) vslider.cpp
vslider2.obj: vslider.cpp
  $(CCS2) vslider.cpp


