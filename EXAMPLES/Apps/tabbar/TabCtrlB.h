//----------------------------------------------------------------------------
//  
//  Copyright © 2000. All Rights Reserved.
//
//  FILE:         tabctrlb.h
//  AUTHOR:       Jogy
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TTabbedControlBar (TLayoutWindow).
//
//----------------------------------------------------------------------------
#if !defined(TABCTRLB_H)              // Sentry, use file only if it's not already included.
#define TABCTRLB_H

#include <owl/layoutwi.h>
#include <owl/toolbox.h>
//#include <owl/tabctrl.h>

#if !defined(OWL_TABBEDTOOLBAR)
#	if defined(UNICODE) // for Resource Workshop
#		define OWL_TABBEDTOOLBAR L"OWL_TabbedToolbar"
#	else
#		define OWL_TABBEDTOOLBAR "OWL_TabbedToolbar"
#	endif
#endif

class TToolBoxList;
class TControlBarList;

class TTabToolBox : public TToolBox
{
  public:
    TTabToolBox(TWindow*        parent,
								TTileDirection  direction = Horizontal,  // Row Major
								TModule*        module = 0)
    : TToolBox(parent, AS_MANY_AS_NEEDED, 2, direction, module) {}
};

//{{TLayoutWindow = TTabbedControlBar}}
class TTabbedControlBar : public TLayoutWindow {
  public:
    TTabbedControlBar(TWindow* parent, TFont* font = 0, TModule* module = 0);
    virtual ~TTabbedControlBar();

		TGadget* FindGadget(int id = -1, bool selectTab = true);

		void InsertTab(TGadgetWindow* tab, LPCTSTR name = 0);
		void InsertToolBox(TTabToolBox* toolbox);

    void ActivateTab(TWindow* tab);

//{{TTabbedControlBarVIRTUAL_BEGIN}}
  public:
		virtual TWindow* GetCurTabWindow();
		virtual void GetTabSize(TSize& rect);
		virtual void SetTabSel(int index);
    virtual void AdjustTabClient();
    virtual void GetDesiredSize(TSize& size);
  protected:
		virtual void GetTabClientRect(TRect& rect);
		virtual TWindow* CreateTabControl();
    virtual void SetTabCaptions();
    virtual void SetMetrics();
    virtual LPCTSTR GetClassName();
    virtual void SetupWindow();
    virtual bool Create();
    virtual bool IdleAction(long idleCount);
    virtual TResult EvCommand(uint id, HWND hWndCtl, uint notifyCode);
    virtual void EvCommandEnable(TCommandEnabler& ce);
    virtual void InitTabs();
//{{TTabbedControlBarVIRTUAL_END}}

		void ShowCurTabWindow();
		void HideCurTabWindow();

//{{TTabbedControlBarRSP_TBL_BEGIN}}
  protected:
    void EvSize(uint sizeType, TSize& size);
    bool EvSelChanging(TNotify&);
    void EvSelChange(TNotify&);
    void EvWindowPosChanging(WINDOWPOS& windowPos);
//{{TTabbedControlBarRSP_TBL_END}}

	protected:
		TWindow*	TabControl;
    TFont*		Font;
  
    TToolBoxList *ToolBoxList;
    TControlBarList *ControlBarList;

DECLARE_RESPONSE_TABLE(TTabbedControlBar);
};    //{{TTabbedControlBar}}


//{{TTabbedControlBar = TNoteTabControlBar}}
class TNoteTabControlBar : public TTabbedControlBar {
  public:
    TNoteTabControlBar(TWindow* parent, TFont* font = 0, TModule* module = 0);

//{{TNoteTabControlBarVIRTUAL_BEGIN}}
  public:
		virtual TWindow* GetCurTabWindow();
		virtual void GetTabSize(TSize& rect);
		virtual void SetTabSel(int index);
  protected:
		virtual void GetTabClientRect(TRect& rect);
    virtual void SetTabCaptions();
		virtual TWindow* CreateTabControl();
    virtual void SetMetrics();
    virtual void InitTabs();
//{{TNoteTabControlBarVIRTUAL_END}}
};    //{{TNoteTabControlBar}}


#endif  // TABCTRLB_H sentry.

