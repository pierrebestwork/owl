//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owlusingvclapp.cpp
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TOWLusingVCLApp (TApplication).
//
//----------------------------------------------------------------------------

#include <owl/pch.h>

#include <owl/buttonga.h>
#include <owl/statusba.h>
#include <owl/docmanag.h>

//#include <classlib/cmdline.h>
#include <owl/cmdline.h>
//#include <classlib/filename.h>
#include <owl/registry.h>

#include "owlusingvclapp.h"
#include "owl_vclmdiclient.h"
#include "owl_vclmdichild.h"
#include "owlusingvclaboutdlg.h"                        // Definition of about dialog.

// Include the customized doc/view classes (which are wrapped
// around VCL-based classes):
#include "ExOwlDoc.h"
#include "ExOwlView.h"

//
// Generated help file.
//
const char *HelpFileName = "OWL_VCL.hlp";


//{{TOWLusingVCLApp Implementation}}



//{{DOC_VIEW}}
DEFINE_DOC_TEMPLATE_CLASS(TExampleOwlDocument, TExampleOwlView, DocType1);
//{{DOC_VIEW_END}}

//{{DOC_MANAGER}}
DocType1 __dvt1("Text Files", "*.txt", 0, "txt", (dtAutoDelete | dtAutoOpen | dtUpdateDir | dtOverwritePrompt) & ~dtSingleView);
//{{DOC_MANAGER_END}}


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE2(TOWLusingVCLApp, TRecentFiles, TApplication)
//{{TOWLusingVCLAppRSP_TBL_BEGIN}}
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_COMMAND(CM_FILESEND, CmFileSend),
  EV_COMMAND_ENABLE(CM_FILESEND, CeFileSend),
  EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
  EV_COMMAND(CM_HELPCONTENTS, CmHelpContents),
  EV_COMMAND(CM_HELPUSING, CmHelpUsing),
  EV_WM_DROPFILES,
  EV_WM_WININICHANGE,
  EV_OWLDOCUMENT(dnCreate, EvOwlDocument),
  EV_OWLDOCUMENT(dnRename, EvOwlDocument),
  EV_REGISTERED(MruFileMessage, CmFileSelected),
//{{TOWLusingVCLAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
//
TOWLusingVCLApp::TOWLusingVCLApp() : TApplication("OWL_VCL"), TRecentFiles(".\\OWL_VCL.ini", 4)
{
  HelpState = false;
  ContextHelp = false;
  HelpCursor = 0;

  Printer = 0;
  Printing = 0;

  SetDocManager(new TDocManager(dmMDI, this));

  ApxMail = new TMailer();

  // INSERT>> Your constructor code here.
}


TOWLusingVCLApp::~TOWLusingVCLApp()
{
  delete Printer;

  delete ApxMail;

  // INSERT>> Your destructor code here.
}


bool TOWLusingVCLApp::CanClose()
{
  bool result = TApplication::CanClose();

  // Close the help engine if we used it.
  //
  if (result && HelpState)
    GetMainWindow()->WinHelp(HelpFileName, HELP_QUIT, 0);

  return result;
}


void TOWLusingVCLApp::CreateGadgets(TDockableControlBar* cb, bool server)
{
  if (!server) {
    cb->Insert(*new TButtonGadget(CM_MDIFILENEW, CM_MDIFILENEW));
    cb->Insert(*new TButtonGadget(CM_MDIFILEOPEN, CM_MDIFILEOPEN));
    cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
    cb->Insert(*new TSeparatorGadget(6));
  }

  cb->Insert(*new TButtonGadget(CM_EDITCUT, CM_EDITCUT));
  cb->Insert(*new TButtonGadget(CM_EDITCOPY, CM_EDITCOPY));
  cb->Insert(*new TButtonGadget(CM_EDITPASTE, CM_EDITPASTE));
  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(CM_EDITUNDO, CM_EDITUNDO));
  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(CM_EDITFIND, CM_EDITFIND));
  cb->Insert(*new TButtonGadget(CM_EDITFINDNEXT, CM_EDITFINDNEXT));

  if (!server) {
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_FILEPRINT, CM_FILEPRINT));
    cb->Insert(*new TButtonGadget(CM_FILEPRINTPREVIEW, CM_FILEPRINTPREVIEW));
  }

  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(CM_HELPCONTENTS, CM_HELPCONTENTS));

  // Add caption and fly-over help hints.
  //
  cb->SetCaption("Toolbar");
  cb->SetHintMode(TGadgetWindow::EnterHints);
}


