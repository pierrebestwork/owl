//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.27 $
//$Author: jogybl $
//$Date: 2007-09-15 11:43:48 $
//
// Implementation of class TDecoratedFrame
//----------------------------------------------------------------------------
#if  defined(__BORLANDC__)
#pragma hdrignore SECTION
#endif
#include <owl/pch.h>

#if !defined(OWL_DECFRAME_H)
# include <owl/decframe.h>
#endif
#if !defined(OWL_MESSAGEB_H)
# include <owl/messageb.h>
#endif
#if !defined(OWL_TOOLTIP_H)
# include <owl/tooltip.h>
#endif
#if !defined(OWL_CONTROLB_H)
# include <owl/controlb.h>
#endif
#if  !defined(OWL_FUNCTOR_H)
#include <owl/functor.h>
#endif
#if !defined(OWL_BARDESCR_H)
# include <owl/bardescr.h>
#endif

__OWL_USING_STD_NAMESPACE

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;
DIAG_DECLARE_GROUP(OwlWin);   // diagnostic group for windows
DIAG_DECLARE_GROUP(OwlCmd);   // diagnostic group for commands

#if !defined(SECTION) || SECTION == 1

DEFINE_RESPONSE_TABLE2(TDecoratedFrame, TFrameWindow, TLayoutWindow)
  EV_WM_ENTERIDLE,
  EV_WM_MENUSELECT,
  EV_WM_SIZE,
END_RESPONSE_TABLE;

const uint  BLANK_HELP = UINT_MAX;

//
//
//
static TGadgetWindow* CreateToolBar(TWindow* wnd)
{
  return new TControlBar(wnd);
}

bool EvBarInserter(TDecoratedFrame& frame, TGadgetWindow& destWindow, TAbsLocation loc)
{
  if(frame.GetHandle())
    destWindow.Create();
  frame.Insert(destWindow, TDecoratedFrame::TLocation(loc));
  return true;
}

//
//
//
TDecoratedFrame::TDecoratedFrame(TWindow*        parent,
                                 LPCTSTR         title,
                                 TWindow*        clientWnd,
                                 bool            trackMenuSelection,
                                 TModule*        module)
:
  TLayoutWindow(parent, title, module)
{
  // Initialize virtual bases, in case the derived-most used default ctor
  //
  TWindow::Init(parent, title, module);
  TFrameWindow::Init(clientWnd, false);

  TLayoutMetrics  metrics;

  TrackMenuSelection = trackMenuSelection;
  MenuItemId = 0;
  SettingClient   = false;
  BarCreatorFunct = new TGadgetWndFunctor;
  BarInserterFunct = new TBarInsertFunctor;
  SetBarCreator(TGadgetWnd_FUNCTOR(CreateToolBar));
  SetBarInserter(TBarInsert_FUNCTOR(EvBarInserter));


  if (!ClientWnd)
    ClientWnd = new TWindow(this, _T("\007"));  // create dummy placeholder

  if (ClientWnd->Attr.Style & WS_BORDER) {
    metrics.X.SameAs(lmParent, lmLeft);
    metrics.Y.SameAs(lmParent, lmTop);
    metrics.Width.SameAs(lmParent, lmRight);
    metrics.Height.SameAs(lmParent, lmBottom);
  }
  else {
    metrics.X.Set(lmLeft, lmRightOf, lmParent, lmLeft);
    metrics.Y.Set(lmTop, lmBelow, lmParent, lmTop);
    metrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight);
    metrics.Height.Set(lmBottom, lmAbove, lmParent, lmBottom);
  }

  SetChildLayoutMetrics(*ClientWnd, metrics);
}

//
//
//
TDecoratedFrame::~TDecoratedFrame()
{
  delete BarCreatorFunct;
  delete BarInserterFunct;
}
//
//
//
void TDecoratedFrame::SetBarCreator(const TGadgetWndFunctor& funct)
{
  *BarCreatorFunct = funct;
}
//
//
void TDecoratedFrame::SetBarInserter(const TBarInsertFunctor& funct)
{
  *BarInserterFunct = funct;
}
//
// Override SetupWindow in order to layout & position decorations
//
void
TDecoratedFrame::SetupWindow()
{
  TFrameWindow::SetupWindow();

  // Size/position child windows. Don't wait until we get the WM_SIZE event
  // because by then the windows will have been displayed on the screen
  //
  TRect  clientRect = GetClientRect();

  ClientSize.cx = clientRect.right;
  ClientSize.cy = clientRect.bottom;

  if(IsFlagSet(wfMainWindow) && GetBarDescr())
    CreateBar();
   Layout();
}

