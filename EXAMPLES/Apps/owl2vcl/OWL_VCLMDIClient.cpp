//----------------------------------------------------------------------------
//  Project OWL_VCL
//  bullock@mrcmry.com
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    OWL_VCL Application
//  FILE:         owl_vclmdiclient.cpp
//  AUTHOR:       Greg Bullock
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TOWL_VCLMDIClient (TMDIClient).
//
//----------------------------------------------------------------------------

#include <owl/pch.h>

#include <owl/docmanag.h>
#include <owl/listbox.h>
#include <stdio.h>

#include "owlusingvclapp.h"
#include "owl_vclmdichild.h"
#include "owl_vclmdiclient.h"
#include "apxprint.h"
#include "apxprev.h"


//{{TOWL_VCLMDIClient Implementation}}


//
// Build a response table for all messages/commands handled
// by TOWL_VCLMDIClient derived from TMDIClient.
//
DEFINE_RESPONSE_TABLE1(TOWL_VCLMDIClient, TMDIClient)
//{{TOWL_VCLMDIClientRSP_TBL_BEGIN}}
  EV_COMMAND(CM_FILEPRINT, CmFilePrint),
  EV_COMMAND(CM_FILEPRINTERSETUP, CmFilePrintSetup),
  EV_COMMAND(CM_FILEPRINTPREVIEW, CmFilePrintPreview),
  EV_COMMAND_ENABLE(CM_FILEPRINT, CmPrintEnable),
  EV_COMMAND_ENABLE(CM_FILEPRINTERSETUP, CmPrintEnable),
  EV_COMMAND_ENABLE(CM_FILEPRINTPREVIEW, CmPrintEnable),
  EV_WM_DROPFILES,
//{{TOWL_VCLMDIClientRSP_TBL_END}}
END_RESPONSE_TABLE;


//--------------------------------------------------------
// TOWL_VCLMDIClient
// ~~~~~~~~~~~
// Construction/Destruction handling.
//
TOWL_VCLMDIClient::TOWL_VCLMDIClient(TModule* module)
:
  TMDIClient(module)
{
  ChildCount = 0;

  // INSERT>> Your constructor code here.

}


TOWL_VCLMDIClient::~TOWL_VCLMDIClient()
{
  Destroy();

  // INSERT>> Your destructor code here.

}


//--------------------------------------------------------
// TOWL_VCLMDIClient
// ~~~~~~~~~~~
// MDIClient site initialization.
//
void TOWL_VCLMDIClient::SetupWindow()
{
  // Default SetUpWindow processing.
  //
  TMDIClient::SetupWindow();

  // Accept files via drag/drop in the client window.
  //
  DragAcceptFiles(true);
}


//--------------------------------------------------------
// TOWL_VCLMDIClient
// ~~~~~~~~~~
// Menu File Print command
//
void TOWL_VCLMDIClient::CmFilePrint()
{
  // Create Printer object if not already created.
  //
  TOWLusingVCLApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TOWLusingVCLApp);
  if (theApp) {
    if (!theApp->Printer)
      theApp->Printer = new TPrinter(theApp);

    TAPointer<char> docName = new char[_MAX_PATH];

    TDocument* currentDoc = theApp->GetDocManager()->GetCurrentDoc();
    if (currentDoc->GetTitle())
      strcpy(docName, currentDoc->GetTitle());
    else
      strcpy(docName, "Document");

    // Create Printout window and set characteristics.
    //
    TApxPrintout printout(theApp->Printer, docName, GetActiveMDIChild()->GetClientWindow(), true);

    theApp->Printing++;

    // Bring up the Print dialog and print the document.
    //
    theApp->Printer->Print(GetWindowPtr(GetActiveWindow()), printout, true);

    theApp->Printing--;
  }
}


//--------------------------------------------------------
// TOWL_VCLMDIClient
// ~~~~~~~~~~
// Menu File Print Setup command
//
void TOWL_VCLMDIClient::CmFilePrintSetup()
{
  TOWLusingVCLApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TOWLusingVCLApp);
  if (theApp) {
    if (!theApp->Printer)
      theApp->Printer = new TPrinter(theApp);

    // Bring up the Print Setup dialog.
    //
    theApp->Printer->Setup(this);
  }
}


//--------------------------------------------------------
// TOWL_VCLMDIClient
// ~~~~~~~~~~
// Menu File Print Preview command
//
void TOWL_VCLMDIClient::CmFilePrintPreview()
{
  TOWLusingVCLApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TOWLusingVCLApp);
  if (theApp) {
    if (!theApp->Printer)
      theApp->Printer = new TPrinter(GetApplication());

    theApp->Printing++;

    TApxPreviewWin* prevW = new TApxPreviewWin(Parent, theApp->Printer, GetActiveMDIChild()->GetClientWindow(), "Print Preview",
      new TLayoutWindow(reinterpret_cast<TWindow *>(0)));
    prevW->Create();

    // Here we resize the preview window to take the size of the MainWindow, then
    // hide the MainWindow.
    //
    TFrameWindow * mainWindow =  GetApplication()->GetMainWindow();
    TRect r = mainWindow->GetWindowRect();
    prevW->MoveWindow(r);
    prevW->ShowWindow(SW_SHOWNORMAL);
    mainWindow->ShowWindow(SW_HIDE);

    GetApplication()->BeginModal(GetApplication()->GetMainWindow());

    // After the user closes the preview Window, we take its size and use it
    // to size the MainWindow, then show the MainWindow again.
    //
    r = prevW->GetWindowRect();
    mainWindow->MoveWindow(r);
    mainWindow->ShowWindow(SW_SHOWNORMAL);

    // We must destroy the preview window explicitly.  Otherwise, the window will
    // not be destroyed until it's parent the MainWindow is destroyed.
    //
    prevW->Destroy();
    delete prevW;

    theApp->Printing--;
  }
}


//--------------------------------------------------------
// TOWL_VCLMDIClient
// ~~~~~~~~~~
// Menu enabler used by Print, Print Setup and Print Preview.
//
void TOWL_VCLMDIClient::CmPrintEnable(TCommandEnabler& tce)
{
  if (GetActiveMDIChild()) {
    TOWLusingVCLApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TOWLusingVCLApp);
    if (theApp) {
      // If we have a Printer already created just test if all is okay.
      // Otherwise create a Printer object and make sure the printer really
      // exists and then delete the Printer object.
      //
      if (!theApp->Printer) {
        theApp->Printer = new TPrinter(theApp);
        tce.Enable(!theApp->Printer->GetSetup().Error);
      }
      else
        tce.Enable(!theApp->Printer->GetSetup().Error);
    }
  }
  else
    tce.Enable(false);
}


void TOWL_VCLMDIClient::EvDropFiles(TDropInfo)
{
  Parent->ForwardMessage();
}
