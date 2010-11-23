////////////////////////////////////////////////////////////////////////////////
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/layoutwi.h> //must include for <owl/tabbed.h>
#include <owl/tabbed.h>
#include "testpage.rh"
#include <stdio.h>

class TPagedWindow: public TTabbedWindow {
	private:
		int PageNum;
	public:
		TPagedWindow();
		void CmAddPage();

	DECLARE_RESPONSE_TABLE(TPagedWindow);
};
//
DEFINE_RESPONSE_TABLE1(TPagedWindow, TTabbedWindow)
	EV_COMMAND(CM_ADDPAGE, CmAddPage),
END_RESPONSE_TABLE;
//
TPagedWindow::TPagedWindow()
{
	PageNum = 1;

	TWindow* ptrPage1 = new TWindow(this);
	TWindow* ptrPage2 = new TWindow(this);

	Add(*ptrPage1, "First");
	Add(*ptrPage2, "Second");

	ptrPage1->SetBkgndColor(TColor::LtYellow);
	ptrPage1->ModifyExStyle(0,WS_EX_CLIENTEDGE);
	ptrPage2->SetBkgndColor(TColor::LtGreen);
	ptrPage2->ModifyExStyle(0,WS_EX_CLIENTEDGE);

	GetTabControl()->SetSel(0);
}
//
void TPagedWindow::CmAddPage()
{
	 _TCHAR PageName[15];
	 sprintf(PageName, "New Page %d", PageNum++);
	 TWindow* wnd = new TWindow(this,PageName);
	 wnd->ModifyExStyle(0, WS_EX_CLIENTEDGE);
	 wnd->SetBkgndColor(TColor::LtBlue);

	 AddPage(*wnd);
}

//
class TestApp : public TApplication {
	public:
		TestApp() : TApplication("TabWindow Tester"){}
		void InitMainWindow();
};
//
void
TestApp::InitMainWindow()
{
	TFrameWindow* frame = new TFrameWindow(0, "TabWindow Tester", new TPagedWindow);
	frame->AssignMenu(IDM_MENU1);
	SetMainWindow(frame);
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
	return TestApp().Run();
}
//////////////////
