//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.31 $
//$Author: jogybl $
//$Date: 2009-08-14 08:00:26 $
//
// Implementation of class TNoteTab
// Added Scroll handling. Contributed by Jogy. 
// Added support for set font and set image. Contributed by Jogy. 
//----------------------------------------------------------------------------
#include <owl/pch.h>

#if !defined(OWL_CONTAIN_H)
# include <owl/contain.h>
#endif
#if !defined(OWL_CONTROL_H)
# include <owl/control.h>
#endif
#if !defined(OWL_NOTETAB_H)
# include <owl/notetab.h>
#endif
#if !defined(OWL_UIHELPER_H)
# include <owl/uihelper.h>
#endif
#if !defined(OWL_COMMCTRL_H)
# include <owl/commctrl.h>
#endif
#if !defined(OWL_UIMETRIC_H)
# include <owl/uimetric.h>
#endif
#if !defined(OWL_SCROLLBAR_H)
# include <owl/scrollba.h>
#endif
#if !defined(OWL_UIMETRIC_H)
# include <owl/uimetric.h>
#endif
#if !defined(OWL_CELARRAY_H)
# include <owl/celarray.h>
#endif

#include <owl/template.h>

__OWL_USING_STD_NAMESPACE

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;


//
// Constants used when drawing control
//
const int LabelMargin = 5;        // Pad on each side of tab label
const int LabelVMargin= 1;        // Pad above/below the tab label
const int TabMargin   = 4;        // Margin between two tabs
const int CtrlMargin  = 5;        // Margin on each side of tab row
const int CtrlVMargin = 1;        // Margin above row of tab
const int HorzSelInc  = 2;        // Increment for selected tab
const int VertSelInc  = 2;        // Increment for selected tab


class TNoteTabItemArray : public TIPtrArray<TNoteTabItem*>{
  public:
    TNoteTabItemArray(){}
};
//----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(TNoteTab, TControl)
  EV_WM_SIZE,
  EV_WM_LBUTTONDOWN,
  EV_WM_GETDLGCODE,
  EV_WM_KEYDOWN,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_HSCROLL,
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructor of NoteTab object. Use this constructor when creating a
// notetab control from scratch.
//
TNoteTab::TNoteTab(TWindow*   parent,
                   int        id,
                   int x, int y, int w, int h,
                   TWindow*   buddy,
                   bool       dialogBuddy,
                   TModule*   module)
:
  TControl(parent, id, 0, x, y, w, h, module),
  WindowFace(!dialogBuddy),
  Buddy(buddy)
{
  InitCtrl();
}

//
// Constructor of Notetab object. Use this constructor when aliasing a
// control defined within a dialog template.
//
TNoteTab::TNoteTab(TWindow*   parent,
                   int        resourceId,
                   TWindow*   buddy,
                   bool       dialogBuddy,
                   TModule*   module)
:
  TControl(parent, resourceId, module),
  WindowFace(!dialogBuddy),
  Buddy(buddy)
{
  InitCtrl();
}

//
// Destructor of NoteTab object - Clean up allocated resources.
//
TNoteTab::~TNoteTab()
{
  // Cleanup memory associated with notetab items
  //
  DeleteAll();

  // Cleanup font and tab item collection
  //
  delete TabFont;
  delete TabList;
  if(ShouldDelete)
    delete CelArray;
}

//
// Return the Window class name of the notetab control object
//
LPCTSTR
TNoteTab::GetClassName()
{
  return OWL_NOTETAB;
}

//
// Overriden virtual of TWindow - Initialize font used by control and resize
// accordingly.
//
void
TNoteTab::SetupWindow()
{
  TControl::SetupWindow();

  SetTabFont(0, false);

  // SIR June 20th 2007 max instead of ::max
  TabHeight = Attr.H = max(TabHeight, Attr.H);

  SetWindowPos(0, 0, 0, Attr.W, Attr.H, SWP_NOMOVE|SWP_NOACTIVATE|
                                                     SWP_NOZORDER);

  // Initialize BuddyHandle if we have a Buddy.
  //
  if (Buddy)
    BuddyHandle = Buddy->GetHandle();
}

