//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

// =========================================================
// DocBase.cpp
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
//  Source file for implementation of TBasicDocument.
// =========================================================

#include <sstream>
#include <values.h>

#include "DocBase.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//-------------------------------------------------------------------
// TBasicDocument implementation
//-------------------------------------------------------------------

void TBasicDocument::SetDialogView(TForm *Dlg)
{
  if (Dlg) {
    MyDialog = Dlg;
    SetDialogData(0);
  }
}


