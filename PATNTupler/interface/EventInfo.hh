#ifndef EVENTINFO_HH
#define EVENTINFO_HH

#include <TObject.h>

namespace ran {

  class EventInfo : public TObject {
  public:
    EventInfo();///< EventInfo Constructor
    unsigned int evtNum;///< Event number
    unsigned int runNum;///< Run number
    unsigned int lumiSec;///< lumisec

    ClassDef(EventInfo,1)
  };
}

#endif