//
// Add a new tabitem to the notetab control
//
int
TNoteTab::Add(LPCTSTR label, uint32 clientData, 
              int imageIdx, TAbsLocation imageLoc)
{
  return Insert(label, GetCount(), clientData, imageIdx, imageLoc);
}

//
// Insert a new tabitem at the specified index
//
int
TNoteTab::Insert(LPCTSTR label, int index, uint32 clientData, 
                 int imageIdx, TAbsLocation imageLoc)
{
  PRECONDITION(index >= 0 && index <= GetCount());

  if(index == GetCount())
    TabList->Add(new TNoteTabItem(label, clientData, imageIdx, imageLoc));
  else{
    *(*TabList)[index] = TNoteTabItem(label, clientData, imageIdx, imageLoc);
  }

  SetTabSize(index);
  SetTabRects(FirstVisibleTab);

  // Select [Could selection only if it's the first one inserted]
  //
  SetSel(index);
  return index;
}

//NoSelects added by DLN (3/15/2000)

//------------------------------------------------------------------------------
// Add a new tabitem to the notetab control without selecting it //DLN
//------------------------------------------------------------------------------
int TNoteTab::AddNoSelect(LPCTSTR label, uint32 clientData)
{
  return InsertNoSelect(label, GetCount(), clientData);
}

//------------------------------------------------------------------------------
// Insert a new tabitem at the specified index 
//  (doesn't call SetSel like Owl Insert fcn)
//------------------------------------------------------------------------------
int TNoteTab::InsertNoSelect(LPCTSTR label, int index, uint32 clientData)
{
  PRECONDITION(index >= 0 && index <= GetCount());

  TabList->AddAt(new TNoteTabItem(label, clientData), index);

  SetTabSize(index);
  SetTabRects(FirstVisibleTab);

  // Return the index of the tab
  //
  return index;
}

//
// Remove the tabitem at the specified 'index'
//
bool
TNoteTab::Delete(int index)
{
  // Validate index...
  //
  if (index < GetCount() && index >= 0) {

    // Mark rectangles that need to be redrawn
    //
    if (IsVisible(index)) {
      for (int i = index; i < GetCount(); i++)
        InvalidateTabRect(i);
    }

    // Delete structure representing item
    //
    TabList->Destroy(index);

    // Reset index of the first visible tab
    //
    if (index == FirstVisibleTab)
      if (FirstVisibleTab != 0)
        FirstVisibleTab--;

    // If SelectedTab deleted -> unselect
    //
    //if(index == SelectedTab)
    //  SelectedTab = -1;

    // If SelectedTab deleted and it's not the first tab, mark the
    // tab before it as selected, otherwise mark the next tab
    if(index == SelectedTab){
       if(SelectedTab != 0)
         SelectedTab--;
       else if(GetCount() > 0)
         SelectedTab = 1;
       else
         SelectedTab = -1;
    }
    // Force SelectedTab to have focus
    if( (SelectedTab >= 0) && (SelectedTab < GetCount()) )
       InvalidateTabRect(SelectedTab);

    //
    //
    if (GetCount() > 0)
      SetTabRects(FirstVisibleTab);
    else
      SelectedTab = -1;

    return true;
  }
  return false;
}

//
// Remove all tab items in the notetab control
//
bool
TNoteTab::DeleteAll()
{
  TabList->Flush(true);
  FirstVisibleTab = 0;
  SelectedTab = -1;

  return true;
}

//
// Return the number of tab items in the notetab control
//
int
TNoteTab::GetCount() const
{
  return TabList->Size();
}

//
// Return the index of the selected tabitem.
// NOTE: Returns a zero-based index or -1 if there are no tab items
//       in the notetab control.
int
TNoteTab::GetSel() const
{
  return SelectedTab;
}

