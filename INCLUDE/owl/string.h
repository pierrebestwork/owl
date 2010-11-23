//----------------------------------------------------------------------------
// Borland WinSys Library
// Copyright (c) 1994, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.28 $
//$Author: jogybl $
//$Date: 2007-09-15 11:43:48 $
//
// Definition of class TString, a flexible universal string envelope class.
// Facilitates efficient construction and assignment of many string types
//----------------------------------------------------------------------------

#if !defined(OWL_STRING_H)
#define OWL_STRING_H

#if !defined(OLW_PRIVATE_DEFS_H)
# include <owl/private/defs.h>
#endif
#if !defined(OWL_PRIVATE_OLE2INC_H) && !defined(BI_SUPPRESS_OLE)
# include <owl/private/ole2inc.h>
#endif
#if !defined(OWL_PRIVATE_MEMORY_H)
# include <owl/private/memory.h>
#endif
#if !defined(OWL_MODULE_H)
# include <owl/module.h>
#endif
#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif

#if !defined(BI_PLAT_MSW)
# error Locale classes are only supported under MSW
#endif

#if !defined(OWL_LCLSTRNG_H)
# include <owl/lclstrng.h>
#endif
//#if !defined(OWL_PRIVATE_CSTRING_H )
//# include <owl/private/cstring.h>
//#endif


__OWL_BEGIN_NAMESPACE


// Generic definitions/compiler options (eg. alignment) preceeding the
// definition of classes
#include <owl/preclass.h>

#if !defined(BI_SUPPRESS_OLE)
//
// class TSysStr
// ~~~~~ ~~~~~~~
// System string (BSTR) encapsulation. Also gives BSTRs a unique type
// Always assumes ownership of the BSTR, use Relinquish to take BSTR away.
//
#  if defined(BI_COMP_BORLANDC)
#    pragma warn -inl
#  endif
class _OWLCLASS TSysStr {
  public:
    TSysStr() : P(0) {}
    TSysStr(const BSTR p) : P(p) {}
    TSysStr(const TSysStr& src) : P(TOleAuto::SysAllocString(src.P)) {}

   ~TSysStr() {if (P) TOleAuto::SysFreeString(P);}

    int    operator !() const {return P == 0;}
    operator const BSTR() const {return P;}
    operator BSTR() {return P;}

    TSysStr& operator =(BSTR p) {if (P) TOleAuto::SysFreeString(P); P = p; return *this;}
    operator BSTR *() {if (P) {TOleAuto::SysFreeString(P); P = 0;} return &P;}

    BSTR   Relinquish() {BSTR p = P; P = 0; return p;}

  protected:
    BSTR P;

  private:
#if defined(BI_COMP_GNUC)
    void* operator new(size_t) {throw bad_alloc();} // prohibit use of new, delete, etc
#else
    void* operator new(size_t) {return 0;} // prohibit use of new, delete, etc
#endif
    void  operator delete(void*) {}
};
#  if defined(BI_COMP_BORLANDC)
#    pragma warn .inl
#  endif

#endif

class _OWLCLASS TUString;

//
// class TString
// ~~~~~ ~~~~~~~
// Reference to reference counted string object TUString
// Lightweight reference object consisting of a pointer to actual object
// Facilitates copying and assignment with minimal string reallocations
//
class _OWLCLASS TString {
  public:
    // Construct a TString from any type of string
    //!CQ the assumption that a const char* is non-volatile is bogus!
    //
    TString(const char * s = 0);
#if defined(BI_HAS_WCHAR)
    TString(const wchar_t* s);
#endif
//!CQ no non-const? UString always assumes [W]Const union member
#if !defined(BI_SUPPRESS_OLE)
    TString(BSTR s, bool loan);
    TString(TSysStr& s, bool loan);
#endif
    TString(const owl_string& s);
    TString(TUString* s);
    TString(const TString& src);

   ~TString();

    // Information
    //
    int  Length() const;        // The length in characters of this string
    bool IsNull() const;        // Is the string NULL?
    bool IsWide() const;        // Are the string contents any kind of wide?

