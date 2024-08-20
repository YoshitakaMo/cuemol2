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
    : m_pClient(nullptr)
  {
    LOG_DPRINTLN("XmlRpcConn created(%p)", this);
  }

  XmlRpcConn::~XmlRpcConn()
  {
    LOG_DPRINTLN("XmlRpcConn destroy(%p)", this);
    if (m_pClient != nullptr) {
      auto clientP = static_cast<xmlrpc_client *>(m_pClient);
      xmlrpc_client_destroy(clientP);
    }
  }

  bool XmlRpcConn::connect(const LString &server)
  {
    LOG_DPRINTLN("connecting to server: %s", server.c_str());

    xmlrpc_env env;
    xmlrpc_env_init(&env);
    if (env.fault_occurred) {
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }

    xmlrpc_client *clientP;
    xmlrpc_client_create(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0,
                         &clientP);
    if (env.fault_occurred) {
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }
    m_pClient = clientP;
    // xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);

    m_serverUrl = server;
    return true;
}

  qsys::ObjectPtr XmlRpcConn::molFromSmiles(const LString &smiles)
  {
    if (m_pClient == nullptr || m_serverUrl.isEmpty()) {
      MB_THROW(qlib::IllegalArgumentException, "not connected");
      return qsys::ObjectPtr();
    }
    auto clientP = static_cast<xmlrpc_client *>(m_pClient);

    xmlrpc_value *resultP = nullptr;
    xmlrpc_int32 sum = 0;
    const char *serverUrl = m_serverUrl.c_str();
    const char *methodName = "mol_from_smiles";

    const char *smiles_str = smiles.c_str(); // "c1cc(CCC)cc(CCC)c1";

    xmlrpc_env env;
    xmlrpc_env_init(&env);
    if (env.fault_occurred) {
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      MB_THROW(qlib::IllegalArgumentException, "not connected");
      return qsys::ObjectPtr();
    }

    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(s)", smiles_str);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      MB_THROW(qlib::IllegalArgumentException, "not connected");
      return qsys::ObjectPtr();
    }

    const char *sdf_buf;
    xmlrpc_read_string(&env, resultP, &sdf_buf);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      MB_THROW(qlib::IllegalArgumentException, "not connected");
      return qsys::ObjectPtr();
    }
    LOG_DPRINTLN("The sum is %d", sum);

    xmlrpc_DECREF(resultP);
    xmlrpc_client_cleanup();

    LOG_DPRINTLN("Received: %s", sdf_buf);

    auto strMgr = qsys::StreamManager::getInstance();
    auto reader = qsys::ObjReaderPtr(strMgr->createReaderPtr("sdf"));
    
    // auto sceneMgr = qsys::SceneManager::getInstance();
    // auto scene = sceneMgr->getScene(sceneMgr->getActiveSceneID());

    // reader->attach();
    // reader->read(strstr);
    qlib::StrInStream strstr(sdf_buf);
    auto obj = reader->load(strstr);

    // obj->setPropStr("name", "xxx");
    // scene->addObject(obj);

    free((void *) sdf_buf);
    return obj;
  }

}
