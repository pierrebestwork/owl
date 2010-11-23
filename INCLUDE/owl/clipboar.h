//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.27 $
//$Author: jogybl $
//$Date: 2007-07-20 10:47:32 $
//
// Definition of classes for clipboard Encapsulation
//----------------------------------------------------------------------------

#if !defined(OWL_CLIPBOAR_H)
#define OWL_CLIPBOAR_H

#if !defined(OWL_DEFS_H)
# include <owl/defs.h>
#endif
#if !defined(OWL_EXCEPT_H)
# include <owl/except.h>         // Owl exception classes
#endif
#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif


__OWL_BEGIN_NAMESPACE

// Generic definitions/compiler options (eg. alignment) preceeding the
// definition of classes
#include <owl/preclass.h>

//
// class TClipboard
// ~~~~~ ~~~~~~~~~~
// The clipboard class encapsulates the methods for the clipboard object
// of Windows.
//
class _OWLCLASS TClipboard {
  public:
    // Constructors / destructor
    //
    TClipboard(HWND hWnd, bool mustOpen=true);  // aquire & open the clipboard
   ~TClipboard();

    // Close & reopen the clipboard
    //
    void        CloseClipboard();
    bool        OpenClipboard(HWND hWnd);

    operator    bool() const;

    HANDLE      GetClipboardData(uint format) const;
    HWND        GetOpenClipboardWindow() const;
    HWND        GetClipboardOwner() const;
    HWND        GetClipboardViewer() const;
    int         GetClipboardFormatName(uint format, LPTSTR formatName,
                                       int maxCount) const;
    int         GetPriorityClipboardFormat(uint * priorityList, int count) const;
    int         CountClipboardFormats() const;
    bool        IsClipboardFormatAvailable(uint format) const;
    bool        EmptyClipboard();
    uint        RegisterClipboardFormat(LPCTSTR formatName) const;
    HANDLE      SetClipboardData(uint format, HANDLE handle);
    HWND        SetClipboardViewer(HWND hWnd) const;

#if defined(__OLE_H) || defined(_INC_OLE)
    bool        QueryCreate(LPCTSTR protocol = DefaultProtocol,
                            OLEOPT_RENDER   renderopt= olerender_draw,
                            OLECLIPFORMAT   format   = 0);
    bool        QueryLink(LPCTSTR protocol = DefaultProtocol,
                          OLEOPT_RENDER   renderopt= olerender_draw,
                          OLECLIPFORMAT   format   = 0);
#endif

#if defined(OWL2_COMPAT)
    // Obsolete way to retrieve the global clipboard object. The recomended
    // way is to construct a TClipboard object using the TClipboard(HWND) ctor
    //
    static TClipboard& GetClipboard();
    static TClipboard TheClipboard;
#endif

    static LPCTSTR     DefaultProtocol;

  protected_data:
    bool              IsOpen;

    TClipboard();      // used by the obsolete global object
};

//
// class TXClipboard
// ~~~~~ ~~~~~~~~~~~
class _OWLCLASS TXClipboard : public TXOwl {
  public:
    TXClipboard(uint resourceId = IDS_CLIPBOARDBUSY);
#if defined(BI_NO_COVAR_RET)
    TXBase* Clone();
#else
    TXClipboard* Clone();
#endif
    void Throw();

    static void Raise(uint resourceId = IDS_CLIPBOARDBUSY);
};

//
// class TClipboardFormatIterator
// ~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~
class _OWLCLASS TClipboardFormatIterator {
  public:
    TClipboardFormatIterator(const TClipboard&);

    uint          Current();
                  operator void* ();
    uint          operator ++();
    uint          operator ++(int);
    void          Restart();

  private:
    uint          _Current;
};

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>


//----------------------------------------------------------------------------
// Inline implementations
//

#if defined(OWL2_COMPAT)
//
// Return the global clipboard object.
//
inline TClipboard& TClipboard::GetClipboard() {
  return TheClipboard;
}
#endif