    // Assign any type of string into this TString
    //
    TString& operator =(const TString& s);
    TString& operator =(const owl_string& s);
    TString& operator =(const char * s);
    TString& operator =(char* s);
#if defined(BI_HAS_WCHAR)
    TString& operator =(const wchar_t* s);
    TString& operator =(wchar_t* s);
#endif

    // Convert this TString into the desired string type & return pointer into
    // this TString
    //
    operator const char *() const;
    operator char*();
#if defined(BI_HAS_WCHAR)
    operator const wchar_t*() const;
    operator wchar_t*();
#endif
#if !defined(BI_SUPPRESS_OLE)
    BSTR AsBSTR() const;
#endif

    // Relinquish ownership and return contents of this TString. Caller then
    // owns the string & must delete or free it.
    //!CQ currently returns a copy & then frees its copy on destruction
    //
#if !defined(BI_SUPPRESS_OLE)
    BSTR     RelinquishSysStr() const;
#endif
#if defined(BI_HAS_WCHAR)
    wchar_t* RelinquishWide() const;
#endif
    char*    RelinquishNarrow() const;
#if !defined(UNICODE) //!CQ test for default width
    char*    Relinquish() const;
#else
    wchar_t* Relinquish() const;
#endif

    // Language related
    //
    TLangId GetLangId();
    void    SetLangId(TLangId id);

  protected:
    TUString* S;
};

#if !defined(BI_SUPPRESS_OLE)
//
// Provide ANSI to Wide conversion when OLE requires wide chars
// Allocate a unicode BSTR from an ANSI char*
//
#if defined(BI_OLECHAR_WIDE)
# define OleStr(s) __OWL TString(s)
# define OleText(s) L##s
  inline BSTR SysAllocString(const char * str) {
    return TOleAuto::SysAllocString((wchar_t*)(const wchar_t*)TString(str));
  }
#else
# define OleStr(s) s
# define OleText(s) s
#endif
#endif // !defined(BI_SUPPRESS_OLE)

//----------------------------------------------------------------------------

//
// class TUString
// ~~~~~ ~~~~~~~~
// Privately used by TString to manage string pointers
// This is a reference counted union of various string representatons
// Constructors/destructors are private to enforce reference count model
// Create functions are used to facilitate rapid allocation schemes
// Null pointers are never stored; instead a static null object is ref'd
//
#  if defined(BI_COMP_BORLANDC)
#    pragma warn -inl
#  endif
class _OWLCLASS TUString {
  public:
    static TUString* Create(const char * str);
    static TUString* Create(char* str);
#if defined(BI_HAS_WCHAR)
    static TUString* Create(const wchar_t* str);
    static TUString* Create(wchar_t* str);
#endif
#if !defined(BI_SUPPRESS_OLE)
    static TUString* Create(TSysStr& str, bool loan, TLangId lang = 0);
    static TUString* Create(BSTR str, bool loan, TLangId lang = 0);
#endif //#if !defined(BI_SUPPRESS_OLE)
    static TUString* Create(const owl_string& str);

    TUString* Assign(const TUString& s);
    TUString* Assign(const owl_string& s);
    TUString* Assign(const char * s);
    TUString* Assign(char* s);
#if defined(BI_HAS_WCHAR)
    TUString* Assign(const wchar_t* s);
    TUString* Assign(wchar_t* s);
#endif
#if !defined(BI_SUPPRESS_OLE)
    TUString* Assign(BSTR str, TLangId lang);
#endif //#if !defined(BI_SUPPRESS_OLE)
    operator const char *() const;
    operator char*();
#if defined(BI_HAS_WCHAR)
    operator const wchar_t*() const;
    operator wchar_t*();
#endif

    TUString& operator ++();    // Preincrement operator only
    TUString& operator --();    // Predecrement operator only

    int  Length() const;        // Return appropriate string length
    bool IsNull() const;        // Is the string a null string?
    bool IsWide() const;        // Are the string contents any kind of wide?

