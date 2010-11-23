#if defined(BI_COMP_BORLANDC) && defined(BI_STD_RTL) // BI_STD_RTL defined only for Builder
#include <condefs.h>
USERC("diagtst.rc");
//---------------------------------------------------------------------------
#endif

// ---------------------------------------------------------------------------
// Copyright (C) 1995, 1995 Borland International
// ---------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/framewin.h>
#include <owl/editfile.h>
#include "diagtst.rh"

DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlDiagTest, 1, 0);

class TTestWindow;
struct TTestHook: public TDiagBaseHook {
	TTestHook(TTestWindow* parent):Parent(parent){}

	virtual void Output(TDiagBase* group, LPCTSTR str);

	TTestWindow* Parent;
};

//
// TTestWindow
// ~~~~~~~~~~~
class TTestWindow : public TEditFile{
	public:
		TTestWindow();
		void CmPrec();
		void CmPrecX();
		void CmCheck();
		void CmCheckX();
		void CmValidPtr();
		void CmTrace();
		void CmTraceX();
		void CmWarn();
		void CmWarnX();
		void CmFunc();
		void CmFuncX();
		void CmFuncP();
		void CmFuncXP();
		void CmLocalHook();
		void CeLocalHook(TCommandEnabler&);
		void CmGlobalHook();
		void CeGlobalHook(TCommandEnabler&);

		TTestHook TestHook;
		TDiagBaseHook* OldHook;
		bool GlobalSet;
		bool LocalSet;

		DECLARE_RESPONSE_TABLE(TTestWindow);
};

void TTestHook::Output(TDiagBase* /*group*/, LPCTSTR str)
{
	Parent->Insert(str);
	Parent->Insert("\r\n");
}

//
//
//
DEFINE_RESPONSE_TABLE1(TTestWindow, TEditFile)
	 EV_COMMAND(CM_PRECONDITION, 			CmPrec),
	 EV_COMMAND(CM_PRECONDITIONX, 		CmPrecX),
	 EV_COMMAND(CM_CHECK, 						CmCheck),
	 EV_COMMAND(CM_CHECKX, 						CmCheckX),
	 EV_COMMAND(CM_VALIDPTR,					CmValidPtr),
	 EV_COMMAND(CM_TRACE,							CmTrace),
	 EV_COMMAND(CM_TRACEX,						CmTraceX),
	 EV_COMMAND(CM_WARN,							CmWarn),
	 EV_COMMAND(CM_WARNX,							CmWarnX),
	 EV_COMMAND(CM_FUNCTION,					CmFunc),
	 EV_COMMAND(CM_FUNCTIONX,					CmFuncX),
	 EV_COMMAND(CM_FUNCTIONP,					CmFuncP),
	 EV_COMMAND(CM_FUNCTIONXP,				CmFuncXP),
	 EV_COMMAND(CM_LOKALHOOK,					CmLocalHook),
	 EV_COMMAND_ENABLE(CM_LOKALHOOK, 	CeLocalHook),
	 EV_COMMAND(CM_GLOBALHOOK,				CmGlobalHook),
	 EV_COMMAND_ENABLE(CM_GLOBALHOOK, CeGlobalHook),
END_RESPONSE_TABLE;


TTestWindow::TTestWindow()
: TestHook(this),GlobalSet(false),LocalSet(false),OldHook(0),
	 TEditFile()
{
	ModifyExStyle(0,WS_EX_CLIENTEDGE);
}

void TTestWindow::CmPrec()
{
	PRECONDITION(0);
}
void TTestWindow::CmPrecX()
{
	PRECONDITIONX(0,"Just PRECONDITIONX");
}
void TTestWindow::CmCheck()
{
	CHECK(0);
}
void TTestWindow::CmCheckX()
{
	CHECKX(0,"Just CHECKX");
}

void TTestWindow::CmValidPtr()
{
	CHECKX(VALIDPTR(0),"Pointer invalid");
}

void TTestWindow::CmTrace()
{
	TRACE("This is simple trace");
}

void TTestWindow::CmTraceX()
{
	TRACEX(OwlDiagTest,0,"This is TRACEX from local group");
}
void TTestWindow::CmWarn()
{
	WARN(1, "This is simple warn");
}
void TTestWindow::CmWarnX()
{
	WARNX(OwlDiagTest,true, 0,"This is WARNX from local group");
}
void TTestWindow::CmFunc()
{
	TRACE_FUNCTION("TTestWindow::CmFunc()");
}
void TTestWindow::CmFuncX()
{
	TRACE_FUNCTIONX(OwlDiagTest,0, "TTestWindow::CmFuncX()");
}
void TTestWindow::CmFuncP()
{
	TRACE_FUNCTIONP("TTestWindow::CmFuncP()","Add comments");
}
void TTestWindow::CmFuncXP()
{
	TRACE_FUNCTIONXP(OwlDiagTest,0, "TTestWindow::CmFuncXP()","Add comments");
}
void TTestWindow::CmLocalHook()
{
	if(!LocalSet){
		DIAG_REDIRECT(OwlDiagTest, &TestHook);
		LocalSet = true;
	}
	else{
		DIAG_REDIRECT(OwlDiagTest, 0);
		LocalSet = false;
	}
}
void TTestWindow::CeLocalHook(TCommandEnabler& tce)
{
	tce.SetCheck(LocalSet?TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}
void TTestWindow::CmGlobalHook()
{
	// something not work here
//	if(!GlobalSet){
		MessageBox("Attention!!!!\nSetting Global hook may be very dangerous.\n"
							 "Can't set it now, check TTraceWnd to see how it works","Warning",MB_OK);
		// use with caution
		//OldHook = DIAG_VAR_NAME(OwlDiagTest).SetGlobalHook(&TestHook);
		//GlobalSet = true;
//	}
//	else{
		//DIAG_VAR_NAME(OwlDiagTest).SetGlobalHook(OldHook);
		//GlobalSet = false;
//	}
}
void TTestWindow::CeGlobalHook(TCommandEnabler& tce)
{
	tce.SetCheck(GlobalSet?TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

//--------------------------------------------------------------------------------
// TTestApplication
// ~~~~~~~~~~~~~~~~
class TTestApp : public TApplication {
	 public:
		 TTestApp():TApplication("Diagnostic application") {}
		 void InitMainWindow();
};

//
void
TTestApp::InitMainWindow()
{
	 TWindow* testWindow = new TTestWindow;
	 TFrameWindow* frame = new TFrameWindow(0, "Diagnostic test", testWindow);
	 // Set the main window and its menu
	 SetMainWindow(frame);
	 GetMainWindow()->AssignMenu(IDM_COMMANDS);
}

//------------------------------------------------------------------------------
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
	 return TTestApp().Run();
}
//==============================================================================
#if defined(BI_COMP_BORLANDC) && defined(BI_STD_RTL) // BI_STD_RTL defined only for Builder
#define WinMain
#endif
