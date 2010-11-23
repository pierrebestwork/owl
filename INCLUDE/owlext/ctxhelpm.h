// ****************************************************************************
// OWL Extensions (OWLEXT) Class Library
// Copyright (C) 1998 by Dieter Windau
// All rights reserved
//
//$Revision: 1.1.4.3 $
//$Author: jogybl $
//$Date: 2010-02-19 16:49:53 $
//
// ctxhelpm.h:  header file
// Version:     1.2
// Date:        20.10.1998
// Author:      Dieter Windau  (used with permission)
//
// TCtxHelpFileManager is a freeware OWL class derived from THelpFileManager
// that add context sensitive help functions similar to the context help
// generated by AppWizard (MFC) or AppExpert (OWL).
//
// TCtxHelpStatusBar is a freeware OWL class that support context sensitive help
//
// You are free to use/modify this code but leave this header intact.
// May not be sold for profit.
//
// Tested with Borland C++ 5.02, OWL 5.02 under Windows NT 4.0 SP3 but I think
// the class would work with Windows 95 too.
// This file is provided "as is" with no expressed or implied warranty.
// Use at your own risk.
//
// Please send me bug reports, bug fixes, enhancements, requests, etc., and
// I'll try to keep this in next versions:
//   EMail: dieter.windau@usa.net
//   Web:   http://www.members.aol.com/softengage/index.htm
// ****************************************************************************
#if !defined (__OWLEXT_CTXHELPM_H) && !defined (__CTXHELPM_H)
#define __OWLEXT_CTXHELPM_H
#define __CTXHELPM_H

#ifndef   OWL_HEPMANAG_H
#include <owl/hlpmanag.h> // THelpFileManager
#endif
#ifndef   OWL_STATUSBA_H
#include <owl/statusba.h> // TStatusBar
#endif

// ************************* TCtxHelpFileManager ******************************
OWLEXT_BEGIN_NAMESPACE

// Generic definitions/compiler options (eg. alignment) preceeding the
// definition of classes
//
#include <owl/preclass.h>

class OWLEXTCLASS TCtxHelpFileManager : public __OWL THelpFileManager {
	public:
		TCtxHelpFileManager(const owl_string& helpFileName = _T(""));
		virtual ~TCtxHelpFileManager();

	public:
		virtual void SetContextHelp(bool contextHelp);
		bool IsContextHelp() { return ContextHelp; }
		// return true, if the context help (Shift+F1) is active

		HCURSOR GetHelpCursor() { return hHelpCursor; }
		// return the help cursor for context sensitive help

		void WinHelp(UINT helpID);
		// Call WinHelp in mode CONTEXT_HELP with helpID
		// If ContextHelp is true, after the call the context help is set to false

		void SetSupportSimpleF1Help(bool b) { SupportSimpleF1Help = b; }
		bool IsSupportSimpleF1Help() { return SupportSimpleF1Help; }
		// Enable/disable the support for simple F1 help
		// You can disable the simple help (F1) support, if you use only the
		// DEFINE_HELPCONTEXT, HCENTRY_MENU, ... macros for all menu items

		void SetSupportSystemMenu(bool b) { SupportSystemMenu = b; }
		bool IsSupportSystemMenu() { return SupportSystemMenu; }
		// Enable/disable the support for system menu items and app decorations

		void CmHelpContents();
		void CmHelpContext();
		void CeHelpContext(__OWL TCommandEnabler& ce);
		void CmHelpUsing();
		void CmHelpSearch();

		virtual void ActivateHelp(__OWL TWindow* win, int helpFileContextId,
															uint hlpCmd = HELP_CONTEXT);

		// Call a yellow popup help window for dialog windows
		// and the normal online help for other windows or menus

		virtual bool PreProcessAppMsg(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam);
		virtual bool PreProcessAppMsg(MSG& msg);
		// Process application messages to provide context sensitive help

	protected:
		void EvHelp(HELPINFO*);
		// Copy more or less from THelpFileManager. Add simple help support for menus

	protected:

		HCURSOR hHelpCursor;        // Context sensitive help cursor.
		HCURSOR hOldCursor;         // Cursor, before the context help is started

		TCHAR   HintText[128];      // Statusbar hint text if context help is on
		UINT    MenuItemId;         // Id of the actual selected popup menu item

		bool    SupportSimpleF1Help;// Support simple help (F1)
		bool    SupportSystemMenu;  // Support context help for system menu

	DECLARE_RESPONSE_TABLE(TCtxHelpFileManager);
};

// ************************* TCtxHelpStatusBar ********************************

class OWLEXTCLASS TCtxHelpStatusBar : public __OWL TStatusBar {
  public:
    TCtxHelpStatusBar(__OWL TWindow* parent = 0,
  	  __OWL TGadget::TBorderStyle borderStyle = __OWL TGadget::Recessed,
      uint modeIndicators = 0,
      __OWL TFont* font = new __OWL TGadgetWindowFont,
      __OWL TModule* module = 0);

    ~TCtxHelpStatusBar();

    void Block() { block = true; }
    void UnBlock() { block = false; }
    bool IsBlock() { return block; }

    virtual void SetHintText(LPCTSTR text);
    // Show only the hint text, if the output is not blocked

  protected:
    void EvLButtonDown(uint modKeys, __OWL TPoint& point);

  protected:
    bool block;	// Block the output for hint text

  DECLARE_RESPONSE_TABLE(TCtxHelpStatusBar);
};

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>

OWLEXT_END_NAMESPACE

#endif  //__OWLEXT_CTXHELPM_H
