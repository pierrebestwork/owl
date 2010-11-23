// =========================================================
// VCLForm.CPP
//
// VCL Form Wrapper for OWL
//
// Author:  Conrad Herrmann at Borland.  The original is on the BC5.02 CD:
//          \BC5\DOC\DELPHI\BC5_D2\CODE\MDISAMP\DLWrap.h
//          Modified by Greg Bullock
// Written: 25 March 1997
//
// =========================================================

#include <owl\pch.h>
#pragma hdrstop

#include "VCLForm.h"

bool TVCLFormWrapper::PreProcessMsg( MSG &msg )
{
  // duplicates the code in FORMS.PAS for IsKeyMsg()
//    if ((msg.message >= WM_KEYFIRST) && (msg.message <= WM_KEYLAST) &&
  if ((msg.message <= WM_KEYLAST) && ((msg.message >= WM_SYSKEYDOWN) ||
        ((msg.message >= WM_KEYFIRST) && ((msg.lParam & 0x20000000) || (msg.wParam == VK_TAB)))) &&
// msg.lParam & 0x20000000 if Alt key is held down,
// msg.lParam & 0x01000000 if extended key
    (GetCapture() == 0) && pVCLForm) {
    if (SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam) != 0)
      return true;
  }

  // Calling IsDialogMessage when the user strikes an accelerator key for an MDI
  // menu (e.g., Alt-F for the File menu) seems to stall -- the function never
  // returns.  Disable it for now.
  // Indeed, initial tests suggest that everything is working proper now.
//  if (IsDialogMessage( GetHandle(), &msg ))
//    return true;
  return false;
}
