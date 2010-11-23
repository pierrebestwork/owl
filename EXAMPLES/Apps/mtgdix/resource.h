//----------------------------------------------------------------------------
//  Project owltst1
//  My Copmany
//  Copyright 1998. All right reserved.
//
//  SUBSYSTEM:    owltst1 Application
//  FILE:         resource.h
//  AUTHOR:       Author
//
//  OVERVIEW
//  ~~~~~~~~
//  Constant definitions for all resources defined in owltst1.rc
//
//----------------------------------------------------------------------------
//#if !defined(resource_h)   // Sentry use file only if it's not already included.
//#define resource_h


//
// Application specific definitions:
//
#define IDI_SDIAPPLICATION      1001            // Application icon

#define IDM_SDI                 100             // Menu resource ID and Accelerator IDs



//
// CM_FILEnnnn commands (include/owl/editfile.rh except for CM_FILEPRINTPREVIEW)
//
#define CM_FILENEW              24331           // SDI New
#define CM_FILEOPEN             24332           // SDI Open
#define CM_FILECLOSE            24339
#define CM_FILESAVE             24333
#define CM_FILESAVEAS           24334


//
// Window commands (include/owl/window.rh)
//
#define CM_EXIT                 24310


//
// CM_EDITnnnn commands (include/owl/window.rh)
//
#define CM_EDITUNDO             24321
#define CM_EDITCUT              24322
#define CM_EDITCOPY             24323
#define CM_EDITPASTE            24324
#define CM_EDITDELETE           24325
#define CM_EDITCLEAR            24326
#define CM_EDITADD              24327
#define CM_EDITEDIT             24328



//
// Search menu commands (include/owl/editsear.rh)
//
#define CM_EDITFIND             24351
#define CM_EDITREPLACE          24352
#define CM_EDITFINDNEXT         24353



//
// Help menu commands.
//
#define CM_HELPABOUT            2009



//
// About Dialogs
//
#define IDD_ABOUTDLG            22000
#define IDC_VERSION             22001
#define IDC_COPYRIGHT           22002
#define IDC_DEBUG               22003



//
// OWL defined strings
//

//
// Statusbar
//
#define IDS_MODES               32530
#define IDS_MODESOFF            32531

//
// EditFile
//
#define IDS_UNTITLED            32550
#define IDS_UNABLEREAD          32551
#define IDS_UNABLEWRITE         32552
#define IDS_FILECHANGED         32553
#define IDS_FILEFILTER          32554

//
// EditSearch
//
#define IDS_CANNOTFIND          32540


//
// General & application exception messages (include/owl/except.rh)
//
#define IDS_UNKNOWNEXCEPTION    32767
#define IDS_OWLEXCEPTION        32766
#define IDS_OKTORESUME          32765
#define IDS_UNHANDLEDXMSG       32764
#define IDS_UNKNOWNERROR        32763
#define IDS_NOAPP               32762
#define IDS_OUTOFMEMORY         32761
#define IDS_INVALIDMODULE       32760
#define IDS_INVALIDMAINWINDOW   32759
#define IDS_VBXLIBRARYFAIL      32758

//
// Owl 1 compatibility messages
//
#define IDS_INVALIDWINDOW       32709
#define IDS_INVALIDCHILDWINDOW  32708
#define IDS_INVALIDCLIENTWINDOW 32707

//
// TXWindow messages
//
#define IDS_CLASSREGISTERFAIL   32749
#define IDS_CHILDREGISTERFAIL   32748
#define IDS_WINDOWCREATEFAIL    32747
#define IDS_WINDOWEXECUTEFAIL   32746
#define IDS_CHILDCREATEFAIL     32745

#define IDS_MENUFAILURE         32744
#define IDS_VALIDATORSYNTAX     32743
#define IDS_PRINTERERROR        32742

#define IDS_LAYOUTINCOMPLETE    32741
#define IDS_LAYOUTBADRELWIN     32740

//
// TXGdi messages
//
#define IDS_GDIFAILURE          32739
#define IDS_GDIALLOCFAIL        32738
#define IDS_GDICREATEFAIL       32737
#define IDS_GDIRESLOADFAIL      32736
#define IDS_GDIFILEREADFAIL     32735
#define IDS_GDIDELETEFAIL       32734
#define IDS_GDIDESTROYFAIL      32733
#define IDS_INVALIDDIBHANDLE    32732




//
// TInputDialog DIALOG resource (include/owl/inputdia.rh)
//
#define IDD_INPUTDIALOG         32514
#define ID_PROMPT               4091
#define ID_INPUT                4090


//
// TSlider bitmaps (horizontal and vertical) (include/owl/slider.rh)
//
#define IDB_HSLIDERTHUMB        32000
#define IDB_VSLIDERTHUMB        32001


//
// Validation messages (include/owl/validate.rh)
//
#define IDS_VALPXPCONFORM       32520
#define IDS_VALINVALIDCHAR      32521
#define IDS_VALNOTINRANGE       32522
#define IDS_VALNOTINLIST        32523


//#endif  // resource_h


