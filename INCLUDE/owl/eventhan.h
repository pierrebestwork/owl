//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.29 $
//$Author: jogybl $
//$Date: 2009-08-05 10:48:52 $
//
// Definition of TEventHandler and related classes & macros
//----------------------------------------------------------------------------

#if !defined(OWL_EVENTHAN_H)
#define OWL_EVENTHAN_H

#if !defined(OWL_SIGNATUR_H)
# include <owl/signatur.h>
#endif
#if !defined(OWL_DISPATCH_H)
# include <owl/dispatch.h>
#endif
#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif

#if !defined(OWL_PRIVATE_STRMDEFS_H) && (defined(__TRACE) || defined(__WARN))
#include <owl/private/strmdefs.h>
#endif

__OWL_BEGIN_NAMESPACE

// Generic definitions/compiler options (eg. alignment) preceeding the 
// definition of classes
#include <owl/preclass.h>

//
// Class/operator which converts a Windows message to its string equivalent for
// use with the diagnostic library
//
#if defined(__TRACE) || defined(__WARN)
class _OWLCLASS MsgName;
_OWLCFUNC(_tostream &) operator <<(_tostream& os, const MsgName& msg);
  class _OWLCLASS MsgName {
    public:
      friend _tostream& operator <<(_tostream& os, const MsgName& msg);
      MsgName(uint msg) : Message(msg) {}
    private:
      uint Message;
  };
#endif

//
// Modifier macro used with response tables to put them in  segments if
// needed.
//
#if defined(OWLRTFAR)
# define __RTFAR  __far
#else
# define __RTFAR
#endif

//
// Messages defined for OWL use - the top of the user range of ids is reserved
//
#define WM_OWLLAST          0x7FFF
#define WM_OWLFIRST        (WM_OWLLAST - 0x03FF)

#define WM_COMMAND_ENABLE  (WM_OWLLAST - 0)
#define WM_CHILDINVALID    (WM_OWLLAST - 1)
#define WM_OWLDOCUMENT     (WM_OWLLAST - 2)
#define WM_OWLVIEW         (WM_OWLLAST - 3)
#define WM_OWLNOTIFY       (WM_OWLLAST - 4)
#define WM_OWLPREPROCMENU  (WM_OWLLAST - 5)
#define WM_OWLCANCLOSE     (WM_OWLLAST - 6)
#define WM_VBXINITFORM     (WM_OWLLAST - 7)
#define WM_VBXNAME         (WM_OWLLAST - 8)
#define WM_VBXBASE         (WM_OWLLAST - 8 - 256)
#define WM_OWLWAKEUP       (WM_VBXBASE - 1)
#define WM_OWLFRAMESIZE    (WM_VBXBASE - 2) // Notify children of frame resizing
#define WM_OWLSLIPDBLCLK   (WM_VBXBASE - 3) // Notify parent of user dblclick of edge slip
#define WM_OWLWINDOWDOCKED (WM_VBXBASE - 4) // Notify window it was [un]docked/reparented
#define WM_OWLCREATETTIP   (WM_VBXBASE - 5) // Notify gadget window to create tooltips
#define WM_OWLHELPHIT      (WM_VBXBASE - 6) // Notify Help manager with hit point

//
// Portable types for message & event components
//
typedef LRESULT TResult;  // Returned result of a message
typedef uint    TMsgId;   // Message ID type
typedef WPARAM  TParam1;  // Type of first message parameter (uint)
typedef LPARAM  TParam2;  // Type of second message parameter (uint32)

template <class T1, class T2>
inline TParam2 MkParam2(const T1& lo, const T2& hi) {
  return (uint32(hi) << 16) | uint16(lo);
}
inline TParam1 MkParam1(uint lo, uint hi) {
  return (uint32(hi) << 16) | uint16(lo);
}

struct Msg {  //  : public tagMSG
  HWND    HWnd;
  uint    Message;
  TParam1 Param1;
  TParam2 Param2;
  uint32  Time;
  TPoint  Pt;         //??????????????????????????

  operator MSG&() {return *this;}
};

//
// Forward declarations
//
template <class T> class _OWLCLASS TResponseTableEntry;
typedef TResponseTableEntry<GENERIC>  TGenericTableEntry;

//
// class TEventHandler
// ~~~~~ ~~~~~~~~~~~~~
// Base class from which to derive classes that can handle events
//
#  if defined(BI_COMP_BORLANDC)
#    pragma warn -inl
#  endif
class _OWLCLASS _RTTI TEventHandler {
  public:
    class TEventInfo {
      public:
        const uint                  Msg;
        const uint                  Id;
        GENERIC*                    Object;
        TGenericTableEntry __RTFAR* Entry;

        TEventInfo(uint msg, uint id=0) : Msg(msg), Id(id) {Entry = 0;}
    };
    typedef bool(*TEqualOperator)(TGenericTableEntry __RTFAR&, TEventInfo&);

    // Searches the list of response table entries looking for a match
    //
    virtual bool     Find(TEventInfo& info, TEqualOperator op = 0);

    TResult          Dispatch(TEventInfo& info, TParam1 p1, TParam2 p2 = 0);
    TResult          DispatchMsg(uint msg, uint id, TParam1 p1, TParam2 p2);

