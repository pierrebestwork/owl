//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ExDoc1.h"
#include "ExView1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// Here's a example of a customized TBasicDocument, which is the base
// class for a VCL "document" that can be used in an OWL application.
//-------------------------------------------------------------------
// class TExampleDocument1
//-------------------------------------------------------------------

TExampleDocument1::TExampleDocument1()
{
}
//---------------------------------------------------------------------------


void TExampleDocument1::get(std::istream& s)
{
  // Initialize to default
  *this = TExampleDocument1();

  using namespace std;
  getline(s,Text,string::traits_type::to_char_type(string::traits_type::eof()));
}
//---------------------------------------------------------------------------

void TExampleDocument1::put(std::ostream& s) const
{
  s << Text;
}
//---------------------------------------------------------------------------

void TExampleDocument1::SetDialogData(TForm *Dlg, TObject *Component)
{
  TExampleView1 *ExampleView1 = dynamic_cast<TExampleView1 *>(MyDialog);
  if (!ExampleView1) return;

  ExampleView1->Memo1->Lines->Text = Text.c_str();
}
//---------------------------------------------------------------------------

bool TExampleDocument1::GetDialogData(TForm *Dlg, TObject *Component)
{
  TExampleView1 *ExampleView1 = dynamic_cast<TExampleView1 *>(MyDialog);
  if (!ExampleView1) return false;

  Text = ExampleView1->Memo1->Lines->Text.c_str();
  return true;
}
//---------------------------------------------------------------------------