void TOWLusingVCLApp::SetupSpeedBar(TDecoratedMDIFrame* frame)
{
  ApxHarbor = new THarbor(*frame);

  // Create default toolbar New and associate toolbar buttons with commands.
  //
  TDockableControlBar* cb = new TDockableControlBar(frame);
  CreateGadgets(cb);

  // Setup the toolbar ID used by OLE 2 for toolbar negotiation.
  //
  cb->Attr.Id = IDW_TOOLBAR;

  ApxHarbor->Insert(*cb, alTop);
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Application main window construction & intialization.
//
void TOWLusingVCLApp::InitMainWindow()
{
  if (nCmdShow != SW_HIDE)
    nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  MdiClient = new TOWL_VCLMDIClient(this);
  TDecoratedMDIFrame* frame = new TDecoratedMDIFrame(Name, IDM_MDI, *MdiClient, true, this);

  // Override the default window style for the main window.
  //
    frame->Attr.Style |= WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
    frame->Attr.Style &= ~(WS_CHILD);

  // Enable acceptance of dropped files
  //
  frame->Attr.ExStyle |= WS_EX_ACCEPTFILES;

  nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  // Assign icons for this application.
  //
  frame->SetIcon(this, IDI_MDIAPPLICATION);
  frame->SetIconSm(this, IDI_MDIAPPLICATION);

  // Associate with the accelerator table.
  //
  frame->Attr.AccelTable = IDM_MDI;

  TStatusBar* sb = new TStatusBar(frame, TGadget::Recessed,
                                  TStatusBar::CapsLock        |
                                  TStatusBar::NumLock         |
                                  TStatusBar::ScrollLock);
  frame->Insert(*sb, TDecoratedFrame::Bottom);

  SetupSpeedBar(frame);

  SetMainWindow(frame);

  frame->SetMenuDescr(TMenuDescr(IDM_MDI));

}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Application instance initialization.
//
void TOWLusingVCLApp::InitInstance()
{
  TApplication::InitInstance();

  ProcessCmdLine(_argv[1]);
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Process command line parameters.
//
void TOWLusingVCLApp::ProcessCmdLine(char * CmdLine)
{
  TCmdLine cmd(CmdLine ? CmdLine : "");

  while (cmd.Kind != TCmdLine::Done) {
    if (cmd.Kind == TCmdLine::Option) {
      if (strnicmp(cmd.Token, "unregister", cmd.TokenLen) == 0) {
        UnRegisterInfo();
        return;
      }
    }
    cmd.NextToken();
  }

  RegisterInfo();
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Register application info.
//
void TOWLusingVCLApp::RegisterInfo()
{
  TAPointer<char> buffer = new char[_MAX_PATH];

  GetModuleFileName(buffer, _MAX_PATH);

  TRegKey(TRegKey::GetClassesRoot(), "OWL_VCL.Application\\DefaultIcon").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
  strcat(buffer, ",1");
  TRegKey(TRegKey::GetClassesRoot(), "OWL_VCL.Document.1\\DefaultIcon").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
  strcpy(buffer, "OWL_VCL.Document.1");
  TRegKey(TRegKey::GetClassesRoot(), ".txt").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Unregister application info.
//
void TOWLusingVCLApp::UnRegisterInfo()
{
  TAPointer<char> buffer = new char[_MAX_PATH];

  GetModuleFileName(buffer, _MAX_PATH);

  TRegKey(TRegKey::GetClassesRoot(), "OWL_VCL.Application").DeleteKey("DefaultIcon");
  TRegKey(TRegKey::GetClassesRoot(), "OWL_VCL.Document.1").DeleteKey("DefaultIcon");

  TRegKey::ClassesRoot().DeleteKey("OWL_VCL.Application");
  TRegKey::ClassesRoot().DeleteKey("OWL_VCL.Document.1");
  TRegKey::ClassesRoot().DeleteKey(".txt");
}



//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Response Table handlers:
//
void TOWLusingVCLApp::EvNewView(TView& view)
{
  TMDIClient* mdiClient = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(), TMDIClient);
  if (mdiClient) {
	bool IsDialog = dynamic_cast<TDialog *>(&view)
	  || dynamic_cast<TVCLDialogView *>(&view)
	  ? true : false;
	TOWL_VCLMDIChild* child = new TOWL_VCLMDIChild(*mdiClient, 0, view.GetWindow(), IsDialog);

    if (IsDialog)
      child->Attr.Style &= ~(WS_THICKFRAME|WS_MAXIMIZEBOX);

    // Set the menu descriptor for this view.
    //
    if (view.GetViewMenu())
      child->SetMenuDescr(*view.GetViewMenu());

    // Assign icons with this child window.
    //
    child->SetIcon(this, IDI_DOC);
    child->SetIconSm(this, IDI_DOC);

    child->Create();
  }
}


void TOWLusingVCLApp::EvCloseView(TView&)
{
}


void TOWLusingVCLApp::CeFileSend(TCommandEnabler& ce)
{
  ce.Enable((GetDocManager()->GetCurrentDoc() != 0)
            && ApxMail->IsMAPIAvailable());
}


void TOWLusingVCLApp::CmFileSend ()
{
  // Check to see if a document exists
  //
  TDocument* currentDoc = GetDocManager()->GetCurrentDoc();

  if (currentDoc) {
    TAPointer<char> savedPath = new char[_MAX_PATH];
    TAPointer<char> docName = new char[_MAX_PATH];

    bool dirtyState = currentDoc->IsDirty();

    if (currentDoc->GetDocPath())
      strcpy(savedPath, currentDoc->GetDocPath());
    else
      strcpy(savedPath, "");

    if (currentDoc->GetTitle())
      strcpy(docName, currentDoc->GetTitle());
    else
      strcpy(docName, "Document");

    TFileName tempFile(TFileName::TempFile);

    currentDoc->SetDocPath(tempFile.Canonical().c_str());
    currentDoc->Commit(true);

    currentDoc->SetDocPath(savedPath);
    currentDoc->SetDirty(dirtyState);

    ApxMail->SendDocuments(GetMainWindow(), tempFile.Canonical().c_str(), docName, false);

    tempFile.Remove();
  }
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Menu Help Contents command
//
void TOWLusingVCLApp::CmHelpContents()
{
  // Show the help table of contents.
  //
  HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_CONTENTS, 0);
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~
// Menu Help Using Help command
//
void TOWLusingVCLApp::CmHelpUsing()
{
  // Display the contents of the Windows help file.
  //
  HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_HELPONHELP, 0);
}


//--------------------------------------------------------
// TOWLusingVCLApp
// ~~~~~~~~~~~
// Menu Help About OWL_VCL command
void TOWLusingVCLApp::CmHelpAbout()
{
  // Show the modal dialog.
  //
  TOWLusingVCLAboutDlg(GetMainWindow()).Execute();
}


void TOWLusingVCLApp::EvDropFiles(TDropInfo drop)
{
  TFileDropletList files;

  // Iterate thru the entries in drop and create FileDrops objects for each
  // one.
  //
  int fileCount = drop.DragQueryFileCount();  // Number of files dropped.
  for (int i = 0; i < fileCount; i++)
    files.Add(new TFileDroplet(drop, i));

  // Open the files that were dropped.
  //
  AddFiles(files);

  // Release the memory allocated for this handle with DragFinish.
  //
  drop.DragFinish();
}


void TOWLusingVCLApp::AddFiles(TFileDropletList& files)
{
  // Open all files dragged in.
  //
  for (TFileDropletListIter fileIter(files); fileIter; fileIter++) {
    TDocTemplate* tpl = GetDocManager()->MatchTemplate(fileIter.Current()->GetName());
    if (tpl)
      GetDocManager()->CreateDoc(tpl, fileIter.Current()->GetName());
  }
}


void TOWLusingVCLApp::EvOwlDocument(TDocument& doc)
{
  if (doc.GetDocPath())
    SaveMenuChoice(doc.GetDocPath());
}


int32 TOWLusingVCLApp::CmFileSelected(uint wp, int32)
{
  TAPointer<char> text = new char[_MAX_PATH];

  GetMenuText(wp, text, _MAX_PATH);
  TDocTemplate* tpl = GetDocManager()->MatchTemplate(text);
  if (tpl)
    GetDocManager()->CreateDoc(tpl, text);
  return 0;
}



//
// Process application messages to provide context sensitive help
//
bool TOWLusingVCLApp::ProcessAppMsg(MSG& msg)
{
  if (msg.message == WM_COMMAND) {
    if (ContextHelp || ::GetKeyState(VK_F1) < 0) {
      ContextHelp = false;
      GetMainWindow()->WinHelp(HelpFileName, HELP_CONTEXT, msg.wParam);
      return true;
    }
  }
  else
    switch(msg.message) {
    case WM_KEYDOWN:
      if (msg.wParam == VK_F1) {
        // If the Shift/F1 then set the help cursor and turn on the modal help state.
        //
        if (::GetKeyState(VK_SHIFT) < 0) {
          ContextHelp = true;
          HelpCursor = new TCursor(GetMainWindow()->GetModule()->GetHandle(), TResId(IDC_HELPCURSOR));
          ::SetCursor(*HelpCursor);
          return true;        // Gobble up the message.
        }
        else {
          // If F1 w/o the Shift key then bring up help's main index.
          //
          GetMainWindow()->WinHelp(HelpFileName, HELP_INDEX, 0);
          return true;        // Gobble up the message.
        }
      }
      else {
        if (ContextHelp && msg.wParam == VK_ESCAPE) {
          if (HelpCursor)
            delete HelpCursor;
          HelpCursor = 0;
          ContextHelp = false;
          GetMainWindow()->SetCursor(0, IDC_ARROW);
          return true;    // Gobble up the message.
        }
      }
      break;

    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
      if (ContextHelp) {
        ::SetCursor(*HelpCursor);
        return true;        // Gobble up the message.
      }
      break;

    case WM_INITMENU:
      if (ContextHelp) {
        ::SetCursor(*HelpCursor);
        return true;    // Gobble up the message.
      }
      break;
    case WM_ENTERIDLE:
      if (msg.wParam == MSGF_MENU)
        if (GetKeyState(VK_F1) < 0) {
          ContextHelp = true;
          GetMainWindow()->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
          return true;     // Gobble up the message.
        }
      break;
    default:
      ;
    }  // End of message switch

  // Continue normal processing.
  //
  return TApplication::ProcessAppMsg(msg);
}


void TOWLusingVCLApp::EvWinIniChange(char far* section)
{
  if (strcmp(section, "windows") == 0) {
    // If the device changed in the WIN.INI file then the printer
    // might have changed.  If we have a TPrinter(Printer) then
    // check and make sure it's identical to the current device
    // entry in WIN.INI.
    //
    if (Printer) {
      const int bufferSize = 255;
      char printDBuffer[bufferSize];
      LPSTR printDevice = printDBuffer;
      LPSTR devName;
      LPSTR driverName = 0;
      LPSTR outputName = 0;
      if (::GetProfileString("windows", "device", "", printDevice, bufferSize)) {
        // The string which should come back is something like:
        //
        //      HP LaserJet III,hppcl5a,LPT1:
        //
        // Where the format is:
        //
        //      devName,driverName,outputName
        //
        devName = printDevice;
        while (*printDevice) {
          if (*printDevice == ',') {
            *printDevice++ = 0;
            if (!driverName)
              driverName = printDevice;
            else
              outputName = printDevice;
          }
          else
            printDevice = ::AnsiNext(printDevice);
        }

        if (Printer->GetSetup().Error != 0                ||
            strcmp(devName, Printer->GetSetup().GetDeviceName()) != 0  ||
            strcmp(driverName, Printer->GetSetup().GetDriverName()) != 0 ||
            strcmp(outputName, Printer->GetSetup().GetOutputName()) != 0 ) {
          // New printer installed so get the new printer device now.
          //
          delete Printer;
          Printer = new TPrinter(this);
        }
      }
      else {
        // No printer installed(GetProfileString failed).
        //
        delete Printer;
        Printer = new TPrinter(this);
      }
    }
  }
}


int OwlMain(int , char* [])
{
  TOWLusingVCLApp   app;
  return app.Run();
}
