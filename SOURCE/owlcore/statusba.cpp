//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.27 $
//$Author: jogybl $
//$Date: 2007-07-20 10:47:44 $
//
// Implementation of TStatusBar
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_DEFS_H)
# include <owl/defs.h>
#endif
#if !defined(OWL_STATUSBA_H)
# include <owl/statusba.h>
#endif
#if !defined(OWL_MODEGAD_H)
# include <owl/modegad.h>
#endif
#if !defined(OWL_UIMETRIC_H)
# include <owl/uimetric.h>
#endif
#if !defined(OWL_STATUSBA_RH)
# include <owl/statusba.rh>
#endif
#if !defined(OWL_LAYOUTWI_H)
# include <owl/layoutwi.h>
#endif

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;

//
// Local class to manage the mode indicator strings
//
class TModeStrings {
  public:
    // Constructor
    //
    TModeStrings() : NumModes(0), ModeBuff(0), ModeStrings(0) {}
   ~TModeStrings()
    {
      delete[] ModeBuff;
      delete[] ModeStrings;
    }

    void Init(TModule* module, uint16 resId, LPCTSTR defaultModes);

    // Accessors
    int          Count() const            { return NumModes; }
    LPCTSTR  operator [](int i) const { return ModeStrings[i]; }

  private:
    int    NumModes;
    TCHAR*  ModeBuff;
    TCHAR** ModeStrings;
};

//
// Gets the mode string array from a single resource string, like:
//   "EXT|CAPS|NUM|SCRL|OVR|REC"
// using the provided default string if resource not found.
//
void
TModeStrings::Init(TModule* module, uint16 resId, LPCTSTR defaultModes)
{
  if (!ModeStrings) {
    owl_string modeResString(module->LoadString(resId));
    ModeBuff = strnewdup(modeResString.length() > 0 ?
                         modeResString.c_str() :
                         defaultModes);
    NumModes = 1;
    for (TCharIterator<TCHAR> i(ModeBuff); ; i++) {
      if (!*i.Current())
        break;
      if (*i.Current() == _T('|')) {
        *i.Current() = 0;
        NumModes++;
      }
    }

    typedef _TCHAR* pchar;
    ModeStrings = new pchar[NumModes];
    TCHAR* p = ModeBuff;
    for (int j = 0; j < NumModes; j++) {
      ModeStrings[j] = p;
      p += ::_tcslen(p) + 1;
    }
  }
}

//
// Static mode string object, & default mode indicator strings
//
static _TCHAR  DefOnModes[] =  _T("EXT|CAPS|NUM|SCRL|OVR|REC");
static TModeStrings&
GetModeOnStrings()
{
  static TModeStrings ModeOnStrings;
  return ModeOnStrings;
}

static int VkKeys[] = {
  0,
  VK_CAPITAL,
  VK_NUMLOCK,
  VK_SCROLL,
  VK_INSERT,
  0
};

static int GadgetIds[] = {
  IDG_STATUS_EXT,
  IDG_STATUS_CAPS,
  IDG_STATUS_NUM,
  IDG_STATUS_SCRL,
  IDG_STATUS_OVR,
  IDG_STATUS_REC
};

//----------------------------------------------------------------------------

IMPLEMENT_CASTABLE(TStatusBar);

DEFINE_RESPONSE_TABLE1(TStatusBar, TMessageBar)
  EV_WM_NCHITTEST,
  EV_OWLFRAMESIZE,
END_RESPONSE_TABLE;

//
// Construct a status bar with a given style, font, etc.
// borderStyle is used as the default for inserted gadgets.
//
TStatusBar::TStatusBar(TWindow*              parent,
                       TGadget::TBorderStyle borderStyle,
                       uint                  modeIndicators,
                       TFont*                font,
                       TModule*              module)
