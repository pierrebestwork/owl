//----------------------------------------------------------------------------
//  ObjectWindows, OWL NExt
//  Copyright 1998-1999 by Yura Bidus. All Rights reserved.
//
//$Revision: 1.24.4.1 $
//$Author: jogybl $
//$Date: 2010-02-23 10:45:14 $
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TMemComboBox (TComboBox).
//  (Generated by OWL 6.x Class Expert for MS VC++, Version 1.5)
//----------------------------------------------------------------------------

#if !defined(OWL_MEMCBOX_H) // Sentry, use file only if it's not already included.
#define OWL_MEMCBOX_H

#include <owl/combobox.h>
#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif

__OWL_DECLCLASS(TConfigFile);

__OWL_BEGIN_NAMESPACE

// Generic definitions/compiler options (eg. alignment) preceeding the
// definition of classes
#include <owl/preclass.h>

//
// class TMemComboBox
// ~~~~~ ~~~~~~~~~~~~
// class remember old user selections.
// Saves strings in registry under: 
// CURRENT_USER\Software\OWL NExt\History Lists\[Name]
// Where [Name] - parameter passed in constructor.
// Override CreateConfigFile(); to customize this
// Support Transfer mechanizm, to pass value to Edit part of ComboBox. 
// No ComboBox transfer Data, 
// Use as transfer structure: _TCHAR buffer[textLimit]; 
// textLimit -  parameter passed in constructor.
// Example:
// struct TXFer{
//   uint16 CheckBox1;
//   uint16 CheckBox2;
//   _TCHAR Edit1[255];
//   _TCHAR MemComboBox[255];
// };
//  
//

// If you use this class as is, all programs will share common data,
// Example Find dialog, all programms can share Search string data 
// Common values, use them, and send me your suggestions 
#define HL_SearchText  "hlSearchText"
#define HL_ReplaceText "hlReplaceText"
#define HL_GotoLine    "hlGotoLine"
#define HL_BinGoto     "hlBinGoto"
#define HL_BlockIndent "hlBlockIndent"
#define HL_RightMargin "hlRightMargin"
#define HL_GutterWidth "hlGutterWidth"
#define HL_UndoLimit   "hlUndoLimit"
#define HL_TabStops    "hlTabStops"
#define HL_OtherText   "hlOtherText"

class _OWLCLASS TMemComboBox: public TComboBox {
  typedef TComboBox Inherited;
  public:
    TMemComboBox(TWindow*          parent, 
                 int               id, 
                 int x, int y, int w, int h, 
                 const owl_string& name, 
                 uint              textLimit = 255, 
                 uint              itemLimit = 25, 
                 TModule*          module = 0);
    TMemComboBox(TWindow*          parent, 
                 int               resId, 
                 const owl_string& name, 
                 uint              textLimit = 255, 
                 uint              itemLimit = 25, 
                 TModule*          module = 0);
  
  protected:
    owl_string  Name;
    uint        ItemLimit;

    void LoadContents();
    void SaveContents();

  public:
    virtual void CleanupWindow();
    virtual void SetupWindow();
    // transfer like edit box, no combobox transfer
    virtual uint Transfer(void* buffer, TTransferDirection direction);

  protected:
    void EvKillFocus(HWND hWndGetFocus);
    virtual TConfigFile* CreateConfigFile();
    virtual void AddToList(LPTSTR text);
    virtual owl_string GetSectionName();

  DECLARE_RESPONSE_TABLE(TMemComboBox);
  DECLARE_STREAMABLE_OWL(__OWL TMemComboBox, 1);
}; 

// define streameable inlines (VC)
DECLARE_STREAMABLE_INLINES( __OWL TMemComboBox );

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>

__OWL_END_NAMESPACE

#endif  // OWL_MEMCBOX_H sentry.
