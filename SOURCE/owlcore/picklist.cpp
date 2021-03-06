//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.27 $
//$Author: jogybl $
//$Date: 2007-09-15 11:43:49 $
//
// Implementation of classes TPickListPopup & TPickListDialog.
//
// Both of these modal windows allow the selction of a string from a list.
// TPickListPopup uses a lean popup menu for a quick selection, while
// TPickListDialog uses a listbox in a dialog with OK and Cancel buttons.
//----------------------------------------------------------------------------
#include <owl/pch.h>

#if !defined(OWL_PICKLIST_H)
# include <owl/picklist.h>
#endif
#if !defined(OWL_PICKLIST_RH)
# include <owl/picklist.rh>
#endif

__OWL_USING_STD_NAMESPACE

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;

//
// Construct a popup menu.
// Add the title at the top of the menu.
//
TPickListPopup::TPickListPopup(TWindow* parent, LPCTSTR title)
:
  TWindow(parent, 0, 0),
  Count(0)
{
  PRECONDITION(Popup.GetHandle());

  if (title) {
    Popup.AppendMenu(MF_GRAYED, 0, title);
    Popup.AppendMenu(MF_SEPARATOR);
  }
}

//
// Construct a popu menu.
// Add the title from resource at the top of the menu.
//
TPickListPopup::TPickListPopup(TWindow* parent, uint titleId)
:
  TWindow(parent, 0, 0),
  Count(0)
{
  PRECONDITION(Popup.GetHandle());

  TCHAR buf[_MAX_PATH];
  if (!titleId || !GetApplication()->LoadString(titleId, buf, _MAX_PATH))
    return;

  Popup.AppendMenu(MF_GRAYED, 0, buf);
  Popup.AppendMenu(MF_SEPARATOR);
}

//
// Clear all strings from the popup
//
void
TPickListPopup::ClearStrings()
{
  while (Count)
    Popup.RemoveMenu(--Count, MF_BYCOMMAND);
}

//
// Add a string to the popup
//
int
TPickListPopup::AddString(LPCTSTR str)
{
  PRECONDITION(Popup.GetHandle());
  Popup.AppendMenu(MF_ENABLED, Count++, str);
  return Count;
}

//
// Displays the popup menu and returns the command id of the menu item the
// user selected.
//
int
TPickListPopup::Execute()
{
  // Create the hidden window
  //
  Create();

  // Reset the index result
  //
  Result = -2;

  // Grab current mouse location
  //
  TPoint cursorLoc;
  GetCursorPos(cursorLoc);

  // Display section & cleanup
  //
  Popup.TrackPopupMenu(0, cursorLoc, 0, GetHandle());
  if(!IsWindow())
    return -2;
//  while (Popup.GetMenuItemCount())     //!CQ Menus clean up their own items
//    Popup.DeleteMenu(0, MF_BYPOSITION);

  // Dispatch any pending WM_COMMAND stuck in the message queue.
  //
  CHECK(GetApplication());
  GetApplication()->PumpWaitingMessages();

  return Result;
}

//
// Generic handler for WM_COMMAND messages.
// Result is set to the id of the menu item clicked.
//
TResult
TPickListPopup::EvCommand(uint id, THandle /*hWndCtl*/, uint /*notifyCode*/)
{
//  WARNX(OwlDocView, id > Count, 0, "TPickListPopup index invalid");
  Result = id;
  return 0;
}

//----------------------------------------------------------------------------

//
//
//
DEFINE_RESPONSE_TABLE1(TPickListDialog, TDialog)
  EV_COMMAND(IDOK,     CmOK),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_LBN_DBLCLK(IDC_LIST, CmOK),
END_RESPONSE_TABLE;

//
// Initialize the dialog.
// Sets the initial selection of the listbox.
// Allocate strings if necessary.
//
TPickListDialog::TPickListDialog(TWindow*      parent,
                                 TStringArray* strings,
                                 int           initialSelection,
                                 TResId        templateId,
                                 LPCTSTR       title,
                                 TModule*      module)
:
  TDialog(parent, templateId ? templateId : TResId(IDD_PICKLISTDIALOG), module),
  List(this, IDC_LIST, module),
  Result(initialSelection),
  Strings(strings ? strings : new TStringArray),
  NewedStrings(!strings)
{
  if (title)
    SetCaption(title);
}

//
// Deletes any allocated string.
//
TPickListDialog::~TPickListDialog()
{
  if (NewedStrings)
    delete Strings;
}

//
// Clear all strings from the list
//
void
TPickListDialog::ClearStrings()
{
  Strings->Flush();
  if (List.GetHandle())
    List.ClearList();
}

//
// Add a string to the Strings list, and to the List box if it has already been
// created.
//
int
TPickListDialog::AddString(LPCTSTR str)
{
  Strings->Add(str);
  if (List.GetHandle())
    List.AddString(str);
  return Strings->Size()-1;
}

//
// Add a string to a listbox, passed via the args from ForEach.

static void
AddToList(owl_string& str, void* param)
{
  TListBox* list = REINTERPRET_CAST(TListBox*,param);
  if (list && list->GetHandle()) // just in case
    list->AddString(str.c_str());
}
//
// Override from TDialog.
// Adds each string into the listbox.
//
void
TPickListDialog::SetupWindow()
{
  TDialog::SetupWindow();

  // Add each string in Strings to the List box
  //
  Strings->ForEach(NS_OWL::AddToList, &List);

  // Set inital state of listbox
  //
  if (List.GetCount() > Result) {
    List.SetSelIndex(Result);
    List.SetCaretIndex(Result, false);
  }
}

//
// User selected OK. Get selection from the listbox & return it.
//
void
TPickListDialog::CmOK()
{
  int index = List.GetSelIndex();
  if (index >= 0) {
    // Save the selection index.
    Result = index;
  }
  CloseWindow(Result);
}

//
// User selected Cancel. Return < 0 (Can't use -1 since it signals a dialog
// failure)
//
void
TPickListDialog::CmCancel()
{
  CloseWindow(-2);
}


__OWL_END_NAMESPACE
/* ========================================================================== */

