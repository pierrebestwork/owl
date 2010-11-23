//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEUNIT("DocBase.cpp");
USERES("ViewDlgs.res");
USEFORM("DlgBase.cpp", DialogViewBaseForm);
USEFORM("ExView1.cpp", ExampleView1);
USEUNIT("ExDoc1.cpp");
//---------------------------------------------------------------------------
#define __BUILDING_VIEWDLGS_DLL
#include <sstream>
#include <fstream>
#include "ViewDlgs.h"
#include "ExView1.h"
#include "ExDoc1.h"

// ========================================================
// Define the functions that apply to all VCL-based Document/View classes:
// ========================================================
//---------------------------------------------------------------------------

void  DLLFUNC SetVCLMainWindowHandle(HWND MainWindowHandle)
{
  Application->Handle = MainWindowHandle;
  // The online VCL Help for Application->Handle says to do this
}
//---------------------------------------------------------------------------

void DLLFUNC VCLDialogSetFileDocument(void * VCLDlg, void * VCLDocFile)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  TBasicDocument *doc = static_cast<TBasicDocument *>(VCLDocFile);
  if (form && doc)
    form->SetDocument(doc);
}
//---------------------------------------------------------------------------

void DLLFUNC VCLDialogShow(void *VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  if (form)
    form->Show();
}
//---------------------------------------------------------------------------

int DLLFUNC VCLDialogHasChanged(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  return form && form->GetHasChanged();
}
//---------------------------------------------------------------------------

void DLLFUNC VCLDialogClearHasChanged(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  if (form)
    form->SetHasChanged(false);
}
//---------------------------------------------------------------------------

int DLLFUNC VCLDialogCanUndo(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  return form &&
    (dynamic_cast<TCustomEdit *>(form->ActiveControl)
          && SendMessage(form->ActiveControl->Handle, EM_CANUNDO, 0,0));
}
//---------------------------------------------------------------------------

void DLLFUNC VCLDialogDoUndo(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  if (!form) return;
  if (dynamic_cast<TCustomEdit *>(form->ActiveControl))
    SendMessage(form->ActiveControl->Handle, EM_UNDO, 0,0);
}
//---------------------------------------------------------------------------

int DLLFUNC VCLDialogCanCutOrCopy(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  return form &&
    (dynamic_cast<TCustomEdit *>(form->ActiveControl)
      && dynamic_cast<TCustomEdit *>(form->ActiveControl)->SelLength > 0);
}
//---------------------------------------------------------------------------

void DLLFUNC VCLDialogDoCutOrCopy(void * VCLDlg, int Copy)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  if (!form) return;
  if (dynamic_cast<TCustomEdit *>(form->ActiveControl)) {
    if (Copy)
      dynamic_cast<TCustomEdit *>(form->ActiveControl)->CopyToClipboard();
    else
      dynamic_cast<TCustomEdit *>(form->ActiveControl)->CutToClipboard();
  }
}
//---------------------------------------------------------------------------

int DLLFUNC VCLDialogCanPaste(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  return form && dynamic_cast<TCustomEdit *>(form->ActiveControl);
}
//---------------------------------------------------------------------------

void DLLFUNC VCLDialogDoPaste(void * VCLDlg)
{
  TDialogViewBaseForm *form = dynamic_cast<TDialogViewBaseForm *>(static_cast<TObject *>(VCLDlg));
  if (!form) return;
  if (dynamic_cast<TCustomEdit *>(form->ActiveControl))
    dynamic_cast<TCustomEdit *>(form->ActiveControl)->PasteFromClipboard();
}
//---------------------------------------------------------------------------

int DLLFUNC VCLFileDocumentLoad(void * const VCLDocFile, const char * const Path)
{
  int result(0);
  TBasicDocument *doc = static_cast<TBasicDocument * const>(VCLDocFile);
  if (doc) {
    std::ifstream is(Path);
    is >> *doc;
    result = !is.bad();
  }
  return result;
}
//---------------------------------------------------------------------------

int DLLFUNC VCLFileDocumentLoadStream(void * const VCLDocFile, const char * const Stream, const long int Count)
{
  int result(0);
  TBasicDocument *doc = static_cast<TBasicDocument * const>(VCLDocFile);
  if (doc) {
    std::istringstream is(std::string(Stream,Count));
    is >> *doc;
    result = !is.bad();
  }
  return result;
}
//---------------------------------------------------------------------------

int DLLFUNC VCLFileDocumentSave(void * const VCLDocFile, const char * const Path, const int BinaryMode)
{
  // BinaryMode == 0 ==> For DOS or Windows text files
  // BinaryMode != 1 ==> For Unix text files
  int result(0);
  TBasicDocument * const doc = static_cast<TBasicDocument * const>(VCLDocFile);
  if (doc && doc->GetDialogData(0)) {
    std::ios_base::openmode mode(std::ios_base::out);
    if (BinaryMode)
      mode |= std::ios_base::binary;
    std::ofstream os(Path, mode);
    os << *doc;
    result = !os.bad();
  }
  return result;
}
//---------------------------------------------------------------------------

void DLLFUNC VCLFileDocumentDestroy(void * VCLDocFile)
{
  TBasicDocument *doc = static_cast<TBasicDocument *>(VCLDocFile);
  if (doc)
    delete doc;
}
//---------------------------------------------------------------------------

// ========================================================
// Define the functions that are for customized, application-specific
// VCL-based Document/View classes:
// ========================================================

HWND  DLLFUNC CreateExampleView1(HWND hwndParent, const char * const HelpFileName, LPUNKNOWN &pDlg)
{
  HWND Result(0);
  TDialogViewBaseForm::HWndParent = reinterpret_cast<HWND>(hwndParent);
  Application->CreateForm(__classid(TExampleView1), &pDlg);
  TDialogViewBaseForm::HWndParent = 0;
  if (pDlg) {
	Result = reinterpret_cast<TExampleView1 *>(pDlg)->Handle;
	if (HelpFileName)
	  Application->HelpFile = AnsiString(HelpFileName);
  }
  return Result;
}
//---------------------------------------------------------------------------

DWORD DLLFUNC CreateExampleDocument1()
{
  return reinterpret_cast<DWORD>(new TExampleDocument1());
}
//---------------------------------------------------------------------------


int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
  return 1;
}
//---------------------------------------------------------------------------
