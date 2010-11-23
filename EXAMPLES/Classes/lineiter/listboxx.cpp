//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/listbox.h>
#include <owl/opensave.h>
#include <owl/file.h>
#include <string.h>
#include <owl/editfile.rh>
#include "listbox.rh"

//----------------------------------------------------------------------------

//
// class TListBoxApp
// ~~~~~ ~~~~~~~~~~~
class TListBoxApp : public TApplication {
  public:
    void InitMainWindow();

  void   CmFileOpen();

  DECLARE_RESPONSE_TABLE(TListBoxApp);
};

//
//
DEFINE_RESPONSE_TABLE1(TListBoxApp,TApplication)
  EV_COMMAND(CM_FILEOPEN, CmFileOpen),
END_RESPONSE_TABLE;


void
TListBoxApp::CmFileOpen()
{
  static TOpenSaveDialog::TData data (
    OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOREADONLYRETURN,
    "Text Files (*.txt)|*.txt|",  0,  "",  "txt" );

  TListBox* lb = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(), TListBox);
  CHECK(lb);

  if (TFileOpenDialog(GetMainWindow(), data).Execute() == IDOK) {
    char fileTitle[MAX_PATH];
    TOpenSaveDialog::GetFileTitle(data.FileName, fileTitle, MAX_PATH);

    TTextFile file(data.FileName);
    if(!file.IsOpen())
    	return;

   	lb->ClearList();
    TFileLineIterator fitr(file);
    while(fitr){
			lb->AddString(fitr);
      ++fitr;
    }
    GetMainWindow()->SetCaption(strlwr(fileTitle));
  }
}

void
TListBoxApp::InitMainWindow()
{
  TFrameWindow* frame = new TFrameWindow(0, "ListBox Example", new TListBox(0,123,0,0,0,0));
  frame->GetClientWindow()->ModifyStyle(LBS_SORT, 0);
  frame->AssignMenu(IDM_LISTBOX_MENU);
  SetMainWindow(frame);
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TListBoxApp().Run();
}
