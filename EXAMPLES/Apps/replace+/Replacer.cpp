#include "owlpch.h"
#pragma hdrstop

#include <strstream>

#include <owl/filename.h>

#include "app.rh"            // Definition of all resources.

#include "replacer.h"

void FileRecurser::Filter::SetStr(const char *_filter)
{
  if (filter)
    delete [] filter;

  if (_filter) {
    filter = new char[strlen(_filter) + 1];
    strcpy(filter, _filter);
  }
  else
    filter = 0;
}

FileRecurser::FileRecurser(char *_folder, char *_filter, bool _subfolders,
                           FileProcessorBase *_fileProcessor)
: folder(_folder), subfolders(_subfolders), fileProcessor(_fileProcessor)
{
  filterCount = 1;  // At least one filter should be given

  // Determine number of file filters
  for (int index = 0; _filter[index] != '\0'; index++)
    if (_filter[index] == ';')
      filterCount++;

  filters = new Filter[filterCount];

  // Split filter string into array of individual filters  
  int filterIndex = 0;
  char *beg = _filter;
  char *end = strchr(beg, ';');

  while (end) {
    *end = '\0';
    filters[filterIndex++].SetStr(beg);
    beg = end + 1;
    end = strchr(beg, ';');
  }

  filters[filterIndex++].SetStr(beg);
}

bool FileRecurser::RecurseFolders()
{
  char *p = folder + strlen(folder);

  if (*p == '\\')
    p--;

  if (subfolders) {
    strcpy(p, "\\*.*");

	  // Recurse subfolders
    for (TFileNameIterator iter(folder); iter; ++iter) {
      const TCHAR *filename = iter.Current().fullName;

      if (iter.Current().attribute & FILE_ATTRIBUTE_DIRECTORY &&
          strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
        strcpy(p + 1, filename);
        RecurseFolders();
      }
    }
  }

  // Iterate files by filter
  for (int filterIndex = 0; filterIndex < filterCount; filterIndex++) {
    strcpy(p, "\\");
    strcat(p, filters[filterIndex].GetStr());

    for (TFileNameIterator iter(folder); iter; ++iter) {
      if (!(iter.Current().attribute & (FILE_ATTRIBUTE_DIRECTORY |
                                     FILE_ATTRIBUTE_HIDDEN |
                                     FILE_ATTRIBUTE_READONLY))) {
        strcpy(p + 1, iter.Current().fullName);

        TFileName filename(folder);

        fileProcessor->ProcessFile(filename);
      }
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Replacer::ProcessFile(TFileName &filename)
{
  TFileName temp(TFileName::TempFile);

  ifstream ifs(filename.Canonical().c_str(), ios::in | ios::binary);
  ofstream ofs(temp.Canonical().c_str(), ios::out | ios::binary);

  if (!ifs)
    return false;

  if (!ofs)
    return false;

  char match[MAX_PATH];
  int pos = 0;

  int local_replacements = 0;

  while (!ifs.eof()) {
    char c = (char)ifs.get();

    if (ifs.eof())
      break;

    if (c == find[pos]) {
      match[pos++] = c;
      if (find[pos] == '\0') {
        for (int index = 0; replace[index] != '\0'; index++)
          ofs.put(replace[index]);        

        replacements++;
        local_replacements++;

        pos = 0;
      }
    }
    else {
      if (pos > 0) {
        for (int index = 0; index < pos; index++)
          ofs.put(match[index]);
        pos = 0;
      }

      ofs.put(c);
    }
  }

  ifs.close();
  ofs.close();

  if (local_replacements > 0) {
    if (!temp.Copy(filename, false))
      return false;
  }

  if (!temp.Remove())
    return false;

  files++;

  return true;
}

void Replacer::ReportResults(TWindow *window)
{
  ostrstream str;

  if (files == 0)
    str << "No files processed.";
  else if (files == 1)
    str << "1 file processed.";
  else
    str << files << " files processed.";

  str << endl;

  if (replacements == 0)
    str << "No replacements made.";
  else if (replacements == 1)
    str << "1 replacement made.";
  else
    str << replacements << " replacements made.";

  str << ends;

  window->MessageBox(str.str(), "Replace", MB_OK);
}


////////////////////////////////////////////////////////////////////////////////

bool Toucher::ProcessFile(TFileName &filename)
{
  TFileStatus status;

  if (!filename.GetStatus(status))
    return false;

  status.modifyTime = filetime;

  if (!filename.SetStatus(status))
    return false;

  files++;

  return true;
}

void Toucher::ReportResults(TWindow *window)
{
  ostrstream str;

  if (files == 0)
    str << "No files processed.";
  else if (files == 1)
    str << "1 file processed.";
  else
    str << files << " files processed.";

  str << ends;

  window->MessageBox(str.str(), "Replace", MB_OK);
}