:
  TMessageBar(parent, font, module)
{
  BorderStyle = borderStyle;
  ModeIndicators = modeIndicators;
  ModeIndicatorState = 0;
  NumModeIndicators = 0;
  WideHintText = true;
  GetModeOnStrings().Init(GetModule(), IDS_MODES, DefOnModes);

  // Statusbars should always stay flush to the decorated frame's border
  //
  SetFlag(wfInsertAtEdge);

  if (BorderStyle == TGadget::Plain || BorderStyle == TGadget::Embossed)
    HighlightLine = false;
  else
    Spacing.Value = 2;  // Hilight line + 1/4-em margin on raised & recessed

  switch (BorderStyle) {
    case TGadget::None:
    case TGadget::Embossed:
    case TGadget::Grooved:
    case TGadget::ButtonUp:
    case TGadget::ButtonDn:
    case TGadget::WndRaised:
    case TGadget::WndRecessed:
    case TGadget::WellSet:
      break;

    case TGadget::Raised:
    case TGadget::Recessed:
      // We want one border height along the top and bottom and 1/2 an em quad
      // along the left and right so we will set pixels and compute the lengths
      // ourselves
      //
      Margins.Units = TMargins::Pixels;
      Margins.Left  = LayoutUnitsToPixels(4);
      Margins.Right = LayoutUnitsToPixels(4);

    
      // The UI for a sizegrip isn't available in 16-bit Windows
      //
      if (ModeIndicators & SizeGrip)
        Margins.Right = LayoutUnitsToPixels(1);
      Margins.Top = Margins.Bottom = TUIMetric::CyBorder;
      break;

    case TGadget::Plain:
      Margins.Units = TMargins::BorderUnits;
      Margins.Left = Margins.Top = Margins.Right = Margins.Bottom = -1;
      break;
    default: //JJH added empty default construct
      break;
  }

  Gadgets->SetBorderStyle(BorderStyle); // Set border style for first gadget

  // Create text gadgets for any mode indicators the user requested,
  // using provided border style.
  //
  TScreenDC   dc;
  dc.SelectObject(*Font);

  for (int i = 0; i < GetModeOnStrings().Count(); i++)
    if (ModeIndicators & (1 << i)) {
      TTextGadget* gadget;
      if (NS_OWL::VkKeys[i] == 0) {
        gadget = new TTextGadget(GadgetIds[i], BorderStyle, TTextGadget::Left,
                                 lstrlen(GetModeOnStrings()[i]), 
                                 GetModeOnStrings()[i], 0);
        gadget->SetEnabled(false);
      }
      else
        gadget = new TModeGadget(VkKeys[i], GetModeOnStrings()[i], 
                                 GadgetIds[i],
                                 BorderStyle, TModeGadget::Center, 0);

      TMargins margins = gadget->GetMargins();

      // Use small left and right margins
      //
      margins.Left = margins.Right = 2;
      gadget->SetMargins(margins);

      TMessageBar::Insert(*gadget);
      NumModeIndicators++;
    }

  if (ModeIndicators & SizeGrip) {
    InsertSizeGrip();
  }

}

//
//  Create a size grip and place it on the status bar.
//  (Size grips are not available on 16-bit Windows.)
//
void
TStatusBar::InsertSizeGrip()
{
  PRECONDITION(
    TYPESAFE_DOWNCAST(GadgetWithId(IDG_SIZEGRIP), TSizeGripGadget) == 0);

  TSizeGripGadget* gadget = new TSizeGripGadget;
  TMessageBar::Insert(*gadget, TGadgetList::After);
  TRect bounds = gadget->GetBounds();
  bounds.Inflate(1, 1);
  gadget->SetBounds(bounds);
  NumModeIndicators++;
}

//
// Determines if a given gadget is being used as a mode indicator
//
bool
TStatusBar::IsModeIndicator(TGadget* gadget)
{
  int      nonModeIndicators = NumGadgets - NumModeIndicators;
  TGadget* g = Gadgets;

  for (int i = 0; i < nonModeIndicators; i++) {
    if (gadget == g)
      return false;

    g = g->NextGadget();
  }

  return true;
}

//
// Override TGadgetWindow's callback during tiling to tweak the spacing
// around the mode indicators
//
void
TStatusBar::PositionGadget(TGadget*, TGadget* next, TPoint& origin)
{
  int  cxBorder = TUIMetric::CxBorder;

  if (BorderStyle == TGadget::Plain)
    origin.x -= cxBorder;  // overlap the borders

  TSizeGripGadget* gadget = TYPESAFE_DOWNCAST(next, TSizeGripGadget);
  if (gadget)
    return;

  // Leave extra spacing between the mode indicators
  //
  if (IsModeIndicator(next))
    switch (Spacing.Units) {
      case TMargins::Pixels:
        origin.x += Spacing.Value;
        break;

      case TMargins::LayoutUnits:
        origin.x += LayoutUnitsToPixels(Spacing.Value);
        break;

      case TMargins::BorderUnits:
        origin.x += Spacing.Value * cxBorder;
        break;
    }
}

