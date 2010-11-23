#include <owl/pch.h>
#include <owl/notetab.h>

#include "tabwin.h"

////////////////////////////////////////////////////////////////////////////////

NoteTabOptions::NoteTabOptions()
{
	Style3d = true;
	WindowFace = false;
	RightScroller = true;
}

NoteTabOptions options;

////////////////////////////////////////////////////////////////////////////////
class TMyFaceWindow : public TWindow {
	public:
		TMyFaceWindow(TWindow* parent, bool smile, LPCTSTR title = 0, TModule* module = 0)
		: TWindow(parent, title, module), Smile(smile) {}

		void Paint(TDC& dc, bool erase, TRect& rect);

		void EvSize(uint sizeType, TSize& size);

	private:
		bool Smile;

DECLARE_RESPONSE_TABLE(TMyFaceWindow);
};

DEFINE_RESPONSE_TABLE1(TMyFaceWindow, TWindow)
	EV_WM_SIZE,
END_RESPONSE_TABLE;

void TMyFaceWindow::Paint(TDC& dc, bool erase, TRect& rect)
{
	TWindow::Paint(dc, erase, rect);

	TRect rectClient = GetClientRect();
	int radius = min(rectClient.Width(), rectClient.Height()) / 3;

	TPoint center((rectClient.left + rectClient.right) / 2,
								(rectClient.top + rectClient.bottom) / 2);

	dc.SelectObject(TBrush(TColor::LtYellow));

	dc.Ellipse(TRect(center, TSize(0, 0)).Inflate(radius, radius));

	int mouthRadius = radius / 2;

	if (Smile)
		dc.Arc(TRect(center, TSize(0, 0)).Inflate(mouthRadius, mouthRadius),
					 TPoint(center.x - mouthRadius, center.y + mouthRadius),
					 TPoint(center.x + mouthRadius, center.y + mouthRadius));
	else
		dc.Arc(TRect(TPoint(center.x, center.y + radius), TSize(0, 0)).Inflate(mouthRadius, mouthRadius),
					 TPoint(center.x + mouthRadius, center.y + mouthRadius),
					 TPoint(center.x - mouthRadius, center.y + mouthRadius));

	dc.SelectObject(TBrush(TColor::Black));

	TPoint leftEye(center.x - radius / 3, center.y - radius / 3);
	TPoint rightEye(center.x + radius / 3, center.y - radius / 3);

	int eyeRadius = radius / 10;

	dc.Ellipse(TRect(leftEye, TSize(0, 0)).Inflate(eyeRadius, eyeRadius));
	dc.Ellipse(TRect(rightEye, TSize(0, 0)).Inflate(eyeRadius, eyeRadius));
}

void TMyFaceWindow::EvSize(uint sizeType, TSize& size)
{
	TWindow::EvSize(sizeType, size);

	// Recalc and repaint the face
	Invalidate(true);
}

////////////////////////////////////////////////////////////////////////////////

const idNoteTab = 1;

DEFINE_RESPONSE_TABLE1(TMyTabbedWindow, TWindow)
	EV_TCN_SELCHANGE(idNoteTab, TabSelChange),
	EV_TCN_SELCHANGING(idNoteTab, TabSelChanging),
	EV_WM_SIZE,
	EV_WM_PAINT,
END_RESPONSE_TABLE;


TMyTabbedWindow::TMyTabbedWindow(TWindow *parent, TCelArray* CelArray)
:
	TWindow(parent, "")
{
	tabs = new TNoteTab(this, idNoteTab, 0, 0, 0, 0, 0, false);

	tabs->SetStyle3d(options.Style3d);
	tabs->SetWindowFace(options.WindowFace);

	tabs->SetCelArray(CelArray, NoAutoDelete);

	tabs->SetScrollLocation(options.RightScroller ? alRight : alLeft);

	tabs->Add("Smile", (uint32)new TMyFaceWindow(this, true, "Smile"), 0);
	tabs->Add("Frown", (uint32)new TMyFaceWindow(this, false, "Frown"), 1);
	tabs->Add("Text only", (uint32)new TWindow(this, "Text only"));
	tabs->Add("Right Image", (uint32)new TWindow(this, "Right Image"), 2, alRight);
	OldTabIndex = -1;
}

void TMyTabbedWindow::TabSelChange(TNotify far&)
{
	int index = tabs->GetSel();

	// If the clicked tab is same as previous, do nothing
	if (index == OldTabIndex)
		return;

	if (OldTabIndex >= 0) {
		TNoteTabItem item;

		tabs->GetItem(OldTabIndex, item);

		TWindow *window = (TWindow *)item.ClientData;

		if (window)
			window->ShowWindow(SW_HIDE);
	}

	if (index >= 0) {
		TNoteTabItem item;

		tabs->GetItem(index, item);

		TWindow *window = (TWindow *)item.ClientData;
		if (window) {
			TRect rect = GetClientRect();
			window->ShowWindow(SW_SHOWNORMAL);
			window->MoveWindow(0, 0, rect.Width(),
												 rect.Height() - tabs->GetTabHeight(), true);
		}
	}
}

bool TMyTabbedWindow::TabSelChanging(TNotify far&)
{
	OldTabIndex = tabs->GetSel();
	return false;
}

void TMyTabbedWindow::EvSize(uint sizeType, TSize& size)
{
	TWindow::EvSize(sizeType, size);
	Resize();
}

void TMyTabbedWindow::EvPaint()
{
	TWindow::EvPaint();
	// To prevent an ugly effect when resizing horizontally
	tabs->Invalidate();
}

void TMyTabbedWindow::Resize()
{
	TRect rect = GetClientRect();
	// TNoteTab::GetTabHeight() returns the desired height of the tab window
	//   which is needed to draw the tabs.

	tabs->MoveWindow(0, rect.Height() - tabs->GetTabHeight(),
									 rect.Width(), tabs->GetTabHeight(), true);

	int index = tabs->GetSel();

	if (index >= 0) {
		TNoteTabItem item;

		tabs->GetItem(index, item);

		TWindow *window = (TWindow *)item.ClientData;

		if (window)
			window->MoveWindow(0, 0, rect.Width(),
												 rect.Height() - tabs->GetTabHeight(), true);
	}
}


void TMyTabbedWindow::SetFont(TFont *font)
{
	tabs->SetTabFont((HFONT)(*font), true);

	Resize();
}
////