//
// Select the tabitem at the specified index.
// NOTE: SetSel does not send any notifications to the parent and/or buddy
//
int
TNoteTab::SetSel(int index)
{
  if (index < GetCount() && index >= 0) {
    if (index != SelectedTab) {

      // Invalidate area occupied by previously selected item
      //
      if ( (SelectedTab >= 0) && (SelectedTab < GetCount()) ) {
        InvalidateTabRect(SelectedTab);
      }

      // Update selected index
      //
      SelectedTab = index;

      // Invalidate area occupied by new selection
      //
      if ( (SelectedTab >= 0) && (SelectedTab < GetCount()) ) {
        InvalidateTabRect(SelectedTab);
      }
    }
    return index;
  }
  return -1;
}

//
// Retrieve information about the tab item at the specified index
//
bool
TNoteTab::GetItem(int index, TNoteTabItem& tabItem) const
{
  PRECONDITION(index >= 0);
  PRECONDITION(index < GetCount());

  tabItem = *(*TabList)[index];
  return true;
}

//
// Update information about the tab item at the specified index
//
bool
TNoteTab::SetItem(int index, const TNoteTabItem& tabItem)
{
  if (index < GetCount() && index >= 0) {

    *(*TabList)[index] = tabItem;

    //!BB Need to recalc/invalidate etc...
    SetTabSize(index);
    SetTabRects(FirstVisibleTab);
    return true;
  }
  return false;
}

//
// Return handle of buddy window associated with the notetab control
//
HWND
TNoteTab::GetBuddy() const
{
  return BuddyHandle;
}

//
// Set handle of the buddy window associated with this notetab control
//
void
TNoteTab::SetBuddy(HWND hwnd)
{
  BuddyHandle = hwnd;
  Buddy = GetWindowPtr( BuddyHandle );
  if( !Buddy )
    Buddy = new TWindow( hwnd );
}

//
// No transfers for NoteTab controls
//
uint
TNoteTab::Transfer(void* /*buffer*/, TTransferDirection /*direction*/)
{
  return 0;
}

//----------------------------------------------------------------------------

//
// Initialize internal variables used by notetab
//
void
TNoteTab::InitCtrl()
{
  // Initialize internal structures when using OWL's implementation
  //
  TabList = new TNoteTabItemArray;
  TabFont = 0;
  SelectedTab = -1;
  FirstVisibleTab = 0;
  ShouldDelete = false;
  CelArray = 0;
  TabHeight = 0;
  ScrollLoc = alRight;

  // If on new shell, use 3d look by default, and add extra space for a margin
  // at the top
  //
  Style3d = true;
  TopMargin = TUIMetric::CySizeFrame;

  SetBkgndColor(TColor::Sys3dFace);

  ScrollBar = new TScrollBar(this, -1, 0, 0, 0, 0, true);
  ScrollBar->ModifyStyle(WS_TABSTOP, 0); // No focus, please.
}

//
// Invalidates the rectangle occupied by the tab at the specified index
//
void
TNoteTab::InvalidateTabRect(int index)
{
  if (GetHandle()) {
    TRect tbRect = (*TabList)[index]->Rect;
    tbRect.Inflate(TabMargin, 1);
    InvalidateRect(tbRect, true);
  }
}