//
// Create an empty clipboard object.
// Set the state of the object to not opened.
//
#  if defined(BI_COMP_BORLANDC)
#    pragma warn -inl
#  endif
inline TClipboard::TClipboard() {
  IsOpen = false;
}
#  if defined(BI_COMP_BORLANDC)
#    pragma warn .inl
#  endif

//
// Return true if the clipboard is currently owned by this application.
//
inline TClipboard::operator bool() const {
  return IsOpen;
}

//
// Return the handle of the data that is on the clipboard.
//
inline HANDLE TClipboard::GetClipboardData(uint Format) const {
  return ::GetClipboardData(Format);
}

//
// Return the HWND of the application that has the clipboard opened.
//
inline HWND TClipboard::GetOpenClipboardWindow() const {
  return ::GetOpenClipboardWindow();
}

//
// Return the HWND of the application that owns the clipboard.
//
inline HWND TClipboard::GetClipboardOwner() const {
  return ::GetClipboardOwner();
}

//
// Return the first HWND of the clipboard viewing chain.
//
inline HWND TClipboard::GetClipboardViewer() const {
  return ::GetClipboardViewer();
}

//
// Retrieve the name of the format that is on the clipboard.
//
inline int TClipboard::GetClipboardFormatName(uint Format, LPTSTR FormatName, int MaxCount) const {
  return ::GetClipboardFormatName(Format, FormatName, MaxCount);
}

//
// Retrieve the name of the first format on the clipboard.
//
inline int TClipboard::GetPriorityClipboardFormat(uint * priorityList, int count) const {
  return ::GetPriorityClipboardFormat(priorityList, count);
}

//
// Return the number of formats available on the clipboard.
//
inline int TClipboard::CountClipboardFormats() const {
  return ::CountClipboardFormats();
}

//
// Return true if the format is available.
//
inline bool TClipboard::IsClipboardFormatAvailable(uint format) const {
  return ::IsClipboardFormatAvailable(format);
}

//
// Return true if the clipboard is empty.
//
inline bool TClipboard::EmptyClipboard() {
  return ::EmptyClipboard();
}

//
// Register the name of a user-custom format.
// The number returned is the custom format number that should be used with
// SetClipboardData.
//
inline uint TClipboard::RegisterClipboardFormat(LPCTSTR formatName) const {
  return ::RegisterClipboardFormat(formatName);
}

//
// Copy the data onto the clipboard in the format.
//
inline HANDLE TClipboard::SetClipboardData(uint Format, HANDLE Handle) {
  return ::SetClipboardData(Format,Handle);
}

//
// Add the HWND to the clipboard viewer chain.
//
inline HWND TClipboard::SetClipboardViewer(HWND Wnd) const {
  return ::SetClipboardViewer(Wnd);
}

#if defined(__OLE_H) || defined(_INC_OLE)
//
// Return true if the object on the clipboard can be rendered in the
// requested format.
//
inline bool TClipboard::QueryCreate(
                LPCTSTR protocol,
                OLEOPT_RENDER   renderopt,
                OLECLIPFORMAT   format
              )
{
  return ::OleQueryCreateFromClip(protocol, renderopt, format) == OLE_OK;
}

//
// Return true if the object on the clipboard can be linked to in the
// requested format.
//
inline bool TClipboard::QueryLink(
                LPCTSTR protocol,
                OLEOPT_RENDER   renderopt,
                OLECLIPFORMAT   format
              )
{
  return ::OleQueryLinkFromClip(protocol, renderopt, format) == OLE_OK;
}
#endif


//
// Return the current clipboard format.
//
inline uint TClipboardFormatIterator::Current() {
  return _Current;
}

//
// Return true if the iterator is not at an end of the clipboard format
// chain.
//
inline TClipboardFormatIterator::operator void* () {
  return _Current ? this : 0;
}


__OWL_END_NAMESPACE

#endif  // OWL_CLIPBOAR_H
