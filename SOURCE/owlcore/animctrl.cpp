//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.26 $
//$Author: jogybl $
//$Date: 2007-05-11 15:29:17 $
//
// Implementation of the TAnimateCtrl class
//----------------------------------------------------------------------------
#if  defined(__BORLANDC__)
#pragma hdrignore SECTION
#endif
#include <owl/pch.h>

#if !defined(OWL_ANIMCTRL_H)
# include <owl/animctrl.h>
#endif

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;
DIAG_DECLARE_GROUP(OwlCommCtrl);

#if !defined(SECTION) || SECTION == 1
// Constructor for a TAnimateCtrl to be associated with a new underlying
// control.
//
TAnimateCtrl::TAnimateCtrl(TWindow* parent, int id, int x, int y, int w, int h,
                           TModule* module)
:
  TControl(parent, id, _T(""), x, y, w, h, module)
{
  if (!TCommCtrl::IsAvailable())
    TXCommCtrl::Raise();
  Attr.Style = WS_CHILD | WS_VISIBLE | ACS_CENTER | ACS_TRANSPARENT;
  TRACEX(OwlCommCtrl, OWL_CDLEVEL, _T("TAnimateCtrl constructed @") << (void*)this);
}

#endif
#if !defined(SECTION) || SECTION == 2

// Constructor to alias a control defined in a dialog resource
//
TAnimateCtrl::TAnimateCtrl(TWindow* parent, int resourceId, TModule* module)
:
 TControl(parent, resourceId, module)
{
  if (!TCommCtrl::IsAvailable())
    TXCommCtrl::Raise();
  TRACEX(OwlCommCtrl, OWL_CDLEVEL, _T("TAnimateCtrl constructed @") << (void*)this);
}

#endif
#if !defined(SECTION) || SECTION == 3
// Destructor
//
TAnimateCtrl::~TAnimateCtrl()
{
  TRACEX(OwlCommCtrl, OWL_CDLEVEL, _T("TAnimateCtrl deleted @") << (void*)this);
}

//
// Returns the predefined class registered by the Common Control library
// for the Animation control.
//
LPCTSTR
TAnimateCtrl::GetClassName()
{
  return ANIMATE_CLASS;
}

#endif
#if !defined(SECTION) || SECTION == 4
// Opens an .AVI file and displays the first frame.
// The 'res' parameter may be a resourceId if the .AVI is from a resource.
// It may also be a full path of an .AVI file.
// Use NULL (0) as 'res' to close any previously opened .AVI file.
// if module != 0 and version of commctrl.dll >= 4.7
// will open resource from this module 
bool
TAnimateCtrl::Open(LPCTSTR res, TModule* module)
{
  if(module && TCommCtrl::Dll()->GetCtrlVersion() > ComCtlVersionIE4)
    return SendMessage(ACM_OPEN, TParam1(module->GetHandle()), TParam2(res)) != 0;
   return SendMessage(ACM_OPEN, 0, TParam2(res)) != 0;
}

#endif
#if !defined(SECTION) || SECTION == 5

// Plays the .AVI file from frame index 'start' to frame index 'end'.
// The 'repeat' parameter is the number of times to play the frames.
// Use -1 for repeat to play indefinitely.
// NOTE: The control plays the clip in the backgroud while the 
//       current thread continues executing.
//
bool
TAnimateCtrl::Play(uint16 start, uint16 end, uint repeat)
{
  return SendMessage(ACM_PLAY, repeat, MkUint32(start, end)) != 0;
}

// Stops playing the current AVI file.
//
void
TAnimateCtrl::Stop()
{
  SendMessage(ACM_STOP);
}

#endif
#if !defined(SECTION) || SECTION == 6

// Seek to frame index 'frame'. The value is zero-based and must not
// exceed 65,536.
//
bool
TAnimateCtrl::Seek(uint16 frame)
{
  return Play(frame, frame, 0);
}

#endif

__OWL_END_NAMESPACE
/* ========================================================================== */

