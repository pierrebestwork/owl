//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ExView1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DlgBase"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
// Here's a example of a customized TDialogViewBaseForm, which is the base
// class for a VCL form that can be shown in an OWL application.

__fastcall TExampleView1::TExampleView1(TComponent* Owner)
  : TDialogViewBaseForm(Owner)
{
}
//---------------------------------------------------------------------------
