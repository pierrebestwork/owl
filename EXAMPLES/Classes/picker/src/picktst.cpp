//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1998 by Bidus Yura, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>        
#pragma hdrstop

#include <owl/decframe.h>
#include <owl/dialog.h>
#include <owl/celarray.h>
#include <owl/glyphbtn.h>
#include <owl/statusba.h>
#include <owl/controlb.h>
#include <owl/editfile.rh>
#include <owl/btntextg.h>
#include <owl/popupgad.h>
#include <owl/picker.h>

#include "picktst.rh"


static COLORREF ColorTable16[16] = {
		{ RGB(0x00,0x00,0x00)},  // Black
		{ RGB(0x80,0x00,0x00)},  // Dark Red
		{ RGB(0x00,0x80,0x00)},  // Green
		{ RGB(0x80,0x80,0x00)},  // Dark yellow
		{ RGB(0x00,0x00,0x80)},  // Dark Blue
		{ RGB(0x80,0x00,0x80)},  // Violet
		{ RGB(0x00,0x80,0x80)},  // Dark Olive Green
		{ RGB(0x80,0x80,0x80)},  // Grey - 50
		{ RGB(0xC0,0xC0,0xC0)},
		{ RGB(0xFF,0x00,0x00)},
		{ RGB(0x00,0xFF,0x00)},
		{ RGB(0xFF,0xFF,0x00)},
		{ RGB(0x00,0x00,0xFF)},
		{ RGB(0xFF,0x00,0xFF)},
		{ RGB(0x00,0xFF,0xFF)},
		{ RGB(0xFF,0xFF,0xFF)}
};

static COLORREF ColorTable8[8] =
{
	RGB(0x00, 0x00, 0x00),
	RGB(0x7F, 0x7F, 0x7F),
	RGB(0x00, 0x00, 0xFF),
	RGB(0xFF, 0x00, 0x00),
	RGB(0xFF, 0xFF, 0x00),
	RGB(0x00, 0xFF, 0x00),
	RGB(0xC0, 0xC0, 0xC0),
	RGB(0xFF, 0xFF, 0xFF)
};

static COLORREF ColorTable40[40] =
{
	RGB(0x00, 0x00, 0x00),
	RGB(0xA5, 0x2A, 0x00),
  RGB(0x00, 0x40, 0x40),
  RGB(0x00, 0x55, 0x00),
  RGB(0x00, 0x00, 0x5E),
  RGB(0x00, 0x00, 0x8B),
  RGB(0x4B, 0x00, 0x82),
	RGB(0x28, 0x28, 0x28),
	RGB(0x8B, 0x00, 0x00),
  RGB(0xFF, 0x68, 0x20),
  RGB(0x8B, 0x8B, 0x00),
  RGB(0x00, 0x93, 0x00),
  RGB(0x38, 0x8E, 0x8E),
  RGB(0x00, 0x00, 0xFF),
  RGB(0x7B, 0x7B, 0xC0),
  RGB(0x66, 0x66, 0x66),
  RGB(0xFF, 0x00, 0x00),
  RGB(0xFF, 0xAD, 0x5B),
  RGB(0x32, 0xCD, 0x32),
  RGB(0x3C, 0xB3, 0x71),
  RGB(0x7F, 0xFF, 0xD4),
  RGB(0x7D, 0x9E, 0xC0),
  RGB(0x80, 0x00, 0x80),
  RGB(0x7F, 0x7F, 0x7F),
  RGB(0xFF, 0xC0, 0xCB),
  RGB(0xFF, 0xD7, 0x00),
  RGB(0xFF, 0xFF, 0x00),
  RGB(0x00, 0xFF, 0x00),
  RGB(0x40, 0xE0, 0xD0),
  RGB(0xC0, 0xFF, 0xFF),
  RGB(0x48, 0x00, 0x48),
  RGB(0xC0, 0xC0, 0xC0),
  RGB(0xFF, 0xE4, 0xE1),
  RGB(0xD2, 0xB4, 0x8C),
  RGB(0xFF, 0xFF, 0xE0),
  RGB(0x98, 0xFB, 0x98),
  RGB(0xAF, 0xEE, 0xEE),
  RGB(0x68, 0x83, 0x8B),
  RGB(0xE6, 0xE6, 0xFA),
  RGB(0xFF, 0xFF, 0xFF)
};



