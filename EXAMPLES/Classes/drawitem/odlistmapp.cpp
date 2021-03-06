//----------------------------------------------------------------------------
//  Project odlistm
//  My Company
//  Copyright 1998. All rights reserved.
//
//  SUBSYSTEM:    odlistm Application
//  FILE:         odlistmapp.cpp
//  AUTHOR:       Author
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TodlistmAppl (TApplication).
//  (Generated by OWL 6.0 Wizard for VC.)
//----------------------------------------------------------------------------
#include "owlpch.h"
#include <owl/listbox.h>
#include <owl/combobox.h>
#include <owl/celarray.h>
#include <owl/drawitem.h>  // drawitem


#include "resource.h"

#include <stdio.h>


#include "odlistmapp.h"
#include "odlistmcl.h" // Definition of client class.

#include "odlistmab.h"  // Definition of about dialog.

///////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------
class TDrawListBoxItem: public TButtonTextDrawItem{
	public:
		TDrawListBoxItem(TDrawItemProxy* proxy, int index, LPCTSTR title);
};
//
TDrawListBoxItem::TDrawListBoxItem(TDrawItemProxy* proxy, int index, LPCTSTR title)
:
	TButtonTextDrawItem(proxy)
{
	SetText(title);
	SetIndex(index);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//{{TSDIDecFrame Implementation}}

DEFINE_RESPONSE_TABLE1(TSDIDecFrame, TDecoratedFrame)
//  EV_WM_INITMENU,
//  EV_WM_DRAWITEM,
//  EV_WM_MEASUREITEM,
  EV_WM_MENUCHAR,
END_RESPONSE_TABLE;


TDrawMenuItemProxy* Proxy;


TSDIDecFrame::TSDIDecFrame(TWindow* parent, LPCTSTR title, TWindow* clientWnd, bool trackMenuSelection, TModule* module)
:
  TDecoratedFrame(parent, title, !clientWnd ? new TodlistmView(0, 0, 0) : clientWnd, trackMenuSelection, module)
{
  Proxy = new TDrawMenuItemProxy(IDB_BITMAP2);
}


TSDIDecFrame::~TSDIDecFrame()
{
  delete Proxy;
}
//
void TSDIDecFrame::SetupWindow()
{
	TDecoratedFrame::SetupWindow();
	Proxy->RemapMenu(GetSystemMenu(false));
	Proxy->RemapMenu(GetMenu());

	TDrawMenuItem* item = (TDrawMenuItem*)Proxy->FindItem(CM_FIRST_ITEM);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_3Angle);

	item = (TDrawMenuItem*)Proxy->FindItem(CM_SECOND_ITEM);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_Arrow);

	item = (TDrawMenuItem*)Proxy->FindItem(CM_THIRD_ITEM);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_Box);

	item = (TDrawMenuItem*)Proxy->FindItem(CM_ITEM4);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_Diamond);
	item = (TDrawMenuItem*)Proxy->FindItem(CM_ITEM5);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_Dot);
	item = (TDrawMenuItem*)Proxy->FindItem(CM_ITEM6);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_Plus);
	item = (TDrawMenuItem*)Proxy->FindItem(CM_ITEM7);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_V);
	item = (TDrawMenuItem*)Proxy->FindItem(CM_ITEM8);
	if(item)
		item->SetCheckPainter(TDrawMenuItem::PaintCheck_X);

	//DrawMenuBar();
}
//
int32 TSDIDecFrame::EvMenuChar(uint nChar, uint menuType, HMENU hMenu)
{
	TParam2 retval;
  if(Proxy->EvMenuChar(nChar, menuType, hMenu, retval))
    return retval;
  return TDecoratedFrame::EvMenuChar(nChar, menuType, hMenu);
}

//
//------------------------------------
//
// class TDialogCountry
// ~~~~~ ~~~~~~~~~~~~~~
//
class TDialogCountry : public TDialog {
  TListBox*		country;
	TComboBox*	ODBox;

public:

  TDialogCountry(TWindow*);
  ~TDialogCountry();
  void SetupWindow();
  int Execute();


  void EvDrawItem(UINT, DRAWITEMSTRUCT far&);
  void EvMeasureItem(UINT, MEASUREITEMSTRUCT far&);

	TDrawItemProxy* Proxy;

  DECLARE_RESPONSE_TABLE(TDialogCountry);
};

