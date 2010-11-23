#if !defined(__ExOwlDoc_h)
#define __ExOwlDoc_h

#include "ViewDlgs.h"
#include "VCLDoc.h"

class TExampleOwlDocument : public TVCLFileDocument
{
public:
  TExampleOwlDocument(TDocument* parent = 0)
    : TVCLFileDocument(CreateExampleDocument1,parent) {}
};

#endif
