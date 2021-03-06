//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1993, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.28.2.1 $                                                             
//$Author: jogybl $                                                            
//$Date: 2009-11-23 08:45:18 $                                                    
//
// Implementation of class TDocManager
//----------------------------------------------------------------------------
#if defined(__BORLANDC__) 
#pragma hdrignore SECTION
#endif
#include <owl/pch.h>

#if !defined(OWL_DOCMANAG_H)
# include <owl/docmanag.h>
#endif
#if !defined(OWL_APPDICT_H)
# include <owl/appdict.h>
#endif
#if !defined(OWL_OPENSAVE_H)
# include <owl/opensave.h>
#endif
#if !defined(OWL_PICKLIST_H)
# include <owl/picklist.h>
#endif
#if !defined(OWL_SYSTEM_H)
# include <owl/system.h>
#endif
#if !defined(OWL_FILENAME_H)
# include <owl/filename.h>
#endif
#if !defined(OWL_DOCVIEW_RH)
# include <owl/docview.rh>
#endif
#include <string.h>
#include <stdio.h>
#include <dlgs.h>

__OWL_BEGIN_NAMESPACE

//
// (Readable versions of) IDs of controls used by CommonDialogs
//
#define IDC_FILENAMETXT       stc3
#define IDC_FILENAME          edt1
#define IDC_FILELIST          lst1
#define IDC_CURDIR            stc1
#define IDC_DIRLIST           lst2
#define IDC_FILETYPEEXT       stc2
#define IDC_FILETYPES         cmb1
#define IDC_DRIVESTEXT        stc4
#define IDC_DRIVES            cmb2
#define IDC_HELPF             pshHelp
#define IDC_READONLY          chx1

typedef TDocTemplate*         TDocTemplatePtr;

OWL_DIAGINFO;
#if defined(SECTION) && SECTION != 1
DIAG_DECLARE_GROUP(OwlDocView);        // General Doc/View diagnostic group
#endif

#if !defined(SECTION) || SECTION == 1

DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlDocView, 1, 0);

//---------------------------------------------------------------------------
// class TDvOpenSaveData
//

//
// Constructor of class representing the data to be displayed in the
// Open/Save dialogs used by the DocManager.
//
TDvOpenSaveData::TDvOpenSaveData(uint32 flags, LPTSTR filter,
                                 LPTSTR initialDir,
                                 LPTSTR defExt,
                                 int filterIndex, TDocTemplate **tmplList,
                                 int tmplCount)
:
 TOpenSaveDialog::TData(flags, filter, 0/*customFilter*/,
                        initialDir, defExt, 0/*maxPath*/, filterIndex),
 TmplCount(tmplCount),
 TmplList(tmplList)
{
}

//
// Constructor of mixin. class used by the DocManager's version of the
// Open/Save dialogs.
//
TDvOpenSaveDataOwner::TDvOpenSaveDataOwner(TDvOpenSaveData& data)
:
  DvData(data)
{
}

//
//'HandleTemplateChanged' is invoked whenever the user switches the
// file type in the FileOpen or FileSave dialog. The function updates
// the internal OPENFILENAME structure based on the properties of the
// new template selected.
//
static void
handleTemplateChanged(TDialog& dialog,
                      TDvOpenSaveDataOwner& dataOwner,
                      OPENFILENAME& ofn)
{
  int index = (int)dialog.SendDlgItemMsg(IDC_FILETYPES, CB_GETCURSEL, 0, 0);
  if (index >= 0) {
    CHECK(index < dataOwner.GetTmplCount());
    TDocTemplate* tpl = dataOwner.GetTmplList()[index];

    // Update default extension in OFN structure
    //
    CHECK(ofn.lpstrDefExt);
    ofn.lpstrDefExt = tpl->GetDefaultExt();
  }
}

//---------------------------------------------------------------------------
// class TDvFileOpenDialog
//

//
// Constructor of object encapsulating the Open File dialog used by the
// DocManager.
//
TDvFileOpenDialog::TDvFileOpenDialog(TWindow* parent, TDvOpenSaveData& data,
                                     LPCTSTR title)
:
  TFileOpenDialog(parent, data, 0/*templateId*/, title),
  TDvOpenSaveDataOwner(data)
{
}

//
// Overriden virtual function of TFileOpenDialog to allow us to detect when
// the user switches between file types [i.e. DocView templates].
//
bool
TDvFileOpenDialog::DialogFunction(uint message, TParam1 p1, TParam2 p2)
{
  if (message == WM_COMMAND && LOWORD(p1) == IDC_FILETYPES &&
                               HIWORD(p1) == CBN_SELCHANGE) {
    handleTemplateChanged(*this, *this, GetOFN());
  }

  return TFileOpenDialog::DialogFunction(message, p1, p2);
}

//---------------------------------------------------------------------------
// class TDvFileSaveDialog
//

//
// Constructor of object encapsulating the FileSave dialog used by the
// DocManager.
//
TDvFileSaveDialog::TDvFileSaveDialog(TWindow* parent, TDvOpenSaveData& data,
                                     LPCTSTR title)
:
  TFileSaveDialog(parent, data, 0/*templateId*/, title),
  TDvOpenSaveDataOwner(data)
{
}

//
// Overriden virtual function of TFileOpenDialog to allow us to detect when
// the user switches between file types [i.e. DocView templates].
//
bool
TDvFileSaveDialog::DialogFunction(uint message, TParam1 p1, TParam2 p2)
{
  if (message == WM_COMMAND && LOWORD(p1) == IDC_FILETYPES &&
                               HIWORD(p1) == CBN_SELCHANGE) {
    handleTemplateChanged(*this, *this, GetOFN());
  }

  return TFileSaveDialog::DialogFunction(message, p1, p2);
}

//----------------------------------------------------------------------------
// class TDocManager
//

DEFINE_RESPONSE_TABLE (TDocManager)
  EV_WM_CANCLOSE,
  EV_WM_PREPROCMENU,
  EV_WM_WAKEUP,
  EV_COMMAND(CM_FILENEW,           CmFileNew),
  EV_COMMAND(CM_FILEOPEN,          CmFileOpen),
  EV_COMMAND(CM_FILESAVE,          CmFileSave),
  EV_COMMAND(CM_FILESAVEAS,        CmFileSaveAs),
  EV_COMMAND(CM_FILEREVERT,        CmFileRevert),
  EV_COMMAND(CM_FILECLOSE,         CmFileClose),
  EV_COMMAND(CM_VIEWCREATE,        CmViewCreate),
  EV_COMMAND_ENABLE(CM_FILENEW,    CeFileNew),
  EV_COMMAND_ENABLE(CM_FILEOPEN,   CeFileOpen),
  EV_COMMAND_ENABLE(CM_FILESAVE,   CeFileSave),
  EV_COMMAND_ENABLE(CM_FILESAVEAS, CeFileSaveAs),
  EV_COMMAND_ENABLE(CM_FILEREVERT, CeFileRevert),
  EV_COMMAND_ENABLE(CM_FILECLOSE,  CeFileClose),
  EV_COMMAND_ENABLE(CM_VIEWCREATE, CeViewCreate),
