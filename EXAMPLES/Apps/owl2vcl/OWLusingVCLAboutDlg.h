//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owlusingvclaboutdlg.h
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TOWLusingVCLAboutDlg (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(owlusingvclaboutdlg_h)              // Sentry, use file only if it's not already included.
#define owlusingvclaboutdlg_h

#include <owl/static.h>

#include "owlusingvclapp.rh"                  // Definition of all resources.


//{{TDialog = TOWLusingVCLAboutDlg}}
class TOWLusingVCLAboutDlg : public TDialog {
  public:
    TOWLusingVCLAboutDlg(TWindow* parent, TResId resId = IDD_ABOUT, TModule* module = 0);
    virtual ~TOWLusingVCLAboutDlg();

//{{TOWLusingVCLAboutDlgVIRTUAL_BEGIN}}
  public:
    void SetupWindow();
//{{TOWLusingVCLAboutDlgVIRTUAL_END}}
};    //{{TOWLusingVCLAboutDlg}}


// Reading the VERSIONINFO resource.
//
class TProjectRCVersion {
  public:
    TProjectRCVersion(TModule* module);
    virtual ~TProjectRCVersion();

    bool GetProductName(LPSTR& prodName);
    bool GetProductVersion(LPSTR& prodVersion);
    bool GetCopyright(LPSTR& copyright);
    bool GetDebug(LPSTR& debug);

  protected:
    uint8 far*  TransBlock;
    void far*   FVData;

  private:
    // Don't allow this object to be copied.
    //
    TProjectRCVersion(const TProjectRCVersion&);
    TProjectRCVersion& operator = (const TProjectRCVersion&);
};


#endif  // owlusingvclaboutdlg_h sentry.
