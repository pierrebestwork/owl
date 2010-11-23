//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include "owlpch.h"
#include <owl/dc.h>
#include <owl/inputdia.h>
#include <owl/chooseco.h>
#include <owl/gdiobjec.h>
#include <owl/docmanag.h>
#include <owl/filedoc.h>
#include "src/step12dv.rc"

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

    // Functions to modify and query pen attributes.
    int QueryPenSize() const
    {
      return PenSize;
    }
    const TColor& QueryColor() const
    {
      return Color;
    }
    void SetPen(TColor& newColor, int penSize = 0);
    void SetPen(int penSize);

    // TLine draws itself.  Returns true if everything went OK.
    virtual bool Draw(TDC&) const;

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

typedef std::vector<TLine> TLines;
typedef std::vector<TLine>::iterator TLinesIterator;

class _USERCLASS TDrawDocument : public TFileDocument {
  public:
    TDrawDocument(TDocument* parent = 0) : TFileDocument(parent), Lines(0) {}
   ~TDrawDocument()
    {
      delete Lines;
    }

    // implement virtual methods of TDocument
    bool   Open(int mode, const char far* path=0);
    bool   Close();
    bool   IsOpen()
    {
      return Lines != 0;
    }
    bool   Commit(bool force = false);
    bool   Revert(bool clear = false);

    // data access functions
    const TLine* GetLine(unsigned int index);
    int   AddLine(TLine& line);

  protected:
    TLines* Lines;
};

class _USERCLASS TDrawView : public TWindowView {
  public:
    TDrawView(TDrawDocument& doc, TWindow* parent = 0);
   ~TDrawView()
    {
      delete DragDC;
      delete Line;
    }
    static const char far* StaticName()
    {
      return "Draw View";
    }

  protected:
    TDrawDocument* DrawDoc;  // same as Doc member, but cast to derived class
    TDC* DragDC;
    TPen*  Pen;
    TLine* Line; // To hold a single line sent or received from document
    void GetPenSize(); // GetPenSize always calls Line->SetPen().

    // Message response functions
    void EvLButtonDown(uint, TPoint&);
    void EvRButtonDown(uint, TPoint&);
    void EvMouseMove(uint, TPoint&);
    void EvLButtonUp(uint, TPoint&);
    void Paint(TDC&, bool, TRect&);
    void CmPenSize();
    void CmPenColor();

    // Document notifications
    bool VnCommit(bool force);
    bool VnRevert(bool clear);

  DECLARE_RESPONSE_TABLE(TDrawView);
};

