//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1995 by Borland International, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/dialog.h>
#include <owl/framewin.h>
#if !defined(OWL_GLYPHBTN_H)
# include <owl/glyphbtn.h>
#endif
#if !defined(OWL_CHECKBOX_H)
# include <owl/checkbox.h>
#endif
#if !defined(OWL_COLOR_H)
# include <owl/color.h>
#endif
#include "glyphDem.rh"

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

    void SetupWindow() {
      TDialog::SetupWindow();
      EnableHelp->Check();
    }

    void CmEnableHelp();

  private:
    TCheckBox* EnableHelp;
    bool       bEnable;

		TGlyphButton* Button1;
		TGlyphButton* Button2;
		TGlyphButton* Button3;
	  TGlyphButton* Button4;
		TGlyphButton* Button5;
	  TGlyphButton* Button6;
	  TGlyphButton* Button7;
	  TGlyphButton* Button8;
	  TGlyphButton* Button9;
	  TGlyphButton* Button10;
	  TGlyphButton* Button11;
	  TGlyphButton* Button12;
	  TGlyphButton* Button13;
		TGlyphButton* Button14;
	  TGlyphButton* Button15;
		TGlyphButton* Button16;
	  TGlyphButton* Button17;
		TGlyphButton* Button18;
		TButton* 			Button19;

    void CeButton1 (TCommandEnabler&	tce);
    void CeButton2 (TCommandEnabler&	tce);
    void CeButton3 (TCommandEnabler&	tce);
    void CeButton4 (TCommandEnabler&	tce);
    void CeButton5 (TCommandEnabler&	tce);
    void CeButton6 (TCommandEnabler&	tce);
    void CeButton7 (TCommandEnabler&	tce);
    void CeButton8 (TCommandEnabler&	tce);
    void CeButton9 (TCommandEnabler&	tce);
    void CeButton10 (TCommandEnabler&	tce);
    void CeButton11 (TCommandEnabler&	tce);
    void CeButton12 (TCommandEnabler&	tce);
    void CeButton13 (TCommandEnabler&	tce);
    void CeButton14 (TCommandEnabler&	tce);
    void CeButton15 (TCommandEnabler&	tce);
    void CeButton16 (TCommandEnabler&	tce);
    void CeButton17 (TCommandEnabler&	tce);
    void CeButton18 (TCommandEnabler&	tce);
    void CeButton19 (TCommandEnabler&	tce);

  DECLARE_RESPONSE_TABLE(TClientWindow);
};

DEFINE_RESPONSE_TABLE1(TClientWindow, TWindow)
  EV_COMMAND(IDC_ENABLEHELP, 		CmEnableHelp),
  EV_COMMAND_ENABLE(IDOK, 			CeButton1),
  EV_COMMAND_ENABLE(IDCANCEL,   CeButton2),
  EV_COMMAND_ENABLE(IDYES, 		  CeButton3),
  EV_COMMAND_ENABLE(IDNO, 		  CeButton4),
  EV_COMMAND_ENABLE(IDC_CLOSE,	CeButton5),
  EV_COMMAND_ENABLE(IDABORT, 		CeButton6),
  EV_COMMAND_ENABLE(IDIGNORE,   CeButton7),
  EV_COMMAND_ENABLE(IDHELP, 	  CeButton8),
  EV_COMMAND_ENABLE(IDC_APPLY,	CeButton9),
  EV_COMMAND_ENABLE(IDC_BROWSE, CeButton10),
  EV_COMMAND_ENABLE(IDC_REVERT, CeButton11),
  EV_COMMAND_ENABLE(IDC_ADD,    CeButton12),
  EV_COMMAND_ENABLE(IDC_DELETE, CeButton13),
  EV_COMMAND_ENABLE(IDC_EDIT,   CeButton14),
  EV_COMMAND_ENABLE(IDC_KEY,    CeButton15),
  EV_COMMAND_ENABLE(IDC_SETUP, 	CeButton16),
  EV_COMMAND_ENABLE(IDC_BUTTON3,CeButton17),
  EV_COMMAND_ENABLE(IDC_MHELP, 	CeButton18),
  EV_COMMAND_ENABLE(IDC_BUTTON2,CeButton19),
