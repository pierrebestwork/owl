//----------------------------------------------------------------------------
//
//  Copyright © 2000. All Rights Reserved.
//
//  FILE:         tabctrlb.cpp
//  AUTHOR:       Jogy
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TTabbedControlBar (TLayoutWindow).
//
//----------------------------------------------------------------------------
#include "owlpch.h"

#if !defined(OWL_TABCTRL_H)
# include <owl/tabctrl.h>
#endif
#if !defined(OWL_NOTETAB_H)
# include <owl/notetab.h>
#endif
#if !defined(OWL_TOOLBOX_H)
# include <owl/toolbox.h>
#endif
#if !defined(OWL_CONTAIN_H)
# include <owl/contain.h>
#endif
#if !defined(OWL_UIMETRIC_H)
# include <owl/uimetric.h>
#endif

#include "tabctrlb.h"



class TToolBoxList : public TIPtrArray<TTabToolBox*>{
	public:
		TToolBoxList() {}
};

class TControlBarList : public TIPtrArray<TGadgetWindow*>{
	public:
		TControlBarList() {}
};
//-----------------------------------------------------------------------------
// class TInnerTabControl
// ~~~~~ ~~~~~~~~~~~~~~~~
class TInnerTabControl : public TTabControl {
  public:
    TInnerTabControl(TWindow* parent,
                  int             id,
                  int x, int y, int w, int h,
                  TModule*        module = 0)
    : TTabControl(parent, id, x, y, w, h, module) {}

    TInnerTabControl(TWindow* parent, int resourceId, TModule* module = 0)
			: TTabControl(parent, resourceId, module) {}

  protected:
    TResult EvCommand(uint id, HWND hWndCtl, uint notifyCode);
    void EvCommandEnable(TCommandEnabler& ce);
};
//-----------------------------------------------------------------------------
TResult TInnerTabControl::EvCommand(uint id, HWND hWndCtl, uint notifyCode)
{
//  TRACEX(OwlCmd, 1, "TInnerTabControl::EvCommand - id(" << id << "), ctl(" <<\
//                     hex << uint(hWndCtl) << "), code(" << notifyCode  << ")");

  TWindow* target;
  TFrameWindow* frame;

  // Find the frame who is our latest ancestor and make it our command target
  //
  for (target = GetParentO(); target; target = target->GetParentO()) {
    frame = TYPESAFE_DOWNCAST(target, TFrameWindow);
    if (frame || !target->GetParentO())
      break;
  }

  // Make sure the frame doesn't think we are its command target, or a BAD
  // loop will happen
  //
  if (target && (!frame || frame->GetCommandTarget() != GetHandle())) {
    CHECK(target->IsWindow());
    return target->EvCommand(id, hWndCtl, notifyCode);
  }

  // If all command routing fails, go back to basic dispatching of TWindow
  //
  return TTabControl::EvCommand(id, hWndCtl, notifyCode);
}
//-----------------------------------------------------------------------------
void TInnerTabControl::EvCommandEnable(TCommandEnabler& ce)
{
  TWindow* target = GetParentO();

  // Forward to command target if the enabler was really destined for us, and
  // not a routing from the frame.
  //
  if (target && ce.IsReceiver(*this)) {
    CHECK(target->IsWindow());
    ce.SetReceiver(*target);
    target->EvCommandEnable(ce);
    if (ce.GetHandled())
      return;
  }

  // Default to TWindow's implementation if the above routing fails
  //
  TTabControl::EvCommandEnable(ce);
}
//-----------------------------------------------------------------------------
// class TInnerNoteTab
// ~~~~~ ~~~~~~~~~~~~~
class TInnerNoteTab : public TNoteTab {
  public:
    TInnerNoteTab(TWindow* parent,
                  int      id,
                  int x, int y, int w, int h,
                  TModule* module = 0)
    : TNoteTab(parent, id, x, y, w, h, 0, true, module) {}

    TInnerNoteTab(TWindow* parent, int resourceId, TModule* module = 0)
			: TNoteTab(parent, resourceId, 0, true, module) {}

