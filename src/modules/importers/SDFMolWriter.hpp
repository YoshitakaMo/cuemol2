//
// MOL/SDF format molecule structure writer class
//

#pragma once

#include <modules/molstr/molstr.hpp>
#include <modules/molstr/ResidIndex.hpp>
#include <qlib/LExceptions.hpp>
#include <qlib/mcutils.hpp>
#include <qsys/ObjWriter.hpp>

#include "importers.hpp"

namespace qlib {
class PrintStream;
}

namespace importers {

using molstr::MolCoord;
using molstr::MolCoordPtr;
using molstr::MolResiduePtr;
using molstr::ResidIndex;
using molstr::ResidSet;

//
///  SDF/MOL structure writer class
//
class IMPORTERS_API SDFMolWriter : public qsys::ObjWriter
{
    MC_SCRIPTABLE;

private:
    typedef ObjWriter super_t;

public:
    SDFMolWriter();
    virtual ~SDFMolWriter();

    /// Attach to and lock the target object
    virtual void attach(qsys::ObjectPtr pObj);

    /// write to the stream
    virtual bool write(qlib::OutStream &outs);

    /// get file-type description
    virtual const char *getTypeDescr() const;

    /// get file extension
    virtual const char *getFileExt() const;

    virtual const char *getName() const;

    virtual bool canHandle(qsys::ObjectPtr pobj) const;

private:
    /// Output target selection
    molstr::SelectionPtr m_pSel;

public:
    // Set selection for writing
    void setSelection(molstr::SelectionPtr pSel)
    {
        m_pSel = pSel;
    }

    // Get selection for writing
    molstr::SelectionPtr getSelection() const
    {
        return m_pSel;
    }

private:
    /// building molecular coordinate obj
    molstr::MolCoordPtr m_pMol;

    using ResBondMap = std::map<LString, std::deque<int>>;
    ResBondMap m_resBondMap;
    void writeResidue(const molstr::MolResiduePtr &presid, qlib::PrintStream &prs);
};

}  // namespace importers
