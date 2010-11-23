#include "owlpch.h"
#pragma hdrstop

#include <owl/configfl.h>

#include "client.h"
#include "help.h"
#include "util.h"

#include "replacer.h"

#include "app.rh"            // Definition of all resources.

const char regKeyName[] = "ReplacePlus";

//-----------------------------------------------------------------------------
// Y.B. TConfigFile* must not be persistent variable
class TMyMemComboBox : public TMemComboBox{
	public:
    TMyMemComboBox(TWindow*          parent,
                   int               resId,
                   const owl_string& name,
                   uint              textLimit = 255,
                   uint              itemLimit = 25,
                   TModule*          module = 0)
    : TMemComboBox(parent, resId, name, textLimit, itemLimit, module) {}

		// change default location ("OWL Next")
    TConfigFile* CreateConfigFile() { return new TRegConfigFile(regKeyName); }

    ~TMyMemComboBox() {}
};

//-----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ReplaceClient, TDialog)
//{{ReplaceClientRSP_TBL_BEGIN}}
  //Y.B. EV_BN_CLICKED(IDC_BROWSE, BnBrowseClicked),
	//Y.B.  Since Button send WM_COMMAND after BN_CLICKED we can 
	//Y.B.  also use WM_COMMAND handler, 
	//Y.B.  and WM_COMMAND_ENABLER handler too.
	EV_COMMAND(IDC_BROWSE, BnBrowseClicked),
  EV_BN_CLICKED(IDC_HLP, BnHelpClicked),
  EV_BN_CLICKED(IDC_ACTIONFIND, BnActionClicked),
	EV_BN_CLICKED(IDC_ACTIONTOUCH, BnActionClicked), 
  EV_COMMAND(IDOK, CmOk),
//{{ReplaceClientRSP_TBL_END}}
END_RESPONSE_TABLE;

//-----------------------------------------------------------------------------
ReplaceClient::ReplaceClient(TWindow* parent, TResId resId, TModule* module)
: 
  TDialog(parent, resId == TResId(0) ? TResId(IDD_CLIENT) : resId, module)
{
  gbOk = new TGlyphButton(this, IDOK, TGlyphButton::btOk);                 // Ok
	
	// Y.B.: Only to show how to use (default:=lsH_GST)
  gbOk->SetLayoutStyle(TGlyphButton::lsH_GST);
  gbCancel = new TGlyphButton(this, IDCANCEL, TGlyphButton::btCancel);         // Cancel
  gbCancel->SetLayoutStyle(TGlyphButton::lsH_GST);

  gbBrowse = new TGlyphButton(this, IDC_BROWSE, TGlyphButton::btBrowse);
  gbBrowse->SetLayoutStyle(TGlyphButton::lsH_GST);

  gbHelp = new TGlyphButton(this, IDC_HLP, TGlyphButton::btHelp);
  gbHelp->SetLayoutStyle(TGlyphButton::lsH_GST);

	// Y.B.: Parent,resId, registry location : "ReagistryPlus\History Lists\Folder"
  comboFolder = new TMyMemComboBox(this, IDC_FOLDER, "Folder");
  comboFilter = new TMyMemComboBox(this, IDC_FILTER, "Filter");
  comboFind = new TMyMemComboBox(this, IDC_FIND, "Find");
  comboReplace = new TMyMemComboBox(this, IDC_REPLACE, "Replace");

  pickerTime = new TDateTimePicker(this, IDC_EDITTIME);
  pickerDate = new TDateTimePicker(this, IDC_EDITDATE);

  radioFind = new TRadioButton(this, IDC_ACTIONFIND);
  radioTouch = new TRadioButton(this, IDC_ACTIONTOUCH);

  checkSubfolders = new TCheckBox(this, IDC_SUBFOLDERS);

  stFind    = new TStatic(this, IDC_ST_FIND);
  stReplace = new TStatic(this, IDC_ST_REPLACE);
  stTime    = new TStatic(this, IDC_ST_TIME);

	//Y.B.: Add tooltip control
  tooltip   = new TTooltip(this);
}

//-----------------------------------------------------------------------------
ReplaceClient::~ReplaceClient()
{
  Destroy();
}