//
//
//
void      
TDecoratedFrame::CleanupWindow()
{
  TFrameWindow::CleanupWindow();

  if (!GetBarDescr())
    return;
  TGadgetWindow* toolBar = GetControlBar();
  if(toolBar) // already created
    ((TBarDescr*)GetBarDescr())->RemoveGadgets(*toolBar);
}

//
// Handle SetClientWindow() here to manage fixing up the layout metrics of
// all the children before & after the client is changed.
//
TWindow*
TDecoratedFrame::SetClientWindow(TWindow* clientWnd)
{
  TLayoutMetrics metrics;
  GetChildLayoutMetrics(*ClientWnd, metrics);
  if (!clientWnd)
    clientWnd = new TWindow(this, _T("\007"));  // create dummy placeholder

  clientWnd->SetParent(this);
  SetChildLayoutMetrics(*clientWnd, metrics);

  TWindow* oldWnd = GetClientWindow();

  // Make sure that all child metrics that were based on the old client window
  // get updated to the new client
  //
  TWindow* first = GetFirstChild();
  if (first) {
    TWindow* child = first;
    do {
      if (GetChildLayoutMetrics(*child, metrics)) {
        if (metrics.X.RelWin == oldWnd)
          metrics.X.RelWin = clientWnd;
        if (metrics.Y.RelWin == oldWnd)
          metrics.Y.RelWin = clientWnd;
        if (metrics.Width.RelWin == oldWnd)
          metrics.Width.RelWin = clientWnd;
        if (metrics.Height.RelWin == oldWnd)
          metrics.Height.RelWin = clientWnd;
        SetChildLayoutMetrics(*child, metrics);
      }
      child = child->Next();
    } while (child != first);
  }

  // Now let the TFrameWindow set the client. Then delete the old client if it
  // was our temporary place holder. Set a flag while the client is being set
  // so that RemoveChild() below knows that we are taking care of things.
  //
  SettingClient = true;
  oldWnd = TFrameWindow::SetClientWindow(clientWnd);
  SettingClient = false;

  if (HiUint16(uint32(oldWnd->Title)) && oldWnd->Title[0] == 007) {
    oldWnd->Destroy();
    delete oldWnd;
    oldWnd = 0;
  }

  // Relayout the children to get the new client sized right
  //
  Layout();

  return oldWnd;
}

//
// Make sure that both bases get a chance to see the child removed. TWindow's
// will be called twice, but the second call will be ignored. If we are
// removing the client, and we are not in the process of setting the client,
// then call SetClientWindow to put in a placeholder.
//
void
TDecoratedFrame::RemoveChild(TWindow* child)
{
  if (child == ClientWnd && !SettingClient)
    SetClientWindow(0);
  TFrameWindow::RemoveChild(child);
  TLayoutWindow::RemoveChild(child);
}

//
// Give the decorations an opportunity to do pre-processing. Don't bother
// checking the client window since it is typically in the focus chain and
// will be given an opportunity to do pre-processing
//
bool
TDecoratedFrame::PreProcessMsg(MSG& msg)
{
  TWindow*  firstChild = GetFirstChild();

  if (firstChild) {
    TWindow*  child = firstChild;

    do {
      if (child != ClientWnd && child->GetHandle() &&
         (child->GetWindowLong(GWL_STYLE) & WS_VISIBLE) &&
          child->PreProcessMsg(msg))
        return true;

      child = child->Next();
    } while (child != firstChild);
  }

  return TFrameWindow::PreProcessMsg(msg);
}

