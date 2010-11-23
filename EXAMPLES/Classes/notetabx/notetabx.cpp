////////////////////////////////////////////////////////////////////////////////
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/mdi.h>
#include <owl/mdichild.h>
#include <owl/celarray.h>
#include <owl/choosefo.h>
#include <owl/glyphbtn.h>

#include "tabwin.h"

#include "notetabx.rh"

////////////////////////////////////////////////////////////////////////////////

class TMyMDIClient : public TMDIClient {
  public:
    TMyMDIClient();
    ~TMyMDIClient();

    void CmNewWindow();

    void CmStyle3d();
    void CeStyle3d(TCommandEnabler &tce);

    void CmWindowFace();
    void CeWindowFace(TCommandEnabler &tce);

		void CmLeftScroller();
    void CeLeftScroller(TCommandEnabler &tce);

    void CmRightScroller();
    void CeRightScroller(TCommandEnabler &tce);

    void CmFont();

  private:
	int ChildNum;
	TCelArray* CelArray;
    TFont *Font;

  DECLARE_RESPONSE_TABLE(TMyMDIClient);
};

DEFINE_RESPONSE_TABLE1(TMyMDIClient, TMDIClient)
  EV_COMMAND(CM_NEWWINDOW, CmNewWindow),

  EV_COMMAND(CM_STYLE3D, CmStyle3d),
	EV_COMMAND_ENABLE(CM_STYLE3D, CeStyle3d),

  EV_COMMAND(CM_WINDOWFACE, CmWindowFace),
	EV_COMMAND_ENABLE(CM_WINDOWFACE, CeWindowFace),

  EV_COMMAND(CM_LEFTSCROLLER, CmLeftScroller),
  EV_COMMAND_ENABLE(CM_LEFTSCROLLER, CeLeftScroller),

  EV_COMMAND(CM_RIGHTSCROLLER, CmRightScroller),
  EV_COMMAND_ENABLE(CM_RIGHTSCROLLER, CeRightScroller),

  EV_COMMAND(CM_FONT, CmFont),

END_RESPONSE_TABLE;

TMyMDIClient::TMyMDIClient()
: TMDIClient()
{
  ChildNum = 0;
	if(!options.Style3d && options.WindowFace)
		CelArray = new TCelArray(TDib(*GetModule(),TResId(IDB_IMAGELIST)),3);
	else
		CelArray = new TCelArray(TBtnBitmap(*GetModule(),
													 TResId(IDB_IMAGELIST), TColor::White),3);

	Font = new TFont("MS Sans Serif", -8);
}

TMyMDIClient::~TMyMDIClient()
{
	delete CelArray;
	delete Font;
}


void TMyMDIClient::CmNewWindow()
{
	char childName[15];
	wsprintf(childName, "MDI Child %d", ChildNum++);
	TMyTabbedWindow* wnd = new TMyTabbedWindow(0, CelArray);
	TMDIChild* child = new TMDIChild(*this, childName, wnd);
	child->Create();

	if (Font)
		wnd->SetFont(Font);
}

void TMyMDIClient::CmStyle3d()
{
	options.Style3d = !options.Style3d;
}

void TMyMDIClient::CeStyle3d(TCommandEnabler &tce)
{
	tce.SetCheck(options.Style3d ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyMDIClient::CmWindowFace()
{
	options.WindowFace = !options.WindowFace;
}

void TMyMDIClient::CeWindowFace(TCommandEnabler &tce)
{
	tce.SetCheck(options.WindowFace ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyMDIClient::CmLeftScroller()
{
	options.RightScroller = false;
}

void TMyMDIClient::CeLeftScroller(TCommandEnabler &tce)
{
	tce.SetCheck(!options.RightScroller ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyMDIClient::CmRightScroller()
{
	options.RightScroller = true;
}

void TMyMDIClient::CeRightScroller(TCommandEnabler &tce)
{
	tce.SetCheck(options.RightScroller ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyMDIClient::CmFont()
{
	TChooseFontDialog::TData data;

	Font->GetObject(data.LogFont);

	data.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

	if (TChooseFontDialog(this, data).Execute() == IDOK) {
		delete Font;

		Font = new TFont(&data.LogFont);
	}
}

////////////////////////////////////////////////////////////////////////////////

class TMDIApp : public TApplication {
	public:
		TMDIApp() : TApplication("TNoteTab Example") {}

		void InitMainWindow();
};

void TMDIApp::InitMainWindow()
{
	SetMainWindow(new TMDIFrame(GetName(), IDM_MDIMENU, *new TMyMDIClient));
}

////////////////////////////////////////////////////////////////////////////////
int OwlMain(int, char* [])
{
	return TMDIApp().Run();
}
