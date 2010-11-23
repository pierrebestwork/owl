//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owl_vclmdichild.h
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TOWL_VCLMDIChild (TMDIChild).
//
//----------------------------------------------------------------------------
#if !defined(owl_vclmdichild_h)              // Sentry, use file only if it's not already included.
#define owl_vclmdichild_h

#include "owlusingvclapp.rh"            // Definition of all resources.


//{{TMDIChild = TOWL_VCLMDIChild}}
class TOWL_VCLMDIChild : public TMDIChild {
  public:
    TOWL_VCLMDIChild(TMDIClient& parent, const char far* title, TWindow* clientWnd, bool shrinkToClient = false, TModule* module = 0);
    virtual ~TOWL_VCLMDIChild();
};    //{{TOWL_VCLMDIChild}}


#endif  // owl_vclmdichild_h sentry.
