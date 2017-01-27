// -*-Mode: C++;-*-
//
//  Triangle mesh object
//  $Id: Mesh.hpp,v 1.4 2011/04/07 07:56:47 rishitani Exp $
//

#ifndef GFX_MESH_HPP_INCLUDED
#define GFX_MESH_HPP_INCLUDED

#include "AbstractColor.hpp"
#include "ColorTable.hpp"
#include <qlib/Vector4D.hpp>

namespace gfx {

  using qlib::Vector4D;

  class GFX_API Mesh
  {
  private:
    //typedef ColorTable::elem_t IntColor;

    int m_nVerts;
    int m_nFaces;

    std::vector<float> m_verts;
    std::vector<float> m_norms;
    std::vector<ColorPtr> m_colptrs;
    std::vector<int> m_faces;

    /////

    Vector4D m_curNorm;
    ColorPtr m_pCurCol;

    /////

    // ColorTable m_clut;

  public:
    Mesh()
         : m_nVerts(0), m_nFaces(0)
    {
    }

    virtual ~Mesh()
    {
    }

    void init(int nverts, int nfaces);

    void initVerts(int nverts);
    void initFaces(int nfaces);

    ////////////////////////////////////////

    bool reduce(int nverts, int nfaces);

    void setVertex(int i, const Vector4D &v);

    Vector4D getVertex(int i) const {
      return Vector4D (m_verts[i*3+0],
                       m_verts[i*3+1],
                       m_verts[i*3+2]);
    }

    void normal(const Vector4D &n) {
      m_curNorm = n;
    }
    Vector4D getNormal(int i) const {
      return Vector4D (m_norms[i*3+0],
                       m_norms[i*3+1],
                       m_norms[i*3+2]);
    }

    void color(const ColorPtr &c);
    //void color(const ColorPtr &c, const LString &mtr);

    void setFace(int fid, int vid1, int vid2, int vid3) {
      //MB_ASSERT(m_pFaces!=NULL);
      MB_ASSERT(fid<m_nFaces);
      m_faces[fid*3+0] = vid1;
      m_faces[fid*3+1] = vid2;
      m_faces[fid*3+2] = vid3;
    }

    ////////////////////////////////////////

    void setVertex2(int i, const Vector4D &v) {
      m_verts[i*3+0] = (float) v.x();
      m_verts[i*3+1] = (float) v.y();
      m_verts[i*3+2] = (float) v.z();
    }

    void setNormal2(int i, const Vector4D &v) {
      m_norms[i*3+0] = (float) v.x();
      m_norms[i*3+1] = (float) v.y();
      m_norms[i*3+2] = (float) v.z();
    }

    void setColor2(int i, const ColorPtr &pc) {
      m_colptrs[i] = pc;
    }

    void setFace2(int fid, int vid) {
      m_faces[fid] = vid;
    }

    ////////////////////////////////////////

    int getVertSize() const {
      return m_nVerts;
    }

    int getFaceSize() const {
      return m_nFaces;
    }

    const float *getFloatVerts() const {
      return &m_verts[0];
    }

    const float *getFloatNorms() const {
      return &m_norms[0];
    }

    const int *getFaces() const {
      return &m_faces[0];
    }

    //bool getRGBAFloatCol(float &r, float &g, float &b, float &a, int iv) const;
    bool getCol(ColorPtr &c, int iv) const;

    bool convRGBAByteCols(quint8 *pcols, int nsize, int defalpha=255, qlib::uid_t nSceneID=qlib::invalid_uid) const;

  };

}

#endif //