const int IDC_CONTROL1 = 10001;
const int IDC_CONTROL2 = 10002;
const int IDC_CONTROL3 = 10003;
const int IDC_CONTROL4 = 10004;
const int IDC_CONTROL5 = 10005;
const int IDC_CONTROL6 = 10006;
const int IDC_CONTROL7 = 10007;
const int IDC_CONTROL8 = 10008;


////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
TStatusBar*   StatusBar;
TControlBar*  ControlBar;

// This will be for our "main" framewindow
//
class TClientWindow : public TWindow{
	public:
  	TClientWindow(TWindow*parent=0);
  	~TClientWindow();

    void CmNew();
    void CmOpen();
    void CmSave();
		void CmSaveAs();
		void CmColor();
    void CmBmpTest();

    void CmTextBitmap();
    void CmText();
		void CmBitmap();
		void SetStyle(TButtonTextGadget::TStyle style);
    void CeTextBitmap(TCommandEnabler& ce);
    void CeText(TCommandEnabler& ce);
    void CeBitmap(TCommandEnabler& ce);


		void CmTest();
		void CmAbout();

		void CmClicked1();
		void CmClicked2();
		void CmClicked3();
		void CmClicked4();
		void CmClicked5();
		void CmClicked6();
		void CmClicked7();
		void CmClicked8();
		void CmHiLite8();

		void EvSysColorChange();


		TCelArray* 	  CelArray;
		TGridPicker*  Picker1;
		TGridPicker*  Picker2;
		TGridPicker*  Picker3;
		TGridPicker*  Picker4;
		TGridPicker*  Picker5;
		TGridPicker*  Picker6;
		TGridPicker*  Picker7;
		TGridPicker*  Picker8;

    uint          CurrentStyle;

		DECLARE_RESPONSE_TABLE(TClientWindow);
};

DEFINE_RESPONSE_TABLE1(TClientWindow, TWindow)
	EV_COMMAND(CM_TEST, 			CmTest),
	EV_COMMAND(CM_ABOUT, 			CmAbout),
	EV_COMMAND(CM_FILENEW, 		CmNew),
	EV_COMMAND(CM_FILEOPEN,   CmOpen),
	EV_COMMAND(CM_FILESAVE,	  CmSave),
	EV_COMMAND(CM_FILESAVEAS,	CmSaveAs),
	EV_COMMAND(CM_COLOR,      CmColor),
	EV_COMMAND(CM_BMPTEST,    CmBmpTest),
	EV_COMMAND(CM_TEXTBITMAP, CmTextBitmap),
	EV_COMMAND(CM_TEXT,       CmText),
	EV_COMMAND(CM_BITMAP,     CmBitmap),
  EV_COMMAND_ENABLE(CM_TEXTBITMAP,CeTextBitmap),
  EV_COMMAND_ENABLE(CM_TEXT,      CeText),
  EV_COMMAND_ENABLE(CM_BITMAP,    CeBitmap),


	EV_WM_SYSCOLORCHANGE,
	EV_BN_CLICKED(IDC_CONTROL1, CmClicked1),
	EV_BN_CLICKED(IDC_CONTROL2, CmClicked2),
	EV_BN_CLICKED(IDC_CONTROL3, CmClicked3),
	EV_BN_CLICKED(IDC_CONTROL4, CmClicked4),
	EV_BN_CLICKED(IDC_CONTROL5, CmClicked5),
	EV_BN_CLICKED(IDC_CONTROL6, CmClicked6),
	EV_BN_CLICKED(IDC_CONTROL7, CmClicked7),
	EV_BN_CLICKED(IDC_CONTROL8, CmClicked8),
	EV_BN_HILITE(IDC_CONTROL8, CmHiLite8),

END_RESPONSE_TABLE;

