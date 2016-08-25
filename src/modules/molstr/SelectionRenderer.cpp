// -*-Mode: C++;-*-
//
//    Molecular selection renderer (stick model)
//
// $Id: SelectionRenderer.cpp,v 1.14 2011/03/29 11:03:44 rishitani Exp $

#include <common.h>
#include "SelectionRenderer.hpp"

#include "MolCoord.hpp"
#include "SelCommand.hpp"
#include "AnimMol.hpp"
#include "AtomIterator.hpp"
#include "BondIterator.hpp"
#include <qsys/View.hpp>
#include <qlib/Vector3F.hpp>

// Use OpenGL VBO implementation
#define USE_OPENGL_VBO

using namespace molstr;

using qlib::Vector4D;
using qlib::Vector3F;
using gfx::ColorPtr;

SelectionRenderer::SelectionRenderer()
{
  m_nMode = MODE_STICK;
  m_pNullSel = molstr::SelectionPtr(MB_NEW molstr::SelCommand(LString("!*")));
  m_pVBO = NULL;
}

SelectionRenderer::~SelectionRenderer()
{
  // VBO should be cleaned up here
  //  invalidateDisplayCache() has been called
  //  in unloading() method of DispCacheRend impl.
  MB_ASSERT(m_pVBO==NULL);
}

const char *SelectionRenderer::getTypeName() const
{
  return "*selection";
}

SelectionPtr SelectionRenderer::getSelection() const
{
  MolCoordPtr pClient = qlib::ensureNotNull( getClientMol() );

  SelectionPtr psel = pClient->getSelection();
  if (psel.isnull())
    return m_pNullSel;
  if (psel->toString().isEmpty())
    return m_pNullSel;
  return psel;
}

void SelectionRenderer::setSelection(SelectionPtr pSel)
{
}

void SelectionRenderer::propChanged(qlib::LPropEvent &ev)
{
  super_t::propChanged(ev);

  if (ev.getTarget()==this) {
    if (ev.getName().equals("mode") ||
        ev.getName().equals("linew") ||
        ev.getName().equals("dispx") ||
        ev.getName().equals("dispy") ||
        ev.getName().equals("color")
        ) {
      invalidateDisplayCache();
      return;
    }
  }

}

//////////////////////////////////////////////////////////////////////////
// selection drawing

static void drawSelInterAtomLine(MolAtomPtr pAtom1, MolAtomPtr pAtom2,
                                 DisplayContext *pdl)
{
  if (pAtom1.isnull() || pAtom2.isnull()) return;

  pdl->vertex(pAtom1->getPos());
  pdl->vertex(pAtom2->getPos());
}

static void drawSelAtom(MolAtomPtr pAtom, DisplayContext *pdl)
{
  pdl->drawAster(pAtom->getPos(), 0.25);
}

bool SelectionRenderer::isRendBond() const
{
  if (m_nMode==0)
    return true;
  else
    return false;
}

void SelectionRenderer::beginRend(DisplayContext *pdl)
{
  pdl->color(m_color);
  if (m_nMode==0) {
    pdl->startLines();
  }
  else {
    pdl->setPointSize(m_linew);
    pdl->startPoints();
  }
}

void SelectionRenderer::endRend(DisplayContext *pdl)
{
  if (m_nMode==0) {
    pdl->end();
    pdl->setLineWidth(1.0);
  }
  else {
    pdl->end();
    pdl->setPointSize(1.0);
  }
}

void SelectionRenderer::rendAtom(DisplayContext *pdl, MolAtomPtr pAtom, bool fbonded)
{
  if (m_nMode==0) {
    if (!fbonded)
      drawSelAtom(pAtom, pdl);
  }
  else {
    pdl->vertex(pAtom->getPos());
    //Vector4D pos = pAtom->getPos();
    //pdl->drawPixels(pos, m_boximg, *(m_color.get()));
  }
}

void SelectionRenderer::rendBond(DisplayContext *pdl, MolAtomPtr pAtom1, MolAtomPtr pAtom2, MolBond *pMB)
{
  if (m_nMode==0) {
    drawSelInterAtomLine(pAtom1, pAtom2, pdl);
  }
  else {
  }
}