//
// Retrieves the hint text associated with a particular Id
//
int
TDecoratedFrame::GetHintText(uint id, LPTSTR buf, int size,
                          THintText hintType) const
{
//DLN added to avoid bad resource access on WM_NOTIFY's with NULL LPARAMS
              //see TWindow::WindowProc (look for dumbNMHDR)
  if (id == static_cast<uint>(-1)) //JJH added static_cast
    return 0; 

  //!BB **********************************************
  //!BB Try to consolidate duplicated LoadString below.

  int numBytes = 0;
  if (hintType == htTooltip) {
    // For tooltips, we first look if the same id exists on the
    // window's menu and use the associated menu string.
    //
    if (::IsMenu(GetMenu()) &&
        TUser::GetMenuString(GetMenu(), id, buf, size, MF_BYCOMMAND)) {
      // Trim accelerator text e.g., '\tCtrl+X' (if any)
      //
      LPTSTR c = _tcschr(buf, _T('\t'));
      if (c)
        *c = 0;

      // Accelerator can be \b instead of \t (NOTE: \a in .rc make a \b
      // in res)
      //
      c = _tcschr(buf, _T('\b'));
      if (c)
        *c = 0;

      // Trim trailing dots, e.g., '...' (if any)
      //
      int i;
      for (i = ::_tcslen(buf) - 1; i >= 0; i--)
        if (buf[i] == _T('.'))
          buf[i] = 0;
        else
          break;

#if defined(BI_DBCS_SUPPORT)
      // Trim trailing access key, e.g., '(&N)' (if any)
      //
      if (i >= 3 && buf[i-3] == '(' && buf[i-2] == '&' && buf[i] == ')')
        buf[i-3] = 0;
#endif
      // Eliminate '&' (just emliminate first one)
      //
      i = -1;
      while(buf[++i])
        if (buf[i] == _T('&')) {
          do {
            buf[i] = buf[i+1];
            i++;
          } while (buf[i]);
          break;
        }
        numBytes = ::_tcslen(buf);
    }
    else {
      // If the tip text cannot be retrieved from the menu, we then look
      // for a string resource - The resource is expected to contain a
      // longer status text followed by a '\n' and the tip text.
      //
      TCHAR resStr[256];
      int len = GetModule()->LoadString(id, resStr, 256);
      if (len == 0 && MergeModule != 0)
          len = MergeModule->LoadString(id, resStr, 256);

      WARNX(OwlWin, len == 0, 0,
          _T("TDecoratedFrame::GetHintText LoadString(")
          << *GetModule() << _T(",") << id << _T(") Failed"));

      LPTSTR pLF;
      if (len && (pLF = _tcschr(resStr, _T('\n'))) != 0) {
        _tcsncpy(buf, pLF+1, size);
        numBytes = ::_tcslen(buf);
      }
    }
  }

  if (hintType == htStatus) {
    numBytes = GetModule()->LoadString(id, buf, size);
    if (numBytes == 0 && MergeModule != 0)
        numBytes = MergeModule->LoadString(id, buf, size);

    WARNX(OwlWin, numBytes == 0, 0,
         _T("TDecoratedFrame::GetHintText LoadString(")
         << *GetModule() << _T(",") << id << _T(") Failed"));

    if (numBytes) {

      // Clear beyond the '\n' in case this string resource contains both
      // a status and a tooltip hint text.
      //
      LPTSTR pLF = _tcschr(buf, _T('\n'));
      if (pLF) {
        *pLF = 0;
        numBytes = ((int)(pLF - buf)-1)/sizeof(TCHAR);
      }
    }
  }
  return numBytes;
}


