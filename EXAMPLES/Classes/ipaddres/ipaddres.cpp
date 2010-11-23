//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1998 by Bidus Yura, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_IPADDRES_H)
# include <owl/ipaddres.h>
#endif
#include <owl/dialog.h>
#include <owl/framewin.h>

#include "ipaddres.rh"
//
// Class TSampleApp
// ~~~~~ ~~~~~~~~~~
class TSampleApp : public TApplication {
  public:
    void    InitMainWindow();
};

//
// Class TClientWindow
// ~~~~~ ~~~~~~~~~~~~~
class TClientWindow : public TDialog {
  public:
    TClientWindow(TWindow* parent = 0);
};

TIPAddressBits Data(123,123,33,908);

TClientWindow::TClientWindow(TWindow* parent)
:
 TDialog(parent, IDD_CLIENTDLG, 0)
{
	new TIPAddress(this,IDC_IPADDRESS);
  SetTransferBuffer(&Data);
}

void
TSampleApp::InitMainWindow()
{
  SetMainWindow(new TFrameWindow(0, "IP Address Control",
                                 new TClientWindow(), true));
  GetMainWindow()->ModifyStyle(WS_THICKFRAME,0);
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
 return TSampleApp().Run();
}