//-----------------------------------------------------------------------------
void ReplaceClient::SetupWindow()
{
	TDialog::SetupWindow();

	//Y.B.: Create RegConfigFile instance and read parameters from registry
  TRegConfigFile configFile(regKeyName);
  TConfigFileSection configSection(configFile, "Options");
  bool subfolders = configSection.ReadBool("Subfolders", true);

  int action = configSection.ReadInteger("Action", 1);

  switch (action) {
    default:
    case 1:
      radioFind->SetCheck(BF_CHECKED);
      radioTouch->SetCheck(BF_UNCHECKED);
      break;

    case 2:
      radioFind->SetCheck(BF_UNCHECKED);
      radioTouch->SetCheck(BF_CHECKED);
      break;
  }

  checkSubfolders->SetCheck(subfolders ? BF_CHECKED : BF_UNCHECKED);

  ReplaceData &replaceData = ReplaceData::GetInstance();

  InitCombo(replaceData.GetFolder(), comboFolder);
  InitCombo(replaceData.GetFilter(), comboFilter);
  InitCombo(replaceData.GetFind(),   comboFind);
  InitCombo(replaceData.GetReplace(),comboReplace);

  // If all fields are initialized, perform the operation
  // without showing the dialog
  if (replaceData.GetReplace()) {
    Perform(false);
    PostQuitMessage(0);
  }

  // Add tooltips to various controls
  tooltip->AddTool(TToolInfo(*this, *gbOk, "Perform the action"));
  tooltip->AddTool(TToolInfo(*this, *gbCancel, "Close the program"));

  tooltip->AddTool(TToolInfo(*this, *gbBrowse, "Browse for folder"));
  tooltip->AddTool(TToolInfo(*this, *checkSubfolders, "Recurse subfolders of chosen folder"));

  tooltip->AddTool(TToolInfo(*this, *radioFind, "Find and replace text in files"));
  tooltip->AddTool(TToolInfo(*this, *radioTouch, "Change file date/time"));

  tooltip->AddTool(TToolInfo(*this, *comboFolder, "Enter start folder or select from recent list"));
  tooltip->AddTool(TToolInfo(*this, *comboFilter, "Enter filters like *.cpp;*.c;*.h or select from recent list"));

  tooltip->Activate();

  BnActionClicked();
}

//-----------------------------------------------------------------------------
void ReplaceClient::InitCombo(const char *str, TComboBox *combo)
{
  if (str)
  	combo->SetText(str);
  else if (combo->GetCount() > 0)
  	combo->SetSelIndex(0);
}