//
// Overriden Paint routine
//
void
TNoteTab::Paint(TDC& dc, bool /*erase*/, TRect& /*rect*/)
{
  int i = GetCount();
  if (i) {

    TRect clientRect;
    GetClientRect(clientRect);

    // Create tools & select initial ones into the DC
    //
    TBrush faceBrush(TColor::Sys3dFace);

    // A pen with the active page color
    //
    TPen facePen((WindowFace && !Style3d) ?
                  TColor::SysWindow : TColor::Sys3dFace);

    TPen hilitePen(TColor::Sys3dHilight);
    TPen lightPen(TColor::Sys3dLight);
    TPen edgePen(Style3d ? TColor::Sys3dShadow : TColor::SysWindowFrame);
    TPen dkShadowPen(TColor::Sys3dDkShadow);

    dc.SelectObject(faceBrush);
    dc.SelectObject(*TabFont);

    while (--i >= -1) {
      // Skip the selected tab, it's drawn last. Otherwise get the
      // working index
      //
      if (i == GetSel())
        continue;
      int ti = (i == -1) ? GetSel() : i;

      // Retrieve tab item information
      //
      TNoteTabItem& tab = *(*TabList)[ti];
      TRect& tbRect = tab.Rect;

      // Draw border of tab
      //
      TPoint pt[4] = {                                // Corner points
        TPoint(tbRect.left-TabMargin, tbRect.top-1),  // Top left
        TPoint(tbRect.left, tbRect.bottom),           // Bottom left
        TPoint(tbRect.right, tbRect.bottom),          // Bottom right
        TPoint(tbRect.right+TabMargin, tbRect.top-1)  // Top right
      };

      // If this is the selected tab...
      //
      if (i == -1) {
         // If 3d, draw edge around
         if (Style3d) {
           clientRect.top += TUIMetric::CySizeFrame;
           TUIBorder(clientRect, TUIBorder::WndRecessed).Paint(dc);
         }
         else {
           // Dark line across top of tabs to the left of this one
           dc.MoveTo(clientRect.left, pt[0].y);
           dc.LineTo(tbRect.left-TabMargin, pt[0].y);
           // Line across top of tabs to the right of this one
           //
           dc.MoveTo(pt[3].x, pt[3].y);
           dc.LineTo(clientRect.right, pt[3].y);
         }
      }

      // In 3-d mode, draw all in face, w/ hilite
      //
      if (Style3d) {
        dc.SelectObject(facePen);          // draw tab w/ face
        dc.Polygon(pt, COUNTOF(pt));

        dc.SelectObject(lightPen);         // inside left
        dc.MoveTo(pt[0].x+1, pt[0].y);
        dc.LineTo(pt[1].x+1, pt[1].y);

        dc.SelectObject(edgePen);          // inside bottom & right
        dc.MoveTo(pt[1].x+1, pt[1].y-1);
        dc.LineTo(pt[2].x-1, pt[2].y-1);
        dc.MoveTo(pt[2].x-1, pt[2].y);
        dc.LineTo(pt[3].x-1, pt[3].y);

        dc.SelectObject(hilitePen);        // outside left
        dc.MoveTo(pt[0].x, pt[0].y);
        dc.LineTo(pt[1].x, pt[1].y);

        dc.SelectObject(dkShadowPen);      // outside bottom & right
        dc.LineTo(pt[2].x, pt[2].y);
        dc.LineTo(pt[3].x, pt[3].y);
        dc.RestorePen();
      }
      // In WindowFace mode, draw selected tab in window color
      //
      else if (i == -1 && WindowFace) {
        TBrush windowBrush(TColor::SysWindow);
        dc.SelectObject(facePen);
        dc.SelectObject(windowBrush);
        dc.Polygon(pt, COUNTOF(pt));
        dc.RestoreBrush();
        dc.RestorePen();
        dc.Polyline(pt, COUNTOF(pt));
      }
      else {
        dc.SelectObject(facePen);
        dc.Polygon(pt, COUNTOF(pt));
        dc.RestorePen();
        dc.Polyline(pt, COUNTOF(pt));
      }

      TRect rect(tbRect);

      uint16 formatText = DT_SINGLELINE|DT_VCENTER;
      // Draw tab image
      if (CelArray && tab.ImageIdx >= 0 && !rect.IsNull()) {
        TSize size = CelArray->CelSize();
        
        switch (tab.ImageLoc) {
          default:
          case alLeft:{
#if 0 // not transparent
              rect.left += LabelMargin;
              CelArray->BitBlt(tab.ImageIdx, dc, rect.left, 
                              (rect.top + rect.bottom - size.cy) / 2);
#else
              TRect  srcRect(CelArray->CelRect(tab.ImageIdx));
              TPoint dstPt(LabelMargin,(rect.Height() - size.cy) / 2);
              TUIFace face(rect, CelArray->operator TBitmap&(), TColor::Sys3dFace);
              face.Paint(dc, srcRect, dstPt, TUIFace::Normal, false, false);
              rect.left += LabelMargin;
#endif
              rect.left += size.cx;
              formatText |= DT_LEFT;
            }
            break;

          case alRight:{
#if 0 // not transparent
              rect.right -= size.cx + LabelMargin;
              CelArray->BitBlt(tab.ImageIdx, dc, rect.right, 
                            (rect.top + rect.bottom - size.cy) / 2);
#else
              TRect  srcRect(CelArray->CelRect(tab.ImageIdx));
              TPoint dstPt(rect.Width()-(size.cx + LabelMargin),
                          (rect.Height() - size.cy) / 2);
              TUIFace face(rect, CelArray->operator TBitmap&(), TColor::Sys3dFace);
              face.Paint(dc, srcRect, dstPt, TUIFace::Normal, false, false);
              rect.right -= size.cx + LabelMargin;
#endif
              formatText |= DT_RIGHT;
            }
            break;
        }
      }
      else // No tab image, so draw text centered 
        formatText |= DT_CENTER;


      // Draw tab label
      //
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor((WindowFace && !Style3d) ?
                      TColor::SysWindowText : TColor::SysBtnText);
      dc.DrawText(tab.Label.c_str(), -1, rect, formatText);

      // If the note tab has input focus, draw the focus rectangle
      //
      if (i == -1 && GetFocus() == GetHandle()) 
          DrawFocusRect(dc);
    }

    dc.RestoreBrush();
    dc.RestoreFont();
  }
}

