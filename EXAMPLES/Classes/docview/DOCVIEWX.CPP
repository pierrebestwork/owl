//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1993, 1995 by Borland International, All Rights Reserved
//
//   Example of a minimal doc/view application. Doc/views must be linked in.
//   NOTE: this example interprets command line flags to select frame type.
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/docmanag.h>
#include <owl/decmdifr.h>
#include <owl/statusba.h>
#include <owl/cmdline.h>
#include <owl/bardescr.h>
#include <stdio.h>
#include <dos.h>
#include "docviewx.rc"
//#include <exception>

class MyDVApp : public TApplication {
  public:
    MyDVApp() {}

    void  InitInstance();
    void  InitMainWindow();
    void  EvNewView(TView& view);
    void  EvCloseView(TView& view);
    void  EvDropFiles(TDropInfo dropInfo);
    void  CmDeskClear();
    void  CmDeskSave();
    void  CmDeskRestore();
    void  CmHideView();
    void  CmShowViews();
    void  CmDisableView();
    void  CmEnableViews();

    int         DocMode;
    bool        Decorated;
    TMDIClient* Client;

  DECLARE_RESPONSE_TABLE(MyDVApp);
  DECLARE_STREAMABLE(, MyDVApp, 1);
};

DEFINE_RESPONSE_TABLE1(MyDVApp, TApplication)
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_WM_DROPFILES,
  EV_COMMAND(CM_DESKCLEAR,   CmDeskClear),
#if !defined(BI_NO_OBJ_STREAMING)
  EV_COMMAND(CM_DESKSAVE,    CmDeskSave),
  EV_COMMAND(CM_DESKRESTORE, CmDeskRestore),
#endif
  EV_COMMAND(CM_HIDEVIEW,    CmHideView),
  EV_COMMAND(CM_SHOWVIEWS,   CmShowViews),
  EV_COMMAND(CM_DISABLEVIEW, CmDisableView),
  EV_COMMAND(CM_ENABLEVIEWS, CmEnableViews),
END_RESPONSE_TABLE;

IMPLEMENT_CASTABLE1(MyDVApp, TApplication);
IMPLEMENT_STREAMABLE1(MyDVApp, TApplication);

#if !defined(BI_NO_OBJ_STREAMING)
const char sSDIStream[] = "C:\\DOCVIEWX.SDI";
const char sMDIStream[] = "C:\\DOCVIEWX.MDI";
_OWLLINK(rTMDIChild);  		// force TMDIChild::Streamer to be linked in
_OWLLINK(rTMDIClient); 		// force TMDIClient::Streamer to be linked in
_OWLLINK(rTFileDocument); // force TMDIClient::Streamer to be linked in
#endif

void MyDVApp::EvDropFiles(TDropInfo dropInfo)
{
  int fileCount = dropInfo.DragQueryFileCount();
  int index = 0;
  while (index < fileCount) {
    int fileLength = dropInfo.DragQueryFileNameLen(index)+1;
    char* filePath = new char [fileLength];
    dropInfo.DragQueryFile(index++, filePath, fileLength);
    TDocTemplate* tpl = GetDocManager()->MatchTemplate(filePath);
    GetDocManager()->CreateDoc(tpl, filePath);
    delete filePath;
  }
  dropInfo.DragFinish();
}

void MyDVApp::InitInstance()
{
  TApplication::InitInstance();
  MainWindow->DragAcceptFiles(true);
}