END_RESPONSE_TABLE;

TClientWindow::TClientWindow(TWindow* parent)
:
 TDialog(parent, IDD_CLIENTDLG, 0),
 bEnable(true)
{
  Button1 = new TGlyphButton(this, IDOK, TGlyphButton::btOk);                 // Ok
  Button1->SetLayoutStyle(TGlyphButton::lsH_SGST);
  Button2 = new TGlyphButton(this, IDCANCEL, TGlyphButton::btCancel);         // Cancel
  Button2->SetLayoutStyle(TGlyphButton::lsH_GST);

  // user defined cell array
  Button3 = new TGlyphButton(this, IDYES);                               // Yes
  Button3->SetGlyph(IDB_BORBTNII_OK, GetApplication(), TGlyphButton::gtCellArray);
  Button3->SetLayoutStyle(TGlyphButton::lsH_STSG);
  Button4 = new TGlyphButton(this, IDNO, TGlyphButton::btNo);          // No
  Button4->SetLayoutStyle(TGlyphButton::lsH_TSGS);

  Button5 = new TGlyphButton(this, IDC_CLOSE,TGlyphButton::btClose);    //Close
  Button5->SetLayoutStyle(TGlyphButton::lsV_SGST);
  Button6 = new TGlyphButton(this, IDABORT, TGlyphButton::btAbort);    //Abort
  Button6->SetLayoutStyle(TGlyphButton::lsV_GST);
  Button7 = new TGlyphButton(this, IDIGNORE,TGlyphButton::btIgnore);
  Button7->SetLayoutStyle(TGlyphButton::lsV_STSG);
  Button8 = new TGlyphButton(this, IDHELP, TGlyphButton::btHelp);
  Button8->SetLayoutStyle(TGlyphButton::lsV_TSGS);
  Button9 = new TGlyphButton(this, IDC_APPLY, TGlyphButton::btApply);
  Button10 = new TGlyphButton(this, IDC_BROWSE, TGlyphButton::btBrowse);
  Button11 = new TGlyphButton(this, IDC_REVERT, TGlyphButton::btRevert);
  Button12 = new TGlyphButton(this, IDC_ADD, TGlyphButton::btAdd);
  Button13 = new TGlyphButton(this, IDC_DELETE, TGlyphButton::btDelete);
  Button14 = new TGlyphButton(this, IDC_EDIT, TGlyphButton::btEdit);
  Button15 = new TGlyphButton(this, IDC_KEY, TGlyphButton::btKey);
  Button16 = new TGlyphButton(this, IDC_SETUP, TGlyphButton::btSetup);

  Button17 = new TGlyphButton(this, IDC_BUTTON3);

  Button18 = new TGlyphButton(this, IDC_MHELP);
  Button18->SetGlyph(IDB_BORBTNII_HELP,GetApplication(),TGlyphButton::gtUp);
  Button18->SetGlyph(IDB_BORBTNII_HELPG,GetApplication(),TGlyphButton::gtDisabled);

  EnableHelp = new TCheckBox(this, IDC_ENABLEHELP);

  Button19 = new TButton(this, IDC_BUTTON2);

  SetBkgndColor(TColor::Sys3dFace);
}

//------------------------------------------------------------------------------
void
TClientWindow::CmEnableHelp()
{
	bEnable = !bEnable;
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton1 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton2 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton3 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton4 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton5 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton6 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton7 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton8 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton9 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton10 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton11 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton12 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton13 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton14 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton15 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton16 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton17 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton18 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------
void TClientWindow::CeButton19 (TCommandEnabler& tce)
{
	tce.Enable(bEnable);
}
//------------------------------------------------------------------------------

void
TSampleApp::InitMainWindow()
{
  SetMainWindow(new TFrameWindow(0, "Bitmap Button",
                                 new TClientWindow(), true));
  GetMainWindow()->Attr.Style &= ~WS_THICKFRAME;
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TSampleApp().Run();
}