void SelectionRenderer::preRender(DisplayContext *pdc)
{
  Vector4D dv;

  double delx = m_dispx, dely = m_dispy;
  if (m_nMode!=0) {
    delx -= m_linew/2.0;
    dely += m_linew/2.0;
  }
  qsys::View *pview = pdc->getTargetView();
  if (pview!=NULL)
    pview->convXYTrans(delx, dely, dv);

  pdc->setLineWidth(m_linew);
  pdc->pushMatrix();
  pdc->translate(dv);
  pdc->setLighting(false);
}

void SelectionRenderer::postRender(DisplayContext *pdc)
{
  pdc->popMatrix();
  pdc->setLineWidth(1.0);
}

void SelectionRenderer::objectChanged(qsys::ObjectEvent &ev)
{

#ifdef USE_OPENGL_VBO
  if (ev.getType()==qsys::ObjectEvent::OBE_CHANGED) {
    if (ev.getDescr().equals("atomsMoved")) {
      // OBE_CHANGED && descr=="atomsMoved"
      if (m_pVBO!=NULL) {
        // only update positions
        updateVBO();
        m_pVBO->setUpdated(true);
        return;
      }
    }
    super_t::objectChanged(ev);
    return;
  }
#endif

  if (ev.getType()==qsys::ObjectEvent::OBE_PROPCHG) {
    if (ev.getDescr().equals("sel")) {
      invalidateDisplayCache();
    }
  }

  super_t::objectChanged(ev);
}

bool SelectionRenderer::isTransp() const
{
  return true;
}

///////////////////////////////////////////////////////////////////
// VBO implementation

void SelectionRenderer::display(DisplayContext *pdc)
{
#ifdef USE_OPENGL_VBO
  if (!isUseVBO() || pdc->isFile() || !pdc->isDrawElemSupported()) {
    // case of the file (non-ogl) rendering
    // always use the old version.
    super_t::display(pdc);
    return;
  }

  // new rendering routine using VBO (DrawElem)

  if (m_pVBO==NULL) {
    createVBO();
    updateVBO();
    if (m_pVBO==NULL)
      return; // Error, Cannot draw anything (ignore)
  }
  
  preRender(pdc);
  m_pVBO->setLineWidth(m_linew);
  m_pVBO->setDefColor(m_color);
  pdc->drawElem(*m_pVBO);
  postRender(pdc);
#else
  super_t::display(pdc);
#endif
}

