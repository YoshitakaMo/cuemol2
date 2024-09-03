
#include <common.h>
#include <libcuemol2_api/loader.hpp>

#include <iostream>
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include <qlib/qlib.hpp>
#include <qlib/FileStream.hpp>

#include <qsys/qsys.hpp>
#include <qsys/SceneManager.hpp>
#include <qsys/SysConfig.hpp>

using qlib::LString;
void process_input(const LString &loadscr, const std::deque<LString> &args,bool bInvokeIntrShell);

#ifndef DEFAULT_CONFIG
#define DEFAULT_CONFIG "./sysconfig.xml"
#endif

///
///   main routine for CueTTY (CLI version)
///
int internal_main(int argc, const char *argv[])
{
  if (cuemol2::init_qlib()) {
    LOG_DPRINTLN("cuemol2::init_qlib OK.");
  }
  else {
    printf("Init: ERROR!!\n");
    return -1;
  }

  int i;
  LString loadscr;
  LString confpath;
  std::deque<LString> args2;

  bool bInvokeIntrShell = false;

  for (i=1; i<argc; ++i) {
    MB_DPRINTLN("arg%d=%s", i, argv[i]);
    LString value = argv[i];

    if (value.equals("-i")) {
      bInvokeIntrShell = true;
      continue;
    }
    else if (value.equals("-conf")) {
      ++i;
      if (i>=argc) break;
      confpath = argv[i];
      // ++i;
      continue;
    }
    else {
      break;
    }
  }
  
  for (; i<argc; ++i) {
    MB_DPRINTLN("arg%d=%s", i, argv[i]);
    args2.push_back(argv[i]);
  }

  if (args2.size()>0)
    loadscr = args2.front();

  if (confpath.isEmpty()) {
    confpath = DEFAULT_CONFIG;
  }

  int result = cuemol2::init(confpath, true);
  if (result < 0) {
    return result;
  }

  //if (!loadscr.isEmpty()) {
  process_input(loadscr, args2, bInvokeIntrShell);
  //}

  cuemol2::fini();
  cuemol2::fini_qlib();

  printf("=== Terminated normaly ===\n");
  return 0;
}

int main(int argc, const char *argv[])
{
  try {
    return internal_main(argc, argv);
  }
  catch (const qlib::LException &e) {
    LOG_DPRINTLN("Caught exception <%s>", typeid(e).name());
    LOG_DPRINTLN("Reason: %s", e.getMsg().c_str());
  }
  catch (std::exception &e) {
    LOG_DPRINTLN("Caught exception <%s>", typeid(e).name());
    LOG_DPRINTLN("Reason: %s", e.what());
  }
  catch (...) {
    LOG_DPRINTLN("Caught unknown exception");
  }
}

namespace fs = boost::filesystem;

void process_input(const LString &loadscr, const std::deque<LString> &args, bool bInvokeIntrShell )
{
  qsys::SceneManager *pSM = qsys::SceneManager::getInstance();
  LOG_DPRINTLN("CueMol version %s build %s", pSM->getVersion().c_str(), pSM->getBuildID().c_str());

  fs::path scr_path(loadscr.c_str());
  
  fs::path full_path = fs::system_complete( scr_path );

  if (full_path.extension()==".qsc") {
    //qsys::ScenePtr rscene = pSM->loadSceneFrom(scr_path.file_string(), "xml");
    bInvokeIntrShell = true;
    //qlib::FileOutStream &fos = qlib::FileOutStream::getStdErr();
    //rscene->writeTo(fos, true);
  }
  else {
    // no input file --> try to run interactive shell
    bInvokeIntrShell = true;
  }
  

  MB_DPRINTLN("main> cleanup ...");
  pSM->destroyAllScenes();
  MB_DPRINTLN("main> cleanup done.");
}