void MyDVApp::InitMainWindow()
{
	TCmdLine cmd(GetCmdLine().c_str());
	DocMode = dmMDI;
  Decorated = true;
	while (cmd.Kind != TCmdLine::Done) {
		if (cmd.Kind == TCmdLine::Option) {
			switch (*cmd.Token) {
	  	  case 's': case 'S': DocMode = dmSDI; Decorated = false; break;  // command line: -s
  	  	case 'm': case 'M': DocMode = dmMDI; Decorated = false; break;  // command line: -m
		    case 'd': case 'D': DocMode = dmSDI; Decorated = true;  break;  // command line: -d
			}
		}
		cmd.NextToken();
	}
  DocManager = new TDocManager(DocMode | dmMenu, this);
  TFrameWindow* frame;
  if (Decorated){
    TDecoratedFrame* dframe = (DocMode == dmSDI
           ? new TDecoratedFrame(0, "SDI DocView Example", 0,true)
           : new TDecoratedMDIFrame("MDI DocView Example", 0,
                             *(Client=new TMDIClient), true));
    TStatusBar* sb = new TStatusBar(dframe, TGadget::Recessed,
           TStatusBar::CapsLock | TStatusBar::NumLock | TStatusBar::Overtype);
    dframe->Insert(*sb, TDecoratedFrame::Bottom);
    frame = dframe;
  }
  else { // !decorate
    frame = (DocMode == dmSDI
          ? new TFrameWindow(0, "SDI DocView Example")
          : new TMDIFrame("MDI DocView Example", 0, *(Client=new TMDIClient)));
  }
  SetMainWindow(frame);
  frame->SetMenuDescr(TMenuDescr(DocMode==dmSDI ? IDM_DVSDI : IDM_DVMDI));
  if (Decorated)
  	frame->SetBarDescr(new TBarDescr(DocMode==dmSDI ? IDB_DVSDI : IDB_DVMDI));
  frame->SetIcon(this, IDI_DOCVIEW);
}

void MyDVApp::EvNewView(TView& view)
{
  if (DocMode == dmSDI) {
    GetMainWindow()->SetClientWindow(view.GetWindow());
    if (!view.IsOK())
      GetMainWindow()->SetClientWindow(0);
    else{
    	if (view.GetViewMenu())
      	GetMainWindow()->MergeMenu(*view.GetViewMenu());
    	if (view.GetViewBar())
      	GetMainWindow()->MergeBar(*view.GetViewBar());
    }
  }
  else { // DocMode == dmMDI
    TMDIChild* child = new TMDIChild(*Client, 0, view.GetWindow());
    if (view.GetViewMenu())
      child->SetMenuDescr(*view.GetViewMenu());
   	if (view.GetViewBar())
     	child->SetBarDescr(view.GetViewBar(),NoAutoDelete);
    child->Create();
  }
}

void MyDVApp::EvCloseView(TView& /*view*/)
{
  if (DocMode == dmSDI) {
    GetMainWindow()->SetClientWindow(0);
    GetMainWindow()->RestoreMenu();
    GetMainWindow()->RestoreBar();
    GetMainWindow()->SetCaption("SDI DocView Example");
  }
}

void MyDVApp::CmDeskClear()
{
  delete DocManager;   // delete existing document manager, and doc/views
  DocManager = new TDocManager(DocMode | dmMenu, this);
}

void MyDVApp::CmDeskSave()
{
#if !defined(BI_NO_OBJ_STREAMING)
  ofpstream os(DocMode == dmMDI ? sMDIStream : sSDIStream);
  os << *this;  // start stream with application
  os.close();
  if (os.bad()) {
    unlink(DocMode == dmMDI ? sMDIStream : sSDIStream);
    MainWindow->MessageBox("Unable to write desktop file.", "File error",
                           MB_OK | MB_ICONEXCLAMATION);
  }
#endif  // if !defined(BI_NO_OBJ_STREAMING)
}

void MyDVApp::CmDeskRestore()
{
#if !defined(BI_NO_OBJ_STREAMING)
  char* errorMsg = 0;

  ifpstream is(DocMode == dmMDI ? sMDIStream : sSDIStream);
  if (is.bad())
    errorMsg = "Unable to open desktop file.";

  else {
    delete DocManager;   // delete existing document manager, and doc/views
    DocManager = new TDocManager(DocMode | dmMenu, this);
    is >> *this;         // stream in documents, views, windows
    if (is.bad())
      errorMsg = "Error reading desktop file.";
    is.close();
  }
  if (errorMsg)
    MainWindow->MessageBox(errorMsg, "Error", MB_OK | MB_ICONEXCLAMATION);
#endif  // if !defined(BI_NO_OBJ_STREAMING)
}
#if !defined(BI_NO_OBJ_STREAMING)
void MyDVApp::Streamer::Write(opstream& os) const
{
  MyDVApp* o = GetObject();
  TFrameWindow* mainWnd = o->MainWindow;
  WriteBaseObject((TApplication*)o, os);

  os <<mainWnd->Attr.X <<mainWnd->Attr.Y <<mainWnd->Attr.W <<mainWnd->Attr.H;

  if (o->DocMode == dmMDI) {
    os << *o->Client;// stream out object reference only, no children streamed
    os << *o->DocManager;
    os << o->Client->GetActiveMDIChild();
  } else {
    TWindow* client = mainWnd->SetClientWindow(0);  // unhook view
    if (client) {
      client->ShowWindow(SW_HIDE);
      client->SetParent(0);
      os << *o->DocManager;
      os << client;    // save pointer to client window
      mainWnd->SetClientWindow(client);
    }
  }
}