//
// WM_SIZE handler - Relay tab items
//
void
TNoteTab::EvSize(uint sizeType, TSize& size)
{
  TControl::EvSize(sizeType, size);

  Invalidate();

  // Layout tab items
  //
  SetTabRects(FirstVisibleTab);
}

//
// WM_LBUTTONDOWN handler -
//
void
TNoteTab::EvLButtonDown(uint /*modKeys*/, TPoint& point)
{
  int hitIndex = TabFromPoint(point);
  if (hitIndex != -1) {
    NotifyAndSelect(hitIndex);
  }
  TRect rect;
  GetScrollerArea(rect);
  if(rect.Contains(point))
    return;

  // Grab focus
  SetFocus();
}

//
// Handle WM_SETFOCUS: Draw focus to identify selected tab
//
void
TNoteTab::EvSetFocus(THandle hWndLostFocus)
{
  TControl::EvSetFocus(hWndLostFocus);
  TClientDC dc(*this);
  DrawFocusRect(dc);
}

//
// Handle WM_KillFOCUS: Remove dotted focus rectangle from selected tab
//
void
TNoteTab::EvKillFocus(THandle hWndGetFocus)
{
  TControl::EvKillFocus(hWndGetFocus);
  TClientDC dc(*this);
  DrawFocusRect(dc);
}

//
// Draw a dashed rectangle around the selected tab if it has input focus.
// calling this function a second time for the same tab erases the
// rectangle.
//
void
TNoteTab::DrawFocusRect(TDC& dc)
{
  if (GetCount() > 0 && GetSel() >= 0) {

    // Set the appropriate background color so the the focus rectangle
    // will erase cleanly.
    //
    // Setting color here is redundant when called from Paint, because
    // Windows sets the background color for the Paint DC.  Windows does
    // not set it, however, for the DC passed to SetFocus and KillFocus.
    //
    dc.SetBkColor(TColor::Sys3dFace);

    // Calculate coordinates and draw the focus rectangle
    //
    TRect selectedRect = (*TabList)[GetSel()]->Rect;
    selectedRect.Inflate(-HorzSelInc, -VertSelInc);
    dc.DrawFocusRect(selectedRect);
  }
}

//
// WM_GETDLGCODE handler - Inform dialog manager that we want to handle
// arrow keys
//
uint
TNoteTab::EvGetDlgCode(MSG far* msg)
{
  return TControl::EvGetDlgCode(msg) | DLGC_WANTARROWS;
}

