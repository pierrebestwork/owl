#if !defined(__VCLView_h)              // Sentry, use file only if it's not already included.
#define __VCLView_h

// =========================================================
// VCLView.h
//
// VCL Dialog View Wrapper for OWL
//
// Author:  Greg Bullock of Mission Research Corporation
//          bullock@mrcmry.com
//
//          May be used without restrictions.
//          If you make improvements to this file, I would greatly appreciate
//          it if you would send them to me.
//
// Written: 5 April 2000
//
//  OVERVIEW
//  ========
//  Class definition for TVCLDialogView (TVCLFormWrapper, TView).
// =========================================================

#include <owl\pch.h>
#pragma hdrstop
#include <owl/docview.h>

#include "VCLForm.h"

//
//  class TVCLDialogView
//  ----- -----------
//
class TVCLDialogView : public TVCLFormWrapper, public TView {
  public:
    TVCLDialogView(TWindowFactoryFunc factoryFunc, TDocument& doc, TWindow* parent = 0);
   ~TVCLDialogView() {Destroy();}
    static const char far* StaticName() {return "VCL Dialog View";}  // put in resource

    // inherited virtuals from TVCLFormWrapper
    //
    void SetupWindow();

    // Override virtuals from TWindow
    //
    bool     CanClose() {
        return TWindow::CanClose() &&
               (Doc->NextView(this) ||
                Doc->NextView(0) != this ||
                Doc->CanClose());
      }
      
    // inherited virtuals from TView
    //
    const char far* GetViewName() {return StaticName();}
    TWindow* GetWindow() {return (TWindow*)this;}
    bool     SetDocTitle(const char far* docname, int index)
             { return TWindow::SetDocTitle(docname, index); }

    void SetHasChanged() { HasChanged = true; }
    void ClearHasChanged();

  protected:
    bool HasChanged;

    // event handlers
    //
    void CeFilePrint(TCommandEnabler& ce);
    bool VnIsWindow(HWND hWnd);
    bool VnIsDirty();
    bool VnCommit(bool force);
    bool VnRevert(bool clear);
    void CmUndo();
    void CeUndo(TCommandEnabler& commandHandler);
    void CmCopy();
    void CmCut();
    void CeCopyOrCut(TCommandEnabler& commandHandler);
    void CmPaste();
    void CePaste(TCommandEnabler& commandHandler);

  DECLARE_RESPONSE_TABLE(TVCLDialogView);
};

#endif                                      // __VCLView_h sentry.
