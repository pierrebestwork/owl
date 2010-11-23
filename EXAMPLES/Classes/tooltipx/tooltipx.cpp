//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
// Illustrates usage of TTooltip class
//----------------------------------------------------------------------------

#include <owl/statusba.h>
#include <owl/controlb.h>
#include <owl/decframe.h>
#include <owl/dialog.h>
#include <owl/gadgetwi.h>
#include <owl/combobox.h>
#include <owl/controlg.h>
#include <owl/buttonga.h>
#include <owl/tooltip.h>
#include <owl/static.h>
#include <owl/button.h>
#include <owl/edit.h>

#include <stdio.h>
#include "tooltip.h"

TRect redRect   = TRect(10, 10, 100, 100);
TRect blueRect  = TRect(110, 10, 200, 100);
TRect yellowRect= TRect(10, 110, 100, 200);
TRect greenRect = TRect(110, 110, 200, 200);

//
// class TSampleApp
// ~~~~~ ~~~~~~~~~~
class TSampleApp : public TApplication {
  public:
    TSampleApp(){}

    // Override virtuals from TApplication
    //
    void    InitMainWindow();
};

//
class TAboutDlg : public TDialog {
  public:
    TAboutDlg(TWindow* parent, TModule* module = 0);
    virtual ~TAboutDlg();

  public:
    virtual void SetupWindow();
    virtual void CleanupWindow();

		void OkTipText(TTooltipText far& ttText);
		void CeIdOk(TCommandEnabler& tce);
		void CeIdEdit(TCommandEnabler& tce);

  TStatic* versionCtrl;
  TStatic* copyrightCtrl;
  TStatic* debugCtrl;
  TButton* buttonCtrl;
  TEdit*	 editCtrl;
    
  DECLARE_RESPONSE_TABLE(TAboutDlg);
}; //{{TAboutDlg}}

//////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(TAboutDlg,TDialog)
  EV_TTN_NEEDTEXT(IDOK, OkTipText),
//  EV_COMMAND_ENABLE(IDOK, CeIdOk),
  EV_COMMAND_ENABLE(IDC_EDIT, CeIdEdit),
END_RESPONSE_TABLE;

TAboutDlg::TAboutDlg(TWindow* parent, TModule* module)
:
  TDialog(parent, IDD_ABOUT, module)
{
  versionCtrl   = new TStatic(this, IDC_VERSION, 255);
  copyrightCtrl = new TStatic(this, IDC_COPYRIGHT, 255);
  debugCtrl     = new TStatic(this, IDC_DEBUG, 255);
  buttonCtrl    = new TButton(this, IDOK);
	editCtrl      = new TEdit(this, IDC_EDIT);
}
//
TAboutDlg::~TAboutDlg()
{
  Destroy();
}
//
void TAboutDlg::SetupWindow()
{
  LPSTR prodName = 0, prodVersion = 0, copyright = 0, debug = 0;

  // Get the static text for the value based on VERSIONINFO.
  //
  TDialog::SetupWindow();

  // Process the VERSIONINFO.
  //
	TModuleVersionInfo applVersion(*GetApplication());

  // Get the product name and product version strings.
  //
  if (applVersion.GetProductName(prodName) && applVersion.GetProductVersion(prodVersion)) {
    // IDC_VERSION is the product name and version number, the initial value of IDC_VERSION is
    // the word Version(in whatever language) product name VERSION product version.
    //
    _TCHAR buffer[255];
    _TCHAR versionName[128];

    buffer[0] = _T('\0');
    versionName[0] = _T('\0');

    versionCtrl->GetText(versionName, sizeof versionName);
    wsprintf(buffer, _T("%s %s %s"), prodName, versionName, prodVersion);

    versionCtrl->SetText(buffer);
  }

  // Get the legal copyright string.
  //
  if (applVersion.GetLegalCopyright(copyright))
    copyrightCtrl->SetText(copyright);

  // Only get the SpecialBuild text if the VERSIONINFO resource is there.
  //
  if (applVersion.GetSpecialBuild(debug))
    debugCtrl->SetText(debug);

	// use Global Applicaiton Tooltip
	// Another way would be create our local tooltip
  TTooltip* tooltip = GetApplication()->GetTooltip();
  if (tooltip && tooltip->GetHandle()){
		TToolInfo tiDeb(*this, *debugCtrl);
		// need for Dialog controls in Modal(!!!) dialog
		// because modal dialog not allows PreprocessMsg() call
		// You don't need this in Modless dialog,
		// Subcclassing slow down applicaion.
		tiDeb.EnableSubclassing();
		tooltip->AddTool(tiDeb);

		TToolInfo tiBut(*this, *buttonCtrl);
		tiBut.EnableSubclassing();// need for Dialog controls
		tooltip->AddTool(tiBut);
		
		TToolInfo tiEd(*this, *editCtrl);
		tiEd.EnableSubclassing();// need for Dialog controls
		tooltip->AddTool(tiEd);
	}
}
//
void TAboutDlg::CleanupWindow()
{
	// You must do cleanup of global tooltip
  TTooltip* tooltip = GetApplication()->GetTooltip();
  if (tooltip && tooltip->GetHandle()){

		TToolInfo tiBut(*this, *buttonCtrl);
		tooltip->DeleteTool(tiBut);

		TToolInfo tiEd(*this, *editCtrl);
		tooltip->DeleteTool(tiEd);
	}
	TDialog::CleanupWindow();
}
//
void TAboutDlg::CeIdEdit(TCommandEnabler& tce)
{
	// Note there are can be two types of TCommandEnablers in Dialog: 
	// Control enabler and Tooltip Enabler
	if(typeid(tce)==typeid(TControlEnabler))
		// we don't want set text in Edit control
		;//tce.SetText("Edit ControlEnabler"); 
	else 
		tce.SetText("Edit TooltipEnabler");
}
//
void TAboutDlg::OkTipText(TTooltipText far& ttText)
{
  // Provide tiptext string using constant string
  TTooltipText& foo = *(TTooltipText*)&ttText;
  foo.CopyText("OkTipText");
}
//
void TAboutDlg::CeIdOk(TCommandEnabler& tce)
{
	// Note there are two types of TCommandEnabler in Dialog: 
	//  Control enabler and Tooltip Enabler
	if(typeid(tce)==typeid(TControlEnabler))
		// we don't want set text in Button control
		;//tce.SetText("Ok ControlEnabler");
	else
		tce.SetText("Ok TooltipEnabler");
}