//
// WM_KEYDOWN handler - handle arrow keys to allow user to navigate
// through tab items
// //!Y.B. add HotKey Ctrl+A
void
TNoteTab::EvKeyDown(uint key, uint /*repeatCount*/, uint /*flags*/)
{
  int tab = GetSel();

  // right arrow key pressed
  //
  if (key == VK_RIGHT) {
    if (tab == GetCount() - 1)    // is selected tab already at the left end?
      return;
    else
      NotifyAndSelect(++tab);
  }

  // left arrow key pressed
  //
  if (key == VK_LEFT) {
    if (tab == 0)                 // is selected tab already at the right end?
      return;
    else
      NotifyAndSelect(--tab);
  }
}

//
// Update the internal information stored about the label of a particular
// tab item.
//
void
TNoteTab::SetTabSize(int index)
{
  if (TabFont) {
    // Use screen DC (may be called before 'HWND') & selected font
    //
    TScreenDC dc;
    dc.SelectObject(*TabFont);

    // Compute size of label
    //
    TNoteTabItem& tab = *(*TabList)[index];
    dc.GetTextExtent(tab.Label.c_str(), tab.Label.length(), tab.LabelSize);
    dc.RestoreFont();
    // Add image dimensions to LabelSize
    if (CelArray) {
      TSize size = CelArray->CelSize();
      tab.LabelSize.cx += size.cx;
      tab.LabelSize.cy = max(tab.LabelSize.cy, size.cy);
    }
  }
}

//
// Layout tab items with the specified index at the leftmost.
//
void
TNoteTab::SetTabRects(int firstTab)
{
  // First hide the tabs preceeding the first one
  //
  for (int i = 0; i < firstTab; i++) {
    TRect& tbRect = (*TabList)[i]->Rect;

    if (!tbRect.IsNull()) {
      InvalidateTabRect(i);
      tbRect.SetNull();
    }
  }

  // Retrieve the area where tabs can hang out
  //
  TRect tabArea;
  GetTabsArea(tabArea);

  // Loop vars to keep track of upper left corner assignments
  //
  int x = tabArea.left;
  int y = tabArea.top;

  // 3d tabs need extra room for 3d edges
  //
  int edgeVMargin = Style3d ? TUIMetric::CyBorder*2 : 0;

  // Iterate through each tab
  //
  int index;
  for (index = firstTab; index < GetCount(); index++) {
    TNoteTabItem& tab = *(*TabList)[index];
    TRect& tbRect = tab.Rect;

    // Compute tab's width with margins
    //
    int tabWidth1 = tab.LabelSize.cx + LabelMargin*2;
    // If tab is completely off viewport, break;
    //
    if(ScrollLoc == alNone){
        if ((x+TabMargin) >= tabArea.right)   //If no scrollbar, paint if any is showing
            break;
    }
    else{
        // If this is the last tab, then we may not want to hide if it fits in the area
        // were the scrollbar would be (since if it fits, no scrollbar may be drawn).
        // This special case only applies when all tabs are visible (firstTab == 0).

        if (firstTab == 0 && index == GetCount()-1){  
            TRect scrollRect;     
            GetScrollerArea(scrollRect);
            if ((x+tabWidth1+TabMargin) >= tabArea.right+scrollRect.Width())
                break;
        }
        else{
            if ((x+tabWidth1+TabMargin) >= tabArea.right)
                break;
        }
    }

    // Compute new rectangle
    //
    TRect newRect(x+TabMargin, y, x+TabMargin+tabWidth1,
                  y+tab.LabelSize.cy+LabelVMargin*2+edgeVMargin);

    // Set left side of next tab
    //
    x += tabWidth1+TabMargin;

    // Invalidate rectangle(s) and assign new area to tab
    //
    if (tbRect != newRect) {
      if (!tbRect.IsNull())
        InvalidateTabRect(index);

      tbRect = newRect;

      // Force new rectangle to be repainted
      //
      if (!tbRect.IsNull())
        InvalidateTabRect(index);
    }
  }
  
  if(ScrollBar && ScrollBar->GetHandle() && ScrollLoc != alNone){
    // if not all tabs are visible -> show ScrollBar
    if(index < GetCount() || firstTab > 0){
      ScrollBar->SetRange(0, GetCount() - index/* + firstTab*/);
      ScrollBar->ShowWindow(SW_SHOW);

      TRect rect;
      GetScrollerArea(rect);

      ScrollBar->MoveWindow(rect, true);
    }
    else {
      ScrollBar->ShowWindow(SW_HIDE);

      // if all tabs are visible and the scroller position
      // is to the left -> adjust tab rectangles
      if (ScrollLoc == alLeft) {
        for (int index = firstTab; index < GetCount(); index++) {
          TNoteTabItem& tab = *(*TabList)[index];
          TRect& tbRect = tab.Rect;

          tbRect.Offset(-TUIMetric::CxHScroll*2, 0);
        }
      }
    }
  }

  // Partially show the tab under the scrollbar.
  //
  if (index < GetCount()) {
    TNoteTabItem& tab = *(*TabList)[index];
    TRect& tbRect = tab.Rect;

    if (!tbRect.IsNull())
      InvalidateTabRect(index);

    int tabWidth = tab.LabelSize.cx + LabelMargin*2;
    int tabHeight = tab.LabelSize.cy + LabelVMargin*2;
    TRect newRect(x+TabMargin, y, x+TabMargin+tabWidth, y+tabHeight+edgeVMargin);
    tbRect = newRect;
    index++;
  }

  // Hide any remaining tabs
  //
  while (index < GetCount()) {
    TNoteTabItem& tab = *(*TabList)[index];
    TRect& tbRect = tab.Rect;

    if (!tbRect.IsNull())
      InvalidateTabRect(index);

    tbRect.SetNull();
    index++;
  }
}

