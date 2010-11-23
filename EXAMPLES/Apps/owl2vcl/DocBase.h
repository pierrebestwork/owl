//---------------------------------------------------------------------------
#ifndef DocBaseH
#define DocBaseH
//---------------------------------------------------------------------------

// =========================================================
// DocBase.h
//
// VCL File Document Base class
//
// Author:  Greg Bullock of Mission Research Corporation
//          bullock@mrcmry.com
//
//          May be used without restrictions.
//          If you make improvements to this file, I would greatly appreciate
//          it if you would send them to me.
//
// Written: 5 April 2000
//
//  OVERVIEW
//  ========
//  Class definition for TBasicDocument.
//  This can serve as a base class for all documents using VCL.
// =========================================================

#include <iostream>
#include <Forms.hpp>

//-------------------------------------------------------------------
class TBasicDocument {
//-------------------------------------------------------------------
public:
  TBasicDocument() { MyDialog = 0; }
  virtual ~TBasicDocument() {}

  virtual void get(std::istream& s) {}
  virtual void put(std::ostream& s) const {}

  virtual void SetDialogView(TForm *Dlg);

  friend std::istream& operator >> (std::istream& s, TBasicDocument& d)
    { if (s.good())
        d.get(s);
      return s; }
  friend std::ostream& operator << (std::ostream& s, const TBasicDocument& d)
    { d.put(s); return s; }

  virtual void SetDialogData(TForm *Dlg, TObject *Component = 0)
    {}
  virtual bool GetDialogData(TForm *Dlg, TObject *Component = 0)
    { return true; }

protected:
  TForm *MyDialog;

};
//-------------------------------------------------------------------

#endif