////////////////////////////////////////////////////////////////////////
//
// class TClientWindow
// ~~~~~ ~~~~~~~~~~~~~
class TClientWindow : public TWindow {
  public:
    TClientWindow(TWindow* parent= 0);

  protected:

    void            Paint(TDC& dc, bool erase, TRect& rect);
    void            SetupWindow();
		void            CmHelpAbout();

    // Command handlers
    //
    void            CmToolbarTip();
    void            CeToolbarTip(TCommandEnabler&);
    void            CmWindowTip();
    void            CeWindowTip(TCommandEnabler&);

    // Notification handlers
    //
    void            GreenTipText(TTooltipText far& ttText);
    void            YellowTipText(TTooltipText far& ttText);
		void						CeYellowTipText(TCommandEnabler& tce);
		void						CeGreenTipText(TCommandEnabler& tce);

    // Flags whether tooltip's active
    //
    bool            ToolbarTipActive;
    bool            WindowTipActive;

  DECLARE_RESPONSE_TABLE(TClientWindow);
};


//////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(TClientWindow, TWindow)
//  EV_TTN_NEEDTEXT(ID_YELLOWTOOL, YellowTipText),
  EV_COMMAND_ENABLE(ID_YELLOWTOOL, CeYellowTipText),
  EV_TTN_NEEDTEXT(ID_GREENTOOL, GreenTipText),
//  EV_COMMAND_ENABLE(ID_GREENTOOL, CeGreenTipText),

  EV_COMMAND(CM_TOOLBARTIP, CmToolbarTip),
  EV_COMMAND_ENABLE(CM_TOOLBARTIP, CeToolbarTip),
  EV_COMMAND(CM_WINDOWTIP, CmWindowTip),
  EV_COMMAND_ENABLE(CM_WINDOWTIP, CeWindowTip),
  EV_COMMAND(CM_HELPABOUT, CmHelpAbout),

END_RESPONSE_TABLE;

//
//
//
TClientWindow::TClientWindow(TWindow* parent) 
              :TWindow(parent), ToolbarTipActive(true), WindowTipActive(true)
{
}

//
//
//
void
TClientWindow::SetupWindow()
{
  TWindow::SetupWindow();

	// use Global Application Tooltip
  TTooltip* tooltip = GetApplication()->GetTooltip();
  if (tooltip && tooltip->GetHandle()) {

    // Add tooltip for red rectangle - For this tip, we'll provide
    // a string when adding the tool.
    // NOTE: Since TToolInfo will cache the text, make sure that the
    //       object's lifetime is valid for the lifetime of the tooltip
    //
    TToolInfo tiRed(*this, redRect, ID_REDTOOL, "Red rectangle");
    tooltip->AddTool(tiRed);

    // Add tooltip for blue rectangle specifying string resource
    //
    TToolInfo tiBlue(*this, blueRect, ID_BLUETOOL,
                    ID_BLUETOOL, *GetApplication());
    tooltip->AddTool(tiBlue);

    // Add tooltip for yellow rectangle leaving out the text.
    // Tooltip will send TTN_NEEDTEXT notification at runtime
    //
    TToolInfo tiYellow(*this, yellowRect, ID_YELLOWTOOL);
    tooltip->AddTool(tiYellow);

    // Add tooltip for green rectangle leaving out the text.
    // Tooltip will send TTN_NEEDTEXT notification at runtime
    //
    TToolInfo tiGreen(*this, greenRect, ID_GREENTOOL);
    tooltip->AddTool(tiGreen);

		// Note wee don't use EnableSubclassing here,
		// Subclassing will slow down application
  }
}