TClientWindow::TClientWindow(TWindow*parent)
:
	TWindow(parent),
	CelArray(0),
  CurrentStyle(3)
{
	int i;
	//////////////////////////////////////////////////////////////////////////////
	CelArray = new TCelArray(new TBtnBitmap(*GetModule(),IDB_BMPARRAY), 14);

	//////////////////////////////////////////////////////////////////////////////
	Picker1 = new TGridPicker(this, IDC_CONTROL1, 0, 0, 80, 80);
	for(i = 0; i < 16; i++)
		Picker1->AddCell(new TColorCell(IDS_STRING0+i, TColor(ColorTable16[i])));

	Picker2 = new TGridPicker(this, IDC_CONTROL2, 0, 100, 80, 100);
	Picker2->ModifyPopStyle(0,TGridPicker::psTrackMouse);
	Picker2->EnableDefault("Default",IDS_DEFFIELD,
					new TColorCell(IDS_STRING0+5, TColor(ColorTable16[5])));
	for(i = 0; i < 14; i++)
		Picker2->AddCell(new TColorCell(IDS_STRING0+i, TColor(ColorTable16[i])));

	//////////////////////////////////////////////////////////////////////////////
	Picker3 = new TGridPicker(this, IDC_CONTROL3, 100, 0, 80, 80);
	Picker3->ModifyPopStyle(0,TGridPicker::psFlatSyle);
	for(i = 0; i < 16; i++)
		Picker3->AddCell(new TColorCell(IDS_STRING0+i, TColor(ColorTable16[i])));

	Picker4 = new TGridPicker(this, IDC_CONTROL4, 100, 100,80,130);
	Picker4->ModifyPopStyle(0,TGridPicker::psTrackMouse|TGridPicker::psFlatSyle|
													 TGridPicker::psMessageTips|TGridPicker::psLocalTips);
	Picker4->EnableDefault("Default",IDS_DEFFIELD,
					new TColorCell(IDS_STRING0+5, TColor(ColorTable16[5])));
	Picker4->EnableTooltip();
	for(i = 0; i < 14; i++)
		Picker4->AddCell(new TColorCell(IDS_STRING0+i, TColor(ColorTable16[i])));

	//////////////////////////////////////////////////////////////////////////////
	Picker5 = new TGridPicker(this, IDC_CONTROL5, 200, 0);
	for(i = 0; i < 14; i++)
		Picker5->AddCell(new TBitmapCell(IDS_CHART0+i, CelArray, i));

	Picker6 = new TGridPicker(this, IDC_CONTROL6, 200, 100);
	Picker6->ModifyPopStyle(0,TGridPicker::psTrackMouse|TGridPicker::psLocalTips);
	for(i = 0; i < 14; i++)
		Picker6->AddCell(new TBitmapCell(IDS_CHART0+i, CelArray, i));

	//////////////////////////////////////////////////////////////////////////////
	Picker7 = new TGridPicker(this, IDC_CONTROL7, 300, 0);
	Picker7->SetColumns(8);
	Picker7->ModifyPopStyle(0,TGridPicker::psFlatSyle);
	for(i = 0; i < 14; i++)
		Picker7->AddCell(new TBitmapCell(IDS_CHART0+i, CelArray, i));

	//////////////////////////////////////////////////////////////////////////////
	Picker8 = new TGridPicker(this, IDC_CONTROL8, 300, 80);
	Picker8->ModifyStyle(0,BS_NOTIFY);
	Picker8->SetColumns(4);
	Picker8->ModifyPopStyle(0,TGridPicker::psTrackMouse|TGridPicker::psFlatSyle|
												 TGridPicker::psLocalTips|TGridPicker::psShrinkToCells);
	Picker8->EnableDefault("Default Item",IDS_DEFFIELD,
												 new TBitmapCell(IDS_CHART0+5, CelArray, 5));
	Picker8->EnableButton("Custom field", IDS_CUSTFIELD);
	Picker8->EnableTooltip();
	Picker8->SetMargins(TMargins(TMargins::Pixels, 4, 4, 4, 4));

	for(i = 0; i < 14; i++)
		Picker8->AddCell(new TBitmapCell(IDS_CHART0+i, CelArray, i));

}