//
// Insert a gadget. Default placement is just after the gadget to the left of
// the status mode indicators
//
void
TStatusBar::Insert(TGadget& gadget, TPlacement placement, TGadget* sibling)
{
  gadget.SetBorderStyle(BorderStyle);
  if (!sibling)
    sibling = operator[](NumGadgets - NumModeIndicators - 1);
  TMessageBar::Insert(gadget, placement, sibling);
}

//
// Overriden to allow hint text to be displayed in the text gadget
//
void    
TStatusBar::SetHintText(LPCTSTR text)
{
  if (WideHintText)
    // This displays hint that covers the whole gadget window
    //
    TMessageBar::SetHintText(text);
  else
    // This displays the text in the text gadget.
    //
    SetText(text);
}

//
// Get the text gadget & the mode string associated with a mode indicator
//
bool
TStatusBar::GetGadgetAndStrings(TModeIndicator mode, TTextGadget*& gadget,
                                LPCTSTR& strOn)
{
  if ((ModeIndicators & mode) == 0) {
    return false;  // tracing
  }
  else {
    uint  slot = NumGadgets - 1;
    int   index;

    for (index = GetModeOnStrings().Count() - 1; (1 << index) > mode; index--)
      if (ModeIndicators & (1 << index))
        slot--;

    strOn  = GetModeOnStrings()[index];

    for (gadget = (TTextGadget*)Gadgets; slot > 0; slot--)
      gadget = (TTextGadget*)gadget->NextGadget();

    return true;
  }
}

//
// Toggle the state of a mode indicator gadget
//
void
TStatusBar::ToggleModeIndicator(TModeIndicator mode)
{
  SetModeIndicator(mode, !GetModeIndicator(mode));
}

//
// Enable and disable mode indicators
//
void
TStatusBar::SetModeIndicator(TModeIndicator mode, bool on)
{
  TTextGadget*  gadget;
  const _TCHAR* strOn;

  if (GetGadgetAndStrings(mode, gadget, strOn)) {
    gadget->SetEnabled(on);
  }

  if (on)
    ModeIndicatorState |= mode;
  else
    ModeIndicatorState &= ~mode;
}


//
// If the status bar has a size grip gadget and the mouse is over the gadget,
// simulate resizing of the frame.
//
uint
TStatusBar::EvNCHitTest(TPoint& point)
{
  uint retVal = TMessageBar::EvNCHitTest(point);

  TSizeGripGadget* grip = TYPESAFE_DOWNCAST(GadgetWithId(IDG_SIZEGRIP), TSizeGripGadget);
  if (grip && grip->GetEnabled()) {
    ScreenToClient(point);
    TRect rect = grip->GetBounds();
    rect.right += 1;
    rect.bottom += 1;
    if (rect.Contains(point))   //!BGM use TGadget::PtIn here (so IsVisible() will be checked)
      retVal = HTBOTTOMRIGHT;
  }

  return retVal;
}

//
// If the frame window that owns the status bar maximizes,
// then remove the size grip.  (Otherwise, the size grip
// remains uselessly and confusingly active in a maximized window.)
//
void
TStatusBar::EvOwlFrameSize(uint sizeType, TSize&)
{
  TSizeGripGadget* grip = TYPESAFE_DOWNCAST(GadgetWithId(IDG_SIZEGRIP),
                                            TSizeGripGadget);

  // If the window was maximized and has a grip, then remove the grip.
  //
  bool callresize = false;
  if (sizeType==SIZE_MAXIMIZED) {
    if (grip) {
      Remove(*grip);
      delete grip;
      NumModeIndicators--;
      LayoutSession();
      callresize = true;
    }
  }
  // If the window was restored and if it is supposed to have a size grip
  // but the grip isn't there, then provide a new grip.
  //
  else if (sizeType == SIZE_RESTORED) {
    if (!grip && (GetModeIndicators() & SizeGrip)) {
      InsertSizeGrip();
      LayoutSession();
      callresize = true;
    }
  }
  if(callresize){
    TLayoutWindow* window = TYPESAFE_DOWNCAST(GetParentO(), TLayoutWindow);
    if(window)
      window->Layout();
  }
}


__OWL_END_NAMESPACE
/* ========================================================================== */

