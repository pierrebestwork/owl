#include "owlpch.h"
#pragma hdrstop

#include <owl/shellitm.h>

#include "util.h"

ReplaceData::ReplaceData()
: folder(0), filter(0), find(0), replace(0), recurse(true)
{
}


ReplaceData::~ReplaceData()
{
  delete [] folder;
  delete [] filter;
  delete [] find;
  delete [] replace;
}

ReplaceData &ReplaceData::GetInstance()
{
  static ReplaceData replaceData;
  return replaceData;
}

void ReplaceData::AddString(const char *str, int len)
{
	// All members are initialized, so ignore the next parameters
  if (replace)
    return;

  // Ignore empty parameters
  if (!str || str[0] == '\0' || len <= 1)
    return;

  char *newstr = new char[len + 1];
  strncpy(newstr, str, len);
  newstr[len] = '\0';

  if (!folder)
    folder = newstr;
  else if (!filter)
    filter = newstr;
  else if (!find)
    find = newstr;
  else
    replace = newstr;
}

////////////////////////////////////////////////////////////////////////////////

// Callback for the BrowseForFolder, which purpose is to set the initial folder
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
	switch (uMsg) {
    case BFFM_INITIALIZED:
      ::SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
      break;
  }

  return 0;
}


bool BrowseForFolder(HWND hWnd, char *buf, char * /*prompt*/)
{
  // Global pointer to the shell's IMalloc interface.
  LPMALLOC g_pMalloc;

  // Get the shell's allocator.
  if (!SUCCEEDED(TShell::SHGetMalloc(&g_pMalloc)))
    return false;

  BROWSEINFO 		bi;
  LPITEMIDLIST  pidlMyComputer;  // PIDL for MyComputer folder
  LPITEMIDLIST  pidlBrowse;      // PIDL selected by user

  // Get the PIDL for the Programs folder.
  if (!SUCCEEDED(TShell::SHGetSpecialFolderLocation(hWnd, CSIDL_DRIVES, &pidlMyComputer)))
    return false;

  // Fill in the BROWSEINFO structure.
  bi.hwndOwner = hWnd;
  bi.pidlRoot = pidlMyComputer;
  bi.pszDisplayName = buf;
  bi.lpszTitle = "Select root folder";
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
  bi.lpfn = BrowseCallbackProc;
  bi.lParam = (LPARAM)buf;

  // Browse for a folder and return its PIDL.
  pidlBrowse = TShell::SHBrowseForFolder(&bi);
  bool result = (pidlBrowse != 0);
  if (result) {
  	if (!TShell::SHGetPathFromIDList(pidlBrowse, buf))
    	result = false;

    // Free the PIDL returned by SHBrowseForFolder.
    g_pMalloc->Free(pidlBrowse);
  }

  // Clean up.
  g_pMalloc->Free(pidlMyComputer);

  return result;
}


