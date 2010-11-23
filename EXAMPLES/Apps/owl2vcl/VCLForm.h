#if !defined(__VCLForm_h)              // Sentry, use file only if it's not already included.
#define __VCLForm_h

// =========================================================
// VCLForm.h
//
// VCL Form Wrapper for OWL
//
// Author:  Conrad Herrmann at Borland.  The original is on the BC5.02 CD:
//          \BC5\DOC\DELPHI\BC5_D2\CODE\MDISAMP\DLWrap.h
//          Modified by Greg Bullock
// Written: 25 March 1997
//
// =========================================================

#if (__WIN32__)
// from CONTROLS.PAS Delphi 2.0: 0xBC00
# define CN_BASE 0xBC00
//typedef HWND (__stdcall (*TWindowFactoryFunc))(HWND hwndParent, const char * const HelpFileName, LPUNKNOWN &pObj);
typedef HWND (__stdcall (aWindowFactoryFunc))(HWND hwndParent, const char * const HelpFileName, LPUNKNOWN &pObj);
#else
// from CONTROLS.PAS Delphi 1.0: 0x2000
# define CN_BASE 0x2000
//typedef HWND (FAR PASCAL *TWindowFactoryFunc)(HWND hwndParent, const char * const HelpFileName, LPUNKNOWN &pObj);
typedef HWND (FAR PASCAL aWindowFactoryFunc)(HWND hwndParent, const char * const HelpFileName, LPUNKNOWN &pObj);
#endif
typedef aWindowFactoryFunc* TWindowFactoryFunc;

#ifdef OWL_WINDOW_H

class TVCLFormWrapper: public TWindow
{
public:
  TVCLFormWrapper(TWindowFactoryFunc factoryFunc,
  	TWindow* parent, const char far* title = 0, TModule* module = 0,
    const char * const helpFileName = 0)
  	: TWindow( parent, title, module ),
      Factory(factoryFunc),
      pVCLForm(0),
      FormHelpFileName(helpFileName) { }

  virtual void ConstrainWindowResize(int &Width, int &Height) { }

protected:
  bool PreProcessMsg( MSG &msg );
  bool Register() { return true; } // don't bother, Delphi will do this
  void PerformCreate(int menuOrId) {
      SetHandle(Factory(Parent->GetHandle(), FormHelpFileName, pVCLForm));
  }
public:
  LPUNKNOWN pVCLForm;
  const char * const FormHelpFileName;
private:
  TWindowFactoryFunc Factory;
};



#endif

#ifdef __AFXWIN_H__

class CVCLFormWrapper: public CView
{
public:
  CVCLFormWrapper(TWindowFactoryFunc factoryFunc)
  	: Factory(factoryFunc), pVCLForm(0) { }

protected:
  BOOL PreTranslateMessage(LPMSG lpMsg) {
    // duplicates the code in FORMS.PAS for IsKeyMsg()
    if (m_hWnd && (lpMsg->message >= WM_KEYFIRST) && (lpMsg->message <= WM_KEYLAST) &&
      (::GetCapture() == 0)) {
      if (SendMessage(CN_BASE + lpMsg->message, lpMsg->wParam, lpMsg->lParam) != 0 )
        return true;
    }
    return CWnd::PreTranslateMessage(lpMsg);
  }
  virtual void OnDraw(CDC *)
  {
    // do nothing; we should never be called.
  }

public:
  // for child windows, views, panes etc
  BOOL Create(CWnd *pParentWnd)
  {
      Attach( Factory(pParentWnd->m_hWnd, pVCLForm) );
      return m_hWnd != NULL;
  }
  afx_msg void OnPaint()
  {
    // don't do CView::OnPaint, we are a control!
    Default();
  }

public:
  LPUNKNOWN pVCLForm;
private:
  TWindowFactoryFunc Factory;
};

#endif

#endif                                      // __VCLForm_h sentry.
