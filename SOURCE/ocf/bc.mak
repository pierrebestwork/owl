#-----------------------------------------------------------------------------
# bc.mak - makefile for building OwlNext version of OCF (OCFNext)
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


!if !$d(OWLBASENAME)
OWLBASENAME = owl
!endif

!if !$d(BASENAME)
BASENAME = ocf
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

DEFFILE  = ocf.def
RESFILE  = ocfall.res

CDEFINES       = -D_OCFPCH -DINC_OLE2 -D_OWLPCH -DBI_NOINCLUDE_NS_OWL
DLLCDEFS     = -D_BUILDOCFDLL -D_OWLDLL -D_RTLDLL 

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

LINKOWLLIBS = 1

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
OBJS = \
 PFXappdesc.obj\
 PFXautocli.obj\
 PFXautodisp.obj\
 PFXautoiter.obj\
 PFXautosrv.obj\
 PFXautostck.obj\
 PFXautosym.obj\
 PFXautoval.obj\
 PFXexcept.obj\
 PFXocapp.obj\
 PFXocctrl.obj\
 PFXocdata.obj\
 PFXocdoc.obj\
 PFXocguid.obj\
 PFXoclink.obj\
 PFXocpart.obj\
 PFXocreg.obj\
 PFXocremvie.obj\
 PFXocstorag.obj\
 PFXocview.obj\
 PFXoledialg.obj\
 PFXoledoc.obj\
 PFXoleframe.obj\
 PFXolemdifr.obj\
 PFXoleutil.obj\
 PFXoleview.obj\
 PFXolewindo.obj\
 PFXstgdoc.obj\
 PFXtypelib.obj\
 PFXversion.obj

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
									$(ASM_OBJS:PFX=$(LINKOBJPFX))
!else
	LIBDEPOBJS_S  = $(OBJS:PFX=) $(OBJS_LIB:PFX=)
	LIBOBJS_S     = $(OBJS:PFX=$(LIBOBJPFX_DIR))\
									$(OBJS_LIB:PFX=$(LIBOBJPFX_DIR))\
									$(ASM_OBJS_LIB:PFX=$(LIBOBJPFX))
!endif

LIBDEPOBJS  = $(LIBDEPOBJS_S:SFX=)
LINKDEPOBJS = $(LINKDEPOBJS_S:SFX=)
LIBOBJS     = $(LIBOBJS_S:SFX=$(LIBOBJSFX))
LINKOBJS    = $(LINKOBJS_S:SFX=$(LINKOBJSFX))


# VH, May 2006:
# The resource file (ocfall.res) needs preprocessing of "version.rc".
# Preprocessing is necessary because of the limitations of the 
# preprocessor inside the Borland Resource Compiler. The stand-alone
# preprocessor hasn't got these problems so it is used to preprocess. 
# Then "ocfall.rc" will include "version.i" instead (see "ocfall.rc").

version.i: version.rc $(OWLROOT)\include\ocf\version.h
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


All: $(DLLTARGET)

#$(RESFILE):
#	$(RCC) $&.rc @&&|
#-i$(RCINCDIR);
#| -fo$@

!if $d(WIN32)


#
# Link and implib
#
$(DLLTARGET): $(LINKDEPOBJS) $(LIBDEPOBJS) $(DEFFILE) $(RESFILE)
	@cd $(.PATH.obj)
	$(LINK) @&&|
$(LINKOBJS) $(LINKSFX),$(RESFILE)
|
! if !$d(ILINK)
		$(TDSTRIP) -s $(DLLTARGET)
		$(IMPLIB) $(LIBTARGET) $(DLLTARGET)
! else
	 -del $(LIBTARGET)
	 copy $(DLLTARGET:.dll=.lib) $(LIBTARGET)
	 @-del 	$(DLLTARGET:.dll=.lib)
! endif
  @-del $(LIBTARGET:.lib=.bak)
	@cd $(.PATH.cpp)

#
# Win32 def-file
#
$(DEFFILE):
	@copy &&|
LIBRARY     "$(TARGETNAME)" INITINSTANCE
DESCRIPTION 'ObjectComponent DLL'
EXETYPE     WINDOWS
DATA        MULTIPLE
EXPORTS
!if $d(USE_ORD32)
| + $(.PATH.cpp)\ocf32.ord $(DEFFILE) > NUL
!else
| $@ > NUL
!endif


!else  # ! WIN32

!endif # ! WIN32

!else # ! DLL

All: $(LIBTARGET)

$(LIBTARGET): $(LIBDEPOBJS)
  @if exist $(LIBTARGET)  del $(LIBTARGET)
  $(LIB) "$(LIBTARGET)" @&&|
$(LIBOBJS) $(LIBSFX)
|
  @-del $(FULLTARGET).bak


!endif  # ! DLL


ocguid.obj: ocguid.cpp
  $(CC_OCF) ocguid.cpp