    TLangId Lang;
#if !defined(BI_SUPPRESS_OLE)
    void RevokeBstr(BSTR s);    // Used to restore if Created with loan==true
    void ReleaseBstr(BSTR s);   // Used to unhook  if Created with loan==true
#endif

#if defined(BI_HAS_WCHAR)
    static wchar_t* ConvertAtoW(const char* src, size_t len = (size_t)-1);
    static char* ConvertWtoA(const wchar_t* src, size_t len = (size_t)-1);
#endif
#if !defined(BI_SUPPRESS_OLE)
    static BSTR ConvertAtoBSTR(const char* src);
    BSTR ConvertToBSTR();
#endif //#if !defined(BI_SUPPRESS_OLE)

#if defined(BI_COMP_MSC)    // MSC can't handle the dtor being private
  public:
#else
  private:
#endif
   ~TUString() {Free();}

  private:
    //!CQ the assumption that a const char* is non-volatile is bogus!
    TUString(const char & str);
    TUString(char& str);
#if defined(BI_HAS_WCHAR)
    TUString(const wchar_t& str);
    TUString(wchar_t& str);
#endif
#if !defined(BI_SUPPRESS_OLE)
    TUString(TSysStr& str, bool loan, TLangId lang);
    TUString(BSTR str, bool loan, TLangId lang);
#endif
    TUString(const owl_string& str);

    void Free();
    owl_string& GetOWLString();
    const owl_string& GetOWLString() const;
    void AllocOWLString(const owl_string& as);

    char*    ChangeToCopy();
#if defined(BI_HAS_WCHAR)
    wchar_t* ChangeToWCopy();
#endif

    enum TKind {
      isNull,
      isConst, isCopy,
      isWConst, isWCopy,
      isBstr, isExtBstr,
      isString,
#if 0  // if alighn greater then 2 
    } Kind : 16;
    int16 RefCnt;
#else
    } Kind;
    int RefCnt;
#endif
    union {
      const char * Const;  // Passed-in string, NOT owned here, read-only
      char*            Copy;   // Local copy, must be deleted, read-write
#if defined(BI_HAS_WCHAR)
      const wchar_t*  WConst; // Unicode version of Const (Win32)
      wchar_t*        WCopy;  // Unicode version of Copy (Win32)
#endif
#if !defined(BI_SUPPRESS_OLE)
      BSTR            Bstr;   // Copy of pointer, owned here
#endif
      char            StringMem[sizeof(owl_string)]; // Placeholder for string:: object
    };

    static TUString Null;     // Null TString references this
    TUString() : Lang(0),Kind(isNull),RefCnt(1),Const(0) {} // for Null object

  friend class TString;       // Envelope string class
};
#  if defined(BI_COMP_BORLANDC)
#    pragma warn .inl
#  endif


// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>

//----------------------------------------------------------------------------
// Inlines
//

//
// Construct a TString from a character array
//
inline TString::TString(const char * s)
:
  S(TUString::Create(s))
{
}

#if defined(BI_HAS_WCHAR)
//
// Construct a TString from a wide character array
//
inline TString::TString(const wchar_t* s)
:
  S(TUString::Create(s))
{
}
#endif

#if !defined(BI_SUPPRESS_OLE)
//
// Construct a TString from a BSTR (OLE String)
//
inline TString::TString(BSTR s, bool loan)
:
  S(TUString::Create(s, loan))
{
}

//
// Construct a TString from a System string (BSTR)
//
inline TString::TString(TSysStr& s, bool loan)
:
  S(TUString::Create(s, loan))
{
}
#endif //#if !defined(BI_SUPPRESS_OLE)

//
// Construct a TString from a string
//
inline TString::TString(const owl_string& s)
:
  S(TUString::Create(s))
{
}

//
// Construct a TString from a TUString
//
inline TString::TString(TUString* s)
:
  S(s)
{
}

//
// Construct a TString from a TString (Copy Constructor)
//
inline TString::TString(const TString& src)
:
  S(src.S)
{
  ++*S;
}

//
// Destruct a TString (actually decrements a reference counter)
//
inline TString::~TString()
{
  --*S;
}

//
// Return the length of the string
//
inline int TString::Length() const
{
  return S->Length();
}

//
// Return true if string is empty
//
inline bool TString::IsNull() const
{
  return S->IsNull();
}

//
// Return true if string uses wide character set
//
inline bool TString::IsWide() const
{
  return S->IsWide();
}

//
// Copy contents of TString s into this string
//
inline TString& TString::operator =(const TString& s) 
{
  S = S->Assign(*s.S); return *this;
}

