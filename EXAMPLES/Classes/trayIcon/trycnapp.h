//----------------------------------------------------------------------------
//  Project Trayicon
//  
//  Copyright © 1996. All Rights Reserved.
//
//  SUBSYSTEM:    Trayicon Application
//  FILE:         trycnapp.h
//  AUTHOR:       
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TTrayiconApp (TApplication).
//
//----------------------------------------------------------------------------
#if !defined(trycnapp_h)              // Sentry, use file only if it's not already included.
#define trycnapp_h

#include <owl/opensave.h>
#include <owl/decframe.h>
#include <owl/trayicon.h>


#include "trycnapp.rh"            // Definition of all resources.



//
// FrameWindow must be derived to override Paint for Preview and Print.
//
//{{TDecoratedFrame = TSDIDecFrame}}
class TSDIDecFrame : public TDecoratedFrame {
  public:
    TSDIDecFrame(TWindow* parent, const char far* title, TWindow* clientWnd, bool trackMenuSelection = false, TModule* module = 0);
    ~TSDIDecFrame();

  protected:
    void SetupWindow();
    void CleanupWindow();

  protected:
		TResult EvTrayNotification(TParam1 uID, TParam2 lEvent);
		void    CmConfiguration();

	private:
		TTrayIcon* TrayIcon;

	DECLARE_RESPONSE_TABLE(TSDIDecFrame);
};    //{{TSDIDecFrame}}


//{{TApplication = TTrayiconApp}}
class TTrayiconApp : public TApplication {
  public:
    TTrayiconApp();
    virtual ~TTrayiconApp();

//{{TTrayiconAppVIRTUAL_BEGIN}}
  public:
    virtual void InitMainWindow();
//{{TTrayiconAppVIRTUAL_END}}
};    //{{TTrayiconApp}}


#endif  // trycnapp_h sentry.