//
// Handle WM_MENUSELECT to provide hint text in the status bar based on the
// menu item id. Treat popup items seperatly and ask them for their ids.
//
void
TDecoratedFrame::EvMenuSelect(uint menuItemId, uint flags, HMENU hMenu)
{
  if (TrackMenuSelection) {
    if (flags == 0xFFFF && hMenu == 0) {  // Menu closing
      TMessageBar*  messageBar = TYPESAFE_DOWNCAST(ChildWithId(IDW_STATUSBAR),
                                                   TMessageBar);
      if (messageBar)
        messageBar->SetHintText(0);       // Restore text of status bar
      MenuItemId = 0;                     //  ""  ""  ""  ""  ""  ""
    }
    else if (flags & MF_POPUP) {

#if 0 //!BB
//!BB  TMenu popupMenu(hMenu);
//!BB  int   count = popupMenu.GetMenuItemCount();
//!BB  int   pos = 0;
//!BB  for (;pos < count && popupMenu.GetSubMenu(pos) != HMENU(menuItemId);
//!BB       pos++)
//!BB    ;
//!BB  MenuItemId = popupMenu.GetMenuItemID(pos);
    //!BB
    //!BB The above logic seems suspicious - The docs. seem to imply that
    //!BB if (flags & MF_POPUP) then popup = GetSubMenu(hMenu, menuItemId);
    //!BB I don't understand why we iterate through the menu when we have the
    //!BB index. i.e. menuItemId is then an index not an HMENU as used above.
    //!BB Also, there's no check to prevent the case where the for loop would
    //!BB break until 'pos == count'. CodeGuard points out that the
    //!BB eventual '::GetMenuItemID' call does indeed fail in these cases
    //!BB given that 'pos' is out of range.
    //!BB
#else
      // Retrieve id of popup menu
      //
      MenuItemId = TMenu(hMenu).GetMenuItemID(menuItemId);
#endif
    }
    else if (flags & (MF_SEPARATOR | MF_MENUBREAK | MF_MENUBARBREAK)
#if 0  // changed by Y.B 5/23/98
      || (menuItemId >= IDW_FIRSTMDICHILD && menuItemId < IDW_FIRSTMDICHILD+9)) {
#else
      ){
#endif
      MenuItemId = BLANK_HELP;  // display an empty help message
                                // could also restore bar at this point too
    }
    else {
      MenuItemId = menuItemId;  // display a help message with this string Id
    }
  }
}

//
// Forward our size event to the layout window base, and not the frame base
//
void
TDecoratedFrame::EvSize(uint sizeType, TSize& size)
{
  TLayoutWindow::EvSize(sizeType, size);

  // Since TFrameWindow::EvSize wasn't called, broadcast
  // relevant info to children explicitly.
  //
  TFrameWindow::BroadcastResizeToChildren(sizeType, size);
}

//
// Handle WM_ENTERIDLE in order to display help hints on the messsage bar if
// there is a hint pending & this frame has a message bar.
//
void
TDecoratedFrame::EvEnterIdle(uint source, HWND hDlg)
{
  if (source == MSGF_MENU && MenuItemId) {
    TMessageBar*  messageBar = TYPESAFE_DOWNCAST(ChildWithId(IDW_STATUSBAR),
                                                 TMessageBar);
    if (messageBar) {
      TCHAR  buf[_MAX_PATH];
      if ((MenuItemId == BLANK_HELP) ||
          !GetHintText(MenuItemId, buf, COUNTOF(buf), htStatus))
        *buf = 0;
      messageBar->SetHintText(buf);
    }
    MenuItemId = 0;             // Don't repaint on subsequent EvEnterIdle's
  }
  TFrameWindow::EvEnterIdle(source, hDlg);
}

//
// Get or set location codes stashed in the style long for a decoration
//
const uint32 locMask  = 0x0F00;
const int    locShift = 8;

inline TDecoratedFrame::TLocation GetLocation(TWindow& w) {
  return TDecoratedFrame::TLocation(uint16((w.Attr.Style & locMask) >> locShift));
}

inline void SetLocation(TWindow& w, TDecoratedFrame::TLocation loc) {
  w.Attr.Style = (w.Attr.Style&~locMask) | (uint32(loc) << locShift);
}

//
// Find first sibling of a decoration in the same location
//
struct TSiblingSearch {
  TWindow*                   Decoration;
  TDecoratedFrame::TLocation Location;
  TWindow*                   Sibling;

  TSiblingSearch(TWindow* decoration, TDecoratedFrame::TLocation location)
  {
    Decoration = decoration;
    Sibling = 0;
    Location = location;
  }
};

//
// FirstThat callback function to help EvCommand find decorations that are
// at a given location & that are visible.
//
static bool
findSibling(TWindow* win, void* param)
{
  TSiblingSearch*  search = (TSiblingSearch*)param;

  if (win == search->Decoration)
    return true;

  else if (GetLocation(*win) == search->Location && win->IsWindowVisible())
    search->Sibling = win;

  return false;
}

