//----------------------------------------------------------------------------
// ObjectComponents
// Copyright (c) 1994, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.13 $
//$Author: jogybl $
//$Date: 2007-11-20 09:10:27 $
//
// OLE Registration definitions
//----------------------------------------------------------------------------

#if !defined(OCF_OCREG_H)
#define OCF_OCREG_H

#if !defined(OWL_POINTER_H)
# include <owl/pointer.h>
#endif
//!FIXTHIS: Compile asserts when autodefs is followed by registry
//!fix: move autodefs below registry --aew
#if !defined(OWL_REGISTRY_H)
# include <owl/registry.h>
#endif

#if !defined(OCF_AUTODEFS_H)
# include <ocf/autodefs.h>
#endif
#if !defined(OCF_APPDESC_H)
# include <ocf/appdesc.h>  // private for inline implementation only
#endif

#if defined(BI_HAS_PRAGMA_ONCE)
# pragma once
#endif
#if defined(BI_COMP_WATCOM)
# pragma read_only_file
#endif


__OCF_BEGIN_NAMESPACE

//
// Global registration functions for OLE. Most are wrappers for winsys's
// TRegistry functions
//

long                                   // returns numeric value of <docflags>
OcRegisterClass(__OWL TRegList& regInfo,     // obj holding array of reg parameters
                __OWL TModule* module,       // 0 defaults to current task
                _tostream& out,        // open ostream to stream reg entries
                __OWL TLangId lang,          // language for registration
                LPTSTR filter = 0,      // internal use to restrict entries
                __OWL TRegItem* defaults = 0,// optional registration default list
                __OWL TRegItem* overrides= 0);// optional registration override list

int                                   // returns error count, 0 if success
OcUnregisterClass(__OWL TRegList& regInfo,  // registration table used for register
                  __OWL TRegItem* overrides=0); // optional additional regitration item

inline int OcRegistryValidate(_tistream& in) {
  return __OWL TRegistry::Validate(__OWL TRegKey::GetClassesRoot(), in);
}

inline void OcRegistryUpdate(_tistream& in) {
  __OWL TRegistry::Update(__OWL TRegKey::GetClassesRoot(), in);
}

int                   // return: 0=no debug reg, -1=user clsid, 1=default used
OcSetupDebugReg(__OWL TRegList& regInfo,    // original registration list
                __OWL TRegItem* regDebug,   // temp override list[DebugReplaceCount]
                __OWL TLangId lang,         // language for registration
                LPTSTR clsid);         // default temp debug clsid string buf

//!CQ make these local to ocreg ???
//
const int DebugRegCount = 4+1;        // replacements + room for terminator

extern _TCHAR AppDebugFilter[];       // templates needed for debug app reg
extern _TCHAR DocDebugFilter[];       // templates needed for debug doc reg
extern __OWL TRegItem OcRegNoDebug[];       // override list to shut off "debugger"
extern __OWL TRegItem OcRegNotDll[];        // override to shut off EXE-only items

//----------------------------------------------------------------------------
// TRegistrar class
//

//
// Application running mode and registration flags
// these are initialized when processing command line or registration requests
// the application's copy of the initial option flags is dynamically updated
//
enum TOcAppMode {
  amRegServer    = 0x0001, // complete registration database update requested
  amUnregServer  = 0x0002, // registration database unregistration requested
  amAutomation   = 0x0004, // set from cmdline when EXE lauched for automation
  amEmbedding    = 0x0008, // cmdline, overridden per Instance if embedded DLL
  amLangId       = 0x0010, // user registration requested a particular LangId
  amTypeLib      = 0x0020, // requested typelib to be generated and registered
  amDebug        = 0x0040, // user requested launching for debugging
  amNoRegValidate= 0x0080, // user request to suppress registry validation
  amExeModule    = 0x0100, // set for EXE components, 0 if DLL inproc server
  amExeMode      = 0x0200, // may be overridden per instance if running DLL
  amServedApp    = 0x0400, // per instance flag, app refcnt held by container
  amSingleUse    = 0x0800, // set from app reg, may be forced on per instance
  amQuietReg     = 0x1000, // suppress error UI during registration processing
  amRun          = 0x2000, // set in factory call to run application msg loop
  amShutdown     = 0x4000, // set in factory call to shutdown/delete app
  amAnyRegOption = amRegServer | amUnregServer | amTypeLib,
};

