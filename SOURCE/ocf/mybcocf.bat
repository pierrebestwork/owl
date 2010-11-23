:// -------------------------------------------------------------------------
:// ObjectComponents 
:// Copyright (c) 1993, 1996 by Borland International, All Rights Reserved
://
://$Revision: 1.2 $
://$Author: jogy $
://$Date: 2003/10/23 15:07:29 $
://
:// Simple batch file illustrating command line options to rebuild
:// the OCF libraries
://
:// NOTE : The 'OCFOPTS' macro defined below can be used to set additional
://        options. For example, if you want to rebuild the libraries
://        with DIAGNOSTICS and DEBUG information enabled, you can
://                set OCFOPTS=-DDIAGS -DDEBUG
:// -------------------------------------------------------------------------
:@echo off
  
:// -------------------------------------------------------------------------
://  32-bit versions of Library
:// -------------------------------------------------------------------------

make OWLROOT=D:\OWL BCROOT=C:\BC5 WIN32=1 -DCOMPILE_ASM DLL=1 MT=1 -DNO_OBJ_STREAMING -DNO_OWL5_COMPAT -DNO_EMULATED_CTRL

make OWLROOT=D:\OWL BCROOT=C:\BC5 WIN32=1 DEBUG=2 DIAGS=2 -DCOMPILE_ASM DLL=1 MT=1 -DNO_OBJ_STREAMING -DNO_OWL5_COMPAT -DNO_EMULATED_CTRL IMAGEBASE=0x06000000 CODEGUARD=1
