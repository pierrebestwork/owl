#if !defined(util_h)              // Sentry, use file only if it's not already included.
#define util_h

// This structure may be initialized from the commandline parameters
class ReplaceData
{
  public:
    ~ReplaceData();

    static ReplaceData &GetInstance();

    bool ShouldRecurse() const { return recurse; }
    void SetRecurse(bool _recurse) { recurse = _recurse; }

    // This function adds the string to the first empty member of the struct
    //  the len parameter is needed, because the string coming
    //  from TCmdLine is not null-terminated
    void AddString(const char *str, int len);

    const char *GetFolder() const { return folder; }
    const char *GetFilter() const { return filter; }
    const char *GetFind() const { return find; }
    const char *GetReplace() const { return replace; }

  private:
  	// Only a single instance of this class can exist,
    //  so the constructor is hidden
    ReplaceData();

    char *folder;
    char *filter;
    char *find;
    char *replace;

    bool recurse;

};

extern bool BrowseForFolder(HWND hWnd, char *buf, char * /*prompt*/);

#endif
