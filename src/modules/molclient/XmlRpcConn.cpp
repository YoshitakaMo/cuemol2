#include <common.h>
#include "XmlRpcConn.hpp"

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

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
    const char *methodName = "add";
    xmlrpc_env_init(&env);

    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }

    resultP = xmlrpc_client_call(&env, serverUrl, methodName,
                                 "(ii)", (xmlrpc_int32) 5, (xmlrpc_int32) 7);
    if (env.fault_occurred) {
      // TODO: cleanup
      LOG_DPRINTLN("Error occurred: %d", env.fault_occurred);
      LOG_DPRINTLN("Code: %d", env.fault_code);
      LOG_DPRINTLN("Msg: %s", env.fault_string);
      return false;
    }

    xmlrpc_read_int(&env, resultP, &sum);
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

    return true;
  }

}
