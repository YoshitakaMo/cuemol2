#pragma once

#include "molclient.hpp"

#include <qlib/LScrObjects.hpp>
#include <qlib/SingletonBase.hpp>
#include <qlib/mcutils.hpp>

#include <modules/molstr/molstr.hpp>

namespace molclient {

  using qlib::LString;
  class XmlRpcConn;

  class MOLCLIENT_API MolClientManager : public qlib::LSingletonScrObject,
  public qlib::SingletonBase<MolClientManager>
  {
    MC_SCRIPTABLE;

  public:
    MolClientManager();

    virtual ~MolClientManager();

    qlib::LScrSp<XmlRpcConn> connect(const LString &server) const;

    //////////
    // Initializer/finalizer (called from qlib-appfw)

    static bool initClass(qlib::LClass *pcls)
    {
      return qlib::SingletonBase<MolClientManager>::init();
    }
    
    static void finiClass(qlib::LClass *pcls)
    {
      qlib::SingletonBase<MolClientManager>::fini();
    }

  };

}

SINGLETON_BASE_DECL(molclient::MolClientManager);
