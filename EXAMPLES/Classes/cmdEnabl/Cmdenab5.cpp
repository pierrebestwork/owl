// ---------------------------------------------------------------------------
// ObjectWindows
// Copyright (C) 1994, 1995 by Borland International, All Rights Reserved
//
//  Command-enabling sample application
//  Fifth version adds a dialog enablers.
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/dialog.h>
#include <owl/framewin.h>
#include <owl/decframe.h>
#include <owl/controlb.h>
#include <owl/btntextg.h>
#include <owl/statusba.h>
#include <owl/checkbox.h>
#include <owl/radiobut.h>
#include "cmdenabl.h"

//
// class TCmdEnableApp
// ~~~~~ ~~~~~~~~~~~~~
// Application object initializes TApplication, overrides InitMainWindow.
//
class TCmdEnableApp : public TApplication {
  public:
    TCmdEnableApp() : TApplication() {}

  protected:
    void InitMainWindow();
 
};


//
// class TCmdEnableDialog
// ~~~~~ ~~~~~~~~~~~~~~~~
//
class TCmdEnableDialog: public TDialog{
	public:
		TCmdEnableDialog(TWindow* parent, TResId resId, TModule* module = 0);

    void CeCheckBox1(TCommandEnabler&);
    void CeCheckBox2(TCommandEnabler&);
    void CeCheckBox3(TCommandEnabler&);
    void CeRadioButton1(TCommandEnabler&);
    void CeRadioButton2(TCommandEnabler&);

	protected:
 		TCheckBox*		Check1;
 		TCheckBox*		Check2;
 		TCheckBox*	  Check3;
    TRadioButton* Radio1;
    TRadioButton* Radio2;

  DECLARE_RESPONSE_TABLE(TCmdEnableDialog);
};


//
// class TCmdEnableWindow
// ~~~~~ ~~~~~~~~~~~~~~~~
// Window object initializes TWindow, adds four event handlers,
// adds response table for four events.
class TCmdEnableWindow : public TWindow {
  public:
    TCmdEnableWindow(TWindow* parent = 0);

  protected:
    // Event handlers
    //
    void CmFileNew();
    void CmFileOpen();
    void CmFileSave();
    void CmFileSaveAs();
    void CmToggleDirty();
    void CmToggleNew();
    void CmShowState();
    void CmShowDialog();

    // Command-enabling functions
    //
    void CeFileNew(TCommandEnabler&);
    void CeFileOpen(TCommandEnabler&);
    void CeFileSave(TCommandEnabler&);
    void CeToggleDirty(TCommandEnabler&);
    void CeToggleNew(TCommandEnabler&);

    bool IsDirty;
    bool IsNewFile;

  DECLARE_RESPONSE_TABLE(TCmdEnableWindow);
};

