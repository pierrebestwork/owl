//----------------------------------------------------------------------------
//Test1
//  Project Test1
//My Copmany
//Copyright 1998. All right reserved.
//
//  SUBSYSTEM:    Test1 Application
//  FILE:         Test1.cpp
//  AUTHOR:       Author
//
//  OVERVIEW
//  ~~~~~~~~
//
//  Source file for implementation of TTest1Appl (TApplication).
//----------------------------------------------------------------------------

#include <owl/pch.h>
#include <owl/framewin.h>
#include <owl/opensave.h>
#include <conio.h>

#include <iostream.h>


class TTest1Appl : public TApplication {
  public:
	TTest1Appl() : TApplication("Test1") {}
	void 	InitInstance();
	void 	InitMainWindow();
};

void
TTest1Appl::InitMainWindow()
{
	string wndClass = TEXT("tty");
	if(TSystem::IsNT())
		wndClass = TEXT("ConsoleWindowClass");

	HWND hWnd = 0;
	//Usar AllocConsole or CreateProcess function to create a new console
	bool bRet=AllocConsole();
	if (bRet) {
		// sync stdio with the console
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr); 
		
		// sync iostream with the console
		cin.sync_with_stdio();
		cout.sync_with_stdio();
		cerr.sync_with_stdio();
		clog.sync_with_stdio();
	}

	TAPointer<TCHAR> titles = new TCHAR[MAX_PATH];
	if(GetConsoleTitle(titles, MAX_PATH)){
		hWnd = ::FindWindow(wndClass.c_str(), titles);
		if(!hWnd){
			cerr << "Error found windows" << ends;
		getch();
		}
	}

	// create alias for console window
	if(hWnd)
		SetMainWindow(new TFrameWindow(hWnd, ::Module));
}

void
TTest1Appl::InitInstance()
{
  TApplication::InitInstance();

  // INSERT>> Your all code here.
	TOpenSaveDialog::TData data(0,"All Files (*.*)|*.*",0,"","*");
	TFileOpenDialog(GetMainWindow(),data).Execute();

	cout << "File Selected: ";
	cout << data.FileName << endl;

	char szBuff[32];
	printf("Press [Enter]");
	gets(szBuff);
/*
#if defined(BI_PLAT_BORLANDC)
	getch();
#else
	_getch();
#endif
*/
}


int OwlMain(int , char* [])
{
  return TTest1Appl().Start();
}
