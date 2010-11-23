//----------------------------------------------------------------------------
//  Project Trayicon
//
//  Copyright © 1996. All Rights Reserved.
//
//  SUBSYSTEM:    Trayicon Application
//  FILE:         trycnapp.cpp
//  AUTHOR:
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TTrayiconApp (TApplication).
//
//----------------------------------------------------------------------------

#include <owl/pch.h>

#include <stdio.h>

#include "trycnapp.h"


#define WM_ICON_NOTIFY  WM_USER+10


//{{TTrayiconApp Implementation}}

//--------------------------------------------------------
// TTrayiconApp
//
TTrayiconApp::TTrayiconApp()
:
	TApplication("TrayIcon")
{
  // INSERT>> Your constructor code here.
}
TTrayiconApp::~TTrayiconApp()
{
  // INSERT>> Your destructor code here.
}


//--------------------------------------------------------
// TTrayiconApp
// ~~~~~
// Application intialization.
//
void TTrayiconApp::InitMainWindow()
{
  nCmdShow = SW_HIDE;
  TSDIDecFrame* frame = new TSDIDecFrame(0, GetName(), 0, false);

  // Assign icons for this application.
  frame->SetIcon(this, IDI_SDIAPPLICATION);
  frame->SetIconSm(this, IDI_SDIAPPLICATION);

  SetMainWindow(frame);
}

DEFINE_RESPONSE_TABLE1(TSDIDecFrame, TDecoratedFrame)
//{{TUrllinkAppRSP_TBL_BEGIN}}
  EV_MESSAGE(WM_ICON_NOTIFY, EvTrayNotification),
  EV_COMMAND(CM_CONFIG, CmConfiguration),
//{{TUrllinkAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TSDIDecFrame Implementation}}
TSDIDecFrame::TSDIDecFrame(TWindow* parent, const char far* title,
                           TWindow* /*clientWnd*/,
                           bool trackMenuSelection, TModule* module)
:
 TDecoratedFrame(parent, title, 0, trackMenuSelection, module),
  TrayIcon(0)
{
  // INSERT>> Your constructor code here.
	TrayIcon = new TTrayIcon(this, WM_ICON_NOTIFY, string("Test Test Test"),
                           IDI_SDIAPPLICATION, IDM_TEST);
}

TSDIDecFrame::~TSDIDecFrame()
{
	delete TrayIcon;
}

void TSDIDecFrame::SetupWindow()
{
	TDecoratedFrame::SetupWindow();
	TrayIcon->Create();
}
void TSDIDecFrame::CleanupWindow()
{
	TrayIcon->RemoveIcon();
	TDecoratedFrame::CleanupWindow();
}

void TSDIDecFrame::CmConfiguration()
{
	ShowWindow(SW_SHOWNORMAL);
	TrayIcon->HideIcon();
	MessageBox("Configuration");
	TrayIcon->ShowIcon();
	ShowWindow(SW_HIDE);
}

TResult TSDIDecFrame::EvTrayNotification(TParam1 param1, TParam2 param2)
{
	return TrayIcon->EvTrayNotification(param1, param2);
}
//------------------------------------------------------------------------------
int OwlMain(int , char* [])
{
  TTrayiconApp   app;
  return app.Run();
}
/* ========================================================================== */
