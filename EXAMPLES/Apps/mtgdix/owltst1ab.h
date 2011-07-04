//----------------------------------------------------------------------------
//  Project owltst1
//  My Copmany
//  Copyright 1998. All right reserved.
//
//  SUBSYSTEM:    owltst1 Application
//  FILE:         owltst1ab.h
//  AUTHOR:       Author
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for Towltst1AboutDlg (TDialog).
//  (Generated by OWL 6.0 Wizard for VC.)
//----------------------------------------------------------------------------
#if !defined(owltst1ab_h)              // Sentry, use file only if it's not already included.
#define owltst1ab_h

#include <owl/static.h>

#include "resource.h"                  // Definition of all resources.


//{{TDialog = Towltst1AboutDlg}}
class Towltst1AboutDlg : public TDialog {
  public:
    Towltst1AboutDlg(TWindow* parent, TResId resId = IDD_ABOUTDLG, TModule* module = 0);
    virtual ~Towltst1AboutDlg();

//{{Towltst1AboutDlgVIRTUAL_BEGIN}}
  public:
    void SetupWindow();
//{{Towltst1AboutDlgVIRTUAL_END}}
};    //{{Towltst1AboutDlg}}


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

#endif  // owltst1ab_h sentry.