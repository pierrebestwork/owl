//---------------------------------------------------------------------------
#ifndef ExView1H
#define ExView1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "DlgBase.h"
//---------------------------------------------------------------------------
// Here's a example of a customized TDialogViewBaseForm, which is the base
// class for a VCL form that can be shown in an OWL application.

class TExampleView1 : public TDialogViewBaseForm
{
__published:	// IDE-managed Components
  TMemo *Memo1;
  TCheckBox *CheckBox1;
private:	// User declarations
public:		// User declarations
  __fastcall TExampleView1(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
