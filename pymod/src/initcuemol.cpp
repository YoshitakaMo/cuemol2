//
// Python CueMol module initialization
//
#include <common.h>
#include <libcuemol2_api/loader.hpp>

#include <Python.h>

#include <qlib/EventManager.hpp>
#include <qlib/LProcMgr.hpp>
#include <qlib/qlib.hpp>

#if (GUI_ARCH != MB_GUI_ARCH_CLI)
#include <sysdep/sysdep.hpp>
#endif

#ifndef DEFAULT_CONFIG
#define DEFAULT_CONFIG "./sysconfig.xml"
#endif

#include "wrapper.hpp"

using namespace pybr;

PyMODINIT_FUNC PyInit__cuemol_internal()
{
    qlib::init();
    MB_DPRINTLN("CueMol2 pymodule : INITIALIZED");
    PyObject *module = Wrapper::init();
    return module;
}

namespace pybr {

  bool g_bInitOK = false;
  
  PyObject *isInitialized(PyObject *self, PyObject *args)
  {
    if (g_bInitOK)
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }
  
  /// CueMol initialization routine
  PyObject *initCueMol(PyObject *self, PyObject *args)
  {
    if (g_bInitOK) return Py_BuildValue("");
    
    LString confpath;
    if (PyTuple_GET_SIZE(args) < 1) {
      // without argments --> use embedded path string
      confpath = LString(DEFAULT_CONFIG);
    }
    else {
      const char *config;
      if (!PyArg_ParseTuple(args, "s", &config)) {
        PyErr_SetString(PyExc_RuntimeError, "invalid arg");
        return NULL;
      }
      confpath = LString(config);
    }
    
    LOG_DPRINTLN("initCueMol(%s) called.", confpath.c_str());

    int result = cuemol2::init(confpath, true);
    if (result < 0) {
      PyErr_SetString(PyExc_RuntimeError, "init failed");
      return NULL;
    }

    // TODO: register timer impl

    return Py_BuildValue("");
  }

  //////////
  
  /// CueMol finalization routine
  PyObject *finiCueMol(PyObject *self, PyObject *args)
  {
    if (!g_bInitOK) {
        LOG_DPRINTLN("CueMol> CueMol not initialized!!");
        PyErr_SetString(PyExc_RuntimeError, "not initialized");
        return NULL;
    }

    // // cleanup timer
    // qlib::EventManager::getInstance()->finiTimer();

    cuemol2::fini();
    cuemol2::fini_qlib();

    g_bInitOK = false;

    return Py_BuildValue("");
  }
}


