//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1993, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.26 $
//$Author: jogybl $
//$Date: 2007-05-11 15:29:17 $
//
// Implementation of a bitmap Cel array class.
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_CELARRAY_H)
# include <owl/celarray.h>
#endif

__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;
DIAG_DECLARE_GROUP(OwlGadget);    // Common Controls diagnostic group

bool OwlCopyBmp(TBitmap& dstBM, const TBitmap& srcBM, const TPoint& pt,
                const TSize& size,const TPoint offset=TPoint(0,0));

//
// Construct a TCelArray from a bitmap, slicing a portion of the bitmap up
// into a horizontal array of specified sized cels.
// The CelArray can own (& delete) the bitmap or not, determined by autoDelete
//
TCelArray::TCelArray(TBitmap* bmp, int numCels, TSize celSize,
                     TPoint offset, int numRows, TAutoDelete autoDelete)
{
  PRECONDITION(bmp);

  NGrowBy = 1;
  NCels = NCelsUsed = __max(1, numCels);
  NRows  = __max(1, numRows);
  CSize = celSize.cx && celSize.cy ?
            celSize :
            TSize(bmp->Width() / NCels, bmp->Height() / NRows);
  Offs = offset;
  NCurRow = 0;
  ShouldDelete = ToBool(autoDelete == AutoDelete);

  Bitmap = bmp;

  TRACEX(OwlGadget, OWL_CDLEVEL, "TCelArray constructed @" << (void*)this <<
    " from slicing the bitmap @" << (void*)bmp);
}

//
// Construct a TCelArray from a dib, slicing the dib up into a horizontal array
// of even sized cels
//
TCelArray::TCelArray(const TDib& dib, int numCels, int numRows)
{
  NGrowBy = 1;
  NCels = NCelsUsed = __max(1, numCels);
  NRows  = __max(1, numRows);
  CSize = TSize(dib.Width() / NCels, dib.Height() / NRows);
  NCurRow = 0;
  Offs = 0;
  ShouldDelete = true;

  TPalette palette((HPALETTE)::GetStockObject(DEFAULT_PALETTE));
  Bitmap = new TBitmap(dib, &palette);

  TRACEX(OwlGadget, OWL_CDLEVEL, "TCelArray constructed @" << (void*)this <<
    " from slicing the dib @" << (void*)&dib);
}

//
// Construct a TCelArray as a copy of an existing one. Copy Bitmap iff
// original owned its Bitmap, else just keep a ref to it also.
//
TCelArray::TCelArray(const TCelArray& src)
{
  Offs = src.Offs;
  CSize =  src.CSize;
  NCels = src.NCels;
  NCelsUsed = src.NCelsUsed;
  NGrowBy = src.NGrowBy;
  BkColor = src.BkColor;
  NCurRow = src.NCurRow;
  NRows = src.NRows;

  ShouldDelete = src.ShouldDelete;

  Bitmap = ShouldDelete ? new TBitmap(*src.Bitmap) : src.Bitmap;

  TRACEX(OwlGadget, OWL_CDLEVEL, "Copied constructed TCelArray @" << (void*)this <<
    " from @" << (void*)&src);
}

//
// Construct an empty CelArray of a given size
//
TCelArray::TCelArray(const TSize& size, uint /*flags*/, int initCount, int growBy, int numRows)
{
  // Init variables
  //
  Offs     = 0;
  CSize    = size;
  NCels    = initCount > 1 ? initCount : 1;
  NGrowBy  = growBy;
  NCurRow   = 0;
  NRows     = __max(1, numRows);
  ShouldDelete = true;
  Bitmap   = 0;

  // Allocate bitmap(s)
  //
  Resize(NCels);

  // Update count of Cels
  //
  NCelsUsed = 0;

  TRACEX(OwlGadget, OWL_CDLEVEL, "Empty TCelArray constructed @" << (void*)this <<
    " size (" << size.cx << "x" << size.cy << ")");
}