//
void TClientWindow::CmHelpAbout()
{
	TAboutDlg(this).Execute();
}	

//
//
//
void
TClientWindow::Paint(TDC& dc, bool /*erase*/, TRect& /*rect*/)
{
  dc.TextRect(blueRect, TColor(0, 0, 0xff));
  dc.TextRect(redRect, TColor(0xff, 0, 0));
  dc.TextRect(yellowRect, TColor(0xff, 0xff, 0));
  dc.TextRect(greenRect, TColor(0, 0xff, 0));
}

//
//
//
void
TClientWindow::GreenTipText(TTooltipText far& ttText)
{
  // Provide tiptext string using constant string
  //
  TTooltipText& foo = *(TTooltipText*)&ttText;
  foo.CopyText("Green Rectangle");
}

//
//
//
void
TClientWindow::YellowTipText(TTooltipText far& ttText)
{
  // Provide tiptext using a string resource
  //
  TTooltipText& foo = *(TTooltipText*)&ttText;
  foo.SetText(ID_YELLOWTOOL, *GetApplication());
}
//
void						
TClientWindow::CeYellowTipText(TCommandEnabler& tce)
{		 
	tce.SetText("Tooltip Enable YellowTipText");
  tce.Enable(true);
}
//
void						
TClientWindow::CeGreenTipText(TCommandEnabler& tce)
{
	tce.SetText("Tooltip Enable GreenTipText");
  tce.Enable(true);
}
//
void            
TClientWindow::CmToolbarTip()
{
  ToolbarTipActive = !ToolbarTipActive;
  TWindow* tb = GetApplication()->GetMainWindow()->ChildWithId(IDW_TOOLBAR);
  if (tb) {
    TGadgetWindow *gw = TYPESAFE_DOWNCAST(tb, TGadgetWindow);
    if (gw)
      gw->EnableTooltip(ToolbarTipActive);
  }
}

void            
TClientWindow::CeToolbarTip(TCommandEnabler &ce)
{
  ce.SetCheck(ToolbarTipActive ? TCommandEnabler::Checked :
                                 TCommandEnabler::Unchecked);
}

void            
TClientWindow::CmWindowTip()
{
  WindowTipActive = !WindowTipActive;
	GetApplication()->EnableTooltip(WindowTipActive);
}

void            
TClientWindow::CeWindowTip(TCommandEnabler &ce)
{
  ce.SetCheck(WindowTipActive ? TCommandEnabler::Checked :
                                TCommandEnabler::Unchecked);
}

//----------------------------------------------------------------------------

void
TSampleApp::InitMainWindow()
{
  // Setup main window + client
  //
	TDecoratedFrame* frame = new TDecoratedFrame(0, _T("Tooltip Example"), new TClientWindow());
  SetMainWindow(frame);

  // Create decoration objects
  //
  TStatusBar*  sbar = new TStatusBar(GetMainWindow());
  TControlBar* cbar = new TControlBar(GetMainWindow());
  cbar->Attr.Id = IDW_TOOLBAR;

  // Insert gadgets in control bar
  //
  cbar->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  cbar->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  cbar->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
  cbar->Insert(*new TButtonGadget(CM_FILEPRINT, CM_FILEPRINT));

  cbar->Insert(*new TSeparatorGadget);
  cbar->Insert(*new TButtonGadget(CM_EDITUNDO, CM_EDITUNDO));
  cbar->Insert(*new TButtonGadget(CM_EDITCUT, CM_EDITCUT));
  cbar->Insert(*new TButtonGadget(CM_EDITCOPY, CM_EDITCOPY));
  cbar->Insert(*new TButtonGadget(CM_EDITPASTE, CM_EDITPASTE));

	//Y.B.  Tooltip for Combobox works. Wow !!!
	// See Resource String for ID_COMBOBOX
  cbar->Insert(*new TSeparatorGadget);
  TComboBox* cBox = new TComboBox(0, ID_COMBOBOX, 0, 0, 180, 150, 
                                  CBS_DROPDOWNLIST, -1);
  cbar->Insert(*new TControlGadget(*cBox));

  // Insert decorations in main window
  //
  frame->Insert(*cbar, TDecoratedFrame::Top);
  frame->Insert(*sbar, TDecoratedFrame::Bottom);

  frame->AssignMenu(IDM_MAINMENU);

  // Create instance of Application's Tooltip 
  // 
  EnableTooltip();
}

//
//
//
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TSampleApp().Run();
}
//
