#if !defined(app_h)              // Sentry, use file only if it's not already included.
#define app_h

#include <owl\applicat.h>
#include "app.rh"            // Definition of all resources.

////////////////////////////////////////////////////////////////////////////////

class ReplaceApp : public TApplication
{
  public:
    ReplaceApp();
    virtual ~ReplaceApp();

    virtual void InitInstance();
    virtual void InitMainWindow();

  private:
    void ProcessCmdLine(const char * CmdLine);

    void ShowHelp();
};

#endif  // app_h sentry.
