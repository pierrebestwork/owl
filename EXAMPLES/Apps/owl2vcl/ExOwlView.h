#if !defined(__ExOwlView_h)              // Sentry, use file only if it's not already included.
#define __ExOwlView_h

#include "VCLView.h"
#include "ViewDlgs.h"

class TExampleOwlView : public TVCLDialogView {
  public:
    TExampleOwlView(TDocument& doc, TWindow* parent = 0)
      : TVCLDialogView(CreateExampleView1, doc, parent) {}
    static const char far* StaticName() {return "My Example VCL Dialog View";}  // put in resource

    // inherited virtuals from TView
    //
    const char far* GetViewName() {return StaticName();}
};

#endif                                      // __ExOwlView_h sentry.
