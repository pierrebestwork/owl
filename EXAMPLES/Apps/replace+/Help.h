#if !defined(help_h)              // Sentry, use file only if it's not already included.
#define help_h

#include <owl\dialog.h>
#include <owl\glyphbtn.h>

class THelpDialog : public TDialog
{
  public:
    THelpDialog(TWindow* parent, TResId resId = TResId(0), TModule* module = 0);
    virtual ~THelpDialog();

  protected:
    TGlyphButton *gbOk;
};

#endif  // client_h sentry.
