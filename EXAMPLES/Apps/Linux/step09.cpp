//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//   Tutorial application -- step09.cpp
//----------------------------------------------------------------------------
#include "owlpch.h"

#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/dc.h>
#include <owl/inputdia.h>
#include <owl/opensave.h>
#include <owl/gdiobjec.h>
#include <owl/chooseco.h>
#include <stdlib.h>
#include <string.h>
#include "step09.rc"

// example of using STL
#include <vector>
typedef std::vector<TPoint> TPoints;
typedef std::vector<TPoint>::iterator TPointsIterator;

class TLine;
ostream& operator <<(ostream& os, const TLine& line);
istream& operator >>(istream& is, TLine& line);

class TLine : public TPoints {
  public:
    // Constructor to allow construction from a color and a pen size.
    // Also serves as default constructor.
    TLine(const TColor& color = TColor(0), int penSize = 1) :
      PenSize(penSize), Color(color) {}


#if defined(_DEBUG) && !defined(BI_OWL_NO_DEBUG_CRT)
		void* operator new(unsigned, void* ptr)
						{ return ptr;	}
		void* operator new(size_t sz) 
						{ return ::operator new(sz, _CLIENT_BLOCK, NULL, 0);	}
		void operator delete(void* p){	_free_dbg(p, _CLIENT_BLOCK); }
#	if _MSC_VER >= 1200
		void PASCAL operator delete(void *p, LPCSTR, int)
					{		_free_dbg(p, _CLIENT_BLOCK); }
		void PASCAL operator delete(void*, void*)
					{	; }
#	endif
#endif

    // Functions to modify and query pen attributes.
    int QueryPenSize() const
    {
      return PenSize;
    }

    TColor& QueryColor()
    {
      return Color;
    }

    void SetPen(TColor& newColor, int penSize = 0);
    void SetPen(int penSize);

    // TLine draws itself.  Returns true if everything went OK.
    virtual bool Draw(TDC&);

    // The == operator must be defined for the container class, even if unused
    bool operator ==(const TLine& other) const
    {
      return &other == this;
    }

    friend ostream& operator <<(ostream& os, const TLine& line);
    friend istream& operator >>(istream& is, TLine& line);

  protected:
    int PenSize;
    TColor Color;
};

void
TLine::SetPen(int penSize)
{
  if (penSize < 1)
    PenSize = 1;
  else
    PenSize = penSize;
}

void
TLine::SetPen(TColor& newColor, int penSize)
{
  // If penSize isn't the default (0), set PenSize to the new size.
  if (penSize)
    PenSize = penSize;

  Color = newColor;
}

bool
TLine::Draw(TDC& dc) 
{
  // Set pen for the dc to the values for this line
  TPen pen(Color, PenSize);
  dc.SelectObject(pen);

  // Iterates through the points in the line i.
  bool first = true;

  for(TPointsIterator j = begin(); j != end(); j++) {
    TPoint p = *j;

    if (!first)
      dc.LineTo(p);
    else {
      dc.MoveTo(p);
      first = false;
    }
  }
  dc.RestorePen();
  return true;
}

typedef std::vector<TLine> TLines;
typedef std::vector<TLine>::iterator TLinesIterator;

class TDrawWindow : public TWindow {
  public:
    TDrawWindow(TWindow* parent = 0);
   ~TDrawWindow()
    {
      delete DragDC;
      delete Line;
      delete Lines;
      delete FileData;
    }

  protected:
    TDC* DragDC;
    TPen* Pen;
    TLines* Lines;
    TLine* Line; // To hold a single line at a time that later gets
                 // stuck in Lines
    TOpenSaveDialog::TData* FileData;
    bool IsDirty, IsNewFile;

    void GetPenSize(); // GetPenSize always calls Line->SetPen().

    // Override member function of TWindow
    bool CanClose();