DEFINE_RESPONSE_TABLE1(TDialogCountry, TDialog)
//  EV_WM_DRAWITEM,
//  EV_WM_MEASUREITEM,
END_RESPONSE_TABLE;

TDialogCountry::TDialogCountry(TWindow* parent)
				  : TDialog(parent, IDD_DIALOG1)
{
  country = new TListBox(this, IDC_LIST1);
	ODBox = new TComboBox(this, IDC_COMBO1);
	Proxy = new TDrawItemProxy();

  Proxy->SetCelArray(IDB_BITMAP2,24);
}
//
TDialogCountry::~TDialogCountry()
{
	delete Proxy;
}
//
int TDialogCountry::Execute()
{
  return TDialog::Execute();
}
//
void TDialogCountry::SetupWindow()
{
  TDialog::SetupWindow();

	_TCHAR buffer[80];
	int i;
  for (i = 0; i < 8; i++) {
		wsprintf(buffer,_T("%02d Owner Draw Item"),i);
		TDrawListBoxItem* item = new TDrawListBoxItem(Proxy, 0, buffer);
		item->SetCelArray(IDB_BITMAP2+i,1);
		country->AddString((LPCTSTR)item); // don't use flag item_string
		Proxy->RegisterItem(item); // auto destroy feature
		item = new TDrawListBoxItem(Proxy, i, buffer);
		Proxy->RegisterItem(item); // auto destroy feature
		ODBox->AddString((LPCTSTR)item);
  }
	country->SetSelIndex(0);
	ODBox->SetSelIndex(0);
}
//
void TDialogCountry::EvDrawItem(UINT ctrlId, DRAWITEMSTRUCT far& dis)
{
	if(dis.itemID < 0)
		Proxy->GetItems()[0]->Draw(dis); 
	else 
		TDialog::EvDrawItem(ctrlId, dis);
}
//
void TDialogCountry::EvMeasureItem(UINT ctrlId, MEASUREITEMSTRUCT far& mis)
{
	if(GetHandle() && mis.itemID >= 0){
			TDialog::EvMeasureItem(ctrlId, mis);
	}
	else{ 
		// for empty or not created control
		// First message send here while creating dialog,before SetupWindow()
	  TSize size = Proxy->GetCelArray()->CelSize();
		mis.itemWidth		= size.cx;
		mis.itemHeight	= size.cy;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
//{{TodlistmAppl Implementation}}

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TodlistmAppl, TApplication)
//{{TodlistmApplRSP_TBL_BEGIN}}
  EV_COMMAND(CM_FILENEW, CmFileNew),
  EV_COMMAND(CM_FILEOPEN, CmFileOpen),
	EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
	EV_COMMAND(CM_FIRST_ITEM, CmItem1),
	EV_COMMAND_ENABLE(CM_FIRST_ITEM, CeItem1),
	EV_COMMAND(CM_SECOND_ITEM, CmItem2),
	EV_COMMAND_ENABLE(CM_SECOND_ITEM, CeItem2),
	EV_COMMAND(CM_THIRD_ITEM, CmItem3),
	EV_COMMAND_ENABLE(CM_THIRD_ITEM, CeItem3),
	EV_COMMAND(CM_ITEM4, CmItem4),
	EV_COMMAND_ENABLE(CM_ITEM4, CeItem4),
	EV_COMMAND(CM_ITEM5, CmItem5),
	EV_COMMAND_ENABLE(CM_ITEM5, CeItem5),
	EV_COMMAND(CM_ITEM6, CmItem6),
	EV_COMMAND_ENABLE(CM_ITEM6, CeItem6),
	EV_COMMAND(CM_ITEM7, CmItem7),
	EV_COMMAND_ENABLE(CM_ITEM7, CeItem7),
	EV_COMMAND(CM_ITEM8, CmItem8),
	EV_COMMAND_ENABLE(CM_ITEM8, CeItem8),
	EV_COMMAND(CM_ENABLEALL, CmEnableall),
	EV_COMMAND_ENABLE(CM_ENABLEALL, CeEnableall),
	EV_COMMAND(CM_TEST_DIALOG, CmTestDialog),
//{{TodlistmApplRSP_TBL_END}}
END_RESPONSE_TABLE;


//--------------------------------------------------------
// TodlistmAppl
//

TodlistmAppl::TodlistmAppl() : TApplication("odlistm")
{

  // Common file file flags and filters for Open/Save As dialogs.  Filename and directory are
  // computed in the member functions CmFileOpen, and CmFileSaveAs.
  //
  FileData.Flags |= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  FileData.SetFilter(_T("All Files (*.*)|*.*|"));
  FileData.DefExt = _T("txt");
  // INSERT>> Your constructor code here.
	Flag = 1;
}


TodlistmAppl::~TodlistmAppl()
{
  // INSERT>> Your destructor code here.
}

	
//--------------------------------------------------------
// TodlistmAppl
// ~~~~~
// Application intialization.
//
void TodlistmAppl::InitMainWindow()
{
  if (nCmdShow != SW_HIDE)
    nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  TSDIDecFrame* frame = new TSDIDecFrame(0, GetName(), 0, false);


  // Assign icons for this application.
  //
  frame->SetIcon(this, IDI_SDIAPPLICATION);
  frame->SetIconSm(this, IDI_SDIAPPLICATION);

  // Menu associated with window and accelerator table associated with table.
  //
  frame->AssignMenu(IDM_SDI);

  // Associate with the accelerator table.
  //
  frame->Attr.AccelTable = IDM_SDI;

  SetMainWindow(frame);

  frame->SetMenuDescr(TMenuDescr(IDM_SDI));
}



//--------------------------------------------------------
// TodlistmAppl
// ~~~~~~~~~~~
// Menu File New command
//
void TodlistmAppl::CmFileNew()
{
  TodlistmView* client = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(),
                             TodlistmView);     // Client window for the frame.
  client->NewFile();
}


