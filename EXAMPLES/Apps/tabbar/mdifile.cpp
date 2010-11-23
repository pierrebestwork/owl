//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//
//----------------------------------------------------------------------------
#include "owlpch.h"
#include <owl/controlb.h>
#include <owl/buttonga.h>
#include <owl/statusba.h>
#include <owl/editfile.h>
#include <owl/controlg.h>
#include <owl/decframe.h>
#include <owl/editfile.rh>
#include <owl/mdi.h>
#include <owl/decmdifr.h>

#include "tabctrlb.h"
#include "mdifile.h"

//
//
//
const char AppName[] = "MDI File Editor";

const TDecoratedFrame::TLocation Location[] = {
  TDecoratedFrame::Top,
  TDecoratedFrame::Left,
  TDecoratedFrame::Right
};

//
//
//
class TMDIFileApp : public TApplication {
  public:
	TMDIFileApp(const char* name) : TApplication(name) {}
    void InitMainWindow();

  protected:
    TOpenSaveDialog::TData   FileData;
	TMDIClient*              Client;
    TGadgetWindow::THintMode HintMode;
    TDecoratedFrame*         Frame;
    //TControlBar*             ControlBar;
		TWindow*								 ControlBar;
    int                      ControlBarLoc;

  protected:
    void CmFileNew();
    void CmFileOpen();
    void CmToggleHint();
    void CeToggleHint(TCommandEnabler&);
    void CmToggleBar();
    void CeTestState(TCommandEnabler&);

  DECLARE_RESPONSE_TABLE(TMDIFileApp);

};

DEFINE_RESPONSE_TABLE1(TMDIFileApp, TApplication)
  EV_COMMAND(CM_FILENEW, CmFileNew),
  EV_COMMAND(CM_FILEOPEN, CmFileOpen),
  EV_COMMAND(CM_TOGGLEHINT, CmToggleHint),
  EV_COMMAND_ENABLE(CM_TOGGLEHINT, CeToggleHint),
  EV_COMMAND(CM_TOGGLEBAR, CmToggleBar),
  EV_COMMAND_ENABLE(CM_TESTSTATE, CeTestState),
END_RESPONSE_TABLE;


//
//
//
static TWindow* //TControlBar*
BuildControlBar(TWindow* parent, TControlBar::TTileDirection direction)
{
/*
  TControlBar* cb = new TControlBar(parent,direction);
  cb->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  cb->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
  cb->Insert(*new TButtonGadget(CM_FILESAVEAS, CM_FILESAVEAS));

  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_EDITUNDO, CM_EDITUNDO));
  cb->Insert(*new TButtonGadget(CM_EDITCUT, CM_EDITCUT));
  cb->Insert(*new TButtonGadget(CM_EDITCOPY, CM_EDITCOPY));
  cb->Insert(*new TButtonGadget(CM_EDITPASTE, CM_EDITPASTE));

  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_EDITFIND, CM_EDITFIND));
  cb->Insert(*new TButtonGadget(CM_EDITREPLACE, CM_EDITREPLACE));
  cb->Insert(*new TButtonGadget(CM_EDITFINDNEXT, CM_EDITFINDNEXT));

  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_TOGGLEHINT, CM_TOGGLEHINT,
                                TButtonGadget::NonExclusive));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_TOGGLEBAR, CM_TOGGLEBAR));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_TESTSTATE, CM_TESTSTATE));
*/
  //TTabbedControlBar* tcb = new TTabbedControlBar(parent);
	TNoteTabControlBar* tcb = new TNoteTabControlBar(parent);

	TTabToolBox *tb1 = new TTabToolBox(0);
  tb1->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  tb1->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  tb1->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  tb1->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));

  TTabToolBox *tb2 = new TTabToolBox(0);
  tb2->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  tb2->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  tb2->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  tb2->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));

  tcb->InsertToolBox(tb1);
  tcb->InsertToolBox(tb2);

  TControlBar *cb1 = new TControlBar(0);
  cb1->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  cb1->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  cb1->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  cb1->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));

  TControlBar *cb2 = new TControlBar(0);
  cb2->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  cb2->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
  cb2->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
  cb2->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));

  tcb->InsertTab(cb1, "Tab1");
  tcb->InsertTab(cb2, "Tab2");
  tcb->ModifyStyle(0,WS_CLIPSIBLINGS);    // since toolbar may move around
  tcb->Attr.Id = IDW_TOOLBAR;            // From MDIFILE.H, so we can hide it

//	cb->Insert(*new TControlGadget(*tcb));

//  cb->ModifyStyle(0,WS_CLIPSIBLINGS);    // since toolbar may move around
//  cb->Attr.Id = IDW_TOOLBAR;            // From MDIFILE.H, so we can hide it

  return tcb;
}

