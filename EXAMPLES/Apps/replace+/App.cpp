#include "owlpch.h"
#pragma hdrstop

#include <owl/applicat.h>
#include <owl/cmdline.h>
#include <owl/decframe.h>

#include "app.h"
#include "client.h"          // Definition of client class.
#include "util.h"


//-----------------------------------------------------------------------------
ReplaceApp::ReplaceApp()
: 
  TApplication("ReplacePlus 1.0 by Jogy")
{
}

//-----------------------------------------------------------------------------
ReplaceApp::~ReplaceApp()
{
}

//-----------------------------------------------------------------------------
void ReplaceApp::InitInstance()
{
  ProcessCmdLine(CmdLine.c_str());

  TApplication::InitInstance();
}

//-----------------------------------------------------------------------------
void ReplaceApp::InitMainWindow()
{
  if (nCmdShow != SW_HIDE)
    nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  ReplaceClient *client = new ReplaceClient(0);

  TDecoratedFrame* frame = new TDecoratedFrame(0, GetName(), client, false);
  frame->SetFlag(wfShrinkToClient);

  // Assign icons for this application.
  //
  frame->SetIcon(this, IDI_SDIAPPLICATION);
  frame->SetIconSm(this, IDI_SDIAPPLICATION);

  frame->ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_THICKFRAME,WS_DLGFRAME);

  SetMainWindow(frame);
}

//-----------------------------------------------------------------------------
void ReplaceApp::ProcessCmdLine(const char *CmdLine)
{
  TCmdLine cmd(CmdLine);

  ReplaceData &replaceData = ReplaceData::GetInstance();

  while (cmd.Kind != TCmdLine::Done) {
    if (cmd.Kind == TCmdLine::Option) {
      if (stricmp(cmd.Token, "r+") == 0)
        replaceData.SetRecurse(true);
      else if (stricmp(cmd.Token, "r-") == 0)
        replaceData.SetRecurse(false);
    }
    else if (cmd.Kind == TCmdLine::Name) {
      replaceData.AddString(cmd.Token, cmd.TokenLen);
    }
    cmd.NextToken();
  }
}

////////////////////////////////////////////////////////////////////////////////
int OwlMain(int, char* [])
{
  ReplaceApp app;
  return app.Run();
}
//-----------------------------------------------------------------------------
