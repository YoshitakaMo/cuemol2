#include <common.h>

#include "LoadObjectCommand.hpp"

#include <boost/filesystem.hpp>
#include <qlib/ObjectManager.hpp>
#include <qsys/SceneManager.hpp>
#include <qsys/ObjReader.hpp>
#include <qsys/StreamManager.hpp>

namespace fs = boost::filesystem;

namespace qsys {

// TO DO: use common impl with LoadSceneCommand
LString LoadObjectCommand::guessFileFormat(int nCatID) const
{
    fs::path file_path = m_filePath.c_str();
    auto extension = LString(file_path.extension().string());

    auto strMgr = qsys::StreamManager::getInstance();
    const auto &infos = strMgr->getStreamHandlerInfo();
    for (const auto &i : infos) {
        // LOG_DPRINTLN("%s: %s", nm.c_str(), shi.nickname.c_str());
        if (i.second.nCatID != nCatID) continue;
        auto &&fext = i.second.fext;
        qlib::LStringList exts;
        if (fext.split_of("; ", exts) == 0) {
            LOG_DPRINTLN("error: %s, %s", i.first.c_str(), fext.c_str());
            continue;
        }
        for (const auto &e : exts) {
            // LOG_DPRINTLN("%s: %s", i.first.c_str(), e.c_str());
            if (e.endsWith(extension)) {
                LOG_DPRINTLN("exten mached: %s==%s (%s)", extension.c_str(), e.c_str(),
                             i.first.c_str());
                return i.second.nickname;
            }
        }
    }

    // not found
    return LString();
}

/// Execute the command
void LoadObjectCommand::run()
{
    MB_ASSERT(!m_pTargScene.isnull());

    constexpr int nCatID = InOutHandler::IOH_CAT_OBJREADER;

    if (m_fileFmt.isEmpty()) {
        m_fileFmt = guessFileFormat(nCatID);
        if (m_fileFmt.isEmpty()) {
            // cannot determine file format from the file name
            MB_THROW(qlib::RuntimeException, "cannot guess file type");
            return;
        }
    }

    auto strMgr = qsys::StreamManager::getInstance();
    qsys::ObjReaderPtr reader = strMgr->createHandler(m_fileFmt, nCatID);
    reader->setPath(m_filePath);

    // check compression
    fs::path file_path = m_filePath.c_str();
    auto extension = LString(file_path.extension().string());
    if (extension.equalsIgnoreCase(".gz"))
        reader->setPropStr("compress", "gzip");

    m_pResObj = reader->createDefaultObj();
    reader->attach(m_pResObj);
    reader->read();
    reader->detach();

    if (m_objectName.isEmpty()) {
        auto stem = file_path.stem().string();
        m_pResObj->setPropStr("name", stem);
    }

    // TO DO: setup renderer (optional)

}

void LoadObjectCommand::runGUI(void *pwnd_info) {}

/// Get command's unique name
const char *LoadObjectCommand::getName() const
{
    return "load_object";
}

}  // namespace qsys
