//---------------------------------------------------------------------------- 
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//   Tutorial application -- step08.cpp 
//----------------------------------------------------------------------------
#include "owlpch.h"
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/dc.h>
#include <owl/inputdia.h>
#include <owl/opensave.h>
#include <owl/template.h>
#include <stdlib.h>
#include <string.h>
#include "step08.rc"

// example of using STL
#include <vector>  
typedef std::vector<TPoint> TPoints;
typedef std::vector<TPoint>::iterator TPointsIterator;

class TLine;
ostream& operator <<(ostream& os, const TLine& line);
istream& operator >>(istream& is, TLine& line);

class TLine : public TPoints {
  public:
    TLine(int penSize = 1)
    {
      PenSize = penSize;
    }

    int QueryPen() const
    {
      return PenSize;
    }

    int QueryPen(int penSize);

    // The == operator must be defined for the container class, even if unused
    bool operator ==(const TLine& other) const
    {
      return &other == this;
    }

    friend ostream& operator <<(ostream& os, const TLine& line);
    friend istream& operator >>(istream& is, TLine& line);

  protected:
    int PenSize;
};

int
TLine::QueryPen(int penSize)
{
  if (penSize < 0)
    PenSize = 1;
  else if (penSize > 0)
    PenSize = penSize;
  return PenSize;
}

typedef std::vector<TLine> TLines;
typedef std::vector<TLine>::iterator TLinesIterator;

class TDrawWindow : public TWindow {
  public:
    TDrawWindow(TWindow* parent = 0);
   ~TDrawWindow()
    {
      delete DragDC;
      delete Pen;
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

    void SetPenSize(int newSize);
    int GetPenSize();

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
  EV_COMMAND(CM_ABOUT, CmAbout),
  EV_COMMAND(CM_PENSIZE, CmPenSize),
END_RESPONSE_TABLE;

TDrawWindow::TDrawWindow(TWindow* parent)
{
  Init(parent, 0, 0);
  DragDC    = 0;
  Lines     = new TLines;
  Line      = new TLine;
  Pen       = new TPen(TColor::Black, Line->QueryPen());
  IsNewFile = true;
  IsDirty   = false;
  FileData  = new TOpenSaveDialog::TData(OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
                                        "Point Files (*.PT8)|*.pt8|", 0, "",
                                        "PT8");
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
    DragDC->SelectObject(*Pen);
    DragDC->MoveTo(point);
    Line->insert(Line->end(), point);
    IsDirty = true;
  }
}

void
TDrawWindow::EvRButtonDown(uint, TPoint&)
{
  SetPenSize(GetPenSize());
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
    DragDC = 0;
  }
}

void
TDrawWindow::CmPenSize()
{
  SetPenSize(GetPenSize());
}

int
TDrawWindow::GetPenSize()
{
  char inputText[6];
  int newPenSize = 1;

  wsprintf(inputText, "%d", Line->QueryPen());
  if ((TInputDialog(this, "Line Thickness",
                        "Input a new thickness:",
                        inputText,
                        COUNTOF(inputText))).Execute() == IDOK) {
    newPenSize = atoi(inputText);

    if (newPenSize < 0)
      newPenSize = 1;
  }
  return newPenSize;
}

void
TDrawWindow::SetPenSize(int newSize)
{
  delete Pen;
  Pen = new TPen(TColor::Black, Line->QueryPen(newSize));
}

void
TDrawWindow::Paint(TDC& dc, bool, TRect&)
{
  // Iterates through the array of line objects.
  TLinesIterator i = Lines->begin();
  while (i != Lines->end()){
    // Set pen for the dc to current line's pen.
    TPen pen(TColor::Black, (*i).QueryPen());
    dc.SelectObject(pen);

    // Iterates through the points in the line i.
    bool first = true;
		for(TPointsIterator j = i->begin(); j != i->end(); j++) {
      TPoint p = *j;

      if (!first)
        dc.LineTo(p);
      else {
        dc.MoveTo(p);
        first = false;
      }
    }
    dc.RestorePen();
  }
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
    os << Lines->size() << '\n';

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

    Line->erase(Line->begin(),Line->end());
    Lines->erase(Lines->begin(),Lines->end());

    is >> numLines;

    while (numLines--) {
      TLine line;
      is >> line;
      Lines->insert(Lines->end(), line);
    }
  }
  IsNewFile = IsDirty = false;
  Invalidate();
}

ostream&
operator <<(ostream& os, const TLine& line)
{
  // Write the number of points in the line
  os << line.size();

  // Write the pen size
  os << ' ' << line.PenSize;

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

  is >> line.PenSize;

  while (numPoints--) {
    TPoint point;
    is >> point;
    line.insert(line.end(), point);
  }
  // return the stream object
  return is;
}

////////////////////////////
//
// class TDrawApp
// ~~~~~ ~~~~~~~~
//
class TDrawApp : public TApplication {
  public:
    TDrawApp() : TApplication() {}

    void InitMainWindow()
    {
      SetMainWindow(new TFrameWindow(0, "Drawing Pad", new TDrawWindow));
      GetMainWindow()->AssignMenu("COMMANDS");
    }
};

/////////////////////
int
OwlMain(int /*argc*/, _TCHAR* /*argv*/ [])
{
  return TDrawApp().Run();
}

/* ========================================================================== */



