//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1995 by Borland International, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_APPLICAT_H)
# include <owl/applicat.h>                              
#endif
#if !defined(OWL_FRAMEWIN_H)
# include <owl/framewin.h>
#endif
#if !defined(OWL_HLPMANAG_H)
# include <owl/hlpmanag.h>
#endif
#include <owl/hlpmanag.rh>
#if !defined(OWL_DIALOG_H)
# include <owl/dialog.h>
#endif
#include <owl/statusba.h>
#include <owl/propsht.h>
#include <owl/glyphbtn.h>
#include <owl/propsht.rh>


#include "htmlhelp.rh"
#include "HelpIds.hh"


//
// class CtxtHelpApplication
// ~~~~~ ~~~~~~~~~~~~~~~~~~
class TCtxtHelpApp : public TApplication, public THelpFileManager {
  public:
    TCtxtHelpApp(const char* name):TApplication(name),THelpFileManager("TestHTMLHelp.chm") { }
   ~TCtxtHelpApp() { }

    void 	InitMainWindow();
    bool 	CanClose ();
		bool  ProcessAppMsg (MSG& msg);
  	DECLARE_RESPONSE_TABLE(TCtxtHelpApp);
};


/* -------------------------------------------------------------------------- */
class MMake1Page : public TPropertyPage {
	public:
		MMake1Page(TPropertySheet* parent);
		~MMake1Page(){ Destroy();}

		virtual   int   SetActive(TNotify& );
		virtual   bool 	KillActive(TNotify& );
		virtual   void  SetupWindow();

	protected:
		void 		CmBrowse ();
		virtual void 	Help(TNotify&);

	DECLARE_RESPONSE_TABLE(MMake1Page);
	DECLARE_HELPCONTEXT(MMake1Page);
};

DEFINE_RESPONSE_TABLE1(MMake1Page, TPropertyPage)
//{{MMake1PageRSP_TBL_BEGIN}}
	EV_PSN_SETACTIVE(SetActive),
	EV_PSN_KILLACTIVE(KillActive),
//	EV_COMMAND(IDC_MM1BTN, CmBrowse),
	EV_PSN_HELP(Help),
//{{MMake1PageRSP_TBL_END}}
END_RESPONSE_TABLE;

DEFINE_HELPCONTEXT(MMake1Page)
//	HCENTRY_CONTROL(IDH_MK1NAME, 			IDC_MM1NAME),
//	HCENTRY_CONTROL(IDH_MK1BROWSE,		IDC_MM1BTN),
//	HCENTRY_CONTROL(IDH_MK1SELTYPE,	  IDC_MM1SELTYPE),
//	HCENTRY_CONTROL(IDH_MM1FRAMETYPE, IDC_MM1FRAMETYPE),
//	HCENTRY_CONTROL(IDH_MKNEXT, 			ID_NEXT),
//	HCENTRY_CONTROL(IDH_MKBACK, 			ID_BACK),
END_HELPCONTEXT;

/* -------------------------------------------------------------------------- */
MMake1Page::MMake1Page(TPropertySheet* parent)
:
	TPropertyPage(parent, IDD_MAKEFIRST)
{
//	new MGlyphButton(this, IDC_MM1BTN, MGlyphButton::btBrowse);
}
void MMake1Page::SetupWindow()
{
	TPropertyPage::SetupWindow();
}
void MMake1Page::Help(TNotify& )
{
	HELPINFO Info;
	Info.cbSize       = sizeof(Info);
	Info.iContextType = HELPINFO_MENUITEM;
	Info.iCtrlId      = IDHELP;
	Info.dwContextId  = 0;
	Info.MousePos;
	GetApplication()->GetMainWindow()->SendMessage(WM_HELP, 0, TParam2(&Info));
}
int MMake1Page::SetActive(TNotify&)
{
	SETUP_HELPCONTEXT(TCtxtHelpApp, MMake1Page);
//	GetSheet()->SetWizButtons(PSWIZB_NEXT);
	return 0;
}
bool MMake1Page::KillActive(TNotify&)
{
	CLEANUP_HELPCONTEXT(TCtxtHelpApp, MMake1Page);
	return false;
}
void MMake1Page::CmBrowse ()
{
	return;
}
///////////////////////////////////////////////////////////////////
class MMake2Page : public TPropertyPage {
	public:
		//@cmember MMake2Page
		MMake2Page(TPropertySheet* parent);
		//@cmember ~MMake2Page
		~MMake2Page();

