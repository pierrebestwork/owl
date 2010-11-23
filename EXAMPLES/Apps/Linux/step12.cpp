//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//   Tutorial application -- step12.cpp
//----------------------------------------------------------------------------
#include "owlpch.h"

#include <owl/applicat.h>
#include <owl/decframe.h>
#include <owl/dialog.h>
#include <owl/controlb.h>
#include <owl/buttonga.h>
#include <owl/statusba.h>
#include <owl/docmanag.h>
#include <stdlib.h>
#include <string.h>
#include "src/step12.rc"

class TDrawApp : public TApplication {
  public:
    TDrawApp() : TApplication() {}

  protected:
    // Override methods of TApplication
    void InitInstance();
    void InitMainWindow();

    // Event handlers
    void EvNewView  (TView& view);
    void EvCloseView(TView& view);
    void EvDropFiles(TDropInfo dropInfo);
    void CmAbout();
  DECLARE_RESPONSE_TABLE(TDrawApp);
};

DEFINE_RESPONSE_TABLE1(TDrawApp, TApplication)
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_WM_DROPFILES,
  EV_COMMAND(CM_ABOUT, CmAbout),
END_RESPONSE_TABLE;

void
TDrawApp::InitMainWindow()
{
  // Construct the decorated frame window
  TDecoratedFrame* frame = new TDecoratedFrame(0, "Drawing Pad", 0, true);

  // Construct a status bar
  TStatusBar* sb = new TStatusBar(frame, TGadget::Recessed);
  TGadgetWindow::EnableFlatStyle();

  // Insert the status bar and control bar into the frame
  frame->Insert(*sb, TDecoratedFrame::Bottom);

  // Set the main window and its menu
  SetMainWindow(frame);
  GetMainWindow()->SetMenuDescr(TMenuDescr("COMMANDS",1,0,0,0,0,1));
  GetMainWindow()->SetBarDescr(new TBarDescr(IDB_DVSDI));

  // Install the document manager
  SetDocManager(new TDocManager(dmSDI|dmMenu));
}

void
TDrawApp::InitInstance()
{
  TApplication::InitInstance();
  GetMainWindow()->DragAcceptFiles(true);
  GetDocManager()->CmFileNew();
}

void
TDrawApp::EvDropFiles(TDropInfo dropInfo)
{
  if (dropInfo.DragQueryFileCount() != 1)
    ::MessageBox(0, "Can only drop 1 file in SDI mode", "Drag/Drop Error", MB_OK);
  else {
    int fileLength = dropInfo.DragQueryFileNameLen(0)+1;
    char* filePath = new char [fileLength];
    dropInfo.DragQueryFile(0, filePath, fileLength);
    TDocTemplate* tpl = GetDocManager()->MatchTemplate(filePath);
    if (tpl)
      tpl->CreateDoc(filePath);
    delete filePath;
  }
  dropInfo.DragFinish();
}

void
TDrawApp::EvNewView(TView& view)
{
  GetMainWindow()->SetClientWindow(view.GetWindow());
  if (!view.IsOK())
    GetMainWindow()->SetClientWindow(0);
  else{
   if (view.GetViewMenu())
    GetMainWindow()->MergeMenu(*view.GetViewMenu());
   if (view.GetViewBar())
    GetMainWindow()->MergeBar(*view.GetViewBar());
  }
}

void
TDrawApp::EvCloseView(TView& /*view*/)
{
  GetMainWindow()->SetClientWindow(0);
  GetMainWindow()->RestoreMenu();
  GetMainWindow()->RestoreBar();
  GetMainWindow()->SetCaption("Drawing Pad");
}

void
TDrawApp::CmAbout()
{
  TDialog(GetMainWindow(), IDD_ABOUT).Execute();
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TDrawApp().Run();
}
/* ========================================================================== */
