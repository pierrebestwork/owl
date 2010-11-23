//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1998 by Bidus Yura, All Rights Reserved
//----------------------------------------------------------------------------

#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/dialog.h>

#if !defined(OWL_DATETIME_H)
# include <owl/datetime.h>
#endif

#include "datetime.rh"

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

TDateTimePickerData Data(TSystemTime(2007,6,20));

TClientWindow::TClientWindow(TWindow* parent)
:
 TDialog(parent, IDD_CLIENTDLG, 0)
{
	new TDateTimePicker(this,IDC_DATEPICKER);
  SetTransferBuffer(&Data);
}

void
TSampleApp::InitMainWindow()
{
  SetMainWindow(new TFrameWindow(0, "DateTime Picker",
                                 new TClientWindow(), true));
  GetMainWindow()->ModifyStyle(WS_THICKFRAME,0);
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
 return TSampleApp().Run();
}