DEFINE_DOC_TEMPLATE_CLASS(TDrawDocument, TDrawView,   DrawTemplate);
DrawTemplate drawTpl("Point Files (*.PTS)","*.pts",0,"PTS",dtAutoDelete|dtUpdateDir);

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
TLine::Draw(TDC& dc) const
{
  // Set pen for the dc to the values for this line
  TPen pen(Color, PenSize);
  dc.SelectObject(pen);

  // Iterates through the points in the line i.
  bool first = true;

  for(TPointsIterator j = ((TLine*)this)->begin(); j != ((TLine*)this)->end(); j++) {
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

ostream&
operator <<(ostream& os, const TLine& line)
{
  // Write the number of points in the line
  os << line.size();

  // Get and write pen attributes.
  os << ' ' << (uint32)line.Color << ' ' << line.PenSize;

  // While the iterator is valid (i.e. we haven't run out of points)
  for(TPointsIterator j = ((TLine&)line).begin();  j != ((TLine&)line).end(); j++)
    os << *j;

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

bool
TDrawDocument::Commit(bool force)
{
  if (!IsDirty() && !force)
    return true;

  TOutStream* os = OutStream(ofWrite);
  if (!os)
    return false;

  // Write the number of lines in the figure
  *os << Lines->size();

  // Append a description using a resource string
  *os << ' ' << GetDocManager().GetApplication()->LoadString(IDS_FILEINFO).c_str() << '\n';

  // While the iterator is valid (i.e. we haven't run out of lines)
	for(TLinesIterator i = Lines->begin(); i != Lines->end(); i++)
    *os << *i;

  delete os;

  SetDirty(false);
  return true;
}

bool
TDrawDocument::Revert(bool clear)
{
  if (!TFileDocument::Revert(clear))
    return false;
  if (!clear)
    Open(0);
  return true;
}

bool
TDrawDocument::Open(int /*mode*/, const char far* path)
{
  Lines = new TLines;
  if (path)
    SetDocPath(path);
  if (GetDocPath()) {
    TInStream* is = InStream(ofRead);
    if (!is)
      return false;

    unsigned numLines;
    char fileinfo[100];
    *is >> numLines;
    is->getline(fileinfo, sizeof(fileinfo));
    while (numLines--) {
      TLine line;
      *is >> line;
      Lines->insert(Lines->end(), line);
    }
    delete is;
  }
  SetDirty(false);
  return true;
}

bool
TDrawDocument::Close()
{
  delete Lines;
  Lines = 0;
  return true;
}

const TLine*
TDrawDocument::GetLine(unsigned int index)
{
  if (!IsOpen() && !Open(ofRead | ofWrite))
    return 0;
  return index < Lines->size() ? &(*Lines)[index] : 0;
}

int
TDrawDocument::AddLine(TLine& line)
{
  Lines->insert(Lines->end(), line);
  SetDirty(true);
  return Lines->size()-1;
}

DEFINE_RESPONSE_TABLE1(TDrawView, TWindowView)
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_LBUTTONUP,
  EV_COMMAND(CM_PENSIZE, CmPenSize),
  EV_COMMAND(CM_PENCOLOR, CmPenColor),
  EV_VN_COMMIT,
  EV_VN_REVERT,
END_RESPONSE_TABLE;

TDrawView::TDrawView(TDrawDocument& doc,TWindow* parent) :
  TWindowView(doc, parent), DrawDoc(&doc)
{
  DragDC  = 0;
  Line    = new TLine(TColor::Black, 1);
  SetViewMenu(new TMenuDescr(IDM_DRAWVIEW,0,1,0,0,0,0));
  SetViewBar(new TBarDescr(IDB_DRAWVIEW));
}

void
TDrawView::EvLButtonDown(uint, TPoint& point)
{
  if (!DragDC) {
    SetCapture();
    DragDC = new TClientDC(*this);
    Pen = new TPen(Line->QueryColor(), Line->QueryPenSize());
    DragDC->SelectObject(*Pen);
    DragDC->MoveTo(point);
    Line->insert(Line->end(), point);
  }
}

void
TDrawView::EvRButtonDown(uint, TPoint&)
{
  GetPenSize();
}

void
TDrawView::EvMouseMove(uint, TPoint& point)
{
  if (DragDC) {
    DragDC->LineTo(point);
    Line->insert(Line->end(), point);
  }
}

void
TDrawView::EvLButtonUp(uint, TPoint&)
{
  if (DragDC) {
    ReleaseCapture();
    if (Line->size() > 1)
      DrawDoc->AddLine(*Line);
    Line->erase(Line->begin(),Line->end());
    delete DragDC;
    delete Pen;
    DragDC = 0;
  }
}

void
TDrawView::CmPenSize()
{
  GetPenSize();
}

void
TDrawView::CmPenColor()
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
TDrawView::GetPenSize()
{
  char inputText[6];
  int penSize = Line->QueryPenSize();

  wsprintf(inputText, "%d", penSize);
  if (TInputDialog(this, "Line Thickness",
                        "Input a new thickness:",
                        inputText,
                        sizeof(inputText),gModule).Execute() == IDOK) {
    penSize = atoi(inputText);

    if (penSize < 1)
      penSize = 1;
  }
  Line->SetPen(penSize);
}

void
TDrawView::Paint(TDC& dc, bool, TRect&)
{
  // Iterates through the array of line objects.
  int i = 0;
  const TLine* line;
  while ((line = DrawDoc->GetLine(i++)) != 0)
    line->Draw(dc);
}

bool
TDrawView::VnCommit(bool /*force*/)
{
  // nothing to do here, no data held in view
  return true;
}

bool
TDrawView::VnRevert(bool /*clear*/)
{
  Invalidate();  // force full repaint
  return true;
}
/* ========================================================================== */
