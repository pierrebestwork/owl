#if !defined(client_h)              // Sentry, use file only if it's not already included.
#define client_h

#include <owl\radiobut.h>
#include <owl\glyphbtn.h>
#include <owl\memcbox.h>
#include <owl\datetime.h>
#include <owl\static.h>
#include <owl\tooltip.h>
#include <owl\dialog.h>
#include "app.rh"            // Definition of all resources.

//{{TDialog = ReplaceClient}}
class ReplaceClient : public TDialog{
  public:
    ReplaceClient(TWindow* parent, TResId resId = TResId(0), TModule* module = 0);
    virtual ~ReplaceClient();

  protected:
    void SetupWindow();

    bool PreProcessMsg(MSG &msg);

    // Init the combo box edit field with the given string or the first selection
		void InitCombo(const char *str, TComboBox *combo);
    
    void Perform(bool showResult);

    TGlyphButton *gbOk;
    TGlyphButton *gbCancel;
    TGlyphButton *gbBrowse;
    TGlyphButton *gbHelp;

    TMemComboBox *comboFolder;
    TMemComboBox *comboFilter;
    TMemComboBox *comboFind;
    TMemComboBox *comboReplace;

    TDateTimePicker *pickerTime;
    TDateTimePicker *pickerDate;

    TRadioButton *radioFind;
    TRadioButton *radioTouch;

    TCheckBox *checkSubfolders;

    TStatic *stFind;
    TStatic *stReplace;
    TStatic *stTime;

    TTooltip *tooltip;

//{{ReplaceClientRSP_TBL_BEGIN}}
	protected:
		void BnBrowseClicked();
		void CmOk();
    void BnActionClicked();
    void BnHelpClicked();
//{{ReplaceClientRSP_TBL_END}}

DECLARE_RESPONSE_TABLE(ReplaceClient);
};    //{{ReplaceClient}}


#endif  // client_h sentry.
