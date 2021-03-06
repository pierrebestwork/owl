//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.29 $
//$Author: jogybl $
//$Date: 2009-08-05 10:32:03 $
//
// Implementation of class TMDIClient.  This defines the basic behavior
// of all MDI client windows.
//----------------------------------------------------------------------------
#if defined(__BORLANDC__)  
#pragma hdrignore SECTION    
#endif
#include <owl/pch.h>
#if !defined(OWL_MDI_H)
# include <owl/mdi.h>
#endif

//
// Define to not work around here in the client, the 'creating MDI child
// maximized corrupts the menu bar' bug
//
#if !defined(NO_MDIMAX_WORKAROUND)
# define MDIMAX_WORKAROUND
#endif

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;

#if !defined(SECTION) || SECTION == 1

DEFINE_RESPONSE_TABLE1(TMDIClient, TWindow)
  EV_COMMAND(CM_CREATECHILD, CmCreateChild),
  EV_COMMAND(CM_TILECHILDREN, CmTileChildren),
  EV_COMMAND(CM_TILECHILDRENHORIZ, CmTileChildrenHoriz),
  EV_COMMAND(CM_CASCADECHILDREN, CmCascadeChildren),
  EV_COMMAND(CM_ARRANGEICONS, CmArrangeIcons),
  EV_COMMAND(CM_CLOSECHILDREN, CmCloseChildren),
  EV_COMMAND_ENABLE(CM_TILECHILDREN, CmChildActionEnable),
  EV_COMMAND_ENABLE(CM_TILECHILDRENHORIZ, CmChildActionEnable),
  EV_COMMAND_ENABLE(CM_CASCADECHILDREN, CmChildActionEnable),
  EV_COMMAND_ENABLE(CM_ARRANGEICONS, CmChildActionEnable),
  EV_COMMAND_ENABLE(CM_CLOSECHILDREN, CmChildActionEnable),
  EV_WM_MDICREATE,
  EV_WM_MDIDESTROY,
  EV_WM_DROPFILES,
END_RESPONSE_TABLE;

//
// Constructor for a TMDIClient
//
// Allocates a CLIENTCREATESTRUCT on the heap and
// ClientAttr to point to this space
//
TMDIClient::TMDIClient(TModule* module)
{
  // Initialize virtual base, in case the derived-most used default ctor
  //
  TWindow::Init(0, 0, module);

  Attr.Id = IDW_MDICLIENT;

  // Allow client area to grow scroll bars if necessary
  //
  Attr.Style |= MDIS_ALLCHILDSTYLES | WS_GROUP | WS_TABSTOP | WS_CLIPCHILDREN|
                WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL | WS_BORDER;

	Attr.Style &= ~WS_BORDER;
  Attr.ExStyle |= WS_EX_CLIENTEDGE;

  ClientAttr = new CLIENTCREATESTRUCT;
  ClientAttr->hWindowMenu = 0;
  ClientAttr->idFirstChild = IDW_FIRSTMDICHILD;

  Attr.Param = (LPTSTR)ClientAttr;
  SetFlag(wfStreamTop);
}

//
// Constructor for a TMDIClient which is being used in a DLL as an alias
// for a non-OWL window. This ctor is generally not used by derived
// classes
//
TMDIClient::TMDIClient(THandle handle, TModule* module)
:
  TWindow(handle, module)
{
  ClientAttr = 0;
  SetFlag(wfStreamTop);
}

//
// Frees the memory associated with ClientAttr
//
TMDIClient::~TMDIClient()
{
  delete ClientAttr;
}

//
// Return the Window's defined class name for MDI clients
//
LPCTSTR
TMDIClient::GetClassName()
{
  return _T("MDICLIENT");
}

//
// Use the Windows message to get the active mdi child, and then down cast
// to our MDI child derived class before returning it.
//
TMDIChild*
TMDIClient::GetActiveMDIChild()
{
  THandle hWnd = THandle(HandleMessage(WM_MDIGETACTIVE));

  return TYPESAFE_DOWNCAST(GetWindowPtr(hWnd), TMDIChild);
}

//
//
//
void
TMDIClient::ArrangeIcons()
{
  HandleMessage(WM_MDIICONARRANGE);
}

//
//
//
void
TMDIClient::CascadeChildren()
{
  HandleMessage(WM_MDICASCADE);
}

//
//
//
void
TMDIClient::TileChildren(int tile)
{
  HandleMessage(WM_MDITILE, tile);
}

//
// Preprocess messages in order to translate MDI accelerator keys
//
bool
TMDIClient::PreProcessMsg(MSG& msg)
{
  if (msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN)
    return TranslateMDISysAccel(GetHandle(), &msg);

  else
    return false;
}

//
// Performs actual THandle creation. Fills in child menu handle by querying the
// frame for it.
//
bool
TMDIClient::Create()
{
  TMDIFrame*  frame = TYPESAFE_DOWNCAST(Parent,TMDIFrame);

  CHECK(frame);

  ClientAttr->hWindowMenu = frame->FindChildMenu(frame->GetMenu());
  return TWindow::Create();
}

//
// Creates a valid new MDI child window after calling InitChild() to construct
// a new MDI child window object
//
TWindow*
TMDIClient::CreateChild()
{
  TMDIChild* child = InitChild();
  CHECK(child);
  if (child->Create())
    return child;
  return 0;
}

