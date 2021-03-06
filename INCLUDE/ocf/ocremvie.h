//----------------------------------------------------------------------------
// ObjectComponents
// Copyright (c) 1994, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.14 $
//$Author: jogybl $
//$Date: 2007-09-15 11:43:47 $
//
// Definition of TOcRemView Class
//----------------------------------------------------------------------------

#if !defined(OCF_OCREMVIE_H)
#define OCF_OCREMVIE_H

#if !defined(OCF_OCVIEW_H)
# include <ocf/ocview.h>
#endif

#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif


__OCF_BEGIN_NAMESPACE

//
// class TOcRemView
// ~~~~~ ~~~~~~~~~~
// Remote Viewer object for a server document
//
class _ICLASS TOcRemView : public TOcView, public IBPart2 {
  public:
    TOcRemView(TOcDocument& doc, TOcContainerHost* cs, TOcServerHost* ss,
               __OWL TRegList* regList=0, IUnknown* outer=0);

    // Compatibility
    TOcRemView(TOcDocument& doc, __OWL TRegList* regList = 0, IUnknown* outer = 0);

    virtual void   ReleaseObject();

    // IBSite pass-thrus
    //
    void    Invalidate(TOcInvalidate);
    void    Disconnect();
    virtual bool EvSetFocus(bool set);

    // IBDocument pass-thrus
    //
    void   EvClose();
    void   Rename();
    __OWL TString GetContainerTitle();
    void   GetInitialRect(bool selection = false);
    bool   Save(IStorage* storageI);
    bool   Load(IStorage* storageI);

    // IBApplication pass-thrus
    //
    bool   SetContainerStatusText(LPCTSTR text);

    // Server State and Kind accessors, etc.
    //
    enum TState {                  // Current state of this server view
      Hidden,                        //  Hidden & ready to go
      Closing,                       //  Shutting down
      OpenEditing,                   //  Open editing
      InPlaceActive,                 //  Inplace active
    };
    enum TKind {                   // What kind of server view this is
      DontKnow,                      //  Don't know yet, wait for Init()
      Embedding,                     //  Normal Embedding
      LoadFromFile,                  //  Transient load-from-file
      Link,                          //  Load-from-file is really a link
    };
    TState  GetState() const {return State;}
    TKind   GetKind() const {return Kind;}

    // Used by TOcLinkView to let us know our state was changed
    //
    void    SetOpenEditing() {State = OpenEditing;}

    // Object reference & lifetime managment
    //
    ulong   _IFUNC AddRef();
    ulong   _IFUNC Release();
    HRESULT _IFUNC QueryInterface(const GUID & iid, void ** iface);

    HRESULT _IFUNC  GetPartSize(__OWL TSize *);

  protected:
    // IBContains forwarding to base
    //
    HRESULT _IFUNC Init(LPCOLESTR path);
    HRESULT _IFUNC GetPart(IBPart * *, LPCOLESTR);

    // IBContainer implementation for Bolero to use
    //
    HRESULT  _IFUNC BringToFront();

    // TUnknown virtual overrides
    //
    HRESULT      QueryObject(const IID & iid, void * * iface);

    // IBContainer forwarding to base
    //
    void    _IFUNC DragFeedback(__OWL TPoint * p, const __OWL TRect * r,
                                TOcMouseAction a, uint cf, HRESULT& hr);

    HRESULT _IFUNC AllowInPlace();

    // IBDataNegotiator implementation to eliminate ambiguity
    //
    uint     _IFUNC CountFormats();
    HRESULT  _IFUNC GetFormat(uint index, TOcFormatInfo * fmt);

    // IBDataProvider2 implementation
    //
    HANDLE  _IFUNC  GetFormatData(TOcFormatInfo *);
    HRESULT _IFUNC  SetFormatData(TOcFormatInfo * fmt, HANDLE data, BOOL release);
    HRESULT _IFUNC  Draw(HDC, const RECTL *, const RECTL *, TOcAspect, TOcDraw bd);
    HRESULT _IFUNC  Save(IStorage*, BOOL sameAsLoad, BOOL remember);

    // IBPart(2) implementation
    //
    HRESULT _IFUNC  Init(IBSite *, TOcInitInfo *);
    HRESULT _IFUNC  Close();
    HRESULT _IFUNC  CanOpenInPlace();
    HRESULT _IFUNC  SetPartSize(__OWL TSize *);
    HRESULT _IFUNC  SetPartPos(__OWL TRect *);
    HRESULT _IFUNC  Activate(BOOL);
    HRESULT _IFUNC  Show(BOOL);
    HRESULT _IFUNC  Open(BOOL);
    HRESULT _IFUNC  EnumVerbs(TOcVerb *);
    HRESULT _IFUNC  DoVerb(uint);
    HWND    _IFUNC  OpenInPlace(HWND);
    HRESULT _IFUNC  InsertMenus(HMENU, TOcMenuWidths *);
    HRESULT _IFUNC  ShowTools(BOOL);
    void    _IFUNC  FrameResized(const __OWL TRect *, BOOL);
    HRESULT _IFUNC  DragFeedback(__OWL TPoint *, BOOL);
    HRESULT _IFUNC  GetPalette(LOGPALETTE * *);
    HRESULT _IFUNC  SetHost(IBContainer * objContainer);
    HRESULT _IFUNC  DoQueryInterface(const IID & iid, void * * pif);
    LPOLESTR _IFUNC GetName(TOcPartName);

  protected:
    void            Init();          // Constructor helper
                   ~TOcRemView();

    IBContainer*    BContainerI;     //
    IBLinkable*     BLSiteI;         // for site moniker
    IUnknown*       BSite;           // In-place site helper
    IBSite*         BSiteI;          // Site interface
    IBApplication*  BAppI;           // Site's application interface
    HWND            HRealParent;     // View's real parent
    TOcToolBarInfo  ToolBarInfo;     // Inplace tool bar info

    TState          State;           // Current state of this server view
    TKind           Kind;            // What kind of server view this is
};

//----------------------------------------------------------------------------
// Inline implementations
//

//
inline bool TOcRemView::SetContainerStatusText(LPCTSTR text)
{
  return HRIsOK(BAppI->SetStatusText(OleStr(text)));
}

__OCF_END_NAMESPACE

#endif  // OCF_OCREMVIE_H
