#include <common.h>
#include "MolClientManager.hpp"
#include "XmlRpcConn.hpp"

SINGLETON_BASE_IMPL(molclient::MolClientManager);

namespace molclient {
  
  MolClientManager::MolClientManager()
  {
    MB_DPRINTLN("MolClientManager(%p) created", this);
  }
  
  MolClientManager::~MolClientManager()
  {
    MB_DPRINTLN("MolClientManager(%p) destructed", this);
  }
  
  qlib::LScrSp<XmlRpcConn> MolClientManager::connect(const LString &server) const
  {
    // LOG_DPRINTLN("connecting to server: %s", server.c_str());
    XmlRpcConn *pconn = new XmlRpcConn();
    bool result = pconn->connect(server);
    if (!result) {
      LOG_DPRINTLN("failed to connect server: %s", server.c_str());
      delete pconn;
      return qlib::LScrSp<XmlRpcConn>();
    }
    return qlib::LScrSp<XmlRpcConn>(pconn);
  }

}
