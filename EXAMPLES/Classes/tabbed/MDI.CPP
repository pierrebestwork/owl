//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//----------------------------------------------------------------------------
#include "owlpch.h"
#include <owl/applicat.h>
#include <owl/layoutwi.h> //place before owl/tabbed.h 
#include <owl/tabbed.h>
#include <owl/mdi.h>
#include <owl/mdichild.h>
#include <stdio.h>

#include "mdi.rh"

class TMTabbedWindow: public TTabbedWindow {
	public:
  	TMTabbedWindow();
};

TMTabbedWindow::TMTabbedWindow()
{
	TColor colors[] ={
		TColor::Black,
		TColor::Gray,
		TColor::LtBlue,
		TColor::LtCyan,
		TColor::LtGray,
		TColor::LtGreen,
		TColor::LtMagenta,
		TColor::LtRed,
		TColor::LtYellow,
		TColor::Sys3dDkShadow,
		TColor::Sys3dFace,
	};

	Add(*new TWindow(this, "First"));
	Add(*new TWindow(this, "Second"));
	Add(*new TWindow(this, "Third"));
	Add(*new TWindow(this, "Forth"));
	Add(*new TWindow(this, "Fifth"));
	Add(*new TWindow(this, "Sixht"));
	Add(*new TWindow(this, "Seventh"));
	Add(*new TWindow(this, "Eigth"));
	Add(*new TWindow(this, "Ninght"));
	Add(*new TWindow(this, "Tenth"));
	for(int i = 0; i < 10; i++){
    GetPage(i)->SetBkgndColor(colors[i]);
	  GetPage(i)->ModifyExStyle(0,WS_EX_CLIENTEDGE);
	}
	GetTabControl()->SetSel(0);
}


class TNoteTabDlg : public TDialog, public TTabbedBase {
  public:
    TNoteTabDlg(TWindow* parent, TResId id);

    void SetupWindow();
  	DECLARE_RESPONSE_TABLE(TNoteTabDlg);
};

DEFINE_RESPONSE_TABLE2(TNoteTabDlg, TDialog, TTabbedBase)
END_RESPONSE_TABLE;

TNoteTabDlg::TNoteTabDlg(TWindow* parent, TResId id)
:
	TDialog(parent, id),
  TTabbedBase(this)
{
	Add(*new TDialog(this, IDD_TABDLG1),"");
	Add(*new TDialog(this, IDD_TABDLG2),"");
	Add(*new TDialog(this, IDD_TABDLG3),"");
  GetTabControl()->SetSel(0);
}

//
//
//
void
TNoteTabDlg::SetupWindow()
{
  TDialog::SetupWindow();
  SetupPages();
}

//
// class TMyMDIClient
// ~~~~~ ~~~~~~~~~~~~
class TMyMDIClient : public TMDIClient {
  public:
    TMyMDIClient()
    :
      TMDIClient()
    {
      ChildNum = 0;
    }

    void CmCreateChild();
    void CmCreateChild1();
    void CmCreateChild2();
    void CmCreateChild3();
    void CmShowDialog();

  private:
    int ChildNum;

  DECLARE_RESPONSE_TABLE(TMyMDIClient);
};

DEFINE_RESPONSE_TABLE1(TMyMDIClient, TMDIClient)
  EV_COMMAND(CM_CREATECHILD, CmCreateChild),
  EV_COMMAND(CM_CREATECHILD1, CmCreateChild1),
  EV_COMMAND(CM_CREATECHILD2, CmCreateChild2),
  EV_COMMAND(CM_CREATECHILD3, CmCreateChild3),
  EV_COMMAND(CM_SHOWNOTETAB, CmShowDialog),
END_RESPONSE_TABLE;

void
TMyMDIClient::CmCreateChild()
{
  char childName[15];
  sprintf(childName, "MDI Child %d", ChildNum++);
  TMDIChild* child = new TMDIChild(*this, childName, new TMTabbedWindow);
  child->Create();
}

void
TMyMDIClient::CmCreateChild1()
{
  char childName[15];
  sprintf(childName, "MDI Child %d", ChildNum++);
  TMTabbedWindow* wnd = new TMTabbedWindow;
  wnd->GetTabControl()->SetWindowFace(false);
  TMDIChild* child = new TMDIChild(*this, childName, wnd);
  child->Create();
}

void
TMyMDIClient::CmCreateChild2()
{
  char childName[15];
  sprintf(childName, "MDI Child %d", ChildNum++);
  TMTabbedWindow* wnd = new TMTabbedWindow;
  wnd->GetTabControl()->SetStyle3d(false);
  TMDIChild* child = new TMDIChild(*this, childName, wnd);
  child->Create();
}

void
TMyMDIClient::CmCreateChild3()
{
  char childName[15];
  sprintf(childName, "MDI Child %d", ChildNum++);
  TMTabbedWindow* wnd = new TMTabbedWindow;
  wnd->GetTabControl()->SetWindowFace(true);
  wnd->GetTabControl()->SetStyle3d(false);
  wnd->GetTabControl()->SetBkgndColor(TColor::SysWindow);
  TMDIChild* child = new TMDIChild(*this, childName, wnd);
  child->Create();
}

void TMyMDIClient::CmShowDialog()
{
  TNoteTabDlg(this, IDD_NOTETAB).Execute();
}

//
// class TMDIApp
// ~~~~~ ~~~~~~~
class TMDIApp : public TApplication {
  public:
    TMDIApp()
    :
      TApplication("MDI Conformist")
    {
    }

    void InitMainWindow();

};

// Construct the TMDIApp's MainWindow object, loading its menu
//
void
TMDIApp::InitMainWindow()
{
  SetMainWindow(new TMDIFrame(GetName(), IDM_MDIMENU, *new TMyMDIClient));
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TMDIApp().Run();
}
