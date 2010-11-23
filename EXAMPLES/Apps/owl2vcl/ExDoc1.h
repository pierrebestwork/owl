//---------------------------------------------------------------------------
#ifndef ExDoc1H
#define ExDoc1H
//---------------------------------------------------------------------------

#include <string>
#include "DocBase.h"

// Here's a example of a customized TBasicDocument, which is the base
// class for a VCL "document" that can be used in an OWL application.

//-------------------------------------------------------------------
class TExampleDocument1 : public TBasicDocument {
//-------------------------------------------------------------------
public:
  TExampleDocument1();

  void get(std::istream& s);
  void put(std::ostream& s) const;
  void SetDialogData(TForm *Dlg, TObject *Component = 0);
  bool GetDialogData(TForm *Dlg, TObject *Component = 0);

  std::string Text;
};

#endif