//
// Can be overriden by derived classes to construct an instance of a
// user-defined TMDIChild derived class as an MDI child window
//
TMDIChild*
TMDIClient::InitChild()
{
  return new TMDIChild(*this);
}

static bool
sCannotClose(TWindow* win, void*)
{
  return !win->CanClose();
}

static void
sCloseChild(TWindow* win, void*)
{
  win->Destroy();
  delete win;
}

//
// Closes each MDI child, after calling the child's CanClose() method to
// ensure that it is Ok to do so
//
// Returns true if all children are closed(or there were no children),
// false if any child can't be closed
//
bool
TMDIClient::CloseChildren()
{
  if (!FirstThat(sCannotClose)) {
    ForEach(sCloseChild);
    return true;
  }
  return false;
}

//
// Handle WM_MDICREATE to perform actual creation of MDI children.
//
TResult
TMDIClient::EvMDICreate(MDICREATESTRUCT & createStruct)
{
  // Fill in default child window styles if they request style 0 since this
  // client by default has set allchildstyles
  //
  if ((Attr.Style&MDIS_ALLCHILDSTYLES) && !createStruct.style)
    createStruct.style =
               WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
               WS_SYSMENU | WS_CAPTION | WS_THICKFRAME |
               WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

  // Work around a Windows MDI bug w/ bad menus if MDI child is created
  // maximized, by hiding child now & maximizing later
  //
#if defined(MDIMAX_WORKAROUND)
  uint32 origStyle = createStruct.style;
  if (createStruct.style & WS_MAXIMIZE)
    createStruct.style &= ~(WS_MAXIMIZE | WS_VISIBLE);
#endif

  TResult result = DefaultProcessing();

  // Finish up maximized MDI child workaround
  //
#if defined(MDIMAX_WORKAROUND)
  if (THandle(result) && (origStyle & WS_MAXIMIZE)) {
    HandleMessage(WM_MDIMAXIMIZE, TParam1(result));
    HandleMessage(WM_MDIREFRESHMENU);
  }
#endif

  return result;
}

//
// When an MDI child is destroyed while other children are hidden or disabled,
// the Windows MDI child management gets confused causing subsequent failure.
// To prevent this, we temporarily unhide and enable siblings during destroy.
//
static void sUnHide(TWindow* win, void* hWnd)
{
  if (*win == TWindow::THandle(hWnd) || !win->IsWindow())
    return;
  if (!win->IsWindowVisible()) {
    win->SetWindowPos(0, 0, 0, 0, 0,
                      SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE|
                      SWP_NOZORDER|SWP_SHOWWINDOW);
    win->SetFlag(wfUnHidden);
  }
  if (!win->IsWindowEnabled()) {
    win->EnableWindow(true);
    win->SetFlag(wfUnDisabled);
  }
}

//
//
//
static void sReHide(TWindow* win, void*)
{
  if (!*win)
    return;
  if (win->IsFlagSet(wfUnHidden)) {
    win->ClearFlag(wfUnHidden);
    win->ShowWindow(SW_HIDE);
  }
  if (win->IsFlagSet(wfUnDisabled)) {
    win->ClearFlag(wfUnDisabled);
    win->EnableWindow(false);
  }
}

//
// Destroy an MDI child window. Must temporarily unhide any hidden children,
// and then rehide them after the destruction. Otherwise Window's MDI client
// gets confused
//
void
TMDIClient::EvMDIDestroy(THandle THandle)
{
  ForEach(sUnHide, (void*)THandle);
  DefaultProcessing();
  ForEach(sReHide);
  HandleMessage(WM_MDIREFRESHMENU);
}

//
// Forward dropped file messages by default to the parent (MDI Frame) where
// they can be handled, or allowed to forward to the app where they can be
// handled easier.
//
void
TMDIClient::EvDropFiles(TDropInfo)
{
  Parent->ForwardMessage();
}

//
// Enables any of the child action menu items if any MDI children exit
//
void
TMDIClient::CmChildActionEnable(TCommandEnabler& commandEnabler)
{
  commandEnabler.Enable(GetFirstChild() != 0);
}


#endif
#if !defined(SECTION) || SECTION == 2

IMPLEMENT_STREAMABLE1(TMDIClient, TWindow);

#if !defined(BI_NO_OBJ_STREAMING)

//
// Reads an instance of TMDIClient from the passed ipstream
//
void*
TMDIClient::Streamer::Read(ipstream& is, uint32 /*version*/) const
{
  ReadBaseObject((TWindow*)GetObject(), is);

  if (GetObject()->ClientAttr == 0)
    GetObject()->ClientAttr = new CLIENTCREATESTRUCT;  //

  uint idFirstChild;  // Need temp for near data model since ClientAttr is 
  is >> idFirstChild;
  GetObject()->ClientAttr->idFirstChild = idFirstChild;
  GetObject()->ClientAttr->hWindowMenu = (HMENU) 0;
  GetObject()->Attr.Param = (LPTSTR)GetObject()->ClientAttr;

  return GetObject();
}

//
// Writes the TMDIClient to the passed opstream
//
void
TMDIClient::Streamer::Write(opstream& os) const
{
  WriteBaseObject((TWindow*)GetObject(), os);
  os << GetObject()->ClientAttr->idFirstChild;
}

#endif  // if !defined(BI_NO_OBJ_STREAMING)

#endif

__OWL_END_NAMESPACE
/* ========================================================================== */

