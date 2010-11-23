// =========================================================
// VCLView.cpp
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
//  Source file for implementation of TVCLDialogView (TVCLFormWrapper, TView).
// =========================================================

#include "VCLView.h"
#include "VCLDoc.h"
#include "ViewDlgs.h"
#include <owl/docmanag.h>
#include <all_idds.rh>

extern char * const HelpFileName;

//----------------------------------------------------------------------------
//  class TVCLDialogView
//

DEFINE_RESPONSE_TABLE1(TVCLDialogView,TVCLFormWrapper)
  EV_COMMAND_ENABLE(CM_FILEPRINT, CeFilePrint),
  EV_COMMAND_ENABLE(CM_EDITUNDO, CeUndo),
  EV_COMMAND(CM_EDITUNDO, CmUndo),
  EV_COMMAND_ENABLE(CM_EDITCOPY, CeCopyOrCut),
  EV_COMMAND(CM_EDITCOPY, CmCopy),
  EV_COMMAND_ENABLE(CM_EDITCUT, CeCopyOrCut),
  EV_COMMAND(CM_EDITCUT, CmCut),
  EV_COMMAND_ENABLE(CM_EDITPASTE, CePaste),
  EV_COMMAND(CM_EDITPASTE, CmPaste),
  EV_VN_ISWINDOW,
  EV_VN_ISDIRTY,
  EV_VN_COMMIT,
  EV_VN_REVERT,
END_RESPONSE_TABLE;


TVCLDialogView::TVCLDialogView(TWindowFactoryFunc factoryFunc, TDocument& doc, TWindow* parent)
  : TView(doc),
    TVCLFormWrapper(factoryFunc, parent, 0, doc.GetDocManager().GetApplication(), HelpFileName)
{
  SetVCLMainWindowHandle(GetApplication()->GetMainWindow()->GetHandle());
}


void TVCLDialogView::SetupWindow()
{
  TVCLFormWrapper::SetupWindow();
  VCLDialogSetFileDocument(pVCLForm, ((TVCLFileDocument *)Doc)->GetVCLFileDocument());
  VCLDialogShow(pVCLForm);

  ClearHasChanged();
}


//
// Does a given HWND belong to this view? Yes if it is us, or a child of us
//
bool TVCLDialogView::VnIsWindow(HWND hWnd)
{
  return hWnd == GetHandle() || IsChild(hWnd);
}


void TVCLDialogView::CeFilePrint(TCommandEnabler& ce)
{
  ce.Enable(false);
}


void TVCLDialogView::ClearHasChanged()
{
  HasChanged = false;
  if (pVCLForm)
    VCLDialogClearHasChanged(pVCLForm);
}


bool TVCLDialogView::VnIsDirty()
{
  return HasChanged || (pVCLForm && VCLDialogHasChanged(pVCLForm));
}


bool TVCLDialogView::VnCommit(bool force)
{
  if (!force && !(VnIsDirty()))
    return true;

  bool result = VCLFileDocumentSave(((TVCLFileDocument *)Doc)->GetVCLFileDocument(),
    Doc->GetDocPath(), 0);
  if (result)
    ClearHasChanged();
  else
    Doc->PostError(IDS_WRITEERROR, MB_OK);

  return result;
}

bool TVCLDialogView::VnRevert(bool clear)
{
  if (!clear)
    VCLDialogSetFileDocument(pVCLForm, ((TVCLFileDocument *)Doc)->GetVCLFileDocument());
  ClearHasChanged();
  return true;
}


void TVCLDialogView::CeUndo(TCommandEnabler& commandHandler)
{
  commandHandler.Enable(pVCLForm && VCLDialogCanUndo(pVCLForm));
}

void TVCLDialogView::CmUndo()
{
  if (pVCLForm)
    VCLDialogDoUndo(pVCLForm);
}

void TVCLDialogView::CeCopyOrCut(TCommandEnabler& commandHandler)
{
  commandHandler.Enable(pVCLForm && VCLDialogCanCutOrCopy(pVCLForm));
}

void TVCLDialogView::CmCopy()
{
  if (pVCLForm)
    VCLDialogDoCutOrCopy(pVCLForm,1);
}

void TVCLDialogView::CmCut()
{
  if (pVCLForm)
    VCLDialogDoCutOrCopy(pVCLForm,0);
}

void TVCLDialogView::CePaste(TCommandEnabler& commandHandler)
{
  commandHandler.Enable(pVCLForm && VCLDialogCanPaste(pVCLForm));
}

void TVCLDialogView::CmPaste()
{
  if (pVCLForm)
    VCLDialogDoPaste(pVCLForm);
}