  protected:
    TResult EvCommand(uint id, HWND hWndCtl, uint notifyCode);
    void EvCommandEnable(TCommandEnabler& ce);
	friend class TNoteTabControlBar;
};
//-----------------------------------------------------------------------------
TResult TInnerNoteTab::EvCommand(uint id, HWND hWndCtl, uint notifyCode)
{
//  TRACEX(OwlCmd, 1, "TInnerNoteTab::EvCommand - id(" << id << "), ctl(" <<\
//                     hex << uint(hWndCtl) << "), code(" << notifyCode  << ")");

  TWindow* target;
  TFrameWindow* frame;

  // Find the frame who is our latest ancestor and make it our command target
  //
  for (target = GetParentO(); target; target = target->GetParentO()) {
    frame = TYPESAFE_DOWNCAST(target, TFrameWindow);
    if (frame || !target->GetParentO())
      break;
  }

  // Make sure the frame doesn't think we are its command target, or a BAD
  // loop will happen
  //
  if (target && (!frame || frame->GetCommandTarget() != GetHandle())) {
    CHECK(target->IsWindow());
    return target->EvCommand(id, hWndCtl, notifyCode);
  }

  // If all command routing fails, go back to basic dispatching of TWindow
  //
  return TNoteTab::EvCommand(id, hWndCtl, notifyCode);
}
//-----------------------------------------------------------------------------
void TInnerNoteTab::EvCommandEnable(TCommandEnabler& ce)
{
  TWindow* target = GetParentO();

  // Forward to command target if the enabler was really destined for us, and
  // not a routing from the frame.
  //
  if (target && ce.IsReceiver(*this)) {
    CHECK(target->IsWindow());
    ce.SetReceiver(*target);
    target->EvCommandEnable(ce);
    if (ce.GetHandled())
      return;
  }

  // Default to TWindow's implementation if the above routing fails
  //
  TNoteTab::EvCommandEnable(ce);
}
//-----------------------------------------------------------------------------

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TTabbedControlBar, TLayoutWindow)
//{{TTabbedControlBarRSP_TBL_BEGIN}}
  EV_WM_SIZE,
  EV_WM_WINDOWPOSCHANGING,
//{{TTabbedControlBarRSP_TBL_END}}
  EV_TCN_SELCHANGING(-1, EvSelChanging),
  EV_TCN_SELCHANGE(-1, EvSelChange),
END_RESPONSE_TABLE;


//{{TTabbedControlBar Implementation}}

