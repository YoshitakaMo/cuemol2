//
// MolServer client module
//  module loader/common impl
//

#include <common.h>
#include <qlib/LDebug.hpp>

#include "molclient.hpp"

extern void molclient_regClasses();
extern void molclient_unregClasses();

namespace molclient {

bool init()
{
  molclient_regClasses();
  LOG_DPRINTLN("molclient init: OK");
  return true;
}

void fini()
{
  molclient_unregClasses();
  
  LOG_DPRINTLN("molclient fini: OK");
}

}

