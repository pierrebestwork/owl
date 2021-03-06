//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.30 $
//$Author: jogybl $
//$Date: 2009-08-05 10:32:03 $
//
// Implementation of TDiBitmap class, a combination of a GDI TBitmap object and
// a Device Independent Bitmap TDib object.
//----------------------------------------------------------------------------
#if  defined(__BORLANDC__)
#pragma hdrignore SECTION
#endif
#include <owl/pch.h>
#if !defined(OWL_GDIOBJEC_H)
# include <owl/gdiobjec.h>
#endif
#if !defined(OWL_DIBITMAP_H)
# include <owl/dibitmap.h>
#endif

__OWL_USING_STD_NAMESPACE

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;

//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 1

//
// Initialize our bitmap side from the dib side. TDib::Bits gets setup to
// point to the new DIBSection/WinG managed dib buffer.
//
void TDiBitmap::InitBitmap()
{
//  PRECONDITION(Bits == 0);

  //!CQ hacky!!! need to get a better dc...
  TScreenDC dc;
  TPointer<TPalette> p;
  if (NumColors() != 0)
    p = new TPalette(*(TDib*)this);
  else
    p = new TPalette((HPALETTE)::GetStockObject(DEFAULT_PALETTE));

  dc.SelectObject(*(TPalette*)p);
  dc.RealizePalette();
  void *bits;
  TBitmap::Handle = dc.CreateDIBSection(*GetInfo(), Usage(), &bits);
  memcpy(bits, Bits, SizeImage());
  Bits = bits;
//JJH - maybe this is bad, but this->CheckValid wasn't defined!
  TDib::CheckValid();
  TBitmap::RefAdd(TBitmap::Handle, Bitmap);
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 2  

//    TWinGBitmap(TWinGIdentity&, TDibDC& dc, const TSize& size);
//    TWinGBitmap(TWinGIdentity&, TDibDC& dc, TDib& dib);
//    TWinGBitmap(TWinGIdentity&, TDibDC& dc, int width, int height);


//
// Construct from existing bitmap handle.
//
TDiBitmap::TDiBitmap(HGLOBAL handle, TAutoDelete autoDelete)
:
  TDib(handle, autoDelete)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 3

//
// Construct from bitmap stored in clipboard.
//
TDiBitmap::TDiBitmap(const TClipboard& clipboard)
:
  TDib(clipboard)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 4

//
// Construct a copy from existing TDib.
//
TDiBitmap::TDiBitmap(const TDib& src)
:
  TDib(src)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 5

//
// Construct new bitmap with the passed-in parameters.
//
TDiBitmap::TDiBitmap(int width, int height, int nColors, uint16 mode)
:
  TDib(width, height, nColors, mode)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 6

//
// Construct a bitmap stored as resource.
//
TDiBitmap::TDiBitmap(HINSTANCE module, TResId resid)
:
  TDib(module, resid)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 7

//
// Construct from DIB stored as a file.
//
TDiBitmap::TDiBitmap(LPCTSTR name)
:
  TDib(name)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 8

//
// Construct from a TFile.
//
TDiBitmap::TDiBitmap(TFile& file, bool readFileHeader)
:
  TDib(file, readFileHeader)
{
  InitBitmap();
}

//
// Construct from input stream.
//
TDiBitmap::TDiBitmap( istream& is, bool readFileHeader)
:
  TDib(is, readFileHeader)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 9

//
// Construct from a bitmap and palette information.
//
TDiBitmap::TDiBitmap(const TBitmap& bitmap, const TPalette* pal)
:
  TDib(bitmap, pal)
{
  InitBitmap();
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 10

//
// Restore Bits data member to 0.
//
TDiBitmap::~TDiBitmap()
{
  Bits = 0;  // Make sure that TDib doesn't mess with our unusual bits
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 11

// Implementation of (WinG) Identity Palette object, TIdentityPalette.
// Use of an identity palette allows objects to be blitted without having
// the color mapper come into play.  The overall effect is faster graphics.

//
// Return an identity palette to be used by bitmaps.  This ensures
// speed and color accuracy.
//
TIdentityPalette::TIdentityPalette(const TPalette& palette)
:
  TPalette(0, NoAutoDelete)
{
  TScreenDC sdc;

  int nColors = palette.GetNumEntries();
  int nSysColors = sdc.GetDeviceCaps(SIZEPALETTE);

  LOGPALETTE* logPal = (LOGPALETTE*)new uint8[
     sizeof(LOGPALETTE)+(nColors-1)*sizeof(PALETTEENTRY) ];
  logPal->palVersion  = 0x300;
  logPal->palNumEntries = (uint16)nColors;

  if (sdc.GetSystemPaletteUse() == SYSPAL_NOSTATIC) {
    int colorIndex;

    // Set the entries to PC_NOCOLLAPSE
    //!CQ First colors will get stomped. Need to offset & then fixup dib...
    //
    palette.GetPaletteEntries(0, uint16(nColors), logPal->palPalEntry);
    for (colorIndex=0; colorIndex < nColors; colorIndex++)
      logPal->palPalEntry[colorIndex].peFlags = PC_NOCOLLAPSE;

    // Make the first entry black
    //
    logPal->palPalEntry[0].peRed   = 0;
    logPal->palPalEntry[0].peGreen = 0;
    logPal->palPalEntry[0].peBlue  = 0;
    logPal->palPalEntry[0].peFlags = 0;

    // Make the last entry white
    //
    logPal->palPalEntry[255].peRed   = 255;
    logPal->palPalEntry[255].peGreen = 255;
    logPal->palPalEntry[255].peBlue  = 255;
    logPal->palPalEntry[255].peFlags = 0;
  }
  else {
    // SystemPaletteUse == SYSPAL_STATIC
    //
    int staticColors = sdc.GetDeviceCaps(NUMCOLORS) / 2;
    int usableColors = nColors - staticColors;

    //!TH The passed color table should be indexed from the start not from
    //    the offset of the systems static palette use.
    //!CQ is it really an identity palette then? More work needs to be done.
    //
    sdc.GetSystemPaletteEntries(0, staticColors, logPal->palPalEntry);
    palette.GetPaletteEntries(0, uint16(usableColors), logPal->palPalEntry+staticColors);
    sdc.GetSystemPaletteEntries(nSysColors - staticColors, staticColors,
                                logPal->palPalEntry + nSysColors - staticColors);

    int colorIndex = 0;
    for (; colorIndex < staticColors; colorIndex++)
      logPal->palPalEntry[colorIndex].peFlags = 0;
    for (; colorIndex < nColors; colorIndex++)
      logPal->palPalEntry[colorIndex].peFlags = PC_NOCOLLAPSE;
    for (; colorIndex < nSysColors; colorIndex++)
      logPal->palPalEntry[colorIndex].peFlags = 0;
  }
  Handle = ::CreatePalette(logPal);
  delete[] logPal;

  ShouldDelete = true;
  CheckValid();
  RefAdd(Handle, Palette);
}

#endif
//-------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 12

//
// Default constructor. In 32-bits, retrieves the system's halftone palette.
// In 16-bits, asks WinG for its halftone palette.
//
THalftonePalette::THalftonePalette()
:
  TPalette(::CreateHalftonePalette(TScreenDC()), NoAutoDelete)
{
  ShouldDelete = true;
  CheckValid();
  RefAdd(Handle, Palette);
}

//
// Force the system palette to a known state.  This state will be
// an empty palette.
//
/*
void
TWinGIdentity::ClearSystemPalette()
{
  TWinGLogPalette palette = { PaletteVersion, SupportedColors };

  // Reset everything in the system palette to black
  //
  for (int counter = 0; counter < SupportedColors; counter++) {
    palette.Entries[counter].peRed   = 0;
    palette.Entries[counter].peGreen = 0;
    palette.Entries[counter].peBlue  = 0;
    palette.Entries[counter].peFlags = PC_NOCOLLAPSE;
  }

  // Create, select, realize, deselect, and delete the palette.
  //
  TScreenDC sdc;
  TPalette sp((LOGPALETTE*)&palette);

  sdc.SelectObject(sp, false);
  sdc.RealizePalette();
}
*/

#endif

__OWL_END_NAMESPACE
/* ========================================================================== */