TClientWindow::~TClientWindow()
{
	delete CelArray;
}

//
//
//
void
TClientWindow::EvSysColorChange()
{
	TWindow::EvSysColorChange();
	if(CelArray){
		TCelArray array(new TBtnBitmap(*GetModule(),IDB_BMPARRAY), 14);
		for(int i = 0; i< 14; i++)
			CelArray->Replace(i, array, i);
	}
}

void TClientWindow::CmNew(){}
void TClientWindow::CmOpen(){}
void TClientWindow::CmSave(){}
void TClientWindow::CmSaveAs(){}
void TClientWindow::CmColor(){}
void TClientWindow::CmBmpTest(){}

void TClientWindow::SetStyle(TButtonTextGadget::TStyle style)
{
  CurrentStyle = style;
  for(TGadget* g = ControlBar->FirstGadget(); g; g = g->NextGadget()){
  	TButtonTextGadget* bg = TYPESAFE_DOWNCAST(g, TButtonTextGadget);
	  if(bg)
      bg->SetStyle(style, false);
  }
  ControlBar->LayoutSession();

  TDecoratedFrame* frame = TYPESAFE_DOWNCAST(GetApplication()->GetMainWindow(), TDecoratedFrame);
  if(frame){
    frame->Layout();
    frame->Insert(*ControlBar, TDecoratedFrame::Top);
    frame->Layout();
  }
}

void TClientWindow::CmTextBitmap()
{
  SetStyle(TButtonTextGadget::sBitmapText);
}
void TClientWindow::CmText()
{
  SetStyle(TButtonTextGadget::sText);
}
void TClientWindow::CmBitmap()
{
  SetStyle(TButtonTextGadget::sBitmap);
}

void TClientWindow::CeTextBitmap(TCommandEnabler& ce)
{
  ce.Enable(CurrentStyle!=TButtonTextGadget::sBitmapText);
}

void TClientWindow::CeText(TCommandEnabler& ce)
{
  ce.Enable(CurrentStyle!=TButtonTextGadget::sText);
}
void TClientWindow::CeBitmap(TCommandEnabler& ce)
{
  ce.Enable(CurrentStyle!=TButtonTextGadget::sBitmap);
}

//
void
TClientWindow::CmTest()
{
	MessageBox("Test");
}

void
TClientWindow::CmAbout()
{
	TDialog(this, IDD_ABOUT).Execute();
}

void
TClientWindow::CmClicked1()
{
	char buf[40];
  wsprintf(buf,"Picker1 selection %d", Picker1->GetSelection());
	MessageBox(buf);
}

void
TClientWindow::CmClicked2()
{
	char buf[40];
	wsprintf(buf,"Picker2 selection %d", Picker2->GetSelection());
	MessageBox(buf);
}
void
TClientWindow::CmClicked3()
{
	char buf[40];
	wsprintf(buf,"Picker3 selection %d", Picker3->GetSelection());
	MessageBox(buf);
}
void
TClientWindow::CmClicked4()
{
	char buf[40];
  wsprintf(buf,"Picker4 selection %d", Picker4->GetSelection());
	MessageBox(buf);
}
void
TClientWindow::CmClicked5()
{
	char buf[40];
  wsprintf(buf,"Picker5 selection %d", Picker5->GetSelection());
	MessageBox(buf);
}
void
TClientWindow::CmClicked6()
{
	char buf[40];
  wsprintf(buf,"Picker6 selection %d", Picker6->GetSelection());
	MessageBox(buf);
}
void
TClientWindow::CmClicked7()
{
	char buf[40];
  wsprintf(buf,"Picker7 selection %d", Picker7->GetSelection());
	MessageBox(buf);
}
void
TClientWindow::CmClicked8()
{
	char buf[40];
  wsprintf(buf,"Picker8 selection %d", Picker8->GetSelection());
	MessageBox(buf);
}

void
TClientWindow::CmHiLite8()
{
	char buf[40];
	wsprintf(buf,"Picker8 Hilited cell: %d",Picker8->GetFocus());
	StatusBar->SetText(buf);
}