//
// Set the specified font handle as the one to be used when drawing the
// labels of tab items.
//
void
TNoteTab::SetupFont(HFONT font)
{
  bool autoDelete = false;
  if (font == 0) {
    // Try to get our parent's font
    if (Parent && Parent->GetHandle())
      font = Parent->GetWindowFont();

    // Use the default UI font
    if (!font) {
      LOGFONT lf;
      TDefaultGUIFont().GetObject(lf);
      font = CreateFontIndirect(&lf);
      autoDelete = true;
    }
  }

  if (font){
    delete TabFont;
    TabFont = new TFont(font,autoDelete?AutoDelete:NoAutoDelete);
  }
}

//
// Return the index of the tab item at the specified window coordinate
//
int
TNoteTab::TabFromPoint(const TPoint& pt)
{
  for (int i = 0; i < GetCount(); i++) {
    if ((*TabList)[i]->Rect.Contains(pt))
      return i;
  }
  return -1;
}

//
// Select a tab and send the appropriate notifications
//
void
TNoteTab::NotifyAndSelect(int index)
{
  PRECONDITION(index >= 0);
  PRECONDITION(index < GetCount());

  // First notify that we're about to change selection
  //
  TNotify _not(*this, Attr.Id, TCN_SELCHANGING); /* error if 'not' for GNU */ 
  if (!SendNotification(::GetParent(*this), Attr.Id, _not)) {

    // If notification was not vetoed, proceed...
    //
    SetSel(index);

    // Notify of selection change to the buddy if one, otherwise
    // the parent.
    //
    _not.code = TCN_SELCHANGE;
    if( Buddy )
      SendNotification( BuddyHandle, Attr.Id, _not);
    else
      SendNotification(::GetParent(*this), Attr.Id, _not);
  }
}

//
// Retrieve  the desired location of the scrollers within the tab
//
void
TNoteTab::GetScrollerArea(TRect& rect)
{
  PRECONDITION(GetCount() > 0);

  GetClientRect(rect);

  // Shrink area to match tabs height with a width twice as big
  //
  rect.bottom  -= TUIMetric::CyFixedFrame;
  rect.top = rect.bottom - TUIMetric::CyHScroll;

  switch (ScrollLoc) {
    default:
    case alTop:       //?????????????
    case alBottom:    //??????????????
    case alNone:
      rect.left = rect.right = 0;
      break;

    case alLeft:
      rect.left  += TUIMetric::CxFixedFrame;
      rect.right = rect.left + TUIMetric::CxHScroll*2;
      break;

    case alRight:
      rect.right -= TUIMetric::CxFixedFrame;
      rect.left = rect.right - TUIMetric::CxHScroll*2;
      break;
  }
}

