//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.30 $
//$Author: jogybl $
//$Date: 2009-08-05 10:32:03 $
//
// Implementation of class TFloatingFrame, a popup frame window that has a
// small title bar
//----------------------------------------------------------------------------
#if  defined(__BORLANDC__)
#pragma hdrignore SECTION
#endif
#include <owl/pch.h>
#if !defined(OWL_FLOATFRA_H)
# include <owl/floatfra.h>
#endif
#if !defined(OWL_UIMETRIC_H)
# include <owl/uimetric.h>
#endif

__OWL_USING_STD_NAMESPACE

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;
DIAG_DECLARE_GROUP(OwlCmd);

#if !defined(SECTION) || SECTION == 1

//
// Order is very important.  Must make sure that TTinyCaption gets 1st crack
// after us
//
DEFINE_RESPONSE_TABLE2(TFloatingFrame, TTinyCaption, TFrameWindow)
  EV_WM_SYSCOMMAND,
  EV_WM_NCCALCSIZE,
  EV_WM_NCPAINT,
  EV_WM_NCHITTEST,
  EV_WM_MOUSEACTIVATE,
  EV_WM_NCACTIVATE,
  EV_WM_ACTIVATEAPP,
END_RESPONSE_TABLE;

//
//
//
TFloatingFrame::TFloatingFrame(TWindow* owner, LPCTSTR title,
                               TWindow* client,
                               bool shrinkToClient, int captionHeight,
                               bool popupPalette, TModule* module)
:
  TFrameWindow(owner, title, client, shrinkToClient, module),
  TTinyCaption(),
  Margin(2, 2),
  DragFrame(popupPalette)
{
  if (popupPalette) {
    Attr.Style = WS_POPUP | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    // Use close box (true) for palettes, & calc dimensions w/ new styles
    //
    EnableTinyCaption(captionHeight, true);

    // Windows with a popup style ignore CW_USEDEFAULT style, so we will
    // specify a default size
    //
    // Normal use will be to specify a client and allow frame to sizeToClient
    // so this size will rarely be used.
    //
    Attr.X = Attr.Y = 0;
    Attr.W = 100;
    Attr.H = 50;
  }
  else
    // No close box for this non-popupPalette style
    //
    EnableTinyCaption(captionHeight, false);

  // Assume we should look active since our app just created us.
  //!CQ Could be a problem if our app is not active. Would have to wait until
  //!CQ SetupWindow and then see if the Focus belonged to this app.
  //
  AppearActive = true;

  // The only area exposed when erasing will be the caption, so use an
  // inactive-caption color so that full-drag over paints the right color
  //
  SetBkgndColor(TColor::SysInactiveCaption);
}

//
// Floating frames never route commands down to their children--just bubble it
// up to the parent.
//
HWND
TFloatingFrame::GetCommandTarget()
{
  TRACEX(OwlCmd, 1, "TFloatingFrame::GetCommandTarget - returns " << hex << \
                    (GetParentO() ? uint(HWND(*GetParentO())) : uint(0)));
  return GetParentO() ? HWND(*GetParentO()) : HWND(0);
}

//
// Change the receiver to be the framewindow not the floating frame window. 
//
void
TFloatingFrame::EvCommandEnable(TCommandEnabler& commandEnabler)
{
  if (Parent) {
    // Already being processed?
    //
    if (!commandEnabler.IsReceiver(Parent->GetHandle())) {
      // No, so forward it up to our parent
      //
      commandEnabler.SetReceiver(Parent->GetHandle());
      Parent->EvCommandEnable(commandEnabler);
    }
  }
}

//
// Resolve ambiguous mixin reference by passing EvCommand first to the tiny
// caption and then to the frame bases.
//
TResult
TFloatingFrame::EvCommand(uint id, THandle hWndCtl, uint notifyCode)
{
  TResult er;
  if (TTinyCaption::DoCommand(id, hWndCtl, notifyCode, er) == esComplete)
    return er;
  if (Parent)
    return Parent->EvCommand(id, hWndCtl, notifyCode);
  return TFrameWindow::EvCommand(id, hWndCtl, notifyCode);
}

//
// This is an example of a mix-in that does partial event handling
// We need to call the 'do' function for the mixin instead of the 'Ev' function
// to avoid duplicate default processing
//
void
TFloatingFrame::EvSysCommand(uint cmdType, TPoint& p)
{
  if (TTinyCaption::DoSysCommand(cmdType, p) == esComplete)
    return;
  TFrameWindow::EvSysCommand(cmdType,p);

  // Now we need to undo the activation that SysCommand/Size or Move has done
  // to us. This does result in some flicker since we've already painted, but
  // it would require more work to simulate the size/move to avoid the
  // activation.
  //
  if ((cmdType & 0xFFF0) == SC_SIZE || (cmdType & 0xFFF0) == SC_MOVE)
    GetApplication()->GetMainWindow()->SetActiveWindow();
}

