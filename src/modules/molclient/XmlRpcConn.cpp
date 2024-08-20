#include <common.h>
#include "XmlRpcConn.hpp"

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include <qsys/ObjReader.hpp>
#include <qsys/StreamManager.hpp>
#include <qlib/StringStream.hpp>
#include <qsys/SceneManager.hpp>

#define NAME "Xmlrpc-c Test Client"
#define VERSION "1.0"

namespace molclient {
  XmlRpcConn::XmlRpcConn()
  {
    LOG_DPRINTLN("XmlRpcConn created(%p)", this);
  }

  XmlRpcConn::~XmlRpcConn()
  {
    LOG_DPRINTLN("XmlRpcConn destroy(%p)", this);
  }

  bool XmlRpcConn::connect(const LString &server) const
  {
    LOG_DPRINTLN("connecting to server: %s", server.c_str());

    xmlrpc_env env;
    xmlrpc_value *resultP = nullptr;
    xmlrpc_int32 sum = 0;
    const char *serverUrl = server.c_str();
    const char *methodName = "mol_from_smiles";
    xmlrpc_env_init(&env);

    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }

    const char *smiles_str = "c1cc(CCC)cc(CCC)c1";

    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(s)", smiles_str);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }

    const char *sdf_buf;
    xmlrpc_read_string(&env, resultP, &sdf_buf);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }
    LOG_DPRINTLN("The sum is %d", sum);

    xmlrpc_DECREF(resultP);
    xmlrpc_client_cleanup();

    LOG_DPRINTLN("Received: %s", sdf_buf);

    auto strMgr = qsys::StreamManager::getInstance();
    auto reader = qsys::ObjReaderPtr(strMgr->createReaderPtr("sdf"));
    
    auto sceneMgr = qsys::SceneManager::getInstance();
    auto scene = sceneMgr->getScene(sceneMgr->getActiveSceneID());

    // reader->attach();
    // reader->read(strstr);
    qlib::StrInStream strstr(sdf_buf);
    auto obj = reader->load(strstr);
    obj->setPropStr("name", "xxx");
    scene->addObject(obj);


    free((void *) sdf_buf);
    return true;
  }

}
