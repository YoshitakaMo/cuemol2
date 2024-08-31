
#include <common.h>

#include "binding.hpp"

#include <iostream>
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include <qlib/qlib.hpp>
#include <qlib/FileStream.hpp>
#include <qlib/ClassRegistry.hpp>

#include <gfx/TextRenderManager.hpp>

#include <qsys/qsys.hpp>
#include <qsys/SceneManager.hpp>
#include <qsys/SysConfig.hpp>

namespace cuemol2 {

  using qlib::LString;

  bool hasClass(const qlib::LString &clsname,
                bool *retval,
                qlib::LString &errmsg) noexcept
  {
    *retval = false;

    qlib::ClassRegistry *pMgr = qlib::ClassRegistry::getInstance();
    if (pMgr==NULL) {
      errmsg = "ERROR: CueMol not initialized.";
      return false;
    }

    // qlib::LDynamic *pobj;
    try {
      // qlib::LClass *pcls = pMgr->getClassObjNx(clsname);
      // if (pcls!=NULL)
      //   *retval = true;
      // return true;
      *retval =  pMgr->isRegistered(clsname);
      return true;
    }
    catch (const qlib::LException &e) {
      LOG_DPRINTLN("HasClass> Caught exception <%s>", typeid(e).name());
      LOG_DPRINTLN("HasClass> Reason: %s", e.getMsg().c_str());
      errmsg = LString::format("Caught exception <%s> Reason: %s",
                               typeid(e).name(),
                               e.getMsg().c_str());
    }
    catch (...) {
      LOG_DPRINTLN("HasClass> Caught unknown exception for class name %s", clsname.c_str());
      errmsg = LString::format("Caught unknown exception for class name %s", clsname.c_str());
    }

    return false;
  }

  bool getService(const qlib::LString &svcname,
                  qlib::LDynamic **prval,
                  qlib::LString &errmsg) noexcept
  {
    qlib::ClassRegistry *pMgr = qlib::ClassRegistry::getInstance();
    if (pMgr==NULL) {
      errmsg = "ERROR: CueMol not initialized.";
      return false;
    }
    
    try {
      *prval = pMgr->getSingletonObj(svcname);
      return true;
    }
    catch (const qlib::LException &e) {
      errmsg = LString::format("Exception occured in getService for %s: %s",
                               svcname.c_str(),
                               e.getFmtMsg().c_str());
      LOG_DPRINTLN("GetService> Caught exception <%s>", typeid(e).name());
      LOG_DPRINTLN("GetService> Reason: %s", e.getMsg().c_str());
    }
    catch (...) {
      errmsg = 
        LString::format("Unknown Exception occured in getProp for %s",
                        svcname.c_str());
      LOG_DPRINTLN("Caught unknown exception");
    }
    
    return false;
  }

  bool createObj(const qlib::LString &clsname,
                 const qlib::LString &strval,
                 qlib::LDynamic **prval,
                 qlib::LString &errmsg) noexcept
  {
    qlib::ClassRegistry *pMgr = qlib::ClassRegistry::getInstance();
    if (pMgr==NULL) {
      errmsg = "ERROR: CueMol not initialized.";
      return false;
    }

    try {
      qlib::LClass *pcls = pMgr->getClassObj(clsname);
      if (pcls==NULL)
        MB_THROW(qlib::NullPointerException, "null");
      if (strval.isEmpty())
        *prval = pcls->createScrObj();
      else
        *prval = pcls->createScrObjFromStr(strval);
      return true;
    }
    catch (const qlib::LException &e) {
      errmsg = LString::format("Exception occured in createObj for %s: %s",
                               clsname.c_str(),
                               e.getFmtMsg().c_str());
      LOG_DPRINTLN("CreateObj> Caught exception <%s>", typeid(e).name());
      LOG_DPRINTLN("CreateObj> Reason: %s", e.getMsg().c_str());
      return false;
    }
    catch (...) {
      errmsg = 
        LString::format("Unknown Exception occured in createObj for %s",
                        clsname.c_str());
      LOG_DPRINTLN("CreateObj> Caught unknown exception");
      return false;
    }
    
    errmsg = "Unexpected condition in createObj()";
    return false;
  }

  bool getProp(qlib::LScriptable *pthis,
               const qlib::LString &propname,
               qlib::LVariant &lvar,
               qlib::LString &errmsg) noexcept
  {
    errmsg = "";

    try {
      return pthis->getNestedProperty(propname, lvar);
    }
    catch (qlib::LException &e) {
      errmsg = 
        LString::format("Exception occured in getProp for %s: %s",
                        propname.c_str(),
                        e.getFmtMsg().c_str());
    }
    catch (...) {
      errmsg = 
        LString::format("Unknown Exception occured in getProp for %s",
                        propname.c_str());
    }

    return false;
  }
  
  bool setProp(qlib::LScriptable *pthis,
               const qlib::LString &propname,
               const qlib::LVariant &lvar,
               qlib::LString &errmsg) noexcept
  {
    errmsg = "";
    
    try {
      return pthis->setNestedProperty(propname, lvar);
    }
    catch (qlib::LException &e) {
      errmsg = 
        LString::format("Exception occured in setProp for %s: %s",
                        propname.c_str(), e.getFmtMsg().c_str());
    }
    catch (...) {
      errmsg = 
        LString::format("Unknown Exception occured in setProp for %s",
                        propname.c_str());
    }

    return false;
  }

  bool invokeMethod(qlib::LScriptable *pthis,
                    const qlib::LString &mthnm,
                    qlib::LVarArgs &args,
                    qlib::LString &errmsg) noexcept
  {
    errmsg = "";

    try {
      return pthis->invokeMethod(mthnm, args);
    }
    catch (qlib::LException &e) {
      errmsg = 
        LString::format("Exception occured in native method \"%s\"\nReason: %s",
                        mthnm.c_str(), e.getMsg().c_str());
    }
    catch (...) {
      errmsg = 
        LString::format("Unknown Exception occured in native method \"%s\"",
                        mthnm.c_str());
    }

    return false;
  }

  bool toString(qlib::LScriptable *pthis,
                qlib::LString &retval,
                qlib::LString &errmsg) noexcept
  {
    try {
      retval = pthis->toString();
      return true;
    }
    catch (qlib::LException &e) {
      errmsg = 
        LString::format("Exception occured in toString(): %s",
                        e.getFmtMsg().c_str());
      return false;
    }
    catch (...) {
      errmsg = "Unknown Exception occured in toString()";
      return false;
    }
    
    errmsg = "Unexpected condition in toString()";
    return false;
  }

} // namespace cuemol2