//
// TCmdEnableApp::InitMainWindow
//
void
TCmdEnableApp::InitMainWindow()
{
  // Construct the decorated frame window
  //
  TDecoratedFrame* frame = new TDecoratedFrame(0, "Command-enabling sample application",
    new TCmdEnableWindow, true);

	// Create the font
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

  TGadgetWindow::EnableFlatStyle(); // let system choose better settings

  //  Construct a control bar
  //
  TControlBar* cb = new TControlBar(frame,TControlBar::Horizontal, new TFont(&ncm.lfMessageFont));
  cb->Insert(*new TButtonTextGadget(CM_FILENEW, CM_FILENEW, TButtonTextGadget::sBitmapText, TButtonTextGadget::Command));
  cb->Insert(*new TButtonTextGadget(CM_FILEOPEN, CM_FILEOPEN, TButtonTextGadget::sBitmapText, TButtonTextGadget::Command));
  cb->Insert(*new TButtonTextGadget(CM_FILESAVE, CM_FILESAVE, TButtonTextGadget::sBitmapText, TButtonTextGadget::Command));
  cb->Insert(*new TButtonTextGadget(CM_FILESAVEAS, CM_FILESAVEAS, TButtonTextGadget::sBitmapText, TButtonTextGadget::Command));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonTextGadget(CM_TOGGLEDIRTY, CM_TOGGLEDIRTY, TButtonTextGadget::sBitmapText, TButtonTextGadget::NonExclusive));
  cb->Insert(*new TButtonTextGadget(CM_TOGGLENEW, CM_TOGGLENEW, TButtonTextGadget::sBitmapText, TButtonTextGadget::NonExclusive));
  cb->Insert(*new TButtonTextGadget(CM_SHOWSTATE, CM_SHOWSTATE, TButtonTextGadget::sBitmapText, TButtonTextGadget::Command));

  // Turn hints on to fly-over
  //
  cb->SetHintMode(TGadgetWindow::EnterHints);

  // Create a status bar
  //
  TStatusBar* sb = new TStatusBar(frame);

  // Insert the control bar and status bar into the frame
  //
  frame->Insert(*cb, TDecoratedFrame::Top);
  frame->Insert(*sb, TDecoratedFrame::Bottom);

  // Set the main window and its menu
  //
  SetMainWindow(frame);
  GetMainWindow()->AssignMenu(MN_COMMANDS);
}

//
//
//
DEFINE_RESPONSE_TABLE1(TCmdEnableDialog, TDialog)
  EV_COMMAND_ENABLE(IDC_CHECKBOX1, CeCheckBox1),
  EV_COMMAND_ENABLE(IDC_CHECKBOX2, CeCheckBox2),
  EV_COMMAND_ENABLE(IDC_CHECKBOX3, CeCheckBox3),
  EV_COMMAND_ENABLE(IDC_RADIOBUTTON1, CeRadioButton1),
  EV_COMMAND_ENABLE(IDC_RADIOBUTTON2, CeRadioButton2),
END_RESPONSE_TABLE;

TCmdEnableDialog::TCmdEnableDialog(TWindow* parent, TResId resId, TModule* module)
:
	TDialog(parent, resId, module)
{
	Check1 = new TCheckBox(this,IDC_CHECKBOX1);
 	Check2 = new TCheckBox(this,IDC_CHECKBOX2);
 	Check3 = new TCheckBox(this,IDC_CHECKBOX3);
  Radio1 = new TRadioButton(this, IDC_RADIOBUTTON1);
  Radio2 = new TRadioButton(this, IDC_RADIOBUTTON2);
}

void
TCmdEnableDialog::CeCheckBox1(TCommandEnabler& tce)
{
	tce.SetText(Check1->GetCheck()==BF_CHECKED ? "Disable":"Enable");
}
void
TCmdEnableDialog::CeCheckBox2(TCommandEnabler& tce)
{
	tce.Enable(Check1->GetCheck()==BF_CHECKED);
}
void
TCmdEnableDialog::CeCheckBox3(TCommandEnabler& tce)
{
	tce.Enable(Check1->GetCheck()==BF_CHECKED && Check2->GetCheck()==BF_CHECKED);
}
void
TCmdEnableDialog::CeRadioButton1(TCommandEnabler& tce)
{
	tce.Enable(Check1->GetCheck()==BF_CHECKED && Check3->GetCheck()==BF_CHECKED);
}
void
TCmdEnableDialog::CeRadioButton2(TCommandEnabler& tce)
{
	tce.Enable(Check1->GetCheck()==BF_CHECKED && Check3->GetCheck()==BF_CHECKED);
}

