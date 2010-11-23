//----------------------------------------------------------------------------
//  Project odlistm
//  My Copmany
//  Copyright 1998. All right reserved.
//
//  SUBSYSTEM:    odlistm Application
//  FILE:         odlistmapp.h
//  AUTHOR:       Author
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TodlistmAppl (TApplication).
//  (Generated by OWL 6.0 Wizard for VC.)
//----------------------------------------------------------------------------
#if !defined(odlistmapp_h)              // Sentry, use file only if it's not already included.
#define odlistmapp_h


#include <owl/decframe.h>
#include <owl/opensave.h>


#include "resource.h"            // Definition of all resources.


//
// FrameWindow must be derived to override Paint for Preview and Print.
//
//{{TDecoratedFrame = TSDIDecFrame}}
class TSDIDecFrame : public TDecoratedFrame {
  public:
    TSDIDecFrame(TWindow* parent, LPCTSTR title, TWindow* clientWnd, bool trackMenuSelection = false, TModule* module = 0);
    ~TSDIDecFrame();
//{{TSDIDecFrameVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TSDIDecFrameVIRTUAL_END}}
    int32 EvMenuChar(uint nChar, uint menuType, HMENU hMenu);
DECLARE_RESPONSE_TABLE(TSDIDecFrame);
};    //{{TSDIDecFrame}}


//{{TApplication = TodlistmAppl}}
class TodlistmAppl : public TApplication {
  private:

  public:
    TodlistmAppl();
    virtual ~TodlistmAppl();

    TOpenSaveDialog::TData FileData;  // Data to control open/saveas standard dialog.
    void OpenFile(LPCTSTR fileName = 0);

		int Flag;
//{{TodlistmApplVIRTUAL_BEGIN}}
  public:
    virtual void InitMainWindow();
//{{TodlistmApplVIRTUAL_END}}

//{{TodlistmApplRSP_TBL_BEGIN}}
  protected:
    void CmFileNew();
    void CmFileOpen();
    void CmHelpAbout();
		void CmItem1 ();
		void CeItem1 (TCommandEnabler& tce);
		void CmItem2 ();
		void CeItem2 (TCommandEnabler& tce);
		void CmItem3 ();
		void CeItem3 (TCommandEnabler& tce);
		void CmItem4 ();
		void CeItem4 (TCommandEnabler& tce);
		void CmItem5 ();
		void CeItem5 (TCommandEnabler& tce);
		void CmItem6 ();
		void CeItem6 (TCommandEnabler& tce);
		void CmItem7 ();
		void CeItem7 (TCommandEnabler& tce);
		void CmItem8 ();
		void CeItem8 (TCommandEnabler& tce);
		void CmEnableall ();
		void CeEnableall (TCommandEnabler& tce);
		void CmTestDialog ();
//{{TodlistmApplRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TodlistmAppl);
};    //{{TodlistmAppl}}


#endif  // odlistmapp_h sentry.