//
// Destruct the CelArray and clean up its resources
//
TCelArray::~TCelArray()
{
  if (ShouldDelete)
    delete Bitmap;

  TRACEX(OwlGadget, OWL_CDLEVEL, "TCelArray destructed @" << (void*)this);
}

//
// Assign a CelArray over this CelArray, replacing all contents
//
TCelArray&
TCelArray::operator =(const TCelArray& src)
{
  if (ShouldDelete)
    delete Bitmap;

  ShouldDelete = src.ShouldDelete;
  Bitmap = ShouldDelete ? new TBitmap(*src.Bitmap) : src.Bitmap;

  NCels = src.NCels;
  CSize =  src.CSize;
  NCurRow = src.NCurRow;
  NRows = src.NRows;
  Offs = src.Offs;

  TRACEX(OwlGadget, 1, "TCelArray @" << (void*)this <<
    " contents copied over with TCelArray @" << (void*)&src);
  return *this;
}

//
// Return the offset of a given cel in the CelArray's bitmap
//
TPoint
TCelArray::CelOffset(int cel, int row) const
{
  row = row < 0 ? NCurRow : row;
  return TPoint(Offs.x+cel*CSize.cx, Offs.y+CSize.cy*row);
}

//
// Return the bounding rect of a given cel in the CelArray's bitmap
//
TRect
TCelArray::CelRect(int cel, int row) const
{
  row = row < 0 ? NCurRow : row;
  return TRect(TPoint(Offs.x+cel*CSize.cx, Offs.y+CSize.cy*row), CSize);
}

//
// Add new cel(s) to the CelArray - return index of new addition.
// No mask bitmap is added.
//
int
TCelArray::Add(const TBitmap& image)
{
  int width = image.Width();
  int count = width / CelSize().cx;

  if (!MaybeResize(count))
    return -1;

  OwlCopyBmp(*Bitmap, image, CelOffset(NCelsUsed,0), image.Size());

  int index = NCelsUsed;
  NCelsUsed += count;

  TRACEX(OwlGadget, 1, "TCelArray @" << (void*)this << " added bitmap @" <<
    (void*)&image);

  return index;
}

//
// Add a cel from another CelArray to this CelArray
//
int
TCelArray::Add(const TCelArray& src, int index)
{
//  if (src.CSize != CSize)
//    return -1;

  if (NCelsUsed >= NCels)
    if (!Resize(NCels + NGrowBy))
      return -1;

  OwlCopyBmp(*Bitmap, (const TBitmap&)src, CelOffset(NCelsUsed,0), 
             TSize(CSize.cx,CSize.cy*NRows),CelRect(index,0).TopLeft());

  TRACEX(OwlGadget, 1, "TCelArray @" << (void*)this << " added TCelArray @" <<
    (void*)&src << " index 0x" << index);

  return NCelsUsed++;
}

//
// Remove a cel from this CelArray.
//
bool
TCelArray::Remove(int index)
{
  if (index >= NCelsUsed)
    return false;

  if (index < 0) {
    NCelsUsed = 0;  // Remove all
  }
  else if (index < NCelsUsed) {
    if (index < NCelsUsed-1) {
      TPoint offs(CelOffset(index,0));
      TMemoryDC bitmapDC(*Bitmap);
      bitmapDC.BitBlt(offs.x, offs.y, (NCelsUsed-index-1)*CSize.cx, 
                      CSize.cy*NRows,bitmapDC, offs.x+CSize.cx, offs.y);
    }
    NCelsUsed--;
  }

  TRACEX(OwlGadget, 1, "TCelArray @" << (void*)this << " removed index 0x" << index);
  return true;
}

//
// Replace a cel in this CelArray.
//!CQ can it handle >1 cel?
//
bool
TCelArray::Replace(int index, const TBitmap& image)
{
  if (index >= 0 && index < NCelsUsed) {
    OwlCopyBmp(*Bitmap, image, CelOffset(index,0), TSize(CSize.cx,CSize.cy*NRows));
    TRACEX(OwlGadget, 1, "TCelArray @" << (void*)this << " index 0x" << index <<
      " replaced by TBitmap @" << (void*)&image);
    return true;
  }
  return false;
}