		//@cmember SetActive(
		virtual   int   SetActive(TNotify& );
		//@cmember KillActive(
		virtual   bool 	KillActive(TNotify& );
		//@cmember SetupWindow()
		virtual   void  SetupWindow();
		//@cmember SetPicture
							void 	SetPicture ();

	protected:
		virtual   void 	Help(TNotify& );

	DECLARE_RESPONSE_TABLE(MMake2Page);
	DECLARE_HELPCONTEXT(MMake2Page);
};
/* -------------------------------------------------------------------------- */
// Build a response table for all messages/commands handled
// by the application.
DEFINE_RESPONSE_TABLE1(MMake2Page, TPropertyPage)
//{{MMake2PageRSP_TBL_BEGIN}}
	EV_PSN_SETACTIVE(SetActive),
	EV_PSN_KILLACTIVE(KillActive),
	EV_PSN_HELP(Help),
//{{MMake2PageRSP_TBL_END}}
END_RESPONSE_TABLE;

DEFINE_HELPCONTEXT(MMake2Page)
//	HCENTRY_CONTROL(IDH_MKNEXT, 			ID_NEXT),
//	HCENTRY_CONTROL(IDH_MKBACK, 			ID_BACK),
END_HELPCONTEXT;

//
//
//
MMake2Page::MMake2Page(TPropertySheet* parent)
:
	TPropertyPage(parent,IDD_MAKESECOND)
{
}

//
//
//
MMake2Page::~MMake2Page()
{
	Destroy();
}

//
//
//
void MMake2Page::SetupWindow()
{
	TPropertyPage::SetupWindow();

	THlpWinType HlpType;
  HlpType.pszType=_T("alfXX");
  HlpType.SetStyle(WS_CHILD|WS_MAXIMIZE);
	HlpType.SetProperties(HHWIN_PROP_NOTITLEBAR|HHWIN_PROP_AUTO_SYNC|HHWIN_PROP_TRI_PANE);
  HlpType.SetToolBarFlags(HHWIN_BUTTON_BACK|HHWIN_BUTTON_FORWARD|HHWIN_BUTTON_PRINT);
  HlpType.SetWindowsPos(GetClientRect());

  TCtxtHelpApp* app = TYPESAFE_DOWNCAST(GetApplication(), TCtxtHelpApp);
  if(app){
    string hlpfile = app->GetHelpFile();
  	app->HtmlHelp(this, hlpfile.c_str(), HH_SET_WIN_TYPE, (uint32)&HlpType);
    hlpfile += _T(">alfXX");
  	app->HtmlHelp(this, hlpfile.c_str(), HH_DISPLAY_TOPIC, (DWORD)(LPCTSTR)_T("html/default.htm"));
	}
}

//
//
//
void MMake2Page::Help(TNotify& )
{
	HELPINFO Info;
	Info.cbSize       = sizeof(Info);
	Info.iContextType = HELPINFO_MENUITEM;
	Info.iCtrlId      = IDHELP;
	Info.dwContextId  = 0;
	GetApplication()->GetMainWindow()->SendMessage(WM_HELP, 0, TParam2(&Info));
}

//
//
//
int MMake2Page::SetActive(TNotify&)
{
	SETUP_HELPCONTEXT(TCtxtHelpApp, MMake2Page);
//	GetSheet()->SetWizButtons(PSWIZB_BACK|PSWIZB_NEXT);
	return 0;
}

//
//
//
bool MMake2Page::KillActive(TNotify&)
{
	CLEANUP_HELPCONTEXT(TCtxtHelpApp, MMake2Page);
	return false;
}

///////////////////////////////////////////////////////////////////
class THelpDlg : public TPropertySheet {
	public:
		THelpDlg(TWindow* parent, LPCTSTR title);
		virtual ~THelpDlg();

	public:
		virtual void CleanupWindow();
		virtual void SetupWindow();

//	DECLARE_RESPONSE_TABLE(THelpDlg);
	DECLARE_HELPCONTEXT(THelpDlg);
};

DEFINE_HELPCONTEXT(THelpDlg)
END_HELPCONTEXT;

