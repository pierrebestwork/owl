//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owl_vclmdichild.cpp
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TOWL_VCLMDIChild (TMDIChild).
//
//----------------------------------------------------------------------------

#include <owl/pch.h>

#include "owlusingvclapp.h"
#include "owl_vclmdichild.h"


//{{TOWL_VCLMDIChild Implementation}}


//--------------------------------------------------------
// TOWL_VCLMDIChild
// ~~~~~~~~~~
// Construction/Destruction handling.
//
TOWL_VCLMDIChild::TOWL_VCLMDIChild(TMDIClient& parent, const char far* title, TWindow* clientWnd, bool shrinkToClient, TModule* module)
:
  TMDIChild(parent, title, clientWnd, shrinkToClient, module)
{
  // INSERT>> Your constructor code here.

}


TOWL_VCLMDIChild::~TOWL_VCLMDIChild()
{
  Destroy();

  // INSERT>> Your destructor code here.

}