// Factory routine which creates OLE object
//
#if defined(BI_COMP_MSC)
typedef IUnknown* (*TComponentFactory)(IUnknown* outer, uint32 options, uint32 id);
#else
typedef IUnknown* (*TComponentFactory)(IUnknown* outer, uint32 options, uint32 id = 0);
#endif


class _ICLASS TAppDescriptor;

//
// class TRegistrar
// ~~~~~ ~~~~~~~~~~
// Application registration manager interface class
//
class _ICLASS TRegistrar {
  public:
    TRegistrar(__OWL TRegList& regInfo, TComponentFactory callback,
               owl_string& cmdLine, 
							 __OWL TModule* module = gModule);
    virtual ~TRegistrar();

    // Create/Expose/Destroy automation object proxies
    //
    TUnknown* CreateAutoApp(TObjectDescriptor app, uint32 options,
                            IUnknown* outer=0);
    void      ReleaseAutoApp(TObjectDescriptor app);
    TUnknown* CreateAutoObject(TObjectDescriptor doc, TServedObject& app,
                               IUnknown* outer=0);
    TUnknown* CreateAutoObject(const void* obj, const __OWL_TYPEINFO& objInfo,
                               const void* app, const __OWL_TYPEINFO& appInfo,
                               IUnknown* outer=0);

    virtual void*     GetFactory(const GUID& clsid, const GUID& iid);
    virtual bool      CanUnload();

    // Run (run app instance if EXE) and Shutdown
    //
    virtual int       Run();
    virtual void      Shutdown(IUnknown* releasedObj, uint32 options);

    // Walk linked list of registrars
    //
    static TRegistrar* GetNext(TRegistrar* reg);

    // Registration management functions
    //
    void              RegisterAppClass();
    void              UnregisterAppClass();

    // Command line options accessors
    //
    bool              IsOptionSet(uint32 option) const;
    uint32            GetOptions() const;
    void              SetOption(uint32 bit, bool state);
    void              ProcessCmdLine(owl_string& cmdLine);

    // Accessor to AppDescriptor object (real workhorse object!)
    //
    TAppDescriptor&   GetAppDescriptor() {return AppDesc;}

  protected:
    TRegistrar(TAppDescriptor& appDesc);
    TAppDescriptor& AppDesc;

  private:

    // Members to maintain linked list of registrar objects
    //
    static TRegistrar* RegistrarList;
    TRegistrar*        Next;
};

//----------------------------------------------------------------------------
//  Factory for automated OLE components, no linking/embedding support
//

//
// Simple factor for COM Servers
//
template <class T> class TOcComFactory {
  public:
    operator TComponentFactory() {return Create;}

    // Main Create callback function called to create app and/or object
    //
    static IUnknown* Create(IUnknown* outer, uint32 options, uint32 id);
};

// Main Create callback function called to create COM object
//
template <class T> IUnknown*
TOcComFactory<T>::Create(IUnknown* outer, uint32 /*options*/, uint32 /*id*/)
{
  // Create instance of our object
  //
  T *objPtr = new T;

  // Set the controlling IUnknown
  //
  objPtr->SetOuter(outer);

  // Return IUnknown [operator automatically calls AddRef()]
  //
  return objPtr->operator IUnknown*();
}