//
//
//
static bool
findFirstSiblingAt(TWindow* win, void* param)
{
  TSiblingSearch*  search = (TSiblingSearch*)param;
  if ((win != search->Decoration) &&
      (GetLocation(*win) == search->Location))
    return true;
  return false;
}

//
//
//
TResult
TDecoratedFrame::EvCommand(uint id, HWND hWndCtl, uint notifyCode)
{
  TRACEX(OwlCmd, 1, _T("TDecoratedFrame:EvCommand - id(") << id << _T("), ctl(") <<\
                     hex << uint(hWndCtl) << _T("), code(") << notifyCode  << _T(")"));

  // Check if it's a menu-style command (i.e. skip commands from Controls)
  //
  if (hWndCtl == 0) {

    // Does the command Id correspond to the Id of one of our children
    //
    TWindow*  decoration = ChildWithId(id);

    // If yes, toggle it's visibility
    //
    if (decoration) {
      bool visible = decoration->IsWindowVisible();

      // If it's visible, remove it's layout metrics - We'll 'Layout' below
      // and toggle the actual visibility of the child below.
      //
      if (visible) {
        RemoveChildLayoutMetrics(*decoration);
      }
      else {

        // Check if decoration wants to be against the frame's edge
        //
        bool insertAtEdge = decoration->IsFlagSet(wfInsertAtEdge);

        // We want to display this child window, first retrieve it's location
        //
        TLocation location = GetLocation(*decoration);

        // Find the decoration it should be adjacent to
        //
        TSiblingSearch  search(decoration, location);
        if (insertAtEdge)
          search.Sibling = FirstThat((TCondFunc)findFirstSiblingAt, &search);
        else
          FirstThat(findSibling, &search);

        // Default to client window if we could not find the decoration's
        // potential neighbor.
        //
        if (!search.Sibling)
          search.Sibling = ClientWnd;

        // Insert at its desired location
        //
        switch (location) {
          case None:
            break;
          case Top:
            InsertAtTop(*decoration, search.Sibling);
            break;

          case Bottom:
            InsertAtBottom(*decoration, search.Sibling);
            break;

          case Left:
            InsertAtLeft(*decoration, search.Sibling);
            break;

          case Right:
            InsertAtRight(*decoration, search.Sibling);
            break;
      
    default: //JJH added empty default statement
      break;
        }
      }

      // Force the layout manager to compute new locations and shuffle
      // windows around
      //
      Layout();

      // Hide or display the decoration
      //
      decoration->ShowWindow(visible ? SW_HIDE : SW_SHOWNA);
      return 0;
    }
  }

  return TFrameWindow::EvCommand(id, hWndCtl, notifyCode);
}

//
//!CQ Auto-hide all docking areas? Give them IDW_s
//
//
void
TDecoratedFrame::EvCommandEnable(TCommandEnabler& commandEnabler)
{
  // Provide default command text to TooltipEnablers
  //
  if (TYPESAFE_DOWNCAST(&commandEnabler, TTooltipEnabler)) {
    TCHAR tipText[125];
    if (GetHintText(commandEnabler.GetId(), tipText, COUNTOF(tipText), htTooltip)){
      commandEnabler.SetText(tipText);
      return;
    }
  }

#if 0
  TWindow*  decoration;
  if (DocAreaTop)
    decoration = DocAreaTop->ChildWithId(commandEnabler.Id);
  else
    decoration = ChildWithId(commandEnabler.Id);
#else
  TWindow*  decoration = ChildWithId(commandEnabler.Id);
#endif

  if (!decoration)
    TFrameWindow::EvCommandEnable(commandEnabler);

  else {
    commandEnabler.Enable();
    commandEnabler.SetCheck(decoration->IsWindowVisible() ?
                            TCommandEnabler::Checked :
                            TCommandEnabler::Unchecked);
  }
}

