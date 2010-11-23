//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.28 $
//$Author: jogybl $
//$Date: 2009-08-05 10:31:53 $
//
// Definition of class TFloatingFrame.
//----------------------------------------------------------------------------

#if !defined(OWL_FLOATFRA_H)
#define OWL_FLOATFRA_H

#if !defined(OWL_FRAMEWIN_H)
# include <owl/framewin.h>
#endif
#if !defined(OWL_TINYCAPT_H)
# include <owl/tinycapt.h>
#endif
#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif


__OWL_BEGIN_NAMESPACE

// Generic definitions/compiler options (eg. alignment) preceeding the 
// definition of classes
#include <owl/preclass.h>

//
// class TFloatingFrame
// ~~~~~ ~~~~~~~~~~~~~~
// Implements a floating frame (owned popup frame w/ tiny caption)
//
// If there is a client window then the floating frame can shrink to fit
// the client window leaving room for margins on the left, top, right, and
// bottom
//
// If there is a client window then the floating palette expects the window
// to paint its own background (and hence does nothing in response to the
// WM_ERASEBKGND). If there is no client window then the floating palette
// erases the client area background using TColor::SysUIFace
//
// If popupPalette is true then, dragFrame is enabled, styles are set to popup
// with a tiny close box, and a border.
//
// When dragFrame mode is enabled the floating frame adds an extra non-client
// margin around the outside to allow moving of the frame. This works well with
// a thin frame and is nice when the client area is too full to allow dragging
// from there.
//
class _OWLCLASS TFloatingFrame : public TFrameWindow, public TTinyCaption {
  public:
    TFloatingFrame(TWindow* parent,
                   LPCTSTR  title = 0,
                   TWindow* clientWnd = 0,
                   bool     shrinkToClient = false,
                   int      captionHeight = DefaultCaptionHeight,
                   bool     popupPalette = false,
                   TModule* module = 0);

    void    SetMargins(const TSize& margin);
    void    SetDragFrame(bool dragFrame);

    // Nameless union to declare a constant that is class scoped.
    //
    enum {
      DefaultCaptionHeight = 58
    };

  protected:
    // Override TWindow & TFrameWindow virtuals
    //
    HWND    GetCommandTarget();
    void    EvCommandEnable(TCommandEnabler& commandEnabler);
    TResult EvCommand(uint id, HWND hWndCtl, uint notifyCode);

    // Event handler helper
    //
    TEventStatus  DoNCHitTest(TPoint& screenPt, uint& evRes);

    //      Event Handlers
    //
    void    EvSysCommand(uint cmdType, TPoint& p);
    uint    EvNCCalcSize(bool calcValidRects, NCCALCSIZE_PARAMS & calcSize);
    void     EvNCPaint(HRGN);
    uint    EvNCHitTest(TPoint& screenPt);
    bool    EvNCActivate(bool active);
    uint    EvMouseActivate(THandle hTopLevel, uint hitCode, uint msg);
    void    EvActivateApp(bool active, HTASK hTask);

  private:
    TSize   Margin;       // Left and right, top and bottom
    bool    DragFrame;    // True if frame edges are used to drag, not size window
    bool    AppearActive; // Current fake-activation appearance flag

  DECLARE_RESPONSE_TABLE(TFloatingFrame);
  //DECLARE_STREAMABLE(_OWLCLASS, NS_OWL::TFloatingFrame, 1);
  DECLARE_STREAMABLE_OWL(NS_OWL::TFloatingFrame, 1);
};
// define streameable inlines (VC)
DECLARE_STREAMABLE_INLINES( NS_OWL::TFloatingFrame );

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>

//----------------------------------------------------------------------------
// Inline implementations
//

//
// Set the margins for the client
//
inline void TFloatingFrame::SetMargins(const TSize& margin)
{
  Margin = margin;
}

//
// Set the flag for additional dragging area.
//
inline void TFloatingFrame::SetDragFrame(bool dragFrame)
{
  DragFrame = dragFrame;
}

__OWL_END_NAMESPACE


#endif  // OWL_FLOATFRA_H
