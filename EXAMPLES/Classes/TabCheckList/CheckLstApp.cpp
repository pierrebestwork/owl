// CheckLstApp.cpp implementation file
//
// Test application that demonstate the CheckLst classes
//

#include <owl\pch.h>
#pragma hdrstop

#include <owl/framewin.h>
#include <owl/inputdia.h>
#include <owl/checklst.h>

#include "CheckLstApp.rh"

class TCheckLstDialog : public TDialog {
	public:
  	TCheckLstDialog (TWindow* parent);
  	~TCheckLstDialog ();

  protected:
  	virtual void SetupWindow();

		void ShowInformation();
    void DisableButtons();

		void CmSet3State();
		void CmDisableFirst();

		void CmAdd();
		void CmRename();
		void CmDelete();

    void CmDefaultTab();
    void CmEqualTab();
    void CmDiffTab();

    int GetDialogUnitNumber();

  protected:
    TCheckList* 		CheckList;
    TCheckList* 		SortedCheckList;
    TTabCheckList* 	TabbedCheckList;

	//TIPtrArray<TCheckListItem*> Items;
	TCheckListArray Items;
	//TIPtrArray<TCheckListItem*> TabItems;
	TCheckListArray  TabItems;

   DECLARE_RESPONSE_TABLE(TCheckLstDialog);
};

DEFINE_RESPONSE_TABLE1(TCheckLstDialog, TDialog)
	EV_CHILD_NOTIFY(IDC_LB_SORTED, CBN_SELCHANGE, ShowInformation),
	EV_CHILD_NOTIFY(IDC_LB_SIMPLE, CBN_SELCHANGE, DisableButtons),

  EV_BN_CLICKED(IDC_CB_SET3STATE, CmSet3State),
  EV_BN_CLICKED(IDC_CB_DISABLEFIRST, CmDisableFirst),

  EV_BN_CLICKED(IDC_PB_ADD, CmAdd),
  EV_BN_CLICKED(IDC_PB_RENAME, CmRename),
  EV_BN_CLICKED(IDC_PB_DELETE, CmDelete),

  EV_BN_CLICKED(IDC_RB_DEFAULTTAB, CmDefaultTab),
  EV_BN_CLICKED(IDC_RB_EQUALTAB, CmEqualTab),
  EV_BN_CLICKED(IDC_RB_DIFFTAB, CmDiffTab),
END_RESPONSE_TABLE;

TCheckLstDialog::TCheckLstDialog(TWindow* parent):
  TDialog(parent, IDD_TEST)
{

  // Create first item intermidiate diabled
  //
  Items.Add(new TCheckListItem("a item", BF_GRAYED, false));
  TabItems.Add(new TCheckListItem("first item\t1. item\t1", BF_GRAYED, false));

  // Create second item checked enabled
  //
  Items.Add(new TCheckListItem("c item", true));// can set BF_CHECKED instead true
  TabItems.Add(new TCheckListItem("second item\t1. item\t1", true));

  // Create third item unchecked enabled
  //
  Items.Add(new TCheckListItem("d item", false));// can set BF_UNCHECKED instead false
  TabItems.Add(new TCheckListItem("third item\t1. item\t1", false));

  // Create the fourth item intermidiate enabled
  //
  Items.Add(new TCheckListItem("b item", BF_GRAYED));
  TabItems.Add(new TCheckListItem("fourth item\t1. item\t1", BF_GRAYED));

  CheckList 			= new TCheckList(this, IDC_LB_SIMPLE,Items);
  SortedCheckList = new TCheckList(this, IDC_LB_SORTED,Items);
  TabbedCheckList = new TTabCheckList(this, IDC_LB_TABBED,TabItems);
}
//
//
//
TCheckLstDialog::~TCheckLstDialog()
{
}


void TCheckLstDialog::SetupWindow()
{
  // Insert the items in the Listboxes
  //

  TDialog::SetupWindow();


  // Initialize and update controls
  //
  SortedCheckList->SetSelIndex(0);
  CheckDlgButton(IDC_CB_DISABLEFIRST, true);
  CheckDlgButton(IDC_CB_SET3STATE, true);
  CheckDlgButton(IDC_RB_DEFAULTTAB, true);
  ShowInformation();
  DisableButtons();
  CmSet3State();
}

void TCheckLstDialog::ShowInformation()
{
  // show information for sorted checklistbox
  //
  char idxstr[255], statestr[255], stylestr[255];
  int idx = SortedCheckList->GetSelIndex();
  if (idx == -1){
    // nothing selected. show empty informations
    //
    ::SetWindowText(GetDlgItem(IDC_STATIC_INDEX), "");
    ::SetWindowText(GetDlgItem(IDC_STATIC_STATE), "");
    ::SetWindowText(GetDlgItem(IDC_STATIC_STYLE), "");
    return;
  }
  wsprintf(idxstr, "%d", idx);

  if (SortedCheckList->GetItem(idx)->GetThreeStates())
    wsprintf(stylestr, "%s", "Three state");
  else
    wsprintf(stylestr, "%s", "Two state");

  if (!SortedCheckList->GetItem(idx)->IsEnabled())
    wsprintf(statestr, "%s", "Disabled");
  else{
    switch (SortedCheckList->GetItem(idx)->GetState()){
      case BF_CHECKED:   wsprintf(statestr, "%s", "Checked"); break;
      case BF_UNCHECKED: wsprintf(statestr, "%s", "Unchecked"); break;
      default:           wsprintf(statestr, "%s", "Indeterminate"); break;
    }
  }
  ::SetWindowText(GetDlgItem(IDC_STATIC_INDEX), idxstr);
	::SetWindowText(GetDlgItem(IDC_STATIC_STATE), statestr);
	::SetWindowText(GetDlgItem(IDC_STATIC_STYLE), stylestr);
}

