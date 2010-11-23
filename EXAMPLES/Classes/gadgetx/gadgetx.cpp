#if defined(BI_COMP_BORLANDC) && defined(BI_STD_RTL) // BI_STD_RTL defined only for Builder
#include <condefs.h>
USERC("gadgetx.rc");
#endif
//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1998 by Bidus Yura, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#pragma hdrstop
#include <owl/dialog.h>
#include <owl/decframe.h>
#include <owl/statusba.h>
#include <owl/controlb.h>
#include <owl/editfile.rh>
#include <owl/btntextg.h>
//#include <owl/popupgad.h>

#include "gadgetx.rh"


TStatusBar*   StatusBar;
TControlBar*  ControlBar;

// This will be for our "main" framewindow
//
class TClientWindow : public TWindow{
	public:
  	TClientWindow(TWindow*parent=0);
  	~TClientWindow();

    void CmNew();
    void CmOpen();
    void CmSave();
    void CmSaveAs();
    void CmAbout();
    void CmUndo();
    void CmCut();
    void CmCopy();
    void CmPaste();
    void CmFind();
    void CmFindNext();

		DECLARE_RESPONSE_TABLE(TClientWindow);
};

DEFINE_RESPONSE_TABLE1(TClientWindow, TWindow)
  EV_COMMAND(CM_ABOUT, 				CmAbout),
  EV_COMMAND(CM_FILENEW, 			CmNew),
  EV_COMMAND(CM_FILEOPEN,   	CmOpen),
  EV_COMMAND(CM_FILESAVE,			CmSave),
  EV_COMMAND(CM_FILESAVEAS,		CmSaveAs),
	EV_COMMAND(CM_EDITUNDO, 	  CmUndo),
	EV_COMMAND(CM_EDITCUT, 		  CmCut),
	EV_COMMAND(CM_EDITCOPY, 	  CmCopy),
  EV_COMMAND(CM_EDITPASTE, 	  CmPaste),
  EV_COMMAND(CM_EDITFIND, 		CmFind),
  EV_COMMAND(CM_EDITFINDNEXT, CmFindNext),
END_RESPONSE_TABLE;

TClientWindow::TClientWindow(TWindow*parent)
:
	TWindow(parent)
{
}

TClientWindow::~TClientWindow()
{
}

void TClientWindow::CmNew(){}
void TClientWindow::CmOpen(){}
void TClientWindow::CmSave(){}
void TClientWindow::CmSaveAs(){}
void TClientWindow::CmUndo(){}
void TClientWindow::CmCut(){}
void TClientWindow::CmCopy(){}
void TClientWindow::CmPaste(){}
void TClientWindow::CmFind(){}
void TClientWindow::CmFindNext(){}


void
TClientWindow::CmAbout()
{
	TDialog(this, IDD_ABOUT).Execute();
}

//
//
//
class TestApp : public TApplication {
	public:
		TestApp() : TApplication() {}
		void InitMainWindow();

};
void
TestApp::InitMainWindow()
{
  TDecoratedFrame* frame = new TDecoratedFrame(0, "TestApp", new TClientWindow(),true);

  TGadgetWindow::EnableFlatStyle(TGadgetWindow::FlatStandard|TGadgetWindow::FlatHotText);
  // or use automatic setting
  //TGadgetWindow::EnableFlatStyle();

	// Create the font
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
  ControlBar = new TControlBar(frame,TControlBar::Horizontal, new TFont(&ncm.lfMessageFont));

  TButtonTextGadget* gd = new TButtonTextGadget(CM_FILENEW, CM_FILENEW);
  gd->SetLayoutStyle(TButtonTextGadget::lTextLeft);
	ControlBar->Insert(*gd);

	ControlBar->Insert(*new TSeparatorGadget);
	gd = new TButtonTextGadget(CM_FILEOPEN, CM_FILEOPEN,TButtonTextGadget::sBitmapText, TButtonTextGadget::Exclusive);
  gd->SetLayoutStyle(TButtonTextGadget::lTextTop);
  ControlBar->Insert(*gd);

  gd = new TButtonTextGadget(CM_FILESAVE, CM_FILESAVE,TButtonTextGadget::sBitmapText, TButtonTextGadget::Exclusive);
  gd->SetLayoutStyle(TButtonTextGadget::lTextBottom);
  ControlBar->Insert(*gd);
	ControlBar->Insert(*new TSeparatorGadget);

  gd = new TButtonTextGadget(CM_FILESAVEAS, CM_FILESAVEAS);
  gd->SetLayoutStyle(TButtonTextGadget::lTextRight);
  ControlBar->Insert(*gd);


	gd = new TButtonTextGadget(CM_EDITUNDO, CM_EDITUNDO);
  gd->SetLayoutStyle(TButtonTextGadget::lTextLeft);
  ControlBar->Insert(*gd);

	gd = new TButtonTextGadget(CM_EDITCUT, CM_EDITCUT);
  gd->SetLayoutStyle(TButtonTextGadget::lTextTop);
  ControlBar->Insert(*gd);

	gd = new TButtonTextGadget(CM_EDITCOPY, CM_EDITCOPY);
  gd->SetLayoutStyle(TButtonTextGadget::lTextBottom);
  ControlBar->Insert(*gd);

	gd = new TButtonTextGadget(CM_EDITPASTE, CM_EDITPASTE,TButtonTextGadget::sBitmapText, TButtonTextGadget::NonExclusive);
  gd->SetLayoutStyle(TButtonTextGadget::lTextBottom);
  ControlBar->Insert(*gd);

	ControlBar->Insert(*new TSeparatorGadget);
	gd = new TButtonTextGadget(CM_EDITFIND, CM_EDITFIND,TButtonTextGadget::sBitmapText, TButtonTextGadget::SemiExclusive);
  gd->SetLayoutStyle(TButtonTextGadget::lTextBottom);
  ControlBar->Insert(*gd);

	gd = new TButtonTextGadget(CM_EDITFINDNEXT, CM_EDITFINDNEXT,TButtonTextGadget::sBitmapText, TButtonTextGadget::SemiExclusive);
  gd->SetLayoutStyle(TButtonTextGadget::lTextBottom);
  ControlBar->Insert(*gd);
	ControlBar->Insert(*new TSeparatorGadget);


/*
  TPopupMenuGadget* gd = new TPopupMenuGadget(TMenu(*this, IDM_TRACKMENU),
                             frame, CM_FILENEW, CM_FILENEW);
  gd->SetArrowWidth(12);
  ControlBar->Insert(*gd);
  gd = new TPopupMenuGadget(TMenu(*this, IDM_TRACKMENU),frame, CM_FILEOPEN,
                            CM_FILEOPEN, TPopupMenuGadget::sBitmapText,
                            TPopupMenuGadget::DownArrow);
  ControlBar->Insert(*gd);
*/


	frame->Insert(*ControlBar, TDecoratedFrame::Top);

  StatusBar = new TStatusBar(frame);
	frame->Insert(*StatusBar, TDecoratedFrame::Bottom);
	frame->AssignMenu(IDM_APPMENU);
	SetMainWindow(frame);
}


int
OwlMain(int /*argc*/, char* /*argv*/[])
{
  return TestApp().Run();
}

#if defined(BI_COMP_BORLANDC) && defined(BI_STD_RTL) // BI_STD_RTL defined only for Builder
#define WinMain
#endif
//==============================================================================