//-----------------------------------------------------------------------------
TTabbedControlBar::TTabbedControlBar(TWindow* parent, TFont* font, TModule* module)
:
  TLayoutWindow(parent, 0, module)
{
  Font = font ? font : new TDefaultGUIFont();

  ToolBoxList		 = new TToolBoxList;
  ControlBarList = new TControlBarList;

  // Make sure we don't paint or erase over any child windows
  //
  ModifyStyle(0,WS_CLIPCHILDREN/*|WS_BORDER*/);

  SetBkgndColor(TColor::Sys3dFace);

  SetFlag(wfInsertAtEdge);
}
//-----------------------------------------------------------------------------
TTabbedControlBar::~TTabbedControlBar()
{
  Destroy(IDCANCEL);

  delete ToolBoxList;
  delete ControlBarList;

  delete Font;
}
//-----------------------------------------------------------------------------
LPCTSTR TTabbedControlBar::GetClassName()
{
  return OWL_TABBEDTOOLBAR;
}
//-----------------------------------------------------------------------------
void TTabbedControlBar::SetupWindow()
{
  TLayoutWindow::SetupWindow();

  TabControl->SetWindowFont(*Font, false);

  SetTabCaptions();
  SetMetrics();
  InitTabs();
}
//-----------------------------------------------------------------------------
void TTabbedControlBar::EvSize(uint sizeType, TSize& size)
{
  TLayoutWindow::EvSize(sizeType, size);

  if (size.cx > 0 && size.cy > 0)
    AdjustTabClient();
}
//-----------------------------------------------------------------------------
bool
TTabbedControlBar::EvSelChanging(TNotify&)
{
  HideCurTabWindow();
  return false;
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::EvSelChange(TNotify&)
{
	ShowCurTabWindow();
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::ShowCurTabWindow()
{
	TWindow* curTabWindow = GetCurTabWindow();
  if (curTabWindow)
  	AdjustTabClient();
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::HideCurTabWindow()
{
	TWindow* curTabWindow = GetCurTabWindow();
  if (curTabWindow)
  	curTabWindow->ShowWindow(SW_HIDE);
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::InsertTab(TGadgetWindow* tab, LPCTSTR name)
{
  if (name)
    tab->SetCaption(name);
    
	ControlBarList->Add(tab);
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::InsertToolBox(TTabToolBox* toolBox)
{
	toolBox->SetParent(this);
  
  TMargins margins;
  margins.Units = TMargins::Pixels;
  margins.Top   = 2;
  toolBox->SetMargins(margins);
	ToolBoxList->Add(toolBox);
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::InitTabs()
{
  for (int i = 0; i < (int)ControlBarList->Size(); i++) {
  	TWindow* tab = (*ControlBarList)[i];
    tab->SetParent(TabControl);
    tab->Create();
  }
}
//-----------------------------------------------------------------------------
void 
TTabbedControlBar::GetTabClientRect(TRect& rect)
{
  TYPESAFE_DOWNCAST(TabControl, TTabControl)->AdjustRect(false, rect);
}
//-----------------------------------------------------------------------------
void 
TTabbedControlBar::SetTabSel(int index)
{
	TYPESAFE_DOWNCAST(TabControl, TTabControl)->SetSel(index);
}
//-----------------------------------------------------------------------------
void TTabbedControlBar::GetTabSize(TSize& size)
{
	if(TabControl->GetHandle()){
		TRect rect;
		TYPESAFE_DOWNCAST(TabControl, TTabControl)->GetItemRect(0, rect);
		size = rect.Size();
	}
	else{
		// FintHeight + VertMargin + ClientMargin
		int height = Font->GetHeight() + 3 + 2; 
		size = TSize(20,height);
	}
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::SetTabCaptions()
{
	TTabControl* ctrl = TYPESAFE_DOWNCAST(TabControl, TTabControl);
  int index = ctrl->GetSel();
  if (index == -1) 
    index = 0;

	ctrl->DeleteAll();

  for (int i = 0; i < (int)ControlBarList->Size(); i++) {
  	TWindow* tab = (*ControlBarList)[i];

    TTabItem item(tab->Title, strlen(tab->Title), (uint32)tab);

    ctrl->Add(item);
  }

	ctrl->SetSel(index);
} 
//-----------------------------------------------------------------------------
TWindow*
TTabbedControlBar::GetCurTabWindow()
{
  if (!TabControl->GetHandle())
  	return 0;

  TWindow* wnd = 0;
	TTabControl* ctrl = TYPESAFE_DOWNCAST(TabControl, TTabControl);
  int index = ctrl->GetSel();
  if (index != -1) {
    TTabItem item(TCIF_PARAM);
    ctrl->GetItem(index, item);
    wnd = (TWindow*)item.lParam;
  }
 	return wnd;
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::AdjustTabClient()
{
  if (!TabControl->GetHandle())
  	return;

  // Retrieve the window rectangle of the tab control
  //
  TRect rect;
  TabControl->GetWindowRect(rect);
  if (rect.IsEmpty())
    return;

  // NOTE: GetWindowRect returns screen coordinates... we'll need
  //       to have the coordinates relative to the frame window,
  //       the parent of both the tab and client window
  ::MapWindowPoints(HWND_DESKTOP, *TabControl, (LPPOINT)&rect, 2);

  // Ask the tab for it's 'client-area' based in it window location
	GetTabClientRect(rect);

  TWindow* curTabWindow = GetCurTabWindow();
  if (curTabWindow)
  	curTabWindow->SetWindowPos(0, rect, SWP_SHOWWINDOW | SWP_NOZORDER);
}
//-----------------------------------------------------------------------------
void
TTabbedControlBar::SetMetrics()
{
  if(ToolBoxList->Size()){
	  TLayoutMetrics metrics, tabMetrics;
	  TWindow* left = lmParent;

		metrics.Y.Set(lmTop, lmBelow, lmParent, lmTop, TUIMetric::CyBorder);
		metrics.Height.AsIs(lmHeight);
		metrics.Width.AsIs(lmWidth);

		for (int i = 0; i < (int)ToolBoxList->Size(); i++) {
			TTabToolBox *curToolBox = (*ToolBoxList)[i];
  
			if (left)
    		metrics.X.Set(lmLeft, lmRightOf, left, lmRight, 3);
			else
    		metrics.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 3);
      
	    SetChildLayoutMetrics(*curToolBox, metrics);
		  left = curToolBox;
		}
	  tabMetrics.Y.Set(lmTop, lmBelow, lmParent, lmTop);
		tabMetrics.X.Set(lmLeft, lmRightOf, left, lmRight, 3);
		tabMetrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight);
		tabMetrics.Height.Set(lmBottom, lmAbove, lmParent, lmBottom);
		SetChildLayoutMetrics(*TabControl, tabMetrics);
	}
	else{
	  TLayoutMetrics tabMetrics;
		tabMetrics.X.Set(lmLeft, lmRightOf, lmParent, lmLeft);
		tabMetrics.Y.Set(lmTop, lmBelow, lmParent, lmTop);
		tabMetrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight);
		tabMetrics.Height.Set(lmBottom, lmAbove, lmParent, lmBottom);
		SetChildLayoutMetrics(*TabControl, tabMetrics);
	}
}
//-----------------------------------------------------------------------------
static TGadget* GetGadget(TWindow* win, int id)
{
	TGadgetWindow* gadgetWindow = TYPESAFE_DOWNCAST(win, TGadgetWindow);
  if(gadgetWindow)
		return gadgetWindow->GadgetWithId(id);
  return 0;
}
//-----------------------------------------------------------------------------
TGadget*
TTabbedControlBar::FindGadget(int id, bool selectTab)
{
	TGadget* gadget;

  for (int i = 0; i < (int)ToolBoxList->Size(); i++) {
    gadget = GetGadget((*ToolBoxList)[i], id);
    if (gadget)
      return gadget;
  }  

  for (int j = 0; j < (int)ControlBarList->Size(); j++) {
    gadget = GetGadget((*ControlBarList)[j], id);
    if (gadget){
    	if (selectTab){
  			HideCurTabWindow();
				SetTabSel(j);
  			ShowCurTabWindow();
      }
      return gadget;
    }
  }
  return 0;
}
//-----------------------------------------------------------------------------
void TTabbedControlBar::EvWindowPosChanging(WINDOWPOS& windowPos)
{
  TLayoutWindow::EvWindowPosChanging(windowPos);

  if (!(windowPos.flags&SWP_NOSIZE)){
    TSize size;
    GetDesiredSize(size);
    windowPos.cy = size.cy;
  }
}
//-----------------------------------------------------------------------------
void TTabbedControlBar::GetDesiredSize(TSize& size)
{
  size.cx = size.cy = 0;

  TSize desiredSize;

  for (int i = 0; i < (int)ToolBoxList->Size(); i++) {
    (*ToolBoxList)[i]->GetDesiredSize(desiredSize);

    size.cx += desiredSize.cx;
    if (size.cy < desiredSize.cy)
      size.cy = desiredSize.cy;
  }  

  if (ControlBarList->Size() > 0) {
    TSize tabSize;
    
		GetTabSize(tabSize);

    for (int i = 0; i < (int)ControlBarList->Size(); i++) {
      (*ControlBarList)[i]->GetDesiredSize(desiredSize);

      size.cx += desiredSize.cx;
      if (size.cy < desiredSize.cy + tabSize.cy)
        size.cy = desiredSize.cy + tabSize.cy;
    }
  }

  size.cy += 4 * (TUIMetric::CyBorder + 2);
}
//-----------------------------------------------------------------------------
TWindow* 
TTabbedControlBar::CreateTabControl()
{
	return new TInnerTabControl(this, -1, 0, 0, 0, 0);
}
//-----------------------------------------------------------------------------
bool TTabbedControlBar::Create()
{
  TabControl = CreateTabControl();

  TSize size;

  GetDesiredSize(size);

  Attr.H = size.cy;

  return TLayoutWindow::Create();
}
//-----------------------------------------------------------------------------
bool TTabbedControlBar::IdleAction(long idleCount)
{
  for (int i = 0; i < (int)ToolBoxList->Size(); i++)
    (*ToolBoxList)[i]->IdleAction(idleCount);

	TWindow* wnd = GetCurTabWindow();
	if(wnd)
		wnd->IdleAction(idleCount);
  return TLayoutWindow::IdleAction(idleCount);
}
//-----------------------------------------------------------------------------
// When the gadget window receives a WM_COMMAND message, it is likely
// from a gadget or control within a TControlGadget. Reroute to the command
// target.
//
TResult
TTabbedControlBar::EvCommand(uint id, HWND hWndCtl, uint notifyCode)
{
  // First allow any derived class that wants to handle the command
  // NOTE: This search only caters for menu-style WM_COMMANDs (not those
  //       sent by controls)
  //
  TEventInfo  eventInfo(0, id);
  if (Find(eventInfo)) {
    Dispatch(eventInfo, id);
    return 0;
  }


#if 0
  // Prior versions of TGadgetWindow relied on TWindow's EvCommand for
  // dispatching WM_COMMAND events. This required that one derives from
  // a decoration class (eg. TControlbar, TToolbox) to handle control
  // notifications. The current version uses a more generalized logic
  // involving the CommandTarget and a frame ancestor class. This allows
  // a client window to handle notifications of a control in a toolbar
  // without using a TControlbar-derived class.
  // However, if you need to previous behaviour, simply invoke TWindow's
  // EvCommand from this handler.

  return TLayoutWindow::EvCommand(id, hWndCtl, notifyCode);
#endif

  TWindow* target;
  TFrameWindow* frame;

  // Find the frame who is our latest ancestor and make it our command target
  //
  for (target = GetParentO(); target; target = target->GetParentO()) {
    frame = TYPESAFE_DOWNCAST(target, TFrameWindow);
    if (frame || !target->GetParentO())
      break;
  }

  // Make sure the frame doesn't think we are its command target, or a BAD
  // loop will happen
  //
  if (target && (!frame || frame->GetCommandTarget() != GetHandle())) {
    CHECK(target->IsWindow());
    return target->EvCommand(id, hWndCtl, notifyCode);
  }

  // If all command routing fails, go back to basic dispatching of TWindow
  //
  return TLayoutWindow::EvCommand(id, hWndCtl, notifyCode);
}
//-----------------------------------------------------------------------------
// When the gadget window receives a WM_COMMAND_ENABLE message, it is likely
// from a gadget or control within a TControlGadget. Reroute to the command
// target.
//
void
TTabbedControlBar::EvCommandEnable(TCommandEnabler& ce)
{
  // If someone derived from TGadgetWindow and handles the command there,
  // give these handlers the first crack.
  //
  TEventInfo eventInfo(WM_COMMAND_ENABLE, ce.Id);
  if (Find(eventInfo)) {
    Dispatch(eventInfo, 0, TParam2(&ce));
    return;
  }

  TWindow* target = GetParentO();

  // Forward to command target if the enabler was really destined for us, and
  // not a routing from the frame.
  //
  if (target && ce.IsReceiver(*this)) {
    CHECK(target->IsWindow());
    ce.SetReceiver(*target);
    target->EvCommandEnable(ce);
    if (ce.GetHandled())
      return;
  }

  // Default to TWindow's implementation if the above routing fails
  //
  TLayoutWindow::EvCommandEnable(ce);
}
//-----------------------------------------------------------------------------
TNoteTabControlBar::TNoteTabControlBar(TWindow* parent, TFont* font, 
																			 TModule* module)
:
	TTabbedControlBar(parent, font,module)
{
}
//-----------------------------------------------------------------------------
TWindow* TNoteTabControlBar::CreateTabControl()
{
	return new TInnerNoteTab(this, -1, 0, 0, 0, 0);
}
//-----------------------------------------------------------------------------
TWindow* TNoteTabControlBar::GetCurTabWindow()
{
  if (!TabControl->GetHandle())
  	return 0;

  TWindow* wnd = 0;
	TNoteTab* ctrl = TYPESAFE_DOWNCAST(TabControl, TNoteTab);
  int index = ctrl->GetSel();
  if (index != -1) {
		TNoteTabItem item;
    if(ctrl->GetItem(index, item))
			wnd = (TWindow*)item.ClientData;
  }
 	return wnd;
}
//-----------------------------------------------------------------------------
void TNoteTabControlBar::GetTabSize(TSize& size)
{
	if(TabControl->GetHandle()){
		TNoteTabItem item;
		if(TYPESAFE_DOWNCAST(TabControl, TInnerNoteTab)->GetItem(0, item))
			size = item.Rect.Size();
		if(size.cy!=0 && size.cx!=0)
			return;
	}
	// FintHeight + VertMargin
	int height = Font->GetHeight() + 4; 
	size = TSize(20,height);
}
//-----------------------------------------------------------------------------
void TNoteTabControlBar::SetTabSel(int index)
{
	TYPESAFE_DOWNCAST(TabControl, TInnerNoteTab)->SetSel(index);
}
//-----------------------------------------------------------------------------
void TNoteTabControlBar::GetTabClientRect(TRect& rect)
{
	TInnerNoteTab* noteTab = TYPESAFE_DOWNCAST(TabControl, TInnerNoteTab);
  ::MapWindowPoints(*TabControl, *this, (LPPOINT)&rect, 2);

	TRect clientRect;
	GetClientRect(clientRect);
	
	rect.bottom = rect.top - 6;
	rect.top    = clientRect.top+3;
}
//-----------------------------------------------------------------------------
void
TNoteTabControlBar::SetTabCaptions()
{
	TInnerNoteTab* ctrl = TYPESAFE_DOWNCAST(TabControl, TInnerNoteTab);
  int index = ctrl->GetSel();
  if (index == -1) 
    index = 0;

	ctrl->DeleteAll();

  for (int i = 0; i < (int)ControlBarList->Size(); i++) {
  	TWindow* tab = (*ControlBarList)[i];
    ctrl->Add(tab->Title,(uint32)tab);
  }
	ctrl->SetSel(index);
} 
//-----------------------------------------------------------------------------
void 
TNoteTabControlBar::SetMetrics()
{
  if(ToolBoxList->Size()){
	  TLayoutMetrics metrics, tabMetrics;
	  TWindow* left = lmParent;

		metrics.Y.Set(lmTop, lmBelow, lmParent, lmTop, TUIMetric::CyBorder);
		metrics.Height.AsIs(lmHeight);
		metrics.Width.AsIs(lmWidth);

		for (int i = 0; i < (int)ToolBoxList->Size(); i++) {
			TTabToolBox *curToolBox = (*ToolBoxList)[i];
  
			if (left)
    		metrics.X.Set(lmLeft, lmRightOf, left, lmRight, 3);
			else
    		metrics.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 3);
      
	    SetChildLayoutMetrics(*curToolBox, metrics);
		  left = curToolBox;
		}
		TSize size;
		GetTabSize(size);
		tabMetrics.Y.Set(lmTop, lmAbove, lmParent, lmBottom, size.cy-2);
		tabMetrics.X.Set(lmLeft, lmRightOf, left, lmRight, 3);
		tabMetrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight, 3);
		tabMetrics.Height.Set(lmBottom, lmAbove, lmParent, lmBottom, 2);
		SetChildLayoutMetrics(*TabControl, tabMetrics);
	}
	else{
	  TLayoutMetrics tabMetrics;
		TSize size;
		GetTabSize(size);
		tabMetrics.X.Set(lmLeft, lmRightOf, lmParent, lmLeft);
		tabMetrics.Y.Set(lmTop, lmBelow, lmParent, lmTop,size.cy-3);
		tabMetrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight);
		tabMetrics.Height.Set(lmBottom, lmAbove, lmParent, lmBottom,1);
		SetChildLayoutMetrics(*TabControl, tabMetrics);
	}
}
//-----------------------------------------------------------------------------
void
TNoteTabControlBar::InitTabs()
{
  for (int i = 0; i < (int)ControlBarList->Size(); i++) {
  	TWindow* tab = (*ControlBarList)[i];
    tab->SetParent(this);
    tab->Create();
  }
}
//-----------------------------------------------------------------------------

