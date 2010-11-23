//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1993, 1995 by Borland International, All Rights Reserved
//
//   Provides templates and DLL access to TEditView and TListView
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/listview.h>
#include <owl/editview.h>
#include <owl/filedoc.h>
#include <owl/docmanag.h>
#include <owl/docview.h>
#include <owl/bardescr.h>
#include "editlist.rc"

class MListView : public TListView{
	public:
  	MListView(TDocument& doc, TWindow* parent=0);
  DECLARE_STREAMABLE(,MListView,1);
};

MListView::MListView(TDocument& doc, TWindow* parent)
:
	TListView(doc, parent)
{
	SetViewBar(new TBarDescr(IDB_LISTVIEW));
}

class MEditView : public TEditView{
	public:
  	MEditView(TDocument& doc, TWindow* parent=0);
  DECLARE_STREAMABLE(,MEditView,1);
};
MEditView::MEditView(TDocument& doc, TWindow* parent)
:
	TEditView(doc, parent)
{
	SetViewBar(new TBarDescr(IDB_EDITVIEW));
}

DEFINE_DOC_TEMPLATE_CLASS(TFileDocument, MListView, ListTemplate);
DEFINE_DOC_TEMPLATE_CLASS(TFileDocument, MEditView, EditTemplate);

_OWLLINK(rTListView); // force TListView::Streamer to be linked in
_OWLLINK(rTEditView); // force TEditView::Streamer to be linked in

EditTemplate atpl("Text files, EditView", "*.txt", 0, "TXT", dtAutoDelete|dtUpdateDir);
ListTemplate btpl("Text files, ListView", "*.txt", 0, "TXT", dtAutoDelete|dtUpdateDir);
ListTemplate ctpl("Batch files, ListView", "*.bat", 0, "BAT", dtAutoDelete|dtUpdateDir);
EditTemplate dtpl("Source files, EditView", "*.cpp;*.h",0,0,dtAutoDelete|dtUpdateDir);


IMPLEMENT_STREAMABLE1(MListView, TListView);
IMPLEMENT_STREAMABLE1(MEditView, TEditView);

#if !defined(BI_NO_OBJ_STREAMING)

void*
MListView::Streamer::Read(ipstream& is, uint32 /*version*/) const

{
  ReadBaseObject((TListView*)GetObject(), is);
//	GetObject()->SetViewBar(new TBarDescr(IDB_LISTVIEW));
  return GetObject();
}

void
MListView::Streamer::Write(opstream &os) const
{
  WriteBaseObject((TListView*)GetObject(), os);
}

void*
MEditView::Streamer::Read(ipstream& is, uint32 /*version*/) const

{
  ReadBaseObject((TEditView*)GetObject(), is);
//	GetObject()->SetViewBar(new TBarDescr(IDB_EDITVIEW));
  return GetObject();
}

void
MEditView::Streamer::Write(opstream &os) const
{
  WriteBaseObject((TEditView*)GetObject(), os);
}


#endif  // if !defined(BI_NO_OBJ_STREAMING)