////////////////////////////////////////
class TestApp : public TApplication {
	public:
		TestApp() : CelArray(0){}
		~TestApp() { delete CelArray;}
		void InitMainWindow();
		void AddColorCtrl(TGadgetWindow& cbar);
    void AddBitmapCtrl(TGadgetWindow& cbar);

    TCelArray* CelArray;
};


void
TestApp::AddColorCtrl(TGadgetWindow& cbar)
{

	// first
	TColorPicker* cPicker = new TColorPicker(&cbar, CM_COLOR, 0, 0, 165, 130);
	cPicker->SetColumns(8);
	cPicker->ModifyPopStyle(0,TGridPicker::psTrackMouse|TGridPicker::psFlatSyle|
													TGridPicker::psMessageTips);
	cPicker->EnableDefault("Default",IDS_DEFFIELD,
												 new TColorCell(IDS_STRING1+5, TColor(ColorTable40[5])));
	cPicker->EnableButton("More Colors...",IDS_CUSTFIELD);
	cPicker->EnableTooltip();

	for(int i = 0; i < 39; i++)
		cPicker->AddCell(new TColorCell(IDS_STRING1+i, TColor(ColorTable40[i])));

  cPicker->SetSelection();

	cbar.Insert(*new TColorPickerGadget(*cPicker, CM_COLOR));
}


void
TestApp::AddBitmapCtrl(TGadgetWindow& cbar)
{

	// first
	TPopupPicker* bPicker = new TPopupPicker(&cbar, CM_BMPTEST, 0, 0, 100, 130);
	bPicker->ModifyPopStyle(0,TGridPicker::psTrackMouse|TGridPicker::psFlatSyle|
                            TGridPicker::psLocalTips|TGridPicker::psShrinkToCells);
	bPicker->EnableDefault("Default",IDS_DEFFIELD,
												 new TBitmapCell(IDS_CHART0+5, CelArray, 5));
	//bPicker->EnableTooltip();

	for(int i = 0; i < 14; i++)
		bPicker->AddCell(new TBitmapCell(IDS_CHART0+i, CelArray, i));

  bPicker->SetSelection();

	cbar.Insert(*new TBitmapPickerGadget(*bPicker, CM_BMPTEST));
}

void TestApp::InitMainWindow()
{
	CelArray = new TCelArray(new TBtnBitmap(*this,IDB_BMPARRAY), 14);

	TDecoratedFrame* frame = new TDecoratedFrame(0, "TestApp", new TClientWindow(),true);

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
  //ControlBar = new TControlBar(frame,TControlBar::Horizontal, new TFont(&ncm.lfMessageFont));
	ControlBar = new TControlBar(frame,TControlBar::Horizontal, new TFont(&ncm.lfStatusFont));

	ControlBar->EnableFlatStyle(TGadgetWindow::FlatStandard|TGadgetWindow::FlatHotText);

	ControlBar->Insert(*new TButtonTextGadget(CM_FILENEW, CM_FILENEW));
	ControlBar->Insert(*new TButtonTextGadget(CM_FILEOPEN, CM_FILEOPEN));
	ControlBar->Insert(*new TButtonTextGadget(CM_FILESAVE, CM_FILESAVE));
	ControlBar->Insert(*new TButtonTextGadget(CM_FILESAVEAS, CM_FILESAVEAS));

  TMenu menu(*this, IDM_TRACKMENU);
	ControlBar->Insert(*new TPopupMenuGadget(menu, frame, CM_FILENEW, CM_FILENEW));

  AddBitmapCtrl(*ControlBar);
	AddColorCtrl(*ControlBar);

	frame->Insert(*ControlBar, TDecoratedFrame::Top);

	StatusBar = new TStatusBar(frame);
	frame->Insert(*StatusBar, TDecoratedFrame::Bottom);
	frame->AssignMenu(IDM_APPMENU);
	SetMainWindow(frame);
}

////////////////////////////////////////////////////////////////////////////////
int
OwlMain(int /*argc*/, char* /*argv*/[])
{
	return TestApp().Run();
}
//==============================================================================
