//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//   Tutorial application -- step01.cpp
//----------------------------------------------------------------------------
#include "owlpch.h"
 
class TDrawApp : public TApplication {
  public:
    TDrawApp() : TApplication(_T("TDrawApp")) {}

    void InitMainWindow()
    {
      SetMainWindow(new TFrameWindow(0, _T("Sample ObjectWindows Program")));
    }
};

int
OwlMain(int /*argc*/, _TCHAR* /*argv*/ [])
{
  return TDrawApp().Run();
}
/////////////////