//-----------------------------------------------------------------------------
bool ReplaceClient::PreProcessMsg(MSG &msg)
{
	//Y.B. Relay mouse related messages to tooltip 
#if 0
	// This code is taken from the book Core OWL by Ted Neward
  if (tooltip && tooltip->IsWindow())
    tooltip->RelayEvent(msg);
#else // Y.B. this is more right way: 
	// If tooltip exist and it is my message, and is mouse action RelayEvent
	// See Microsoft docs
  if (tooltip && tooltip->IsWindow()) {
    if (msg.hwnd == *this || IsChild(msg.hwnd)){
      if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
        tooltip->RelayEvent(msg);
    }
  }
#endif
  return TDialog::PreProcessMsg(msg);
}
// Two ways to select folder: raw API and OWL TShellItem
//#define USEOWLBROWSE
#ifdef USEOWLBROWSE
#include <owl/shellitm.h>
int CALLBACK BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);
#endif
//-----------------------------------------------------------------------------
void ReplaceClient::BnBrowseClicked()
{
	// Use fast fixed memory allocator for temporary buffer
  TTmpBuffer<_TCHAR> buf(MAX_PATH);

  comboFolder->GetText(buf, MAX_PATH);
	// 1) Raw Windows API
#ifndef USEOWLBROWSE
  if (BrowseForFolder(GetHandle(), buf, "Select root folder")){
		// not very reliable way
    //comboFolder->SetText(buf);
		if(_tcslen(buf)){
			// fix problem with unexact string search
			int index = comboFolder->FindStringExact(buf,-1);
			if(index<0){
			  comboFolder->SetWindowText(buf);
				comboFolder->SetEditSel(0, _tcslen(buf));
			}
			else
				comboFolder->SetSelIndex(index);
		}
	}
#else
  // another way:
	// 1) Using OWL TShellItem class
  CoInitialize(0);

  TShellItem item(TShellItem::MyComputer);
	TShellItem::TCreateStruct cs;

  if(item.BrowseForFolder(cs, *this, "Select root folder",
		  BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS,
			0,true,BrowseCallbackProc, (uint32)(LPTSTR)buf)==IDOK){
		// not very reliable way
		//comboFolder->SetText(item2.GetPath());
		owl_string text = (_TCHAR*)TShellItem(cs).GetPath();
      // VC, BCB and BCC5.5 use STD template string
#if defined(BI_STD_STRING)
		if(!text.empty()){
#else
		if(!text.is_null()){
#endif
			// fix problem with unexact string search
			int index = comboFolder->FindStringExact(text.c_str(),-1);
			if(index<0){
			  comboFolder->SetWindowText(text.c_str());
				comboFolder->SetEditSel(0, text.length());
			}
			else
				comboFolder->SetSelIndex(index);
		}
  }
  CoUninitialize();
#endif
}
//-----------------------------------------------------------------------------
void ReplaceClient::BnActionClicked()
{
  bool bFindAction = (radioFind->GetCheck() == BF_CHECKED);

  comboFind->EnableWindow(bFindAction);
  comboReplace->EnableWindow(bFindAction);
  comboFind->ShowWindow(bFindAction ? SW_SHOW : SW_HIDE);
  comboReplace->ShowWindow(bFindAction ? SW_SHOW : SW_HIDE);
  stFind->ShowWindow(bFindAction ? SW_SHOW : SW_HIDE);
  stReplace->ShowWindow(bFindAction ? SW_SHOW : SW_HIDE);

  pickerTime->EnableWindow(!bFindAction);
  pickerDate->EnableWindow(!bFindAction);
  pickerTime->ShowWindow(!bFindAction ? SW_SHOW : SW_HIDE);
  pickerDate->ShowWindow(!bFindAction ? SW_SHOW : SW_HIDE);
  stTime->ShowWindow(!bFindAction ? SW_SHOW : SW_HIDE);
}
//-----------------------------------------------------------------------------
void ReplaceClient::CmOk()
{
  Perform(true);

	TDialog::CmOk();
}

//-----------------------------------------------------------------------------
void ReplaceClient::BnHelpClicked()
{
  THelpDialog(this).Execute();
}

//-----------------------------------------------------------------------------
void ReplaceClient::Perform(bool showResult)
{
  TTmpBuffer<_TCHAR> folder(MAX_PATH);
  TTmpBuffer<_TCHAR> filter(MAX_PATH);

  comboFolder->GetText(folder, MAX_PATH);
  comboFilter->GetText(filter, MAX_PATH);

  if(folder[0] == '\0'){
    MessageBox("Enter folder.");
    return;
  }

  if (filter[0] == '\0') {
    MessageBox("Enter filter.");
    return;
  }

  bool subfolders = (checkSubfolders->GetCheck() == BF_CHECKED);

  TRegConfigFile configFile(regKeyName);
  TConfigFileSection configSection(configFile, "Options");

  if (radioFind->GetCheck() == BF_CHECKED) {
    TTmpBuffer<_TCHAR> search(MAX_PATH);
    TTmpBuffer<_TCHAR> replace(MAX_PATH);

    comboFind->GetText(search, MAX_PATH);
    comboReplace->GetText(replace, MAX_PATH);

    if (search[0] == '\0') {
      MessageBox("Enter search string.");
      return;
    }

    if (replace[0] == '\0') {
      MessageBox("Enter replace string.");
      return;
    }

    configSection.WriteBool("Subfolders", subfolders);
    configSection.WriteInteger("Action", 1);

    Replacer replacer(search, replace);

    if (FileRecurser(folder, filter, subfolders, &replacer).RecurseFolders())
      if (showResult)
        replacer.ReportResults(this);
  }
  else {
    TSystemTime time, date;

    pickerTime->GetTime(time);
    pickerDate->GetTime(date);

    TTime filetime(date, time.GetHour(), time.GetMinute(), time.GetSecond());

    TFileTime ft(filetime);
    ft.ToUniversalTime();

    filetime = ft;

    configSection.WriteBool("Subfolders", subfolders);
    configSection.WriteInteger("Action", 2);

    Toucher toucher(filetime);

    if (FileRecurser(folder, filter, subfolders, &toucher).RecurseFolders())
      if (showResult)
        toucher.ReportResults(this);
  }
}
//-----------------------------------------------------------------------------
