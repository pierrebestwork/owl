//----------------------------------------------------------------------------
// ObjectComponents
// Copyright (c) 1994, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.14 $
//$Author: jogybl $
//$Date: 2007-09-15 11:43:48 $
//
// OLE Automation - Implementation of TAutoIterator
//----------------------------------------------------------------------------
#define INC_OLE2
#include <ocf/pch.h>
#if !defined(OCF_APPDESC_H)
# include <ocf/appdesc.h>
#endif
#if !defined(OCF_OCREG_H)
# include <ocf/ocreg.h>
#endif

__OCF_BEGIN_NAMESPACE

__OWL_USING_NAMESPACE

//
//
//
TAutoIterator::TAutoIterator(TAutoCreator& creator,IUnknown* owner,TLangId lang)
              :Creator(creator), Owner(owner), Symbol(0), 
               RefCnt(0), Class(0), Lang(lang) 
{
  // NOTE: RefCnt = 0 on creation, will ++ in TAutoVal operator(IUnknown*)
}
     

//
//
//
TAutoIterator::TAutoIterator(TAutoIterator& copy)
              :Symbol(copy.Symbol), Owner(copy.Owner), Class(copy.Class), 
               Lang(copy.Lang), Creator(copy.Creator), RefCnt(1)
{
  Owner->AddRef();
}

//
//
//
TAutoIterator::~TAutoIterator()
{
  Owner->Release();
}

//
// IEnumVARIANT implementation
//
HRESULT _IFUNC
TAutoIterator::QueryInterface(const GUID& iid, void** pif)
{
  if (iid!=IID_IUnknown && iid!=IID_IEnumVARIANT) {
    *pif = 0;
    return HR_NOINTERFACE;
  }
  *pif = this;
  ++RefCnt;
  return HR_NOERROR;
}

//
//
//
unsigned long _IFUNC TAutoIterator::AddRef()
{
  return ++RefCnt;
}

//
//
//
unsigned long _IFUNC TAutoIterator::Release()
{
  return --RefCnt==0 ? delete this,0 : RefCnt;
}

//
//
//
HRESULT _IFUNC TAutoIterator::Next(unsigned long count, VARIANT* retvals,
                                   unsigned long* retcount)
{
  unsigned long index = 0;
  try {
    while(index < count) {
      if (!Test())
        break;
      TAutoVal & retval = ((TAutoVal*)retvals)[(int)index];
      Return(retval);
      if (Symbol->IsEnum())
        Symbol->GetEnum()->Convert(retval, Lang);
      TObjectDescriptor objDesc;
      if (retval.GetObjDesc(objDesc)) {
        if (!objDesc.Object)     // null pointer returned from function
          retval = TAutoVoid();  // return an empty value if no object
        else
          retval = Creator.CreateDispatch(objDesc);
      }
      Step();
      index++;
    }
  }
  catch(...) {
  }
  if (retcount)
    *retcount = index;
  return index==count ? HR_NOERROR : HR_FALSE;
}

//
//
//
HRESULT _IFUNC TAutoIterator::Skip(unsigned long count)
{
  while(count--) {
    if (!Test())
      return HR_FALSE;
    Step();
  }
  return HR_NOERROR;
}

//
//
//
HRESULT _IFUNC TAutoIterator::Reset()
{
  Init();
  return HR_NOERROR;
}

//
//
//
HRESULT _IFUNC TAutoIterator::Clone(IEnumVARIANT** ppenum)
{
  try {
    *ppenum = Copy();
    return HR_NOERROR;
  }
  catch (...) {
    return HR_OUTOFMEMORY;
  }
}


__OCF_END_NAMESPACE

//==============================================================================