//
// Simple factory for Automation Server
//
template <class T> class TOcAutoFactory {
  public:
    operator TComponentFactory() {return Create;}

    // Callouts to allow replacement of individual creation steps
    //
    static T*        CreateApp(HINSTANCE hInst, uint32 options);
    static int       RunApp(T* app);
    static void      DestroyApp(T* app);

    // Main Create callback function called to create app and/or object
    //
    static IUnknown* Create(IUnknown* outer, uint32 options, uint32 id);
};

//
// Called when the app is not found and needs to be created
//
template <class T> T*
TOcAutoFactory<T>::CreateApp(HINSTANCE hInst, uint32 options)
{
  T* app = new T(hInst, options);
  return app;
}

//
// Called to run the application message loop if an EXE, or DLL in amExeMode
//
template <class T> int
TOcAutoFactory<T>::RunApp(T* /*app*/)
{
  MSG msg;
  while(GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

//
// Called to destroy the application previously created
//
template <class T> void
TOcAutoFactory<T>::DestroyApp(T* app)
{
  delete app;
}

//
// Main Create callback function called to create app
//
template <class T> IUnknown*
TOcAutoFactory<T>::Create(IUnknown* outer, uint32 options, uint32 /*id*/)
{
  static T* exeApp;     // used to hold EXE object until OLE factory call
  T* app;
  IUnknown* ifc = 0;
  if (options & amShutdown)
    return (options & amServedApp) ? 0 : outer;
  if ((options & amAutomation) && (options & amServedApp)) {
    app = exeApp;    // if EXE, retrieve app created on initial call from main
  } else {
    app = CreateApp(_hInstance, options);
  }
  if ((options & amAutomation) && !(options & amServedApp)) {
    exeApp = app;    // if EXE, hold app until factory call when outer known
  } else {
    ifc = *::Registrar->CreateAutoApp(TAutoObjectDelete<T>(app),options,outer);
  }
  if (options & amRun) {
    RunApp(app);
    DestroyApp(app);
  } // else DLL server, ifc will be released by controller, which deletes app
  return ifc;
}

//----------------------------------------------------------------------------

#if !defined(OCF_APPDESC_H)
# include <ocf/appdesc.h>  // private for inline implementation only
#endif

//
//
//
inline TUnknown*
TRegistrar::CreateAutoApp(TObjectDescriptor app, uint32 opts, IUnknown* outer) {
  return AppDesc.CreateAutoApp(app, opts, outer);
}

//
//
//
inline void
TRegistrar::ReleaseAutoApp(TObjectDescriptor app) {
  AppDesc.ReleaseAutoApp(app);
}

//
//
//
inline TUnknown*
TRegistrar::CreateAutoObject(TObjectDescriptor doc, TServedObject& app,
                             IUnknown* outer) {
  return AppDesc.CreateAutoObject(doc, app, outer);
}

//
//
//
inline TUnknown*
TRegistrar::CreateAutoObject(const void* obj, const __OWL_TYPEINFO& objInfo,
                             const void* app, const __OWL_TYPEINFO& appInfo,
                             IUnknown* outer) {
  return AppDesc.CreateAutoObject(obj, objInfo, app, appInfo, outer);
}

//
//
//
inline void 
TRegistrar::RegisterAppClass() {
  AppDesc.RegisterClass();
}

//
//
//
inline void 
TRegistrar::UnregisterAppClass()  {
  AppDesc.UnregisterClass();
}

//
//
//
inline bool 
TRegistrar::IsOptionSet(uint32 option) const {
  return AppDesc.IsOptionSet(option);
}

//
//
//
inline uint32 TRegistrar::GetOptions() const {
  return AppDesc.GetOptions();
}

//
//
//
inline void TRegistrar::SetOption(uint32 bit, bool state) {
  AppDesc.SetOption(bit,state);
}

//
//
//
inline void 
TRegistrar::ProcessCmdLine(owl_string& cmdLine) {
  AppDesc.ProcessCmdLine(cmdLine);
}

__OCF_END_NAMESPACE

#endif  // OCF_OCREG_H