void SelectionRenderer::createVBO()
{
  quint32 i, j;
  quint32 nbons = 0, natoms = 0, nmbons = 0, nva = 0;
  MolCoordPtr pCMol = getClientMol();

  AnimMol *pMol = static_cast<AnimMol *>(pCMol.get());

  std::deque<int> isolated_atoms;
  
  // build bond data structure/estimate VBO size

    std::set<int> bonded_atoms;
    BondIterator biter(pCMol, getSelection());

    for (biter.first(); biter.hasMore(); biter.next()) {
      MolBond *pMB = biter.getBond();
      int aid1 = pMB->getAtom1();
      int aid2 = pMB->getAtom2();

      bonded_atoms.insert(aid1);
      bonded_atoms.insert(aid2);

      MolAtomPtr pA1 = pMol->getAtom(aid1);
      MolAtomPtr pA2 = pMol->getAtom(aid2);

      if (pA1.isnull() || pA2.isnull())
        continue; // skip invalid bonds

      ++nbons;
    }

    m_sbonds.resize(nbons);

    i=0;
    j=0;
    int iva = 0;
    for (biter.first(); biter.hasMore(); biter.next()) {
      MolBond *pMB = biter.getBond();
      int aid1 = pMB->getAtom1();
      int aid2 = pMB->getAtom2();

      MolAtomPtr pA1 = pMol->getAtom(aid1);
      MolAtomPtr pA2 = pMol->getAtom(aid2);

      if (pA1.isnull() || pA2.isnull())
        continue; // skip invalid bonds
      
      m_sbonds[i].aid1 = aid1;
      m_sbonds[i].aid2 = aid2;
      m_sbonds[i].ind1 = pMol->getCrdArrayInd(aid1) * 3;
      m_sbonds[i].ind2 = pMol->getCrdArrayInd(aid2) * 3;
      m_sbonds[i].vaind = iva;
      iva+=4;
      ++i;
    }

  // build isolated atom data structure/estimate VBO size
  AtomIterator aiter(pCMol, getSelection());
  for (aiter.first(); aiter.hasMore(); aiter.next()) {
    int aid = aiter.getID();
    MolAtomPtr pAtom = pMol->getAtom(aid);
    if (pAtom.isnull()) continue; // ignore errors
    if (bonded_atoms.find(aid)!=bonded_atoms.end())
      continue; // already bonded
    isolated_atoms.push_back(aid);
  }
  natoms = isolated_atoms.size();
  m_atoms.resize(natoms);
  for (i=0; i<natoms; ++i) {
    int aid1 = isolated_atoms[i];
    m_atoms[i].aid1 = aid1;
    m_atoms[i].ind1 = pMol->getCrdArrayInd(aid1) * 3;
    m_atoms[i].vaind = iva;
    iva += 2*3;
  }
  
  nva = iva;
    
  if (m_pVBO!=NULL)
    delete m_pVBO;
    
  m_pVBO = MB_NEW gfx::DrawElemV();
  m_pVBO->alloc(nva);
  m_pVBO->setDrawMode(gfx::DrawElemVC::DRAW_LINES);
  LOG_DPRINTLN("SelectionRenderer> %d elems VBO created", nva);
}

void SelectionRenderer::updateVBO()
{
  quint32 i = 0, j = 0;
  quint32 aid1, aid2;
  quint32 nbons = m_sbonds.size();
  quint32 natoms = m_atoms.size();
  
  MolCoordPtr pCMol = getClientMol();
  AnimMol *pMol = static_cast<AnimMol *>(pCMol.get());
  
  qfloat32 *crd = pMol->getAtomCrdArray();

  MolAtomPtr pA1, pA2;

  // ColorPtr pcol1, pcol2;
  Vector3F pos1, pos2;

  // Single bonds
  for (i=0; i<nbons; ++i) {
    aid1 = m_sbonds[i].ind1;
    aid2 = m_sbonds[i].ind2;
    j = m_sbonds[i].vaind;

    m_pVBO->vertexfp(j, &crd[aid1]);
    ++j;
    m_pVBO->vertexfp(j, &crd[aid2]);
    //++j;
  }

  // Isolated atoms
  
  // size of the star
  const qfloat32 rad = 0.25;
  const qfloat32 rad2 = rad*2.0f;

  for (i=0; i<natoms; ++i) {
    quint32 aid1 = m_atoms[i].ind1;
    quint32 j = m_atoms[i].vaind;

    pos1.set(&crd[aid1]);

    pos1.x() -= rad;
    m_pVBO->vertex3f(j, pos1);
    ++j;
    pos1.x() += rad2;
    m_pVBO->vertex3f(j, pos1);
    pos1.x() -= rad;
    ++j;

    pos1.y() -= rad;
    m_pVBO->vertex3f(j, pos1);
    ++j;
    pos1.y() += rad2;
    m_pVBO->vertex3f(j, pos1);
    ++j;
    pos1.y() -= rad;

    pos1.z() -= rad;
    m_pVBO->vertex3f(j, pos1);
    ++j;
    pos1.z() += rad2;
    m_pVBO->vertex3f(j, pos1);
    ++j;
  }
}

void SelectionRenderer::invalidateDisplayCache()
{
#ifdef USE_OPENGL_VBO
  if (m_pVBO!=NULL) {
    delete m_pVBO;
    m_pVBO = NULL;

    m_sbonds.clear();
    m_atoms.clear();
  }
#endif

  super_t::invalidateDisplayCache();
}

