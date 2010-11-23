//---------------------------------------------------------------------------
#ifndef DlgBaseH
#define DlgBaseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "DocBase.h"
//---------------------------------------------------------------------------

// When overriding FormCreate in a decendent class, be sure to call the
// parent FormCreate *after* performing necessary setup.

class TDialogViewBaseForm : public TForm
{
  typedef TForm inherited;
__published:	// IDE-managed Components
  TBitBtn *HelpBtn;
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall HelpBtnClick(TObject *Sender);
  void __fastcall AnyControlChanged(TObject *Sender);

private:	// User declarations
  virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	HIDESBASE MESSAGE void __fastcall WMNCDestroy(Messages::TWMNoParams &Message);

protected:
  bool DlgHasChanged;
  TBasicDocument  *MyDocument;

  void __fastcall Loaded(void);

public:		// User declarations
  __fastcall TDialogViewBaseForm(TComponent* Owner);
  void __fastcall SetDocument(TBasicDocument *Doc);
  TBasicDocument * __fastcall GetDocument() const {return MyDocument; }
  int __fastcall GetHasChanged();
  void __fastcall SetHasChanged(const bool value);

  static HWND HWndParent;

BEGIN_MESSAGE_MAP
MESSAGE_HANDLER(WM_NCDESTROY,Messages::TWMNoParams,WMNCDestroy)
END_MESSAGE_MAP(inherited)
};
//---------------------------------------------------------------------------
#endif
 