//
// Copy contents of string s into this string
//
inline TString& TString::operator =(const owl_string& s)  
{
  S = S->Assign(s); return *this;
}

//
// Copy contents of const char* s into this string
//
inline TString& TString::operator =(const char * s)
{
  S = S->Assign(s); return *this;
}

//
// Copy contents of char* s into this string
//
inline TString& TString::operator =(char* s)
{
  S = S->Assign(s); return *this;
}

#if defined(BI_HAS_WCHAR)

//
// Copy contents of const wchar_t* s into this string
//
inline TString& TString::operator =(const wchar_t* s)
{
  S = S->Assign(s); return *this;
}

//
// Copy contents of wchar_t* s into this string
//
inline TString& TString::operator =(wchar_t* s)       
{
  S = S->Assign(s); return *this;
}
#endif

//
// Return string as a const char *
//
inline TString::operator const char *() const
{
  return S->operator const char *();
}

//
// Return string as a char*
//
inline TString::operator char*()          
{
  return S->operator char*();
}

#if defined(BI_HAS_WCHAR)

//
// Return string as a const wchar_t*
//
inline TString::operator const wchar_t*() const 
{
  return S->operator const wchar_t*();
}

//
// Return string as a wchar_t*
//
inline TString::operator wchar_t*()       
{
  return S->operator wchar_t*();
}
#endif

#if !defined(BI_SUPPRESS_OLE)
//
// Return a BSTR object owned by 'this' (not by the caller)
//
inline BSTR TString::AsBSTR() const 
{
  return S->ConvertToBSTR();
}

//
// Return a pointer (BSTR) to a copy of the string
//
inline BSTR TString::RelinquishSysStr() const
{
#if defined(BI_OLECHAR_WIDE)
  return TOleAuto::SysAllocString((wchar_t*)(const wchar_t*)*S);
#else
  return ::SysAllocString((const char*)*S);
#endif
}
#endif //#if !defined(BI_SUPPRESS_OLE)

#if defined(BI_HAS_WCHAR)
//
// Return a pointer (wchar_t*) to a copy of the string
//
inline wchar_t* TString::RelinquishWide() const
{
  return strnewdup((const wchar_t*)*S);
}
#endif

//
// Return a pointer (char*) to a copy of the string
//
inline char* TString::RelinquishNarrow() const
{
  return strnewdup((const char*)*S);
}

//
// Return a pointer (_TCHAR*) to a copy of the string
//
#if !defined(UNICODE) //!CQ better test for default width
inline char* TString::Relinquish() const
{
  return RelinquishNarrow();
}
#else
inline wchar_t* TString::Relinquish() const
{
  return RelinquishWide();
}
#endif

//
// Get Language Id of this string
//
inline TLangId TString::GetLangId()
{
  return S->Lang;
}


//
// Set Language Id of this string
//
inline void TString::SetLangId(TLangId id)
{
  S->Lang = id;
}

//----------------------------------------------------------------------------

//
// Increment reference counter for this string
//
inline TUString& TUString::operator ++()
{
  ++RefCnt;
  return *this;
}

//
// Decrement reference counter for this string
//
inline TUString& TUString::operator --()
{
  if (--RefCnt != 0)
    return *this;
  delete this;
  return Null;
}

//
// Return true if string is empty
//
inline bool TUString::IsNull() const
{
  return Kind == isNull;
}

//
// Return true if string uses wide character set
//
inline bool TUString::IsWide() const
{
#if defined(BI_OLECHAR_WIDE)
  return Kind == isWConst || Kind == isWCopy || Kind == isBstr || Kind == isExtBstr;
#else
  return Kind == isWConst || Kind == isWCopy;
#endif
}

inline owl_string& TUString::GetOWLString()
{
  CHECK(Kind == isString);
  return *reinterpret_cast<owl_string*>(StringMem);
}

inline const owl_string& TUString::GetOWLString() const
{
  CHECK(Kind == isString);
  return *reinterpret_cast<const owl_string*>(StringMem);
}

inline void TUString::AllocOWLString(const owl_string& as)
{
  new(StringMem) owl_string(as);
}

__OWL_END_NAMESPACE


#endif  // OWL_STRING_H
