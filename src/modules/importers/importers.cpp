// -*-Mode: C++;-*-
//
// PSE loader module
//

#include <common.h>

#include "importers.hpp"

#include <qsys/RendererFactory.hpp>
#include <qsys/StreamManager.hpp>
#include "PSEFileReader.hpp"
#include "MmcifMolReader.hpp"

extern void importers_regClasses();
extern void importers_unregClasses();

namespace importers {

  using qsys::RendererFactory;
  using qsys::StreamManager;

  bool init()
  {
    importers_regClasses();

    // register IO handlers
    StreamManager *pSM = StreamManager::getInstance();
    pSM->registWriter<PSEFileReader>();

    pSM->registWriter<MmcifMolReader>();

    MB_DPRINTLN("importers init: OK");
    return true;
  }

  void fini()
  {
    // importers_unregClasses();

    MB_DPRINTLN("importers fini: OK");
  }

}