//
//
//
DEFINE_RESPONSE_TABLE1(TCmdEnableWindow, TWindow)
  EV_COMMAND(CM_FILENEW, CmFileNew),
  EV_COMMAND(CM_FILEOPEN, CmFileOpen),
  EV_COMMAND(CM_FILESAVE, CmFileSave),
  EV_COMMAND(CM_FILESAVEAS, CmFileSaveAs),
  EV_COMMAND(CM_TOGGLEDIRTY, CmToggleDirty),
  EV_COMMAND(CM_TOGGLENEW, CmToggleNew),
  EV_COMMAND(CM_SHOWSTATE, CmShowState),
  EV_COMMAND(CM_DIALOG, 	 CmShowDialog),
  EV_COMMAND_ENABLE(CM_FILENEW, CeFileNew),
  EV_COMMAND_ENABLE(CM_FILEOPEN, CeFileOpen),
  EV_COMMAND_ENABLE(CM_FILESAVE, CeFileSave),
  EV_COMMAND_ENABLE(CM_TOGGLEDIRTY, CeToggleDirty),
  EV_COMMAND_ENABLE(CM_TOGGLENEW, CeToggleNew),
END_RESPONSE_TABLE;


//
//
//
TCmdEnableWindow::TCmdEnableWindow(TWindow* parent)
:
  TWindow(parent),
  IsNewFile(true),
  IsDirty(false)
{
}


//
//
//
void
TCmdEnableWindow::CmFileNew()
{
  IsDirty   = false;
  IsNewFile = true;
  MessageBox("Opened a new file.\nIsDirty=false\nIsNewFile=true",
    "File action taken");
}


//
//
//
void
TCmdEnableWindow::CmFileOpen()
{
  IsDirty   = false;
  IsNewFile = false;
  MessageBox("Opened an existing file.\nIsDirty=false\nIsNewFile=false",
    "File action taken");
}


//
//
//
void
TCmdEnableWindow::CmFileSave()
{
  IsDirty   = false;
  IsNewFile = false;
  MessageBox("Saved an existing file.\nIsDirty=false\nIsNewFile=false",
    "File action taken");
}


//
//
//
void
TCmdEnableWindow::CmFileSaveAs()
{
  IsDirty   = false;
  IsNewFile = false;
  MessageBox("Saved a file under a new name.\nIsDirty=false\nIsNewFile=false",
    "File action taken");
}


//
//
//
void
TCmdEnableWindow::CmToggleDirty()
{
  IsDirty=!IsDirty;
}


//
//
//
void
TCmdEnableWindow::CmToggleNew()
{
  IsNewFile=!IsNewFile;
}


//
//
//

void
TCmdEnableWindow::CmShowState()
{
  string str(IsDirty ? "IsDirty = true\n" : "IsDirty = false\n");
  str += (IsNewFile ? "IsNewFile = true" : "IsNewFile = false");
  ::MessageBox(GetHandle(),str.c_str(), "Current state",MB_OK);
}


void
TCmdEnableWindow::CmShowDialog()
{
	TCmdEnableDialog(this,IDD_DIALOG1).Execute();
}

//
//
//
void
TCmdEnableWindow::CeFileNew(TCommandEnabler& ce)
{
  // Enable CmFileNew if not dirty
  //
  ce.Enable(!IsDirty);
}


//
//
//
void
TCmdEnableWindow::CeFileOpen(TCommandEnabler& ce)
{
  // Enable CmFileOpen if not dirty
  //
  ce.Enable(!IsDirty);
}


//
//
//
void
TCmdEnableWindow::CeFileSave(TCommandEnabler& ce)
{
  // Enable CmFileSave if not new file and is dirty.
  //
  ce.Enable(!IsNewFile && IsDirty);
}


//
//
//
void
TCmdEnableWindow::CeToggleDirty(TCommandEnabler& ce)
{
  ce.SetCheck(IsDirty ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}


//
//
//
void
TCmdEnableWindow::CeToggleNew(TCommandEnabler& ce)
{
  ce.SetCheck(IsNewFile ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}


//
// Put the OwlMain here just to get it out of the way
//
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TCmdEnableApp().Run();
}


/*
//First, catch the EV_WM_ACTIVATEAPP message in your main application, then...
void TMyApp::EvActivateApp(bool active, HANDLE threadId)
{
 if(active)
  GetMainWindow()->SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

}

*/