//
// Insert a decoration window at the top of the client area, possibly above
// another given decoration or the client
//
void
TDecoratedFrame::InsertAtTop(TWindow& decoration, TWindow* insertAbove)
{
  TLayoutMetrics   metrics;
  TWindow*         insertBelow;

  // Get the layout metrics for "insertAbove" (the window the decoration is
  // going to be inserted above)
  //
  GetChildLayoutMetrics(*insertAbove, metrics);

  insertBelow = metrics.Y.RelWin;

  // If "insertAbove" has a border then set its top edge to be the same as
  // the decoration's bottom edge; otherwise place its top edge below the
  // decoration's bottom edge
  //
  metrics.Y.Set(lmTop,
                insertAbove->Attr.Style & WS_BORDER ? lmSameAs : lmBelow,
                &decoration, lmBottom);
  SetChildLayoutMetrics(*insertAbove, metrics);

  // Now set the layout metrics for the decoration so its top edge is the same
  // as or below the bottom edge of "insertBelow" (they overlap if WS_BORDER)
  //!CQ want to use lmBelow if no border!?
  //
  bool overlap = (decoration.Attr.Style & WS_BORDER);

  metrics.Y.Set(lmTop, overlap ? lmSameAs : lmBelow,
                insertBelow, insertBelow ? lmBottom : lmTop);

  metrics.Height.AsIs(lmHeight);
  metrics.X.Set(lmLeft, overlap ? lmSameAs : lmRightOf, lmParent, lmLeft);
  metrics.Width.Set(lmRight, overlap ? lmSameAs : lmLeftOf, lmParent, lmRight);

  SetChildLayoutMetrics(decoration, metrics);
}

//
// Insert a decoration window at the bottom of the client area, possibly below
// another given decoration or the client
//
void
TDecoratedFrame::InsertAtBottom(TWindow& decoration, TWindow* insertBelow)
{
  TLayoutMetrics  metrics;
  TWindow*        insertAbove;

  // Get the layout metrics for "insertBelow"(the window the decoration is
  // going to be inserted below)
  //
  GetChildLayoutMetrics(*insertBelow, metrics);

  if (insertBelow == ClientWnd) {
    insertAbove = metrics.Height.RelWin;

    // If the client window has a border then set the client window's bottom
    // to be the same as the top edge of the decoration; otherwise set the
    // client window's bottom edge to be above the decoration's top edge
    //
    metrics.Height.Set(lmBottom,
                       ClientWnd->Attr.Style & WS_BORDER ? lmSameAs : lmAbove,
                       &decoration, lmTop);
  }
  else {
    insertAbove = metrics.Y.RelWin;

    // Set the bottom edge of "insertBelow" to be the same as the top edge of
    // the decoration
    //
    metrics.Y.Set(lmBottom, lmSameAs, &decoration, lmTop);
  }
  SetChildLayoutMetrics(*insertBelow, metrics);

  // Now set the layout metrics for the decoration so its bottom edge is the
  // same as the top edge of "insertAbove"
  //
  bool overlap = (decoration.Attr.Style & WS_BORDER);

  metrics.Y.Set(lmBottom, overlap ? lmSameAs : lmAbove, insertAbove,
                insertAbove ? lmTop : lmBottom);

  metrics.Height.AsIs(lmHeight);
  metrics.X.Set(lmLeft, overlap ? lmSameAs : lmRightOf, lmParent, lmLeft);
  metrics.Width.Set(lmRight, overlap ? lmSameAs : lmLeftOf, lmParent, lmRight);

  SetChildLayoutMetrics(decoration, metrics);
}

