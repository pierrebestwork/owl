//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DlgBase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

HWND TDialogViewBaseForm::HWndParent = 0;

//---------------------------------------------------------------------------
__fastcall TDialogViewBaseForm::TDialogViewBaseForm(TComponent* Owner)
  : TForm(Owner)
{
  MyDocument = 0;
  DlgHasChanged = false;
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::CreateParams(Controls::TCreateParams &Params)
{
  if (!IsWindow(HWndParent)) {
    BorderIcons << biSystemMenu << biMinimize << biMaximize;
    Visible = false;
    BorderStyle = bsDialog;
    Position = poScreenCenter;
  }
  inherited::CreateParams(Params);
  Params.WndParent = HWndParent;
  // if there is a parent, then set the style, too
  if (HWndParent)
    Params.Style = (Params.Style | WS_CHILD) & ~WS_POPUP;
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::WMNCDestroy(Messages::TWMNoParams &Message)
{
  inherited::Dispatch(&Message);
  Free();
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::SetDocument(TBasicDocument *Doc)
{
  MyDocument = Doc;
  if (MyDocument)
    MyDocument->SetDialogView(this);
}
//---------------------------------------------------------------------------

int __fastcall TDialogViewBaseForm::GetHasChanged()
{
  for (int i = 0; !DlgHasChanged && (i < ComponentCount); i++) {
    TCustomEdit *Edit = dynamic_cast<TCustomEdit *>(Components[i]);
    if (Edit && Edit->Modified)
      DlgHasChanged = true;
  }
  return DlgHasChanged;
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::SetHasChanged(const bool value)
{
  if (DlgHasChanged == value) return;
  if (!value)
    for (int i = 0; i < ComponentCount; i++) {
      TCustomEdit *Edit = dynamic_cast<TCustomEdit *>(Components[i]);
      if (Edit)
        Edit->Modified = false;
    }
  DlgHasChanged = value;
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::Loaded(void)
{
  // This seems to work around a bug that keeps setting OldCreateOrder=true
  // in inherited forms.
  inherited::Loaded();
  OldCreateOrder = false;
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::FormCreate(TObject *Sender)
{
  // When overriding FormCreate in a decendent class, be sure to call the
  // parent FormCreate *after* performing necessary setup.

  if (IsWindow(HWndParent))
    SetBounds(GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME),
      Width, Height);

  DlgHasChanged = true; // To force the SetHasChange code to clear all TEdits
  SetHasChanged(false);
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::HelpBtnClick(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------

void __fastcall TDialogViewBaseForm::AnyControlChanged(TObject *Sender)
{
  DlgHasChanged = true;
}
//---------------------------------------------------------------------------


