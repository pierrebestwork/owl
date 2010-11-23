#include "owlpch.h"
#pragma hdrstop

#include "help.h"

#include "app.rh"            // Definition of all resources.

THelpDialog::THelpDialog(TWindow* parent, TResId resId, TModule* module)
: TDialog(parent, resId == TResId(0) ? TResId(IDD_HELP) : resId, module)
{
  gbOk = new TGlyphButton(this, IDOK, TGlyphButton::btOk);                 // Ok
  gbOk->SetLayoutStyle(TGlyphButton::lsH_GST);
}

THelpDialog::~THelpDialog()
{
  Destroy();
}


