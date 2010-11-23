#ifdef __BUILDING_VIEWDLGS_DLL
#  define DLLFUNC __stdcall __declspec(dllexport)
#else
#  define DLLFUNC __stdcall __declspec(dllimport)
#endif

#include "VCLForm.h"

extern "C" {
// ========================================================
// The following functions apply to all VCL-based Document/View classes:
// ========================================================
void  DLLFUNC SetVCLMainWindowHandle(HWND MainWindowHandle);

void  DLLFUNC VCLDialogSetFileDocument(void * VCLDlg, void * VCLDocFile);
void  DLLFUNC VCLDialogShow(void *VCLDlg);
int   DLLFUNC VCLDialogHasChanged(void * VCLDlg);
void  DLLFUNC VCLDialogClearHasChanged(void * VCLDlg);
int   DLLFUNC VCLDialogCanUndo(void * VCLDlg);
void  DLLFUNC VCLDialogDoUndo(void * VCLDlg);
int   DLLFUNC VCLDialogCanCutOrCopy(void * VCLDlg);
void  DLLFUNC VCLDialogDoCutOrCopy(void * VCLDlg, int Copy);
int   DLLFUNC VCLDialogCanPaste(void * VCLDlg);
void  DLLFUNC VCLDialogDoPaste(void * VCLDlg);

int   DLLFUNC VCLFileDocumentLoad(void * const VCLDocFile,
		const char * const Path);
int   DLLFUNC VCLFileDocumentLoadStream(void * const VCLDocFile,
		const char * const Stream, const long int Count);
int   DLLFUNC VCLFileDocumentSave(void * const VCLDocFile,
		const char * const Path, const int BinaryMode);
  // BinaryMode == 0 ==> For DOS or Windows text files
  // BinaryMode != 1 ==> For Unix text files
void  DLLFUNC VCLFileDocumentDestroy(void * VCLDocFile);

// ========================================================
// The following functions are for customized, application-specific
// VCL-based Document/View classes:
// ========================================================
HWND  DLLFUNC CreateExampleView1(HWND hwndParent, const char * const HelpFileName, LPUNKNOWN &pObj);
DWORD DLLFUNC CreateExampleDocument1();

}

#ifndef __BUILDING_VIEWDLGS_DLL
#  undef DLLFUNC
#endif

