#if !defined(replacer_h)
#define replacer_h

#include <owl\window.h>
#include <owl\time.h>

__OWL_DECLCLASS(TFileName);

class FileProcessorBase {
  public:
    virtual bool ProcessFile(TFileName &filename) = 0;
    virtual void ReportResults(TWindow *window) = 0;
};

////////////////////////////////////////////////////////////////////////////////

class FileRecurser
{
  public:
    FileRecurser(char *_folder, char *_filter, bool _subfolders,
                 FileProcessorBase *_fileProcessor);
    ~FileRecurser() { delete [] filters; }

    bool RecurseFolders();

  private:
    class Filter
    {
      public:
        Filter() { filter = 0; }
        ~Filter() { delete [] filter; }

        const char *GetStr() const { return filter; }
        void SetStr(const char *_filter);

      private:
        char *filter;
    };

    char *folder;

    int filterCount;
    Filter *filters;

    bool subfolders;

    FileProcessorBase *fileProcessor;
};

////////////////////////////////////////////////////////////////////////////////

// Replaces text in the given file
class Replacer : public FileProcessorBase
{
  public:
    Replacer(char *_find, char *_replace)
    : find(_find), replace(_replace) { files = 0; replacements = 0; }

    bool ProcessFile(TFileName &filename);
    void ReportResults(TWindow *window);

  private:

    char *find;
    char *replace;

    int files;
    int replacements;
};

////////////////////////////////////////////////////////////////////////////////

// Changes file date and time
class Toucher : public FileProcessorBase
{
  public:
    Toucher(const TTime &_filetime)
    : filetime(_filetime) { files = 0; }

    bool ProcessFile(TFileName &filename);
    void ReportResults(TWindow *window);

  private:

    TTime filetime;

    int files;
};

#endif