    // Message response functions
    void EvLButtonDown(uint, TPoint&);
    void EvRButtonDown(uint, TPoint&);
    void EvMouseMove(uint, TPoint&);
    void EvLButtonUp(uint, TPoint&);
    void Paint(TDC&, bool, TRect&);
    void CmFileNew();
    void CmFileOpen();
    void CmFileSave();
    void CmFileSaveAs();
    void CmPenSize();
    void CmPenColor();
    void CmAbout();
    void SaveFile();
    void OpenFile();

  DECLARE_RESPONSE_TABLE(TDrawWindow);
};

DEFINE_RESPONSE_TABLE1(TDrawWindow, TWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_LBUTTONUP,
  EV_COMMAND(CM_FILENEW, CmFileNew),
  EV_COMMAND(CM_FILEOPEN, CmFileOpen),
  EV_COMMAND(CM_FILESAVE, CmFileSave),
  EV_COMMAND(CM_FILESAVEAS, CmFileSaveAs),
  EV_COMMAND(CM_PENSIZE, CmPenSize),
  EV_COMMAND(CM_PENCOLOR, CmPenColor),
  EV_COMMAND(CM_ABOUT, CmAbout),
END_RESPONSE_TABLE;

TDrawWindow::TDrawWindow(TWindow* parent)
{
  Init(parent, 0, 0);
  DragDC    = 0;
  Lines     = new TLines;
  Line      = new TLine(TColor::Black, 1);
  IsNewFile = true;
  IsDirty   = false;
  FileData  = new TOpenSaveDialog::TData(OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
                                        "Point Files (*.PTS)|*.pts|", 0, "",
                                        "PTS");
}

bool
TDrawWindow::CanClose()
{
  if (IsDirty)
    switch(MessageBox("Do you want to save?", "Drawing has changed",
                      MB_YESNOCANCEL | MB_ICONQUESTION)) {
      case IDCANCEL:
        // Choosing Cancel means to abort the close -- return false.
        return false;

      case IDYES:
        // Choosing Yes means to save the drawing.
        CmFileSave();
    }
  return true;
}

void
TDrawWindow::EvLButtonDown(uint, TPoint& point)
{
  if (!DragDC) {
    SetCapture();
    DragDC = new TClientDC(*this);
    Pen = new TPen(Line->QueryColor(), Line->QueryPenSize());
    DragDC->SelectObject(*Pen);
    DragDC->MoveTo(point);
    Line->insert(Line->end(), point);
    IsDirty = true;
  }
}

void
TDrawWindow::EvRButtonDown(uint, TPoint&)
{
  GetPenSize();
}

void
TDrawWindow::EvMouseMove(uint, TPoint& point)
{
  if (DragDC) {
    DragDC->LineTo(point);
    Line->insert(Line->end(), point);
  }
}

void
TDrawWindow::EvLButtonUp(uint, TPoint&)
{
  if (DragDC) {
    ReleaseCapture();
    Lines->insert(Lines->end(), *Line);
    Line->erase(Line->begin(),Line->end());
    delete DragDC;
    delete Pen;
    DragDC = 0;
  }
}

void
TDrawWindow::CmPenSize()
{
  GetPenSize();
}

void
TDrawWindow::CmPenColor()
{
  TChooseColorDialog::TData colors;
  static TColor custColors[16] =
  {
    0x010101L, 0x101010L, 0x202020L, 0x303030L,
    0x404040L, 0x505050L, 0x606060L, 0x707070L,
    0x808080L, 0x909090L, 0xA0A0A0L, 0xB0B0B0L,
    0xC0C0C0L, 0xD0D0D0L, 0xE0E0E0L, 0xF0F0F0L
  };

  colors.Flags = CC_RGBINIT;
  colors.Color = TColor(Line->QueryColor());
  colors.CustColors = custColors;
  if (TChooseColorDialog(this, colors).Execute() == IDOK)
    Line->SetPen(colors.Color);
}

void
TDrawWindow::GetPenSize()
{
  char inputText[6];
  int penSize = Line->QueryPenSize();

  wsprintf(inputText, "%d", penSize);
  if ((TInputDialog(this, "Line Thickness",
                        "Input a new thickness:",
                        inputText,
                        COUNTOF(inputText))).Execute() == IDOK) {
    penSize = atoi(inputText);

    if (penSize < 1)
      penSize = 1;
  }
  Line->SetPen(penSize);
}

void
TDrawWindow::Paint(TDC& dc, bool, TRect&)
{
  // Iterates through the array of line objects.
  TLinesIterator i = Lines->begin();

  while (i != Lines->end())
    (*i++).Draw(dc);
}

void
TDrawWindow::CmFileNew()
{
  if (CanClose()) {
    Line->erase(Line->begin(),Line->end());
    Lines->erase(Lines->begin(),Lines->end());
    Invalidate();
    IsDirty = false;
    IsNewFile = true;
  }
}

void
TDrawWindow::CmFileOpen()
{
  if (CanClose())
    if ((TFileOpenDialog(this, *FileData)).Execute() == IDOK)
      OpenFile();
}

void
TDrawWindow::CmFileSave()
{
  if (IsNewFile)
    CmFileSaveAs();
  else
    SaveFile();
}

void
TDrawWindow::CmFileSaveAs()
{
  if (IsNewFile)
    strcpy(FileData->FileName, "");

  if ((TFileSaveDialog(this, *FileData)).Execute() == IDOK)
    SaveFile();
}

void
TDrawWindow::CmAbout()
{
  TDialog(this, IDD_ABOUT).Execute();
}

void
TDrawWindow::SaveFile()
{
  ofstream os(FileData->FileName);

  if (!os)
    MessageBox("Unable to open file", "File Error", MB_OK | MB_ICONEXCLAMATION);
  else {
    // Write the number of lines in the figure
    os << Lines->size();

    // Append a description using a resource string
    os << ' ' << GetApplication()->LoadString(IDS_FILEINFO).c_str() << '\n';

    // Get an iterator for the array of lines
    TLinesIterator i = Lines->begin();

    // While the iterator is valid (i.e. we haven't run out of lines)
    while (i != Lines->end())
      // Copy the current line from the iterator and increment the array.
      os << *i++;

    // Set new file and dirty display indicator to false.
    IsNewFile = IsDirty = false;
  }
}

void
TDrawWindow::OpenFile()
{
  ifstream is(FileData->FileName);

  if (!is)
    MessageBox("Unable to open file", "File Error", MB_OK | MB_ICONEXCLAMATION);
  else {
    unsigned numLines;
    char fileinfo[100];

    Line->erase(Line->begin(),Line->end());
    Lines->erase(Lines->begin(),Lines->end());

    is >> numLines;
    is.getline(fileinfo, sizeof(fileinfo));
    Parent->SetCaption(fileinfo);

    for (uint i = 0; i < numLines; i++) {
      TLine line;
      is >> line;
      Lines->insert(Lines->end(), line);
    }
  }
  IsNewFile = IsDirty = false;
  Invalidate();
}

class TDrawApp : public TApplication {
  public:
    TDrawApp() : TApplication() {}

    void InitMainWindow()
    {
      SetMainWindow(new TFrameWindow(0, "Drawing Pad", new TDrawWindow));
      GetMainWindow()->AssignMenu("COMMANDS");
    }
};

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TDrawApp().Run();
}

ostream&
operator <<(ostream& os, const TLine& line)
{
  // Write the number of points in the line
  os << line.size();

  // Get and write pen attributes.
  os << ' '<< (uint32)line.Color << ' ';
  os << line.PenSize;

  // Get an iterator for the array of points
  TPointsIterator j = ((TLine&)line).begin();

  // While the iterator is valid (i.e. we haven't run out of points)
  while(j != ((TLine&)line).end())
    // Write the point from the iterator and increment the array.
    os << *j++;
  os << '\n';

  // return the stream object
  return os;
}

istream&
operator >>(istream& is, TLine& line)
{
  unsigned numPoints;
  is >> numPoints;

  COLORREF color;
  int penSize;
	 is >> color >> penSize;
	 TColor clr(color);
	 line.SetPen(clr, penSize);

  while (numPoints--) {
    TPoint point;
    is >> point;
    line.insert(line.end(), point);
  }

  // return the stream object
  return is;
}

/* ========================================================================== */