void TCheckLstDialog::DisableButtons()
{
  int idx = CheckList->GetSelIndex();
  bool state = (idx == -1) ? false : true;
  ::EnableWindow(GetDlgItem(IDC_PB_RENAME), state);
  ::EnableWindow(GetDlgItem(IDC_PB_DELETE), state);
}

void TCheckLstDialog::CmSet3State()
{
  // Set 3 states only for sorted list
  //
  bool set3States = IsDlgButtonChecked(IDC_CB_SET3STATE);
  for (uint i=0; i<Items.Size(); i++)
    SortedCheckList->GetItem(i)->SetThreeStates(set3States);
  SortedCheckList->Invalidate();
  SortedCheckList->UpdateWindow();
  ShowInformation();
}

void TCheckLstDialog::CmDisableFirst()
{
  // disable first only for sorted list
  //
  bool disableFirst = IsDlgButtonChecked(IDC_CB_DISABLEFIRST);
  if (Items.Size() > 0){
    if (disableFirst)
      SortedCheckList->GetItem(0)->Disable();
    else
      SortedCheckList->GetItem(0)->Enable();
    SortedCheckList->Invalidate();
    SortedCheckList->UpdateWindow();
    ShowInformation();
  }
}

void TCheckLstDialog::CmAdd()
{
  char buffer[255];
  *buffer = 0;
  if (TInputDialog(this, "New TCheckListItem", "Enter name of item:",
    buffer, 255).Execute() == IDOK)
  {
    // Create new TCheckListItem and insert them
    //
    TCheckListItem* item = new TCheckListItem(buffer, true);
    int idx = CheckList->AddItem(item);
    Items.AddAt(item, idx);
    CheckList->SetSelIndex(idx);
  }
}

void TCheckLstDialog::CmRename()
{
  int idx = CheckList->GetSelIndex();
  if (idx == -1)
    return;

  char buffer[255];
  Items[idx]->GetText(buffer, 255);
  if (TInputDialog(this, "Rename TCheckListItem", "Enter name of item:",
    buffer, 255).Execute() == IDOK){
    Items[idx]->SetText(buffer);
    CheckList->Invalidate();
    CheckList->UpdateWindow();
  }
}

void TCheckLstDialog::CmDelete()
{
  int idx = CheckList->GetSelIndex();
  if (idx == -1)
    return;

  CheckList->DetachItem(idx);
  Items.Destroy(idx);

  if (idx > 0)
  	CheckList->SetSelIndex(idx-1);
  else
  	CheckList->SetSelIndex(0);
  DisableButtons();
}

void TCheckLstDialog::CmDefaultTab()
{
	TabbedCheckList->SetTabStops(0, 0);
  TabbedCheckList->Invalidate();
  TabbedCheckList->UpdateWindow();
}

void TCheckLstDialog::CmEqualTab()
{
	int nTabStop = GetDialogUnitNumber() / 3;
	int* lpTabStop = new int[2];
	lpTabStop[0] = nTabStop;
	lpTabStop[1] = 2*nTabStop;
 	TabbedCheckList->SetTabStops(1, &nTabStop);
	delete[] lpTabStop;
  TabbedCheckList->Invalidate();
  TabbedCheckList->UpdateWindow();
}

void TCheckLstDialog::CmDiffTab()
{
	int nTabStop = GetDialogUnitNumber() / 6;
	int* lpTabStop = new int[2];
	lpTabStop[0] = nTabStop * 3;
	lpTabStop[1] = nTabStop * 5;
	TabbedCheckList->SetTabStops(2, lpTabStop);
	delete[] lpTabStop;
  TabbedCheckList->Invalidate();
  TabbedCheckList->UpdateWindow();
}

int TCheckLstDialog::GetDialogUnitNumber()
{
	TRect rect;
	TabbedCheckList->GetWindowRect(rect);
	HFONT hFont = TabbedCheckList->GetWindowFont();
	LOGFONT lf;
  ::GetObject((HGDIOBJ)hFont, sizeof(LOGFONT), (LPVOID)&lf);
	int nBaseUnit = lf.lfWidth;
	if (nBaseUnit == 0)
		nBaseUnit = LOWORD(::GetDialogBaseUnits());
	return (rect.Width()*4) / nBaseUnit;
}

class TestWindow : public TWindow {
	public:
   	TestWindow() : TWindow(0, 0, 0) {}
      void CmTest();
      void CmAbout();

   DECLARE_RESPONSE_TABLE(TestWindow);
};

DEFINE_RESPONSE_TABLE1(TestWindow, TWindow)
  EV_COMMAND(CM_TEST, CmTest),
  EV_COMMAND(CM_ABOUT, CmAbout),
END_RESPONSE_TABLE;

void
TestWindow::CmTest()
{
	TCheckLstDialog(this).Execute();
}

void
TestWindow::CmAbout()
{
  MessageBox("CheckLst Example written by Dieter Windau", "About", MB_OK);
}

class TCheckLstApp : public TApplication {
	public:
		TCheckLstApp() : TApplication() {}
    ~TCheckLstApp()
    {
    }

		void InitMainWindow()
		{
			MainWindow = new TFrameWindow(0, "CheckLst Test App", new TestWindow());
			MainWindow->AssignMenu("MENU_1");
    }
};

int
OwlMain(int /*argc*/, char* /*argv*/[])
{
  return TCheckLstApp().Run();
}