END_RESPONSE_TABLE;

//
// Constructor of DocManager
//
TDocManager::TDocManager(int mode, TApplication* app, TDocTemplate*& templateHead)
{
  Init(mode, app, templateHead);
}

//
// Common initialization for TDocManager constructors (above constructor, &
// compatable constructor below)
//
void
TDocManager::Init(int mode, TApplication* app, TDocTemplate*& templateHead)
{
  Mode = mode;
  TemplateList = templateHead;
  TemplateHead = &templateHead;
  UntitledIndex= 0;
  AdditionalFileDialogFlags = 0;

  PRECONDITION(app);
  Application = app;

  // Update docmanger pointer members of static templates
  for (TDocTemplate* tpl = TemplateList; tpl; tpl = tpl->GetNextTemplate())
    tpl->DocManager = this; 

#if defined(OWL2_COMPAT)

  // Setup backward compatibility entry points
  //
  TDocTemplate::SelectSave_ = SelectSave;
  TDocTemplate::InitDoc_    = InitDoc;
  TDocTemplate::InitView_   = InitView;
#endif
}

//
// Destruct this DocManager. Close all open documents (views close with them),
// and delete all non-static doc templates
//
TDocManager::~TDocManager()
{
  // Iterate through document list, closing and deleting each
  //
  TDocument* doc;
  while ((doc = DocList.Next(0)) != 0) {
    if (doc->IsOpen())
      doc->Close();

    // NOTE: deleting the document deletes all attached views, and unlinks
    //       the document from the docmanager's document list
    //
    delete doc;

    // Flush (dispatch) any pending MDI-Child-destroy messages
    //
    GetApplication()->PumpWaitingMessages();
  }

  // Reset the 'Docmanager' pointer of static templates and delete
  // dynamic ones...
  //
  while (TemplateList) {
    TDocTemplate* tpl = TemplateList;
    TemplateList = tpl->GetNextTemplate();
    if (tpl->IsStatic())
      tpl->SetDocManager(0);
    else
      delete tpl;
  }
}

//----------------------------------------------------------------------------

//
// Count the number of templates, filtering by document type and/or view
// type as indicated.
//
int
TDocManager::GetTemplateCount(TDocument* ofDocType, TView* ofViewType)
{
  // No registered templates!!
  //
  if (!TemplateList) {
    TRACEX(OwlDocView, 0, _T("GetTemplateCount(): No registered templates!"));
    return 0;
  }

  // Walk through template list matching doc/view types as indicated
  //
  int tplCount = 0;
  for (TDocTemplate* tpl = TemplateList; tpl; tpl = tpl->GetNextTemplate()) {
    if (ofDocType && !tpl->IsMyKindOfDoc(*ofDocType))
      continue;

    if (ofViewType && !tpl->IsMyKindOfView(*ofViewType))
      continue;

    tplCount++;
  }
  return tplCount;
}

//
// Build a list of templates for creating new documents or opening existing
// documents. Return the number of templates in the list. Passing 0 for
// tplList skips the actual creation of the list, useful for pre-counting
//
#if __DEBUG < 2
int
TDocManager::GetNewTemplates(TDocTemplate** tplList, int, bool newDoc)
#else
int
TDocManager::GetNewTemplates(TDocTemplate** tplList, int size, bool newDoc)
#endif
{
  // Check for no registered templates
  //
  if (!TemplateList) {
    TRACEX(OwlDocView, 0, _T("GetNewTemplates(): No registered templates!"));
    return 0;
  }

  // Walk thru all of the templates looking for visible ones, and if a new
  // doc, non-dtReadOnly and non-dtForbidNew ones.
  //
  int tplCount = 0;
  for (TDocTemplate* tpl = TemplateList; tpl; tpl = tpl->GetNextTemplate()) {
    if (tpl->IsVisible() && 
        !((tpl->IsFlagSet(dtReadOnly) || tpl->IsFlagSet(dtForbidNew)) 
           && newDoc)){
      if (tplList) {
        CHECK(tplCount < size);
        tplList[tplCount] = tpl;
      }
      tplCount++;
    }
  }
  return tplCount;
}

//
// Build a list of templates for saving documents. Return the number of
// templates in the list. Passing 0 for tplList skips the actual creation of
// the list, useful for pre-counting
//
#if __DEBUG < 2
int
TDocManager::GetSaveTemplates(TDocTemplate** tplList, int,
                              TDocument& doc, bool sameDoc)
#else
int
TDocManager::GetSaveTemplates(TDocTemplate** tplList, int size,
                              TDocument& doc, bool sameDoc)
#endif
{
  // Check for no registered templates
  //
  if (!TemplateList) {
    TRACEX(OwlDocView, 0, _T("GetSaveTemplates(): No registered templates!"));
    return 0;
  }

  // Walk thru all of the templates looking for visible, non-dtReadOnly ones,
  // and if the same doc, ones that match the document
  //
  int tplCount = 0;
  for (TDocTemplate* tpl = TemplateList; tpl; tpl = tpl->GetNextTemplate()) {
    if (tpl->IsVisible() && !tpl->IsFlagSet(dtReadOnly)
                         && (!sameDoc || tpl->IsMyKindOfDoc(doc))) {
      if (tplList) {
        CHECK(tplCount < size);
        tplList[tplCount] = tpl;
      }
      tplCount++;
    }
  }
  return tplCount;
}

//
// Build a list of templates for creating views. Return the number of
// templates in the list.
// NOTE: To find out ahead of time how many templates match a particular
//       document type, use 'GetTemplateCount(docType);'
//
#if __DEBUG < 2
int
TDocManager::GetViewTemplates(TDocTemplate** tplList, int,
                              TDocument& doc)
#else
int
TDocManager::GetViewTemplates(TDocTemplate** tplList, int size,
                              TDocument& doc)