  protected:
    bool             SearchEntries(TGenericTableEntry __RTFAR* entries,
                                   TEventInfo& info,
                                   TEqualOperator op);
};
#  if defined(BI_COMP_BORLANDC)
#    pragma warn .inl
#  endif

//
// class TResponseTableEntry<>
// ~~~~~ ~~~~~~~~~~~~~~~~~~~~~
// Entry in a response table
//
template <class T> class _OWLCLASS TResponseTableEntry {
  public:
    typedef void (T::*PMF)();

    union {
      uint          Msg;
      uint          NotifyCode;
    };
    uint            Id;
    TAnyDispatcher  Dispatcher;
    PMF             Pmf;
};

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <owl/posclass.h>

__OWL_END_NAMESPACE

//
// Macros to declare a response table
//
#define DECLARE_RESPONSE_TABLE(cls)\
  private:\
    static NS_OWL::TResponseTableEntry< cls > __RTFAR __entries[];\
    typedef NS_OWL::TResponseTableEntry< cls >::PMF TMyPMF;\
    typedef cls                               TMyClass;\
  public:\
    bool  Find(TEventInfo&, TEqualOperator = 0)

#define DECLARE_EXPORT_RESPONSE_TABLE(cls)\
  private:\
    static NS_OWL::TResponseTableEntry< cls > __RTFAR __entries[];\
    typedef NS_OWL::TResponseTableEntry< cls >::PMF TMyPMF;\
    typedef cls                               TMyClass;\
  public:\
    bool __export _CALLCNVN Find(TEventInfo&, TEqualOperator = 0)

#define DECLARE_IMPORT_RESPONSE_TABLE(cls)\
  private:\
    static NS_OWL::TResponseTableEntry< cls > __RTFAR __entries[];\
    typedef NS_OWL::TResponseTableEntry< cls >::PMF TMyPMF;\
    typedef cls                               TMyClass;\
  public:\
    bool __import _CALLCNVN Find(TEventInfo&, TEqualOperator = 0)

#define END_RESPONSE_TABLE\
  {{0}, 0, 0, 0}}

#define DEFINE_RESPONSE_TABLE_ENTRIES(cls)\
  NS_OWL::TResponseTableEntry< cls > __RTFAR cls::__entries[] = {

//
// Macro to define a response table for a class with no base response tables
//
// Use it like this:
//    DEFINE_RESPONSE_TABLE(cls)
//      EV_WM_PAINT,
//      EV_WM_LBUTTONDOWN,
//    END_RESPONSE_TABLE;
//
#define DEFINE_RESPONSE_TABLE(cls)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
      return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with one base. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE1(cls, base)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
          base::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with two bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE2(cls, base1, base2)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
          base2::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE3(cls, base1, base2, base3)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
          base3::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE4(cls, base1, base2, base3, base4)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
              base3::Find(eventInfo, equal) ||\
          base4::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE5(cls, base1, base2, base3, base4, base5)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
              base3::Find(eventInfo, equal) ||\
              base4::Find(eventInfo, equal) ||\
          base5::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE6(cls, base1, base2, base3, base4, base5, base6)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
              base3::Find(eventInfo, equal) ||\
              base4::Find(eventInfo, equal) ||\
              base5::Find(eventInfo, equal) ||\
          base6::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE7(cls, base1, base2, base3, base4, base5, base6, base7)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
              base3::Find(eventInfo, equal) ||\
              base4::Find(eventInfo, equal) ||\
              base5::Find(eventInfo, equal) ||\
              base6::Find(eventInfo, equal) ||\
          base7::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE8(cls, base1, base2, base3, base4, base5, base6, base7, base8)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
              base3::Find(eventInfo, equal) ||\
              base4::Find(eventInfo, equal) ||\
              base5::Find(eventInfo, equal) ||\
              base6::Find(eventInfo, equal) ||\
              base7::Find(eventInfo, equal) ||\
          base8::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)

//
// Macro to define a response table for a class with three bases. Use this macro
// exactly like macro DEFINE_RESPONSE_TABLE
//
#define DEFINE_RESPONSE_TABLE9(cls, base1, base2, base3, base4, base5, base6, base7, base8, base9)\
  bool  cls::Find(TEventInfo& eventInfo, TEqualOperator equal)\
      {\
      __OWL_USING_NAMESPACE;\
      eventInfo.Object = (GENERIC*)this;\
       return SearchEntries((TGenericTableEntry __RTFAR*)__entries, eventInfo, equal) ||\
              base1::Find(eventInfo, equal) ||\
              base2::Find(eventInfo, equal) ||\
              base3::Find(eventInfo, equal) ||\
              base4::Find(eventInfo, equal) ||\
              base5::Find(eventInfo, equal) ||\
              base6::Find(eventInfo, equal) ||\
              base7::Find(eventInfo, equal) ||\
              base8::Find(eventInfo, equal) ||\
          base9::Find(eventInfo, equal);\
      }\
  DEFINE_RESPONSE_TABLE_ENTRIES(cls)


#endif  // OWL_EVENTHAN_H