THelpDlg::THelpDlg(TWindow* parent, LPCTSTR title)
:
	TPropertySheet(parent, title, 0, false, PSH_DEFAULT|PSH_HASHELP)
{
//	TPropertySheet::UseNative = false;
	EnableSubclass(true); // subclass sheet
//  EnableTimer();


	new MMake1Page(this);
	new MMake2Page(this);
//	new MMake3Page(this);
//	new MMake4Page(this);

  new TGlyphButton(this, IDOK, TGlyphButton::btOk);
	new TGlyphButton(this, IDCANCEL, TGlyphButton::btCancel);
  new TGlyphButton(this, ID_APPLY, TGlyphButton::btApply);
	new TGlyphButton(this, IDHELP, TGlyphButton::btHelp);

	// Give it a context menu
	AssignContextMenu(new TPopupMenu(TMenu(*GetModule(), IDM_WHAT_IS_THIS)));
}

THelpDlg::~THelpDlg()
{
}

void THelpDlg::CleanupWindow()
{
	CLEANUP_HELPCONTEXT(TCtxtHelpApp, THelpDlg);

	TPropertySheet::CleanupWindow();
}
void THelpDlg::SetupWindow()
{
	TPropertySheet::SetupWindow();

	SETUP_HELPCONTEXT(TCtxtHelpApp, THelpDlg);
}



//
// class TCtxtDialog
// ~~~~~ ~~~~~~~~~~~
class TCtxtDialog : public TDialog {
  public:
    TCtxtDialog();

   ~TCtxtDialog() { }

    void  SetupWindow();
    void  CleanupWindow();
    void  CmTestHelp();

 	DECLARE_RESPONSE_TABLE(TCtxtDialog);
  DECLARE_HELPCONTEXT(TCtxtDialog);
};

DEFINE_RESPONSE_TABLE1(TCtxtDialog, TDialog)
  EV_BN_CLICKED(IDC_TEST, CmTestHelp),
END_RESPONSE_TABLE;


//
//
//
TCtxtDialog::TCtxtDialog()
:
	TDialog(0, IDD_DIALOG)
{
  new TGlyphButton(this, IDOK, TGlyphButton::btOk);
	AssignContextMenu(new TPopupMenu(TMenu(*GetModule(), IDM_WHAT_IS_THIS)));
}

//
//
//
void
TCtxtDialog::SetupWindow()
{
  TDialog::SetupWindow();
  SETUP_HELPCONTEXT(TCtxtHelpApp, TCtxtDialog);
}

//
//
//
void
TCtxtDialog::CleanupWindow()
{
  CLEANUP_HELPCONTEXT(TCtxtHelpApp, TCtxtDialog);
  TDialog::CleanupWindow();
}

void
TCtxtDialog::CmTestHelp()
{
	THelpDlg(this,"hELP tEST").Execute();
}

DEFINE_HELPCONTEXT(TCtxtDialog)
  HCENTRY_CONTROL(IDH_IDC_TEST,   IDC_TEST),
  HCENTRY_CONTROL(IDH_IDOK,       IDOK),
  HCENTRY_CONTROL(IDH_IDCANCEL,   IDCANCEL),
  HCENTRY_CONTROL(IDH_IDHELP,     IDHELP),
END_HELPCONTEXT;



//
// InitMainWindow
//
void
TCtxtHelpApp::InitMainWindow()
{
  SetMainWindow(new TFrameWindow(0, _T("Test HTML Help"),new TCtxtDialog,true));
  GetMainWindow()->Attr.Style &= ~WS_THICKFRAME;
}

/* -------------------------------------------------------------------------- */
//@mfunc CanClose ()
bool
TCtxtHelpApp::CanClose ()
{
	bool result = TApplication::CanClose();

	// Close the help engine if we used it.
	if (result && HelpState)
		DeactivateHelp();
	return result;
}

//
//
//
bool
TCtxtHelpApp::ProcessAppMsg (MSG& msg)
{
	if(ProcessHelpMsg(msg))
  	return true;
	return TApplication::ProcessAppMsg (msg);
}

DEFINE_RESPONSE_TABLE2(TCtxtHelpApp, THelpFileManager, TApplication)
END_RESPONSE_TABLE;

//
// OwlMain
//
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TCtxtHelpApp("HTMLHELP").Run();
}
//------------------------------------------------------------------------------