void* MyDVApp::Streamer::Read(ipstream& is, uint32 /*version*/ ) const

{
  MyDVApp* o = GetObject();
  TFrameWindow* mainWnd = o->MainWindow;
  ReadBaseObject((TApplication*)o, is);

  is >>mainWnd->Attr.X >>mainWnd->Attr.Y >>mainWnd->Attr.W >>mainWnd->Attr.H;
  mainWnd->MoveWindow(mainWnd->Attr.X, mainWnd->Attr.Y,
                      mainWnd->Attr.W, mainWnd->Attr.H, true);
  if (o->DocMode == dmMDI) {
    is >> *o->Client;      // skip stream top ref, no data or children read in
    is >> *o->DocManager;  // streams in all templates, docs, views, windows
    o->Client->CreateChildren(); // create MDI children and descendents

    TMDIChild* active;
    is >> active;
    if (active)
      GetObject()->Client->HandleMessage(WM_MDIACTIVATE,(uint)active->GetHandle());

  } else {
    TWindow* client;
    is >> *o->DocManager; // streams in all templates, docs, views, windows
    is >> client;         // streams in pointer to constructed client window
    if (client) {
      mainWnd->SetClientWindow(client);
      TDocument* doc;
      TView* view;
      if ((doc = o->DocManager->DocList.Next(0)) != 0 &&
          (view = doc->NextView(0)) != 0)
        mainWnd->MergeMenu(*view->GetViewMenu());
    }
  }
  return GetObject();
}
#endif  // if !defined(BI_NO_OBJ_STREAMING)

void MyDVApp::CmHideView()
{
  TMDIChild* child = Client->GetActiveMDIChild();
  if (child)
    child->ShowWindow(SW_HIDE);
}

void sUnHide(TWindow* win, void*)
{
  TMDIChild* child = TYPESAFE_DOWNCAST(win, TMDIChild);
  if (child && !child->IsWindowVisible())
    child->ShowWindow(SW_RESTORE);
}

void MyDVApp::CmShowViews()
{
  Client->ForEach(sUnHide);
}

void MyDVApp::CmDisableView()
{
  TMDIChild* child = Client->GetActiveMDIChild();
  if (child)
    child->EnableWindow(false);
}

void sUnDisable(TWindow* win, void*)
{
  TMDIChild* child = TYPESAFE_DOWNCAST(win, TMDIChild);
  if (child && !child->IsWindowEnabled())
    child->EnableWindow(true);
}

void MyDVApp::CmEnableViews()
{
  Client->ForEach(sUnDisable);
}

int OwlMain(int /*argc*/, char* /*argv*/ [])
{
  MyDVApp* app;
  int status;
  int done;
  do {
    try {
      app = new MyDVApp;
      status = app->Run();
      done = 1;
      if (status) {
        char buf[40];
        sprintf(buf, "Run returned %i", status);
        done = HandleGlobalException(xmsg(string(buf)),
                                     "Abnormal Termination","RunAgain?");
      }
	}
	catch (std::exception& x) {
	  done = status = HandleGlobalException(x,
                            "Abnormal Termination, uncaught xmsg","RunAgain?");
    }
    catch(...) {
      done = status = HandleGlobalException(xmsg(string()),
                            "Abnormal Termination, uncaught ...","RunAgain?");
    }
    delete app;
  } while (!done);
  return status;
}
