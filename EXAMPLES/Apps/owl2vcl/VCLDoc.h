#if !defined(__VCLDoc_h)
#define __VCLDoc_h

// =========================================================
// VCLDoc.h
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
//  Class definition for TVCLFileDocument (TFileDocument).
// =========================================================

#include <owl/pch.h>
#pragma hdrstop
#include <owl/filedoc.h>

typedef DWORD (__stdcall *TVCLDocumentFactoryFunc)();

class TVCLFileDocument : public TFileDocument
{
public:
  TVCLFileDocument(TVCLDocumentFactoryFunc factoryFunc, TDocument* parent = 0);
  virtual ~TVCLFileDocument();

  // implement virtual methods of TDocument
  bool  Open(int mode, const char far* path=0);
  bool  IsOpen() { return true; }

  // implement distinguishing methods of TVCLFileDocument
  bool  LoadData();
  void FAR *GetVCLFileDocument() { return VCLFileDoc; }

protected:
  void FAR *VCLFileDoc;

private:
  TVCLDocumentFactoryFunc Factory;
};

#endif