#endif
{
  // Check for no registered templates
  //
  if (!TemplateList) {
    TRACEX(OwlDocView, 0, _T("GetViewTemplates(): No registered templates!"));
    return 0;
  }

  // Grab a list of templates for creating views
  //
  int tplCount = 0;
  for (TDocTemplate* tpl = TemplateList; tpl; tpl = tpl->GetNextTemplate()) {
    if (tpl->IsMyKindOfDoc(doc)) {

      // Don't grab the same view more than once
      //
      LPCTSTR viewName = tpl->GetViewName();
      int index;
      for (index = 0; index < tplCount; index++) {
        if (tplList[index]->GetViewName() == viewName)
          break;
      }

      // Skip a view if the document already has one and the template
      // specifies 'SingleView'.
      //
      if (tpl->IsFlagSet(dtSingleView)) {
        TView* pview = 0;
        while ((pview = doc.NextView(pview)) != 0)
          if (tpl->IsMyKindOfView(*pview))
            index = -1;
      }

      // Store the template if we have a match...
      //
      if (index == tplCount) {

        CHECK(tplList);
        CHECK(tplCount < size);

        tplList[tplCount++] = tpl;
      }
    }
  }
  return tplCount;
}

//
// Build up a long string containing the template descriptions and file filters
// for every template in the given list. Return the length of the buffer
// needed. Passing 0 for buffer skips actual buffer writing. String will be
// in the form:
//
//   "<descr1> (<filter1>)|(<filter1>)|<descr2> (<filter2>)|(<filter2>)|"
//
#if __DEBUG < 2
int
TDocManager::GetTemplateDescription(TDocTemplate** tpllist, int tplcount,
                                    LPTSTR buffer, int)
#else
int
TDocManager::GetTemplateDescription(TDocTemplate** tpllist, int tplcount,
                                    LPTSTR buffer, int size)
#endif
{

  int len = 0;

  // If buffer is 0, then simply compute the length the template description(s)
  // and filter(s)
  //
  if (!buffer) {
    for (int count=0; count < tplcount; count++) {
      TDocTemplate* tpl = tpllist[count];
      CHECK(tpl);

      LPCTSTR p = tpl->GetDescription();
      int descLen = (p && *p) ? ::_tcslen(p) : 0;

      p = tpl->GetFileFilter();
      int fltrLen = (p && *p) ? ::_tcslen(p) : 0;

      // Description + " (" + filter  + ") " + filter  +"|"
      //
      len += (descLen  +  2 + fltrLen +  2   + fltrLen + 1);
    }
  }
  else {

    // Check that buffer is big enough
    //
    CHECK(GetTemplateDescription(tpllist, tplcount) < size);

    // Put together a string of description and filters
    //
    buffer[0] = 0;
    for (int count = 0; count < tplcount; count++ ) {
      TDocTemplate* tpl = tpllist[count];
      CHECK(tpl);

      LPCTSTR p = tpl->GetDescription();
      if (p && *p)
        _tcscat(buffer, p);

      p = tpl->GetFileFilter();
      if (p && *p) {
        _tcscat(buffer, _T(" ("));
        _tcscat(buffer, p);
        _tcscat(buffer, _T(")|"));
        _tcscat(buffer, p);
      }
      _tcscat(buffer, _T("|"));
    }
    len = ::_tcslen(buffer);
  }

  return len;
}

//----------------------------------------------------------------------------

//
// Create any kind of document, using the supplied path. Prompt user to select
// from the list of non-hidden doc templates.
//
TDocument*
TDocManager::CreateAnyDoc(LPCTSTR path, long flags)
{
  // Are we creating a new document?
  //
  bool  newDoc = (flags & dtNewDoc) ? true : false;

  // Get a count and list of templates
  //
  int tplcount = GetNewTemplates(0, 0, newDoc);

  // Return of there are no usable templates
  //
  if (!tplcount) {
    TRACEX(OwlDocView, 0, _T("No usable templates found in CreateAnyDoc()"));
    return 0;
  }

  // Allocate memory and get templates
  //
  TDocTemplatePtr* tpllist = new TDocTemplatePtr[tplcount];
  TAPointer<TDocTemplatePtr> _clnObj(tpllist);
  GetNewTemplates(tpllist, tplcount, newDoc);

  TCHAR filepath[_MAX_PATH];
  filepath[0] = 0;

  // Find the index of the template to be used
  //
  int index;
  if (newDoc) {

    // If there's only one template, use it - otherwise prompt user
    // to select the document type
    //
    if (tplcount == 1)
      index = 1;
    else
      index = SelectDocType(tpllist, tplcount);
  }
  else {

    // Select the doctemplate using the doc template filters
    //!CQ might be nice to skip UI if path is already known, and maybe only
    //!CQ if there is just one template
    //
    if (path)
      ::_tcscpy(filepath, path);
    else
      filepath[0] = 0;

    index = SelectDocPath(tpllist, tplcount, filepath, COUNTOF(filepath), flags);

    WARNX(OwlDocView, index > tplcount, 0,
          _T("Invalid template index from SelectDocPath"));
  }

  // No index implies user cancelled or error
  //
  if (!index)
    return 0;

  // If opening a document, check that document is not already opened
  //
  if (filepath[0]) {
    TDocument* doc = FindDocument(filepath);
    if (doc != 0) {
      PostDocError(*doc, IDS_DUPLICATEDOC);
      return 0;
    }
  }

  // We have a template, now create the document & return it if successful
  //
  return CreateDoc(tpllist[index-1], filepath, 0, flags);
}

//
// Create a document using a specific doc template, then if successful,
// initialize the document using InitDoc()
//
TDocument*
TDocManager::CreateDoc(TDocTemplate* tpl, LPCTSTR path,
                       TDocument* parent, long flags)
{
  if (!tpl) {
    TRACEX(OwlDocView, 0, _T("CreateDoc(): NULL template specified!"));
    return 0;
  }

  // Creation step 0: Inform docmanager that we're about to create a document
  // and allow docmanager to veto
  //
  if (!CreatingDoc(tpl)) {
    TRACEX(OwlDocView, 1, _T("CreateDoc(): Creation vetoed."));
    return 0;
  }

  // Creation step 1: Construct the document, passing in the parent document
  // Put together a dummy parent document if no parent doc was specified in
  // order to allow us to pass in the DocManager pointer hidden in the parent
  // doc
  //
  TDocument* doc;
  if (!parent){
    TDocument td(this);
     doc = tpl->ConstructDoc(&td);
  }
  else
    doc = tpl->ConstructDoc(parent);

  if (!doc) {
    TRACEX(OwlDocView, 0, _T("CreateDoc(): ConstructDoc call failed"));
    return 0;
  }

  // Creation step2: Initialize the document
  //
  doc->SetTemplate(tpl);
  return InitDoc(doc, path, flags);
}

