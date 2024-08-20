#pragma once

#include "molclient.hpp"

#include <qlib/ObjectManager.hpp>
#include <qlib/LScrObjects.hpp>
#include <qlib/LScrSmartPtr.hpp>
#include <qlib/mcutils.hpp>
#include <qsys/qsys.hpp>

namespace molclient {

  using qlib::LString;

  class MOLCLIENT_API XmlRpcConn : public qlib::LNoCopyScrObject
  {
    MC_SCRIPTABLE;

  private:
    LString m_serverUrl;
    
    void *m_pClient;

  public:
    XmlRpcConn();

    virtual ~XmlRpcConn();

    bool connect(const LString &serverUrl);

    qsys::ObjectPtr molFromSmiles(const LString &serverUrl);

  };

}