//
//
//
bool
TCelArray::Replace(int index, const TCelArray& src, int srcidx)
{
  if (index >= 0 && index < NCelsUsed) {
    OwlCopyBmp(*Bitmap, (const TBitmap&)src, CelOffset(index,0), 
              TSize(CSize.cx,CSize.cy*NRows), CelRect(srcidx,0).TopLeft());
    TRACEX(OwlGadget, 1, "TCelArray @" << (void*)this << " index 0x" << index <<
      " replaced by TCelArray @" << " index 0x" << srcidx);
    return true;
  }
  return false;
}

//
// Draws the cel at index onto the DC at position x and y.
//
bool
TCelArray::BitBlt(int index, TDC& dstDC, int x, int y, uint32 rop)
{
  TMemoryDC srcDC(*Bitmap);
  TRect cr = CelRect(index,-1);
  dstDC.BitBlt(x, y, cr.Width(), cr.Height(), srcDC, cr.left, cr.top, rop);
  return true;
}

//
// Draws the image of the cel onto the DC.
//
bool
TCelArray::BitBlt(int index, TDC& dstDC, int x, int y, int /*dx*/, int /*dy*/,
                  const TColor& /*bgClr*/, const TColor& /*fgClr*/)
{
  TMemoryDC srcDC(*Bitmap);
  TRect cr = CelRect(index,-1);
  dstDC.BitBlt(x, y, cr.Width(), cr.Height(), srcDC, cr.left, cr.top);
  return true;
}

//
// Draws the image of the cel onto the DC.
//
bool
TCelArray::StretchBlt(int index, TDC& dstDC, const TRect& dstRect, uint32 rop)
{
  TMemoryDC srcDC(*Bitmap);
  TRect cr = CelRect(index,-1);
  dstDC.StretchBlt(dstRect.left,dstRect.top, dstRect.Width(),dstRect.Height(),
                   srcDC, cr.left, cr.top, cr.Width(), cr.Height(), rop);
  return true;
}

//------------------------------------------------------------------------
// Helper routines
//

//
// Copy specified bitmap to the destination DC
//
bool
OwlCopyBmp(TBitmap& dstBM, const TBitmap& srcBM, const TPoint& pt, const TSize& size, const TPoint offset)
{
  TMemoryDC srcDC(CONST_CAST(TBitmap&, srcBM));
  TMemoryDC dstDC(dstBM);
  return dstDC.BitBlt(TRect(pt, size), srcDC, offset);
}

//
// Resize CelArray as needed to accomodate 'need' more cels. Return false on
// failure.
//
bool
TCelArray::MaybeResize(int need)
{
  if (NCelsUsed + need > NCels)
    if (!Resize(NCels + NGrowBy*((NCelsUsed+need-NCels) / NGrowBy + 1)))
      return false;
  return true;
}

//
// Resize the CelArray by re-allocating the bitmap(s) to the new size &
// copying over.
//
bool
TCelArray::Resize(int newCount)
{
  // Can't resize--the bitmap is not ours
  //
  if (!ShouldDelete)
    return false;

  TBitmap* bitmap = new TBitmap(TScreenDC(), CSize.cx*newCount, CSize.cy*NRows);

  // Copy old bitmap if there is one
  //
  if (Bitmap) {
    TMemoryDC srcDC(*Bitmap);
    TMemoryDC dstDC(*bitmap);
    dstDC.BitBlt(TRect(0, 0, CSize.cx*NCels, CSize.cy*NRows), srcDC, TPoint(0));

    // Cleanup old bitmap
    //
    srcDC.RestoreBitmap();
    delete Bitmap;
  }

  // Update bitmap data member
  //
  Bitmap = bitmap;

  // Update total number of Cels
  //
  NCels = newCount;
  return true;
}

__OWL_END_NAMESPACE
//==============================================================================

