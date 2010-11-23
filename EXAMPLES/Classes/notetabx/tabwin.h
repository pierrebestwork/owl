#if !defined(tabwin_h)              
#define tabwin_h

#include <owl/window.h>
#include <owl/notetab.h>
#include <owl/celarray.h>


////////////////////////////////////////////////////////////////////////////////

struct NoteTabOptions {
	NoteTabOptions();

	bool Style3d;
	bool WindowFace;
	bool RightScroller;
};

extern NoteTabOptions options;

////////////////////////////////////////////////////////////////////////////////

class TMyTabbedWindow : public TWindow
{
	public:
		TMyTabbedWindow(TWindow *parent, TCelArray* celArray);

		void SetFont(TFont *font);


	protected:
		void TabSelChange(TNotify far&);
		bool TabSelChanging(TNotify far&);
		void EvSize(uint sizeType, TSize& size);
		void EvPaint();

	private:
		TNoteTab *tabs;
		int OldTabIndex;
		void Resize();
	DECLARE_RESPONSE_TABLE(TMyTabbedWindow);
};

#endif