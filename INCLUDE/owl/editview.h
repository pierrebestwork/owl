//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.26 $
//$Author: jogybl $
//$Date: 2007-05-11 15:29:13 $
//
// Definition of class TEditView
//----------------------------------------------------------------------------

#if !defined(OWL_EDITVIEW_H)
#define OWL_EDITVIEW_H

#if !defined(OWL_DOCMANAG_H)
# include <owl/docmanag.h>
#endif
#if !defined(OWL_EDITSEAR_H)
# include <owl/editsear.h>
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
// class TEditView
// ~~~~~ ~~~~~~~~~
class _OWLCLASS TEditView : public TEditSearch, public TView {
  public:
    TEditView(TDocument& doc, TWindow* parent = 0);
   ~TEditView();
    static LPCTSTR StaticName();  // put in resource

    // Overridden virtuals from TView
    //
    LPCTSTR    GetViewName();
    TWindow*   GetWindow();
    bool       SetDocTitle(LPCTSTR docname, int index);

    // Overridden virtuals from TWindow
    //
    bool   Create();
    void   PerformCreate(int menuOrId);
    bool   CanClose();

  protected:
    bool   LoadData();

    long   GetOrigin() const;
    void   SetOrigin(long origin);

    // message response functions
    //
    void   EvNCDestroy();
    bool   VnCommit(bool force);
    bool   VnRevert(bool clear);
    bool   VnIsWindow(HWND hWnd);
    bool   VnIsDirty() ;
    bool   VnDocClosed(int omode);

  protected_data:
    long   Origin;

  DECLARE_RESPONSE_TABLE(TEditView);
  //DECLARE_STREAMABLE(_OWLCLASS, NS_OWL::TEditView,1);
  DECLARE_STREAMABLE_OWL(NS_OWL::TEditView,1);
};
// define streameable inlines (VC)
DECLARE_STREAMABLE_INLINES( NS_OWL::TEditView );

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>


//----------------------------------------------------------------------------
// Inline implementation
//

//
inline LPCTSTR TEditView::StaticName() {
  return _T("Edit View");
}  // put in resource

//
inline LPCTSTR TEditView::GetViewName() {
  return StaticName();
}

//
inline TWindow* TEditView::GetWindow() {
  return (TWindow*)this;
}

//
inline bool TEditView::SetDocTitle(LPCTSTR docname, int index) {
  return TEditSearch::SetDocTitle(docname, index);
}

//
inline bool TEditView::CanClose(){
  return TEditSearch::CanClose() &&
         (Doc->NextView(this) ||
          Doc->NextView(0) != this ||
          Doc->CanClose());
}

//
// Return the position of the stream buffer at which the edit buffer is stored.
//
inline long TEditView::GetOrigin() const {
  return Origin;
}

//
// Sets the position of the stream buffer that the edit buffer is stored.
//
inline void TEditView::SetOrigin(long origin) {
  Origin = origin;
}

//
inline bool TEditView::VnIsDirty() {
  return TEditSearch::IsModified();
}


__OWL_END_NAMESPACE


#endif  // OWL_EDITVIEW_H