//
// Handle WM_NCCALCSIZE to possibly add in the drag frame margins
//
uint
TFloatingFrame::EvNCCalcSize(bool calcValidRects, NCCALCSIZE_PARAMS & calcSize)
{
  uint er = TFrameWindow::EvNCCalcSize(calcValidRects, calcSize);

  // Adjust margins for extra edge around palette
  //
  if (DragFrame && !IsIconic()) {
    calcSize.rgrc[0].left +=   Margin.cx * TUIMetric::CxBorder;
    calcSize.rgrc[0].top +=    Margin.cy * TUIMetric::CyBorder;
    calcSize.rgrc[0].right -=  Margin.cx * TUIMetric::CxBorder;
    calcSize.rgrc[0].bottom -= Margin.cy * TUIMetric::CyBorder;
  }

  // Now invoke the TTinyCaption worker method to do the rest of the
  // calculations
  //
  DoNCCalcSize(calcValidRects, calcSize, er);

  return er;
}

//
// Handle WM_NCPAINT to paint the non-client areas of this window
//
// We only need to paint the drag frame margins. TWindow (via DefWindowProc)
// will paint the borders, & TTinyCaption will paint the caption
//
void TFloatingFrame::EvNCPaint(HRGN)
{
  DefaultProcessing();       // Default border painting--no caption

  // Paint our caption below with possible fake active look. arg is ignored.
  //
  EvNCActivate(true);

  // If enabled, paint drag frame margins in 3d face color
  //
  if (DragFrame) {
    TWindowDC dc(*(TWindow*)this);  // Cast is an MSVC bug workaround
    TRect     wr = GetWindowRect().InflatedBy(-Frame);
    wr -= wr.TopLeft();
    wr += Border;
    wr.top = GetCaptionRect().bottom;
    int mx = Margin.cx * Border.cx;
    int my = Margin.cy * Border.cy;

    dc.SetBkColor(TColor::Sys3dFace);
    dc.TextRect(wr.left, wr.top, wr.left+mx, wr.bottom);           // left
    dc.TextRect(wr.left+mx, wr.top, wr.right-mx, wr.top+my);       // top
    dc.TextRect(wr.right-mx, wr.top, wr.right, wr.bottom);         // right
    dc.TextRect(wr.left+mx, wr.bottom-my, wr.right-mx, wr.bottom); // bottom
  }
}

//
// Return where in the non client area the mouse is. We delegate to tiny
// caption for caption bar area, and when in DragFrame mode, make our frame
// act like a caption for dragging.
//
uint
TFloatingFrame::EvNCHitTest(TPoint& screenPt)
{
  uint er;
  if (TTinyCaption::DoNCHitTest(screenPt, er) == esComplete ||
      DoNCHitTest(screenPt, er) == esComplete)
    return er;
  return TWindow::EvNCHitTest(screenPt);
}

//
//
//
TEventStatus
TFloatingFrame::DoNCHitTest(TPoint& screenPt, uint& evRes)
{
  if (DragFrame && TCEnabled) {
    TPoint clientOffs(0,0);
    ClientToScreen(clientOffs);
    if (!GetClientRect().Contains(screenPt-clientOffs)) {
      evRes = HTCAPTION;
      return esComplete;
    }
  }
  return esPartial;
}

//
// Handle WM_NCMOUSEACTIVATE in order to decline activation. Also, in order to'
// take care of the user trying to activate this app, we activate the main
// window.
//
uint
TFloatingFrame::EvMouseActivate(THandle /*hWndTopLevel*/, uint /*hitTestCode*/,
                                uint /*msg*/)
{
  // Make our main window active if the current active is not in this
  // process
  //!CQ could call SetForegroundWindow(), where is it ?
  //
  if (!GetActiveWindow())
    SetForegroundWindow(GetHandle());
    //GetApplication()->GetMainWindow()->SetActiveWindow();//??????????????????

  EvNCActivate(true);   // Make sure we look active
  return MA_NOACTIVATE;
}

//
// Handle WM_NCACTIVATE to paint the caption with our notion of active
// appearance--either with our own caption paint routine, or with underlying
// classes routine.
//
// The caption will apear active as long as this app is active.
//
bool
TFloatingFrame::EvNCActivate(bool /*active*/)
{
  if (!IsIconic()) {
    if (TCEnabled)
      PaintCaption(AppearActive);
    else
      DefWindowProc(WM_NCACTIVATE, AppearActive, 0);
  }
  return true;
}

//
// Handle WM_ACTIVATEAPP to know & track when to appear active--that is
// whenever this app is activated
//
void
TFloatingFrame::EvActivateApp(bool active, HTASK /*threadId*/)
{
  AppearActive = active;

  EvNCActivate(AppearActive);
  DefaultProcessing();
}

#endif
#if !defined(SECTION) || SECTION == 2

IMPLEMENT_STREAMABLE2(TFloatingFrame, TTinyCaption, TFrameWindow);

#if !defined(BI_NO_OBJ_STREAMING)

//
//
//
void*
TFloatingFrame::Streamer::Read(ipstream& is, uint32 /*version*/) const
{
  TFloatingFrame* o = GetObject();
  ReadBaseObject((TFrameWindow*)o, is);
  ReadBaseObject((TTinyCaption*)o, is);
  is >> o->Margin;
  return o;
}

//
//
//
void
TFloatingFrame::Streamer::Write(opstream& os) const
{
  TFloatingFrame* o = GetObject();
  WriteBaseObject((TFrameWindow*)o, os);
  WriteBaseObject((TTinyCaption*)o, os);
  os << o->Margin;
}

#endif  // if !defined(BI_NO_OBJ_STREAMING)

#endif

__OWL_END_NAMESPACE

