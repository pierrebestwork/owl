// =========================================================
// VCLDoc.cpp
//
// VCL File Document Wrapper for OWL
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
//  Source file for implementation of TVCLFileDocument (TFileDocument).
// =========================================================

#include "VCLDoc.h"
#include "ViewDlgs.h"
#include <owl/docmanag.h>
#include <owl/filedoc.h>
#include <all_idds.rh>


TVCLFileDocument::TVCLFileDocument(TVCLDocumentFactoryFunc factoryFunc,
    TDocument* parent)
  : TFileDocument(parent),
    Factory(factoryFunc)
{
  TApplication *theApp = GetDocManager().GetApplication();
  if (theApp)
    SetVCLMainWindowHandle(theApp->GetMainWindow()->GetHandle());
  VCLFileDoc = reinterpret_cast<void *>(Factory());
}


TVCLFileDocument::~TVCLFileDocument()
{
  TApplication *theApp = dynamic_cast<TApplication *>(GetDocManager().GetApplication());
  if (theApp && theApp->GetMainWindow())
    theApp->GetMainWindow()->SetCursor(0,IDC_WAIT);
  VCLFileDocumentDestroy(VCLFileDoc);
  VCLFileDoc = 0;
  if (theApp && theApp->GetMainWindow())
    theApp->GetMainWindow()->SetCursor(0,IDC_ARROW);
}


bool TVCLFileDocument::Open(int /*mode*/, const char far* path)
{
  bool result = true;
  if (path)
    SetDocPath(path);
  if (GetDocPath())
    result = LoadData();
  SetDirty(false);
  return result;
}


bool TVCLFileDocument::LoadData()
{
  TApplication *theApp = GetDocManager().GetApplication();
  theApp->GetMainWindow()->SetCursor(0,IDC_WAIT);
  bool result = VCLFileDocumentLoad(VCLFileDoc, GetDocPath());
  theApp->GetMainWindow()->SetCursor(0,IDC_ARROW);

  if (!result)
    PostError(IDS_READERROR, MB_OK);
  return result;
}