//
// Insert a decoration window at the left of the client area, possibly to the
// left of another given decoration or the client
//
void
TDecoratedFrame::InsertAtLeft(TWindow& decoration, TWindow* insertLeftOf)
{
  TLayoutMetrics  metrics;
  TWindow*        insertRightOf;

  // Get the layout metrics for "insertLeftOf"(the window the decoration is
  // going to be inserted to the left of)
  //
  GetChildLayoutMetrics(*insertLeftOf, metrics);

  insertRightOf = metrics.X.RelWin;

  // If "insertLeftOf" has a border then set its left edge to be the same as
  // the decoration's right edge; otherwise place its left edge one pixel to
  // the right of the decoration's right edge
  //
  metrics.X.Set(lmLeft,
                insertLeftOf->Attr.Style & WS_BORDER ? lmSameAs : lmRightOf,
                &decoration, lmRight);
  SetChildLayoutMetrics(*insertLeftOf, metrics);

  // Now set the layout metrics for the decoration so it's left edge is the
  // same as the right edge of "insertRightOf"
  //
  bool overlap = (decoration.Attr.Style & WS_BORDER);

  metrics.X.Set(lmLeft, overlap ? lmSameAs : lmRightOf, insertRightOf,
                insertRightOf ? lmRight : lmLeft);

  metrics.Width.AsIs(lmWidth);

  // If the client window & decoration both have or don't have borders then
  // place the decoration so its "y" and "bottom" are the same as the client
  // windows; otherwise place its "y" above/below the client window's "y" and
  // its "bottom" below/above the client window's "bottom" based on who's has
  // borders & who doesn't
  //
  // This way if there are top or bottom decorations they will be tiled
  // over/under the left/right decorations
  //
  if (ToBool(ClientWnd->Attr.Style & WS_BORDER) == overlap) {
    metrics.Y.SameAs(ClientWnd, lmTop);
    metrics.Height.SameAs(ClientWnd, lmBottom);
  }
  else if (overlap) {
    metrics.Y.Set(lmTop, lmAbove, ClientWnd, lmTop);
    metrics.Height.Set(lmBottom, lmBelow, ClientWnd, lmBottom);
  }
  else {
    metrics.Y.Set(lmTop, lmBelow, ClientWnd, lmTop);
    metrics.Height.Set(lmBottom, lmAbove, ClientWnd, lmBottom);
  }
  SetChildLayoutMetrics(decoration, metrics);
}

//
// Insert a decoration window at the right of the client area, possibly to the
// right of another given decoration or the client
//
void
TDecoratedFrame::InsertAtRight(TWindow& decoration, TWindow* insertRightOf)
{
  TLayoutMetrics  metrics;
  TWindow*        insertLeftOf;

  // Get the layout metrics for "insertRightOf" (the window the decoration is
  // going to be inserted to the right of)
  //
  GetChildLayoutMetrics(*insertRightOf, metrics);

  if (insertRightOf == ClientWnd) {
    insertLeftOf = metrics.Width.RelWin;

    // If the client window has a border then set the client window's right
    // edge to be the same as the left edge of the decoration; otherwise set
    // the client window's right edge to be one pixel to the left of the
    // decoration's left edge
    //
    metrics.Width.Set(lmRight,
                      ClientWnd->Attr.Style & WS_BORDER ? lmSameAs : lmLeftOf,
                      &decoration, lmLeft);
  }
  else {
    insertLeftOf = metrics.X.RelWin;

    // Set the right edge of "insertRightOf" to be the same as the left edge of
    // the decoration
    //
    metrics.X.Set(lmRight, lmSameAs, &decoration, lmLeft);
  }
  SetChildLayoutMetrics(*insertRightOf, metrics);

  // Now set the layout metrics for the decoration so its right edge is the
  // same as the left edge of "insertLeftOf"
  //
  bool overlap = (decoration.Attr.Style & WS_BORDER);

  metrics.X.Set(lmRight, overlap ? lmSameAs : lmLeftOf, insertLeftOf,
                insertLeftOf ? lmLeft : lmRight);

  metrics.Width.AsIs(lmWidth);

  // If the client window & decoration both have or don't have borders then
  // place the decoration so its "y" and "bottom" are the same as the client
  // windows; otherwise place its "y" above/below the client window's "y" and
  // its "bottom" below/above the client window's "bottom" based on who's has
  // borders & who doesn't
  //
  // This way if there are top or bottom decorations they will be tiled
  // over/under the left/right decorations
  //
  if (ToBool(ClientWnd->Attr.Style & WS_BORDER) == overlap) {
    metrics.Y.SameAs(ClientWnd, lmTop);
    metrics.Height.SameAs(ClientWnd, lmBottom);
  }
  else if (overlap) {
    metrics.Y.Set(lmTop, lmAbove, ClientWnd, lmTop);
    metrics.Height.Set(lmBottom, lmBelow, ClientWnd, lmBottom);
  }
  else {
    metrics.Y.Set(lmTop, lmBelow, ClientWnd, lmTop);
    metrics.Height.Set(lmBottom, lmAbove, ClientWnd, lmBottom);
  }
  SetChildLayoutMetrics(decoration, metrics);
}