//
// Initialize document using its specific doc template
// Prompts for pathname using SelectDocPath() if no path is supplied and not
// creating a new document
//
// NOTE: Will delete the passed document if unable to properly initialized doc
//       or if unable to create the view & dtAutoDelete is set.
//
TDocument*
TDocManager::InitDoc(TDocument* doc, LPCTSTR path, long flags)
{
  PRECONDITION(doc);

  TDocTemplate* tpl = doc->GetTemplate();
  if (!tpl) {
    TRACEX(OwlDocView, 0, _T("TDocManager::InitDoc(), doc has not template"));
    return 0;
  }

  // Alter flags with template's flags
  //
  flags ^= tpl->GetFlags();

  // Initialize new document
  //
  if (flags & dtNewDoc) {
    doc->SetDocPath(0);

    // Allow document to initialize and cleanup if unsuccessful
    //
    if (!doc->InitDoc()){
      PostDocError(*doc, IDS_UNABLEOPEN);
      delete doc;
      return 0;
    }
  }
  else {
    if (!path) {

      if (!tpl->GetFileFilter())
        return 0;

      // Prompt user for document's path
      //
      TCHAR filepath[_MAX_PATH];
      filepath[0] = 0;
      int index = SelectDocPath(&tpl, 1, filepath, COUNTOF(filepath), flags);

      // if user cancelled UI, cleanup document
      //
      if (!index) {
        delete doc;
        return 0;
      }

      path = filepath;
    }

    // Update document's path
    //
    doc->SetDocPath(path);

    // Allow document to initialize and cleanup if unsuccessful
    //
    if (!doc->InitDoc()) {
      PostDocError(*doc, IDS_UNABLEOPEN);
      doc->SetDocPath(0); // fix by suggested Greg Chicares 06/04/98
      delete doc;
      return 0;
    }

    // Attempt to open document if 'dtAutoOpen' is enabled. Cleanup if
    // unsuccessful.
    //
    if (flags & dtAutoOpen) {
      if (!doc->Open((flags & dtNoReadOnly) ? ofReadWrite : ofRead, path)) {
        PostDocError(*doc, IDS_UNABLEOPEN);
        doc->SetDocPath(0); // fix by suggested Greg Chicares 06/04/98
        delete doc;
        return 0;
      }
    }
  }

  // Create a view unless 'dtNoAutoView' flag is set. Cleanup on failure if
  // dtAutoDelete is set on behalf of the view.
  //
  if (!(flags & dtNoAutoView)) {
    if (!CreateView(*doc)) {
      if (flags & dtAutoDelete) {
        if (doc->IsOpen())
          doc->Close();
        delete doc;
      }
      return 0;
    }
  }

  // Post event that document has been created
  //
  PostEvent(dnCreate, *doc);

  return doc;
}

//
// Create (Construct and Init) a view from registered templates supporting
// a given doc.
//
// This implementation allows user intervention if there is more than one
// template available, by calling SelectViewType()
//
TView*
TDocManager::CreateAnyView(TDocument& doc, long /*flags*/)
{
  int tplCount = GetTemplateCount(&doc);

  // Abort if there are no usable templates
  //
  if (!tplCount) {
    WARNX(OwlDocView, !tplCount, 0, _T("CreateAnyView(): no usable template"));
    return 0;
  }

  // Allocate memory for templates and get 'em
  //
  TDocTemplatePtr* tplList = new TDocTemplatePtr[tplCount];
  TAPointer<TDocTemplatePtr> _clnObj(tplList);
  tplCount = GetViewTemplates(tplList, tplCount, doc);

  int index;
  if (tplCount > 1)
    index = SelectViewType(tplList, tplCount);
  else
    index = tplCount;

  if (index <= 0) {
    WARNX(OwlDocView, !tplCount, 0, _T("CreateAnyView(): no usable template"));
    WARNX(OwlDocView, tplCount,  0, _T("CreateAnyView(): invalid template index"));
    return 0;
  }

  CHECK(index > 0);
  //CHECK(index < tplCount);//?????????
  CHECK(index <= tplCount);


  // Now create the view on the document using the selected template
  //
  return CreateView(doc, tplList[index-1]);
}

//
// Create a view on a given document using the given tempalte, or document's
// template if none given, and then initialize the view. Return the view if
// all was OK, or 0 if something failed.
//
TView*
TDocManager::CreateView(TDocument& doc, TDocTemplate* tpl)
{
  // Default to the document's template if none provided
  //
  if (!tpl)
    tpl = doc.GetTemplate();
  CHECK(tpl);

  TView* view = tpl->ConstructView(doc);
  WARNX(OwlDocView, !view, 0, _T("CreateView(): ConstructView call failed"));

  view = doc.InitView(view);
  WARNX(OwlDocView, !view, 0, _T("CreateView(): InitView call failed"));

  return view;
}

//----------------------------------------------------------------------------

//
// Displays FileSave dialog prompting for a filename for saving a document.
//
bool
TDocManager::SelectSave(TDocument& doc)
{
  TDocTemplate* tpl = doc.GetTemplate();

  if (!tpl || !tpl->GetFileFilter())
    return false;

  TCHAR filepath[_MAX_PATH];
  if (doc.GetDocPath())
    ::_tcscpy(filepath, doc.GetDocPath());
  else
    filepath[0] = 0;    // no initial file path

  int index = SelectDocPath(&tpl, 1, filepath, COUNTOF(filepath), 0, true, &doc);
  return index ? doc.SetDocPath(filepath) : false;
}

//
// Select a new docpath for a document from the registered templates
// supporting the doc.
//
TDocTemplate*
TDocManager::SelectAnySave(TDocument& doc, bool samedoc)
{
  int tplcount = GetSaveTemplates(0, 0, doc, samedoc);

  // Abort if there are no usable templates
  //
  if (!tplcount) {
    WARNX(OwlDocView, !tplcount, 0, _T("SelectAnySave(): no usable templates"));
    return 0;
  }

  // Allocate memory for templates and get 'em
  //
  TDocTemplatePtr* tpllist = new TDocTemplatePtr[tplcount];
  TAPointer<TDocTemplatePtr> _clnObj(tpllist);
  GetSaveTemplates(tpllist, tplcount, doc, samedoc);

  // Grab the file name, if there's one
  //
  TCHAR filepath[_MAX_PATH];
  if (doc.GetDocPath())
    ::_tcscpy(filepath, doc.GetDocPath());
  else
    filepath[0] = 0;

  // Allow UI for user input
  //
  int index;
  index = SelectDocPath(tpllist, tplcount, filepath, COUNTOF(filepath), 0, true, &doc);
  if (!index)
    return 0;

  CHECK(index >= 1);
  CHECK(index <= tplcount);

  // Perform save by setting the new path into the doc
  //!CQ Also notify doc of save somehow?
  //
  if (!doc.SetDocPath(filepath))
    return 0;

  return tpllist[index-1];
}