//------------------------------------------------------------------------------
// TodlistmAppl
// ~~~~~~~~~~~
// Menu File Open command
//
void TodlistmAppl::CmFileOpen()
{
  // Display standard Open dialog box to select a file name.
  //
  *FileData.FileName = 0;

  TodlistmView* client = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(),
                             TodlistmView);     // Client window for the frame.
  if (client->CanClose())
    if (TFileOpenDialog(GetMainWindow(), FileData).Execute() == IDOK)
      OpenFile();
}

void TodlistmAppl::OpenFile(const char* fileName)
{
  if (fileName)
    _tcscpy(FileData.FileName, fileName);

  TodlistmView* client = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(), TodlistmView);     // Client window for the frame.
  client->ReplaceWith(FileData.FileName);
}

void TodlistmAppl::CmItem1()
{
	Flag = 1;
}
void TodlistmAppl::CeItem1(TCommandEnabler& tce)
{
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==1 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmItem2()
{
	Flag = 2;
}
void TodlistmAppl::CeItem2(TCommandEnabler& tce)
{
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==2 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
} 
void TodlistmAppl::CmItem3()
{
	Flag = 3;
}
void TodlistmAppl::CeItem3(TCommandEnabler& tce)
{
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==3 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmItem4 ()
{	
	Flag = 4;
}
void TodlistmAppl::CeItem4 (TCommandEnabler& tce)
{	
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==4 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmItem5 ()
{	
	Flag = 5;
}
void TodlistmAppl::CeItem5 (TCommandEnabler& tce)
{	
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==5 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmItem6 ()
{	
	Flag = 6;
}
void TodlistmAppl::CeItem6 (TCommandEnabler& tce)
{	
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==6 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmItem7 ()
{	
	Flag = 7;
}
void TodlistmAppl::CeItem7 (TCommandEnabler& tce)
{	
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==7 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmItem8 ()
{	
	Flag = 8;
}
void TodlistmAppl::CeItem8 (TCommandEnabler& tce)
{	
	tce.Enable(Flag>0);
	tce.SetCheck(Flag==8 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmEnableall ()
{	
	Flag = -Flag;
}
void TodlistmAppl::CeEnableall (TCommandEnabler& tce)
{	
	tce.SetCheck(Flag>0 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TodlistmAppl::CmHelpAbout()
{
  // Show the modal dialog.
  //
	TodlistmAboutDlg(GetMainWindow()).Execute();
}
void TodlistmAppl::CmTestDialog()
{
	TDialogCountry(GetMainWindow()).Execute();
}
//================================================
int OwlMain(int , LPTSTR [])
{
  return TodlistmAppl().Run();
}
////////////////////////////////////////////////
