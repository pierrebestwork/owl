//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owl_vclmdiclient.h
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TOWL_VCLMDIClient (TMDIClient).
//
//----------------------------------------------------------------------------
#if !defined(owl_vclmdiclient_h)              // Sentry, use file only if it's not already included.
#define owl_vclmdiclient_h

#include <owl\mdi.h>            // Definition of all resources.

#include "owlusingvclapp.rh"            // Definition of all resources.


//{{TMDIClient = TOWL_VCLMDIClient}}
class TOWL_VCLMDIClient : public TMDIClient {
  public:
    int      ChildCount;                 // Number of child window created.

    TOWL_VCLMDIClient(TModule* module = 0);
    virtual ~TOWL_VCLMDIClient();

    void OpenFile(const char* fileName = 0);

//{{TOWL_VCLMDIClientVIRTUAL_BEGIN}}
  protected:
    virtual void SetupWindow();
//{{TOWL_VCLMDIClientVIRTUAL_END}}

//{{TOWL_VCLMDIClientRSP_TBL_BEGIN}}
  protected:
    void CmFilePrint();
    void CmFilePrintSetup();
    void CmFilePrintPreview();
    void CmPrintEnable(TCommandEnabler& tce);
    void EvDropFiles(TDropInfo);
//{{TOWL_VCLMDIClientRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOWL_VCLMDIClient);
};    //{{TOWL_VCLMDIClient}}


#endif  // owl_vclmdiclient_h sentry.