//
// Retrieve the rectangle within tabs lie. If there are no tabs in the
// control, then only the left, top and right sides are valid as the bottom
// requires tabs to be computed
//
void
TNoteTab::GetTabsArea(TRect& rect)
{
  // First retrieve left, top and right borders
  //
  GetClientRect(rect);
  rect.Inflate(-CtrlMargin, -CtrlVMargin);
  rect.top    += TopMargin;
  //rect.right  -= TUIMetric::CxHScroll*2;

  switch (ScrollLoc) {
    default:
    case alTop:       //?????????????
    case alBottom:    //??????????????
    case alRight:
      rect.right -= TUIMetric::CxHScroll*2; 
      break;
    case alLeft:
      rect.left  += TUIMetric::CxHScroll*2; 
      break;
  }
}

//
// Return true if the tab item at the specified index is visible. Returns
// false otherwise.
//
bool
TNoteTab::IsVisible(int index) const
{
  PRECONDITION(index < GetCount() && index >= 0);
  return (*TabList)[index]->Rect.IsNull() ? false : true;
}

//
//
//
void
TNoteTab::SetFirstVisibleTab(int index)
{
  if (index >= 0 && index < GetCount()) {
    FirstVisibleTab = index;
    SetTabRects(FirstVisibleTab);
  }
}

//
//
//
bool
TNoteTab::EnsureVisible(int index)
{
  if (index == -1)
    index = GetSel();

  TRect& tbRect = (*TabList)[index]->Rect;

  if (tbRect.IsNull())
    SetFirstVisibleTab(index);

  return !tbRect.IsNull();
}

//
//
//
void
TNoteTab::EvHScroll(uint scrollCode, uint thumbPos, HWND /*hWndCtl*/)
{
  int index = GetFirstVisibleTab();
  int min, max;
  ScrollBar->GetRange(min, max);

  switch (scrollCode) {
    case SB_LINEDOWN:       if(max > 0) index++; break;
    case SB_LINEUP:        if(index > min) index--; break;
    case SB_PAGEDOWN:      if(max > 0) index++; break;
    case SB_PAGEUP:        if(index > min) index--; break;
    case SB_THUMBPOSITION: index = thumbPos; break;
    case SB_THUMBTRACK:    index = thumbPos; break;
  }

  SetFirstVisibleTab(index);
  ScrollBar->SetPosition(index);
  SetFocus();
}

//
//
//
void
TNoteTab::EvPaint()
{
  TControl::EvPaint();
  ScrollBar->Invalidate(true);
}
//
//
//
void
TNoteTab::SetTabFont(HFONT font, bool redraw)
{
  TControl::SetWindowFont(font, redraw);

  SetupFont(font);

  // Scan here & update sizes now that we know the font
  //
  TabHeight = TabFont->GetHeight();
  for (int i = 0; i < GetCount(); i++) {
    SetTabSize(i);
    TNoteTabItem& tab = *(*TabList)[i];

    // SIR June 20th 2007 max instead of ::max
    TabHeight = max(TabHeight, (int)tab.LabelSize.cy + CtrlVMargin*2 + LabelVMargin*2);
  }

  SetTabRects(FirstVisibleTab);

  // Add in the margin at the bottom and at the top if 3d
  //
  TabHeight += TUIMetric::CySizeFrame;

  if (Style3d)
    TabHeight += TUIMetric::CyBorder*6;
}

//
//
//
void
TNoteTab::SetScrollLocation(TAbsLocation loc)
{
  if (ScrollLoc == loc)
    return;

  ScrollLoc = loc;

  SetTabRects(FirstVisibleTab);
}

//
//
//
void
TNoteTab::SetCelArray(TCelArray* array, TAutoDelete del)
{
  if (ShouldDelete)
    delete CelArray;

  ShouldDelete = del == AutoDelete;

  CelArray = array;
}

__OWL_END_NAMESPACE
/* ========================================================================== */
