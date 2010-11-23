//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/editfile.h>
#include <owl/mdi.h>
#include <owl/decmdifr.h>
#include <owl/statusba.h>
#include <owl/controlb.h>
#include <owl/buttonga.h>
#include <owl/gdiobjec.h>
#include <owl/bardescr.h>
#include <owl/functor.h>

#include "mdifile.h"
#include <owl/editfile.rh>

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
    TControlBar*             ControlBar;
    int                      ControlBarLoc;

  protected:
    void CmFileNew();
    void CmFileOpen();
    void CmToggleHint();
    void CeToggleHint(TCommandEnabler&);
    void CmToggleBar();
    void CeTestState(TCommandEnabler&);
    TGadgetWindow* CreateToolBar(TWindow* wnd);
    bool EvBarInserter(TDecoratedFrame& frame, TGadgetWindow& destWindow, TAbsLocation loc);

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


TGadgetWindow*
TMDIFileApp::CreateToolBar(TWindow* wnd)
{
  ControlBar = new TControlBar(wnd);
  ControlBar->Attr.Style |= WS_CLIPSIBLINGS;    // since toolbar may move around
	return ControlBar;
}

bool
TMDIFileApp::EvBarInserter(TDecoratedFrame& frame, TGadgetWindow& dst, TAbsLocation )
{
  // Setup the toolbar ID
  if(frame.GetHandle())
  	dst.Create();
  frame.Insert(dst, Location[ControlBarLoc]);
  return true;
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
  SetMainWindow(Frame);

  ControlBar = 0;
  ControlBarLoc = 0;

  Frame->Attr.AccelTable = IDA_MDIFILE;
  Frame->SetMenuDescr(TMenuDescr(IDM_EDITFILE_DOC, 1, 0, 0, 1, 1, 0));
  Frame->SetIcon(this, IDI_MULTIFILE);
  Frame->SetBarCreator(TGadgetWnd_MFUNCTOR(*this, &TMDIFileApp::CreateToolBar));
  Frame->SetBarInserter(TBarInsert_MFUNCTOR(*this,&TMDIFileApp::EvBarInserter));

  // Construct, build and insert a control bar into the frame. Start out at
  // the top of the frame
  //
  HintMode = TGadgetWindow::PressHints;
  Frame->SetBarDescr(new TBarDescr(IDB_EDITFILE_DOC));

  // Construct a status bar & insert it at the bottom
  //
  TStatusBar* sb = new TStatusBar(0, TGadget::Recessed,
           TStatusBar::CapsLock | TStatusBar::NumLock |
           TStatusBar::Overtype | TStatusBar::SizeGrip);

  Frame->Insert(*sb, TDecoratedFrame::Bottom);

  EnableCtl3d(true);

  // Initialize the file data struct used for open and saveas
  //
  FileData.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
  FileData.SetFilter(string(*this, IDS_FILEFILTER).c_str());
}

//
// Respond to "New" command by constructing, creating, and setting up a
// new TFileWindow MDI child
//
void
TMDIFileApp::CmFileNew()
{
  TMDIChild* child = new TMDIChild(*Client, "", new TEditFile(0, 0, 0));
  child->SetMenuDescr(TMenuDescr(IDM_EDITFILE_CHILD, 0, 2, 0, 0, 0, 0));
  child->SetBarDescr(new TBarDescr(IDB_EDITFILE_CHILD));
  child->SetIcon(this, IDI_DOC);

  TMDIChild* curChild = Client->GetActiveMDIChild();
  if (curChild && (curChild->GetWindowLong(GWL_STYLE) & WS_MAXIMIZE))
    child->Attr.Style |= WS_MAXIMIZE;

  child->Create();
}

//
// Respond to "Open" command by constructing, creating, and setting up a
// new TFileWindow MDI child
//
void
TMDIFileApp::CmFileOpen()
{
  *FileData.FileName = 0;
  if (TFileOpenDialog(MainWindow, FileData).Execute() == IDOK) {
    TMDIChild* child = new TMDIChild(*Client, "", new TEditFile(0, 0, 0, 0, 0, 0, 0, FileData.FileName));
    child->SetMenuDescr(TMenuDescr(IDM_EDITFILE_CHILD, 0, 2, 0, 0, 0, 0));
	  child->SetBarDescr(new TBarDescr(IDB_EDITFILE_CHILD));
    child->SetIcon(this, IDI_DOC);

    TMDIChild* curChild = Client->GetActiveMDIChild();
    if (curChild && (curChild->GetWindowLong(GWL_STYLE) & WS_MAXIMIZE))
      child->Attr.Style |= WS_MAXIMIZE;

    child->Create();
  }
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

  ControlBar->SetHintMode(HintMode);
  ControlBar->SetHintCommand(-1);  // make sure we don't leave hint text dangling
}

//
//
//
void
TMDIFileApp::CmToggleBar()
{
  ControlBarLoc = (ControlBarLoc+1) % COUNTOF(Location);
  ControlBar->SetDirection(ControlBarLoc==0 ?
                           TControlBar::Horizontal : TControlBar::Vertical);
  Frame->Insert(*ControlBar, Location[ControlBarLoc]);
  Frame->Layout();
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
