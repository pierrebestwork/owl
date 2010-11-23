//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
// Illustrates usage of Toolbar resource
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_DECFRAME_H)
# include <owl/decframe.h>
#endif
#if !defined(OWL_STATUSBA_H)
# include <owl/statusba.h>
#endif
#if !defined(OWL_CONTROLB_H)
# include <owl/controlb.h>
#endif
#if !defined(OWL_EDITFILE_H)
# include <owl/editfile.h>
#endif
#if !defined(OWL_BARDESCR_H)
# include <owl/bardescr.h>
#endif
#include <stdio.h>

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

void
TSampleApp::InitMainWindow()
{
  // Setup main window + client
  //
  TEditFile* edit = new TEditFile;
  edit->ModifyExStyle(0, WS_EX_CLIENTEDGE);
	TDecoratedFrame* frame = new TDecoratedFrame(0, 0, edit);

  SetMainWindow(frame);
  frame->SetBarDescr(new TBarDescr(1000));

  // Create decoration objects
  //
  TStatusBar* sbar = new TStatusBar(GetMainWindow());

  // Insert decorations in main window
	frame->Insert(*sbar, TDecoratedFrame::Bottom);
  frame->AssignMenu(100);

}

//
//
//
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TSampleApp().Run();
}