//
// Construct the TMDIFileApp's MainWindow of class TMDIFileWindow,
// loading its menu, accelerator table & icon
//
void
TMDIFileApp::InitMainWindow()
{
  Client = new TMDIClient;
  Frame = new TDecoratedMDIFrame(Name, IDM_EDITFILE_DOC, *Client, true);
  MainWindow = Frame;
  
  TGadgetWindow::EnableFlatStyle();

  Frame->Attr.AccelTable = IDA_MDIFILE;
  Frame->SetMenuDescr(TMenuDescr(IDM_EDITFILE_DOC, 1, 0, 0, 1, 1, 0));
  Frame->SetIcon(this, IDI_MULTIFILE);

  // Construct, build and insert a control bar into the frame. Start out at
  // the top of the frame
  //
  HintMode = TGadgetWindow::PressHints;
  ControlBar = BuildControlBar(Frame, TControlBar::Horizontal);
  ControlBarLoc = 0;
  Frame->Insert(*ControlBar, Location[ControlBarLoc]);

  // Construct a status bar & insert it at the bottom
  //
  TStatusBar* sb = new TStatusBar(0, TGadget::Recessed,
           TStatusBar::CapsLock | TStatusBar::NumLock |
           TStatusBar::Overtype | TStatusBar::SizeGrip,
					 new TDefaultGUIFont);

  Frame->Insert(*sb, TDecoratedFrame::Bottom);

  EnableCtl3d(true);

  // Initialize the file data struct used for open and saveas
  //
  FileData.Flags |= OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
  FileData.SetFilter(LoadString(IDS_FILEFILTER).c_str());
}

//
// Respond to "New" command by constructing, creating, and setting up a
// new TFileWindow MDI child
//
void
TMDIFileApp::CmFileNew()
{
  TEditFile* editFile = new TEditFile(0, 0, 0);
  editFile->ModifyExStyle(0,WS_EX_CLIENTEDGE);	
  TMDIChild* child = new TMDIChild(*Client, "", editFile);
  child->SetMenuDescr(TMenuDescr(IDM_EDITFILE_CHILD, 0, 2, 0, 0, 0, 0));
  child->SetIcon(this, IDI_DOC);

  TMDIChild* curChild = Client->GetActiveMDIChild();
  if (curChild && (curChild->GetWindowLong(GWL_STYLE) & WS_MAXIMIZE))
    child->Attr.Style |= WS_MAXIMIZE;

  child->Create();
}
int CALLBACK OWL_EXPORT16
HookDlgProc(HWND hDlg, uint msg, TParam1 param1, TParam2 param2)
{
  return 0;  // default value returned when exception caught
}
//--------------------------------------------------------------------------------------
//
// Respond to "Open" command by constructing, creating, and setting up a
// new TFileWindow MDI child
//
void
TMDIFileApp::CmFileOpen()
{
//	GetFileLocation("Titles", 0);


	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for filename
	*szFile = 0;
	
	// Initialize OPENFILENAME
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize       = sizeof ofn;
	ofn.hwndOwner         = MainWindow->GetHandle();
	ofn.hInstance 				= GetHandle();
	ofn.lpstrFilter       = "All\0*.*\0Text\0*.TXT\0";
	ofn.nMaxFile          = _MAX_PATH;
	ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrDefExt       = _T("txt");
	ofn.Flags             = OFN_HIDEREADONLY|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ENABLESIZING;
	ofn.lpstrFile					= szFile;
	ofn.lpstrTitle				= _T("Save");

	if(!GetOpenFileName(&ofn)){
	   int error = TCommDlg::CommDlgExtendedError();
		 error = error;
	}

/*
  *FileData.FileName = 0;
  if (TFileOpenDialog(MainWindow, FileData).Execute() == IDOK) {
    TMDIChild* child = new TMDIChild(*Client, "", new TEditFile(0, 0, 0, 0, 0, 0, 0, FileData.FileName));
    child->SetMenuDescr(TMenuDescr(IDM_EDITFILE_CHILD, 0, 2, 0, 0, 0, 0));
    child->SetIcon(this, IDI_DOC);

    TMDIChild* curChild = Client->GetActiveMDIChild();
    if (curChild && (curChild->GetWindowLong(GWL_STYLE) & WS_MAXIMIZE))
      child->Attr.Style |= WS_MAXIMIZE;

    child->Create();
  }
*/
}

//
//
//
void
DoCreateChild(TWindow* win, void*)
{
  if (win->IsFlagSet(wfAutoCreate))
    win->Create();
}

//
//
//
void
TMDIFileApp::CeToggleHint(TCommandEnabler& ce)
{
  ce.SetCheck(HintMode == TGadgetWindow::EnterHints);
}

//
//
//
void
TMDIFileApp::CmToggleHint()
{
  HintMode = HintMode==TGadgetWindow::PressHints ?
               TGadgetWindow::EnterHints :
               TGadgetWindow::PressHints;

//  ControlBar->SetHintMode(HintMode);
//  ControlBar->SetHintCommand(-1);  // make sure we don't leave hint text dangling
}

//
//
//
void
TMDIFileApp::CmToggleBar()
{
//  ControlBarLoc = (ControlBarLoc+1) % COUNTOF(Location);
//  ControlBar->SetDirection(ControlBarLoc==0 ?
//                           TControlBar::Horizontal : TControlBar::Vertical);
//  Frame->Insert(*ControlBar, Location[ControlBarLoc]);
//  Frame->Layout();
}

//
//
//
void
TMDIFileApp::CeTestState(TCommandEnabler& ce)
{
  ce.Enable();
  ce.SetCheck(TCommandEnabler::Indeterminate);
}

//
//
//
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TMDIFileApp(AppName).Run();
}
#if defined(BI_COMP_BORLANDC) && defined(BI_STD_RTL) // BI_STD_RTL defined only for Builder
#define WinMain
#endif

