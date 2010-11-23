//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owlusingvclapp.h
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TOWLusingVCLApp (TApplication).
//
//----------------------------------------------------------------------------
#if !defined(owlusingvclapp_h)              // Sentry, use file only if it's not already included.
#define owlusingvclapp_h

#include <owl/controlb.h>
#include <owl/docking.h>
#include <owl/mailer.h>
#include <owl/printer.h>
#include <owl/rcntfile.h>
#include <owl/decmdifr.h>
//#include <classlib/bags.h>

#include "owl_vclmdiclient.h"

#include "owlusingvclapp.rh"            // Definition of all resources.

//{{TApplication = TOWLusingVCLApp}}
class TOWLusingVCLApp : public TApplication, public TRecentFiles {
  private:
	bool            HelpState;                          // Has the help engine been used.
	bool            ContextHelp;                        // SHIFT-F1 state(context sensitive HELP).
	TCursor*        HelpCursor;                         // Context sensitive help cursor.

	void SetupSpeedBar(TDecoratedMDIFrame* frame);
    void AddFiles(TFileDropletList& files);
    void ProcessCmdLine(char * CmdLine);
    void RegisterInfo();
    void UnRegisterInfo();

  public:
    TOWLusingVCLApp();
    virtual ~TOWLusingVCLApp();

    void CreateGadgets(TDockableControlBar* cb, bool server = false);
    THarbor*        ApxHarbor;

    TOWL_VCLMDIClient* MdiClient;

    // Public data members used by the print menu commands and Paint routine in MDIChild.
    //
    TPrinter*       Printer;                           // Printer support.
    int             Printing;                          // Printing in progress.

    TMailer*        ApxMail;

//{{TOWLusingVCLAppVIRTUAL_BEGIN}}
  public:
    virtual void InitMainWindow();
    virtual void InitInstance();
    virtual bool CanClose();
    virtual bool ProcessAppMsg(MSG& msg);
//{{TOWLusingVCLAppVIRTUAL_END}}

//{{TOWLusingVCLAppRSP_TBL_BEGIN}}
  protected:
    void EvNewView(TView& view);
    void EvCloseView(TView& view);
    void CeFileSend(TCommandEnabler& ce);
    void CmFileSend();
    void CmHelpAbout();
    void CmHelpContents();
    void CmHelpUsing();
    void EvDropFiles(TDropInfo drop);
    void EvWinIniChange(char far* section);
    void EvOwlDocument(TDocument& doc);
    int32 CmFileSelected(uint wp, int32 lp);
//{{TOWLusingVCLAppRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOWLusingVCLApp);
};    //{{TOWLusingVCLApp}}


#endif  // owlusingvclapp_h sentry.
