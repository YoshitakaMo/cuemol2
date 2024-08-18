#pragma once

#include "molclient.hpp"

#include <qlib/ObjectManager.hpp>
#include <qlib/LScrObjects.hpp>
#include <qlib/LScrSmartPtr.hpp>
#include <qlib/mcutils.hpp>

namespace molclient {

  using qlib::LString;

  class MOLCLIENT_API XmlRpcConn : public qlib::LNoCopyScrObject
  {
    MC_SCRIPTABLE;

  private:
    LString m_serverUrl;
    
  public:
    XmlRpcConn();

    virtual ~XmlRpcConn();

    bool connect(const LString &serverUrl) const;

  };

}