//
// Method invoked when specified document is about to be closed.
// Returns 'true' if DocManager should proceed with the closing
// stages of the document, or 'false' otherwise.
//
bool
TDocManager::FlushDoc(TDocument& doc)
{
  while (doc.IsDirty()) {
    int saveOrNot = doc.IsEmbedded() ?
                      IDYES :
                      PostDocError(doc, IDS_DOCCHANGED, MB_YESNOCANCEL);

    switch (saveOrNot) {
      case IDYES:
        // Prompt the user for filename in save-as situation
        //
        if (!doc.IsEmbedded() && doc.GetDocPath() == 0) {

        //!BB
        //!BB It does not make sense to invoke SelectAnySave
        //!BB with false here... This would allow the user
        //!BB to switch to any available template when saving the
        //!BB document. In other words, a user would be allowed to
        //!BB save a .TXT file as a .PTS file although they are
        //!BB not related whatsoever...
        //!BB
        //!BB I'm switching this to use true - let me know if you
        //!BB know of a reason for the prior behaviour.
        //!BB

#if defined(OLD_DOCVIEW)
          TDocTemplate* tpl = SelectAnySave(doc, false);
#else
          TDocTemplate* tpl = SelectAnySave(doc, true);
#endif
          if (!tpl)
            continue;

          //!BB
          //!BB The following is suspicious: Is there a reason
          //!BB to allow the user to switch the template in the first
          //!BB place?? OK, if everyone agrees that same TDocument-derived
          //!BB type implies compatible document, that would be OK.
          //!BB However, that's not what we've encouraged. Our own
          //!BB examples use the same TFileDocument for incompatible
          //!BB document types. Hence, if an app. has a
          //!BB TBitmapView/TFileDocument and a TTextView/TFileDocument pair,
          //!BB the following would allow the user to save a text file as a .BMP
          //!BB Ack!!
          //!BB If the following is really the intent, then DV users must be
          //!BB conscious that they will more often than not be using TFileDocument-
          //!BB derived documents as a method to specify compatible and incompatible
          //!BB document types.
          //
          if (tpl != doc.Template)
            doc.SetTemplate(tpl);
        }
        if (doc.Commit())
          return true;
        continue;

      case IDNO:
        if (doc.Revert(true))
          return true;
        return false;

      case IDCANCEL:
        return false;
    }
  }
  return true;
}

//----------------------------------------------------------------------------

//
// Return a matching template given a document path.
//
// This implementation compares the path's extension with the ';' separated
// filter lists in each template looking for a case-insensitive match. Allows
// '?'s in the template filters to match any character in the path's extension
//
// Example:
//   The path: "documents.1995\accounts_receivable.plan"
//   Would match the filter: "*.xls;*.wk?;*.plan"
//
TDocTemplate*
TDocManager::MatchTemplate(LPCTSTR path)
{

  //!BB if (FindDocument(path))
  //!BB  return 0;

  // Iterate through templates
  //
  for (TDocTemplate* tpl = TemplateList; tpl != 0; tpl = tpl->GetNextTemplate()) {

    // Skip hidden templates or templates without filters specified
    //
    LPCTSTR tplFilter = tpl->GetFileFilter();
    if (!tplFilter || tpl->IsFlagSet(dtHidden))
      continue;

    // Make locale copy of filter
    //
    TAPointer<TCHAR> fltrCopy = nstrnewdup(tplFilter);
    LPTSTR fltr = fltrCopy;
    LPTSTR nxtFltr;

    // For each template, try each wilcard specified
    //
    while (fltr) {

      // Is there another wildcard following - Null terminate at ';'
      //
      nxtFltr = _tcschr(fltr, _T(';'));
      if (nxtFltr)
        *nxtFltr++ = 0;

      if (IsAMatch(path, fltr))
        return tpl;

      // Try next filter
      //
      fltr = (nxtFltr && *nxtFltr) ? nxtFltr : 0;
    }
  }
  return 0;
}