//
// Insert a decoration window into position at one of the four edges.
//
void
TDecoratedFrame::Insert(TWindow& decoration, TLocation location)
{
  // Store away location for remove/re-insetion
  //
  SetLocation(decoration, location);

  // Make sure the decoration has clipsiblings style, since our rearranging
  // causes corners to overlap sometimes.
  //
  ModifyStyle(0, WS_CLIPSIBLINGS);//|WS_CLIPCHILDREN);///????

  // Parent to decframe and remove layoutmetrics in case it's a re-insert
  //
  decoration.SetParent(this);
  RemoveChildLayoutMetrics(decoration);

  // If the window should be visible, proceed with insertion.
  // NOTE: Should we check the 'wfInsertAtEdge' flag here? It mostly 
  //       important when hiding/showing decorations [i.e. In EvCommand
  //       handler]. However, it would be nice to check for it and use 
  //       something other than ClientWnd if necessary.
  //
  if (decoration.Attr.Style & WS_VISIBLE) {
    switch (location) {
      case None:
        break;
      case Top:
        InsertAtTop(decoration, ClientWnd);
        break;
      case Bottom:
        InsertAtBottom(decoration, ClientWnd);
        break;
      case Left:
        InsertAtLeft(decoration, ClientWnd);
        break;
      case Right:
        InsertAtRight(decoration, ClientWnd);
        break;
      default: //JJH added empty default statement
  break; 
    }
  }
}

static bool IsHaveGadgetWindow(TWindow* wnd, void*)
{
  return wnd ? (wnd->ChildWithId(IDW_TOOLBAR) ? true : false) : false;
}

//
//
//
TGadgetWindow*
TDecoratedFrame::GetControlBar()
{
  TWindow* wnd = ChildWithId(IDW_TOOLBAR);
  if(!wnd){
    wnd = FirstThat(IsHaveGadgetWindow);
    if(!wnd || (wnd = wnd->ChildWithId(IDW_TOOLBAR))==0)
      return 0;
  }
  TGadgetWindow* toolBar = TYPESAFE_DOWNCAST(wnd, TGadgetWindow);
  if(toolBar)
    return toolBar;
  return 0;
}


//
//
//
bool
TDecoratedFrame::CreateBar()
{
  TGadgetWindow* toolBar = GetControlBar();
  if(!toolBar){ // already created
    toolBar = (*GetBarCreator())(this);
    toolBar->Attr.Id = IDW_TOOLBAR;
  }
  if(((TBarDescr*)GetBarDescr())->Create(*toolBar))
    return (*GetBarInserter())(*this, *toolBar, alTop);
  return false;
}

//
//
//
bool
TDecoratedFrame::MergeBar(const TBarDescr& childBarDescr)
{
  if (!GetHandle() || !GetBarDescr())
    return false;
  TGadgetWindow* toolBar = GetControlBar();
  if(toolBar) // already created
    ((TBarDescr*)GetBarDescr())->Merge(childBarDescr, *toolBar);
  return true;
}

//
//
//
bool
TDecoratedFrame::RestoreBar()
{
  if (!GetHandle() || !GetBarDescr())
    return false;
  TGadgetWindow* toolBar = GetControlBar();
  if(toolBar) // already created
    ((TBarDescr*)GetBarDescr())->Restore(*toolBar);
  return true;
}

#endif
#if !defined(SECTION) || SECTION == 2

IMPLEMENT_STREAMABLE3(TDecoratedFrame, TFrameWindow, TLayoutWindow, TWindow);

#if !defined(BI_NO_OBJ_STREAMING)

//
//
//
void*
TDecoratedFrame::Streamer::Read(ipstream& is, uint32 /*version*/) const
{
  ReadVirtualBase((TFrameWindow*)GetObject(), is);
  ReadBaseObject((TLayoutWindow*)GetObject(), is);
  return GetObject();
}

//
//
//
void
TDecoratedFrame::Streamer::Write(opstream& os) const
{
  WriteVirtualBase((TFrameWindow*)GetObject(), os);
  WriteBaseObject((TLayoutWindow*)GetObject(), os);
}

#endif  // if !defined(BI_NO_OBJ_STREAMING)

#endif

__OWL_END_NAMESPACE