//
// Internal routine used to match a portion of a filename (i.e. name or
// extension) to a wilcard pattern.
//
static bool
nameMatch(LPCTSTR name, LPCTSTR wc)
{
  while (true) {
    //if (*wc == _T('*') || (!*wc && !*name))
    // Greg Bullock suggestion
    if ((*wc == _T('*') && (wc[1] == _T('\0'))) || (!*wc && !*name)) 
      return true;
    if (*wc == _T('*')) {
      // The wildcard string begins with '*' but ends with something else (e.g., "*~")
      // We'll need to verify that the ending of name matches the wildcard string.
      wc++;
      size_t namelen = _tcslen(name);
      size_t wclen = _tcslen(wc);
      if (namelen > wclen)
        name += namelen - wclen;
    }
#if defined(BI_DBCS_SUPPORT)
    int n = CharSize(name);
    if (*wc == _T('?') && *name) {
      wc++;
      name += n;
      continue;
    }
    else if (n > 1) { // if double byte character
      if (_tcsncmp(name, wc, n) == 0) {
        wc += n;
        name += n;
        continue;
      }
    }
    else if (_totupper(*name) == _totupper(*wc)) {
#else
    if ((*wc == _T('?') && *name) || (_totupper(*name) == _totupper(*wc))) {
#endif
      wc++;
      name++;
      continue;
    }
    break;
  }
  return false;
}

//
// Helper method used by 'MatchTemplate' to determine whether a filename
// fits the pattern [wilcard] described by a filter.
//
bool
TDocManager::IsAMatch(LPCTSTR path, LPCTSTR fltr)
{
  PRECONDITION(path);
  PRECONDITION(fltr);

  // Ensure path has at least a valid file name
  //
  TFileName fname(path);
  if (!fname.HasParts(TFileName::File)) {
    TRACEX(OwlDocView, 0, _T("IsAMatch: Invoked with invalid path"));
    return false;
  }

  // Retrieve name/extension from path
  //
  owl_string name = fname.GetParts(TFileName::File);
  owl_string ext  = fname.GetParts(TFileName::Ext);

  // Get pointers to path's name and extension
  //
  LPCTSTR ppn = name.c_str();
  LPCTSTR ppe = fname.HasParts(TFileName::Ext) ? ext.c_str() : _T("");
  if (*ppe == _T('.'))
    ppe++;

  // Get pointers to filter's name and extension
  //
  TAPointer<_TCHAR> filter = nstrnewdup(fltr);
  _TCHAR* pfn = filter;
  _TCHAR* pfe = _tcsrchr(filter, _T('.'));

  // Skip past '.' separator of filter
  //
  if (pfe)
    *pfe++ = 0;
  else
    pfe = _T("");

  // Match the name and extension
  //
  return nameMatch(ppn, pfn) && nameMatch(ppe, pfe);
}

//
// Return the document which is 'current'. This is based on which visible view
// has user focus.
//
TDocument*
TDocManager::GetCurrentDoc()
{
  PRECONDITION(GetApplication());
  PRECONDITION(GetApplication()->GetMainWindow());

  HWND hWnd = GetApplication()->GetMainWindow()->GetCommandTarget();
  TDocument* doc = 0;

#if defined(OLD_DOCVIEW)
  //!BB This older implementation of GetCurrentDoc relies on the
  //!BB document's HasFocus method which does not allow 'GetCurrentDoc'
  //!BB to return child documents....
  //!BB
  //!BB This obviously causes some problems (for example, closing a view
  //!BB associated with a child document closes the whole document and
  //!BB all their associated views!).
  //!BB
  //!BB However is there code that relies on this behaviour - Investigate
  //!BB

  if (hWnd && ::IsWindow(hWnd)) {
    while ((doc = DocList.Next(doc)) != 0 && !doc->HasFocus(hWnd))
      ;
  }

#else
  if (hWnd && ::IsWindow(hWnd)) {
    while ((doc = DocList.Next(doc)) != 0 ) {
      TDocument* childDoc = doc->DocWithFocus(hWnd);
      if (childDoc) {
        doc = childDoc;
        break;
      }
    }
  }
#endif
  return doc;
}

//
// Returns a pointer to the TDocument object if the specified document
// is currently opened and manager by the DocManager. Returns 0 otherwise.
//
TDocument*
TDocManager::FindDocument(LPCTSTR path)
{
  TDocument* doc = 0;
  while ((doc = DocList.Next(doc)) != 0)
    if (path) {
      if (doc->GetDocPath() && _tcscmp(doc->GetDocPath(), path) == 0)
        break;
    }
    else {
      if (doc->GetDocPath() == 0)
        break;
    }
  return doc;
}

//----------------------------------------------------------------------------

//
// Adds a template to the template list managed by the DocManager.
//
void
TDocManager::AttachTemplate(TDocTemplate& tpl)
{
  tpl.SetDocManager(this);
  TDocTemplate::AddLink((TRegLink**)&TemplateList, &tpl);
}

//
// Removes a template from the list of templates currently managed by
// the DocManager.
//
void
TDocManager::DeleteTemplate(TDocTemplate& tpl)
{
  // Skip static templates
  //
  if (tpl.IsStatic()) {
    TRACEX(OwlDocView, 0, _T("TDocManager::DeleteTemplate() invoked for static"));
    return;
  }

  // Check if it has an owner
  //
  if (!tpl.GetDocManager()) {
    TRACEX(OwlDocView, 0, _T("TDocManager::DeleteTemplate(), templ. has no owner"));
    return;
  }

  // Unreference the template - will be deleted unless documents
  // still reference template.
  //
  if (TDocTemplate::RemoveLink((TRegLink**)&TemplateList, &tpl)) {
    UnRefTemplate(tpl);
    return;
  }

  TRACEX(OwlDocView, 0, _T("TDocManager::DeleteTemplate(), not in app list"));
}

//----------------------------------------------------------------------------

//
// Displays a MessageBox about a document-related error. Returns an integer
// identifying the MessageBox option (push-button) selected by the user.
//
uint
TDocManager::PostDocError(TDocument& doc, uint sid, uint choice)
{
  PRECONDITION(GetApplication());
  PRECONDITION(GetApplication()->GetMainWindow());

  TCHAR buf[256];
  if (GetApplication()->LoadString(sid, buf, sizeof(buf) / sizeof(_TCHAR)) == 0)
#if BI_MSG_LANGUAGE == 0x0411
    _stprintf(buf, "ｴﾗｰ: 文字列 ID %u が見つかりません", sid);
#else
    _stprintf(buf, _T("Error: Message [string ID %u] not found"), sid);
#endif

  if (choice != MB_OK)
    choice |= MB_ICONQUESTION;
  return GetApplication()->GetMainWindow()->MessageBox(buf, doc.GetTitle(), choice);
}

//
// Post a OWL-defined message regarding an event [identified by the 'id'
// parameter] related to the specified document ('doc').
//
void
TDocManager::PostEvent(int id, TDocument& doc)
{
  TWindow* win = GetApplication()->GetMainWindow();
  if (win && win->GetHandle())
    win->SendMessage(WM_OWLDOCUMENT, id, TParam2(&doc));
}

//
// Post a OWL-defined message regarding an event [identified by the 'id'
// parameter] related to the specified view ('view').
//
void
TDocManager::PostEvent(int id, TView& view)
{
  TWindow* win = GetApplication()->GetMainWindow();
  if (win && win->GetHandle())
    win->SendMessage(WM_OWLVIEW, id, TParam2(&view));
}

//----------------------------------------------------------------------------

//
// Given a list of templates, prompt user to select a document class for a
// new document,
// Return the template index used for the selection (1-based), 0 if failure
//
int
TDocManager::SelectDocType(TDocTemplate** tpllist, int tplcount)
{
  TPickListPopup pickl(GetApplication()->GetMainWindow(), IDS_DOCLIST);
  while (tplcount--) {
    CHECK(*tpllist);
    pickl.AddString((*tpllist++)->GetDescription());
  }
  int index = pickl.Execute() + 1;
  if (index == -1)
    return 0;
  return index;
//!TH This does not look right, crashes when user cancels twice
//  return pickl.Execute() + 1;
}

//
// Given a list of templates, prompt user with one or all templates to select
// file to open
// Returns the template index used for the selection (1-based), 0 if failure
//
// This is Windows-specific, using the system-provided file open dialog box
//
int
TDocManager::SelectDocPath(TDocTemplate** tpllist, int tplcount,
                           LPTSTR path, int buflen, long flags,
                           bool save, TDocument* doc)
{
  // Compute length of description(s) and filter(s)
  //
  int len = GetTemplateDescription(tpllist, tplcount);

  // Put together a string of description and filters
  //
  TAPointer<TCHAR> filtbuf = new TCHAR[++len];
  GetTemplateDescription(tpllist, tplcount, filtbuf, len);

  // Find the (default) template to select
  //
  int index, count;
  TDocument* curDoc = save ? (doc ? doc : GetCurrentDoc()) : 0;
  CHECK(!save || curDoc);
  if (save) {
    for (index = count = 0; count < tplcount; count++) {
      if (tpllist[count] == curDoc->GetTemplate()) {
        index = count;
        break;
      }
    }
  }
  else {
    for (index = count = 0; count < tplcount; count++) {
      if (tpllist[count]->IsFlagSet(dtSelected)) {
        index = count;
        break;
      }
    }
  }

  // Initialize data structure used for launching Common Dialog
  //
  flags = (tpllist[index]->GetFlags() | flags);
  flags &= 0x000FFFFF; // Clear Doc/View related flags.
  flags |= AdditionalFileDialogFlags; // Add extended flags, e.g. OFN_ENABLESIZING.
  flags &= ~dtProhibited; // Clear unsupported flags, e.g. OFN_ENABLETEMPLATE.
  TDvOpenSaveData data(flags,                             // flags
                       filtbuf,                           // filter
     CONST_CAST(LPTSTR, tpllist[index]->GetDirectory()),  // initDir.
     CONST_CAST(LPTSTR, tpllist[index]->GetDefaultExt()), // defExt.
                       index ? index+1 : 0,               // filterIndex
                       tpllist,                           // template list
                       tplcount);                         // template count

  //--- Sirma (Krasi)
  {
    LPCTSTR fName = path && *path ? path : (doc ? doc->GetTitle() : 0);
    if (fName && *fName)
      _tcsncpy(data.FileName, fName, buflen);
  }
  //--- Sirma (Krasi) ---

  // Execute dialog
  //
  int result;
  TWindow* parent = GetApplication()->GetMainWindow();
  if (save)
    result = TDvFileSaveDialog(parent, data).Execute();
  else
    result = TDvFileOpenDialog(parent, data).Execute();

  // Dialog was cancelled!
  //
  if (result != IDOK) {
    WARNX(OwlDocView, data.Error != 0, 0, _T("Common dialog error: ") << \
                      data.Error << _T(" in SelectDocPath()") );
    return 0;
  }
  //!BB
  // Here, there's a major dilemma! How, do we know the user did not
  // mislead us? For example, the user may have selected the *wrong*
  // template for saving a particular document... This is crucial when
  // saving documents!!!
  //

  // Update templates to 'remember' the template last used
  //
  for (count = 0; count < tplcount; count++) {
    if (count == index-1)
      tpllist[count]->SetFlag(dtSelected);
    else
      tpllist[count]->ClearFlag(dtSelected);
  }

  // Update selected template with directory
  //
  // //!BB Yeah! Where's the meat?


  // Copy file name over
  //
  CHECK(path);
  if (data.FileName && data.FileName[0]) {
    WARNX(OwlDocView, ::_tcslen(data.FileName) < (size_t)buflen, 0,
          _T("SelectDocPath: Specified buffer is too small"));
    _tcsncpy(path, data.FileName, buflen);
  }
  else
    *path = 0;

  // Return index [1-based] of selected template
  //
  CHECK(data.FilterIndex >= 1);
  CHECK(data.FilterIndex <= tplcount);
  return data.FilterIndex;
}

//
// Given a list of templates, prompt user to select a view class for a new
// view on an existing document.
// Return the template index used for the selection (1-based), 0 if failure
//
int
TDocManager::SelectViewType(TDocTemplate** tpllist, int tplcount)
{
  TPickListPopup pickl(GetApplication()->GetMainWindow(), IDS_VIEWLIST);
  while (tplcount--) {
    CHECK(*tpllist);
    pickl.AddString((*tpllist++)->GetViewName());
  }
  return pickl.Execute() + 1;
}

//
// This method is invoked just before the DocumentManager creates a new
// document. The default behaviour is to close and delete the current
// document if we're in SDI mode.
//
bool
TDocManager::CreatingDoc(TDocTemplate* /*tpl*/)
{
  if (Mode & dmSDI) {
    TDocument* doc = DocList.Next(0);
    if (doc) {
      if (!doc->CanClose())
        return false;
      if (!doc->Close()) {
        PostDocError(*doc, IDS_UNABLECLOSE);
        return false;
      }
      delete doc;
    }
  }
  return true;
}

//
// Upate the specified menu bar by removing the current '&File' popup
// menu [assumed to be the first popup of the menubar] and replacing it
// with the ObjectWindows' standard File popup menu resource.
//
void
TDocManager::UpdateFileMenu(TMenu& menubar)
{
  PRECONDITION(GetApplication());
  PRECONDITION(gModule);

  HMENU hMenu = GetApplication()->LoadMenu(IDM_DOCMANAGERFILE);
  if (!hMenu)
    hMenu = gModule->LoadMenu(IDM_DOCMANAGERFILE);
  if (hMenu) {
    _TCHAR menuTitle[40];
    GetApplication()->LoadString(IDS_DOCMANAGERFILE, menuTitle, sizeof(menuTitle) / sizeof(_TCHAR));

    TMenu filePopup(hMenu);
    if (Mode & dmNoRevert)
      filePopup.DeleteMenu(CM_FILEREVERT, MF_BYCOMMAND);

    // Assumes '&File' popup is first entry of menubar...
    //
    menubar.DeleteMenu(0, MF_BYPOSITION);
    menubar.InsertMenu(0, MF_BYPOSITION|MF_POPUP, (uint)hMenu, menuTitle);
  }
  WARNX(OwlDocView, !hMenu, 0, _T("Cannot load DocManager's File Menu"));
}

//----------------------------------------------------------------------------

//
//
//
bool
TDocManager::EvCanClose()
{
  TDocument* doc = 0;
  while ((doc = DocList.Next(doc)) != 0) {
    if (!doc->CanClose())  // normally calls back to FlushDoc()
      return false;
  }
  return true;
}

//
// Preprocess the app frame's menu to replace the file menu with one managed
// by this docmanager
//
void
TDocManager::EvPreProcessMenu(HMENU hMenuBar)
{
  if (Mode & dmMenu){
    TMenu menu(hMenuBar);
    UpdateFileMenu(menu);
  }
}

//
//
//
void
TDocManager::EvWakeUp()
{
  TDocument* doc = 0;
  while ((doc = DocList.Next(doc)) != 0)
    doc->ReindexFrames();
}

//----------------------------------------------------------------------------
// Command event handlers
//

//
//
//
void
TDocManager::CeFileNew(TCommandEnabler& ce)
{
  ce.Enable(TemplateList != 0);
}

//
//
//
void
TDocManager::FileNew()
{
  CreateAnyDoc(0, dtNewDoc);
}

//
//
//
void
TDocManager::CeFileOpen(TCommandEnabler& ce)
{
  ce.Enable(TemplateList != 0);
}

//
//
//
void
TDocManager::FileOpen()
{
  CreateAnyDoc(0, 0);
}

//
//
//
void
TDocManager::CeFileClose(TCommandEnabler& ce)
{
  ce.Enable(GetCurrentDoc() != 0);
}

//
//
//
void
TDocManager::FileClose()
{
  TDocument* doc = GetCurrentDoc();
  if (doc && doc->CanClose()) {  // normally calls back to FlushDoc()
    if (!doc->Close())
      PostDocError(*doc, IDS_UNABLECLOSE);
    else
      delete doc;
  }
  WARNX(OwlDocView, !doc, 0, _T("FileClose invoked with no current doc"));
}

//
//
//
void
TDocManager::CeFileSaveAs(TCommandEnabler& ce)
{
  TDocument* doc = GetCurrentDoc();
  ce.Enable(doc != 0);
}

//
//
//
void
TDocManager::FileSaveAs()
{
  TDocument* doc = GetCurrentDoc();
  if (doc) {
    TDocTemplate* tpl = SelectAnySave(*doc, true);
    if (tpl) {
      if (tpl != doc->Template)
        doc->SetTemplate(tpl);       // replace existing template
      if (doc->Commit(true))         // force rewrite to new path
        PostEvent(dnRename, *doc);   // notify that doc has been renamed
    }
  }
}

//
//
//
void
TDocManager::CeFileSave(TCommandEnabler& ce)
{
  TDocument* doc = GetCurrentDoc();
  ce.Enable(doc && (doc->IsDirty() || (Mode & dmSaveEnable)));
}

//
//
//
void
TDocManager::FileSave()
{
  TDocument* doc = GetCurrentDoc();
  if (doc) {
    if (!doc->GetDocPath()) {
      CmFileSaveAs();
      return;
    }
    if (!(Mode & dmSaveEnable) && !doc->IsDirty()) {
      PostDocError(*doc, IDS_NOTCHANGED);
      return;
    }
    doc->Commit();  // No force of write here since is just to same file
  }
}

//
//
//
void
TDocManager::CeFileRevert(TCommandEnabler& ce)
{
  TDocument* doc = GetCurrentDoc();
  ce.Enable(doc && doc->IsDirty() && doc->GetDocPath());
}

//
//
//
void
TDocManager::FileRevert()
{
  TDocument* doc = GetCurrentDoc();
  if (doc && doc->GetDocPath()) {
    if (!doc->IsDirty()) {
      PostDocError(*doc, IDS_NOTCHANGED);
      return;
    }
    doc->Revert();
  }
}

//
//
//
void
TDocManager::CeViewCreate(TCommandEnabler& hndlr)
{
  TDocument* doc = GetCurrentDoc();
  hndlr.Enable(doc != 0);
}

//
//
//
void
TDocManager::ViewCreate()
{
  TDocument* doc = GetCurrentDoc();
  if (doc)
    CreateAnyView(*doc);
}

#endif //!defined(SECTION) || SECTION == 1
//----------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 2

IMPLEMENT_STREAMABLE(TDocManager);

#if !defined(BI_NO_OBJ_STREAMING)

//
//
//
void*
TDocManager::Streamer::Read(ipstream& is, uint32 /*version*/) const
{
  TDocManager* o = GetObject();

  TDocTemplate* tpl = o->TemplateList;
  for (;;) {
    int isStatic;

    is >> isStatic;
    if (isStatic == -1)
      break;

    if (isStatic) {
      if (tpl) {                         // if static templates available
        is >> *tpl;                      // update static template data
        tpl = o->GetNextTemplate(tpl);
      }
      else {                            // have run out of static templates
        char tbuf[sizeof(TDocTemplate)];  // sink for unused template data
        memset(tbuf, 0, sizeof tbuf);     // force static flag off
        is >> *(TDocTemplate*)tbuf;
      }
    }
    else {                // if dynamic template, object will be constructed
      TModule* module;
      is >> module;
      is >> tpl;
      tpl->SetModule(module);
      o->AttachTemplate(*tpl);
    }
  }

  int count;
  is >> count;               // document count
  while (count--) {
    TDocument*  doc;
    is >> doc;
    doc->SetDocManager(*o);  // inserts properly into list
  }

  // Get application, & post ourselves a wakeup message to finish things up
  // once all windows have been created, etc.
  //
  o->Application = OWLGetAppDictionary().GetApplication(0);
  TWindow* win = o->Application->GetMainWindow();
  if (win && win->GetHandle())
    win->PostMessage(WM_OWLWAKEUP);

  return o;
}

//
//
//
void
TDocManager::Streamer::Write(opstream& os) const
{
  TDocManager* o = GetObject();

  TDocTemplate* tpl = 0;
  while ((tpl = o->GetNextTemplate(tpl)) != 0) {
    int flag = tpl->IsStatic();
    os << flag;
    if (flag) {
      os << *tpl;              // write reference to static template
    }
    else {
      os << tpl->GetModule();  // write template's module pointer first
      os << tpl;               // write pointer to static template
    }
  }
  os << -1;   // template list terminator

  TDocument* doc = 0;
  int count;
  for (count = 0; (doc = o->DocList.Next(doc))!=0; count++) ;
  os << count;

  // Must write documents out in order created, i.e. from end of list forward
  //
  while (count) {
    int i = count--;
    for (doc = 0; i--; doc = o->DocList.Next(doc)) ;
    os << doc;
  }
}

#endif  // if !defined(BI_NO_OBJ_STREAMING)

#endif  //!defined(SECTION) || SECTION == 2
//----------------------------------------------------------------------------
#if !defined(SECTION) || SECTION == 3

#if defined(OWL2_COMPAT)
//
// TDocTemplate backward compatibility code
//

//
//
//
TDocManager::TDocManager(int mode, TDocTemplate*& templateHead)
{
  Init(mode, OWLGetAppDictionary().GetApplication(0), templateHead);
}

//
//
//
bool TDocManager::SelectSave(TDocTemplate* tpl, TDocument& doc)
{
  TDocManager* mgr = tpl->GetDocManager();
  if (!mgr || !tpl || !tpl->GetFileFilter())
    return false;
  TCHAR filepath[_MAX_PATH];
  if (doc.GetDocPath())
    ::_tcscpy(filepath, doc.GetDocPath());
  else
    filepath[0] = 0;    // no initial file path
  int index = mgr->SelectDocPath(&tpl, 1, filepath, COUNTOF(filepath), 0, true, &doc);
  return index ? doc.SetDocPath(filepath) : false;
}

//
//
//
TView*
TDocManager::InitView(TView* view)
{
  if (!view) {
    WARNX(OwlDocView, !view, 0, _T("InitView(), NULL view specified"));
    return 0;
  }
  return view->GetDocument().InitView(view);
}

//
//
//
TDocument*
TDocManager::InitDoc(TDocTemplate& tpl, TDocument* doc,
                     LPCTSTR path, long flags)
{
  TDocManager* mgr = tpl.GetDocManager();
  if (!mgr || !doc)
    return 0;
  doc->SetTemplate(&tpl);
  return mgr->InitDoc(doc, path, flags);
}

//
//
//
void
TDocTemplate::SetFileFilter(LPCTSTR)
{
  TRACEX(OwlDocView, 0, _T("Obsolete function called"));
}

//
// Placeholder for an Obselete method - NOP.
//
void TDocTemplate::SetDescription(LPCTSTR)
{
  TRACEX(OwlDocView, 0, _T("Obsolete function called"));
}

//
//
//
void TDocTemplate::SetDefaultExt(LPCTSTR)
{
  TRACEX(OwlDocView, 0, _T("Obsolete function called"));
}
#endif

#endif  //!defined(SECTION) || SECTION == 3

__OWL_END_NAMESPACE

