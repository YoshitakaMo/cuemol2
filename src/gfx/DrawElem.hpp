// -*-Mode: C++;-*-
//
// Draw element object
//

#ifndef GFX_DRAWELEM_HPP_INCLUDE_
#define GFX_DRAWELEM_HPP_INCLUDE_

#include "gfx.hpp"
#include <qlib/Vector4D.hpp>
#include <qlib/Vector3F.hpp>
#include <qlib/LTypes.hpp>
#include "SolidColor.hpp"
#include "Mesh.hpp"

namespace gfx {

  using qlib::Vector4D;
  using qlib::Vector3F;

  class AbstDrawElem;
  
  /// Draw Element implementation
  class GFX_API DrawElemImpl
  {
  public:
	  virtual ~DrawElemImpl();

    virtual void create(const AbstDrawElem &ade) =0;

    virtual void update(const AbstDrawElem &ade) =0;

    virtual void preDraw(const AbstDrawElem &ade) =0;

    virtual void draw(const AbstDrawElem &ade) =0;

    virtual void postDraw(const AbstDrawElem &ade) =0;
  };

  /////////////////////////////////////////////////////////

  /// Common abstract class for both
  ///   fixed pipeline verteces and glsl attributes
  class GFX_API AbstDrawElem
  {
  private:
    /// size of vertices
    int m_nSize;

  public:
    
    AbstDrawElem();
    virtual ~AbstDrawElem();

    //////////

    /// Draw element/array type
    virtual int getType() const =0;

    /// clear cached data (--> delete VBO)
    virtual void invalidateCache() const;

    virtual const void *getData() const =0; //{ return NULL; }
    virtual size_t getElemSize() const =0;

    /// returns attribute buffer size (in byte unit)
    size_t getDataSize() const {
      return getElemSize() * getSize();
    }

    // index support
    virtual const void *getIndData() const { return NULL; }

    virtual size_t getIndElemSize() const {
      return 0;
    }

    virtual size_t getIndSize() const {
      return 0;
    }

    /// returns index buffer size (in byte unit)
    size_t getIndDataSize() const {
      return getIndElemSize() * getIndSize();
    }

    // Main element count
    int getSize() const { return m_nSize; }
    void setSize(int n) { m_nSize = n; }

    //////////

  private:
    /// Implementation
    mutable DrawElemImpl *m_pImpl;

  public:
    DrawElemImpl *getImpl() const { return m_pImpl; }
    void setImpl(DrawElemImpl *p) const { m_pImpl = p; }

  private:
    /// update flag
    mutable bool m_bUpdate;

  public:
    bool isUpdated() const { return m_bUpdate; }
    void setUpdated(bool b) const { m_bUpdate = b; }
    
  private:
    // VBO instantiation support
    int m_nInst;

  public:
    int getInstCount() const { return m_nInst; }
    void setInstCount(int n) { m_nInst = n; }
    
    //////////////////////////////////////////////////
    // Type ID definitions

    /// vertex, normal, color
    static const int VA_VNC = 1;
    /// vertex, color
    static const int VA_VC = 2;
    /// vertex, normal (color is supplied separatedly)
    static const int VA_VN = 3;
    /// vertex only (color is supplied separatedly)
    static const int VA_V = 4;
    /// vertex, normal, color, and index
    static const int VA_VNCI = 5;

    /// pixel data (UI label, etc)
    static const int VA_PIXEL = 6;

    /// texture map ( to be implemented )
    static const int VA_TEXTURE = 7;

    /// vertex, normal, color, and 32-bit index
    static const int VA_VNCI32 = 8;

    /// arbitary attribute array (for shader impl)
    static const int VA_ATTRS = 9;

    /// arbitary attribute array with indices (for shader impl)
    static const int VA_ATTR_INDS = 10;

  private:
    /// drawing mode
    int m_nDrawMode;
    
  public:
    // drawing mode IDs
    static const int DRAW_POINTS = 1;
    static const int DRAW_LINE_STRIP =2;
    static const int DRAW_LINE_LOOP = 3;
    static const int DRAW_LINES = 4;
    static const int DRAW_TRIANGLE_STRIP = 5;
    static const int DRAW_TRIANGLE_FAN = 6;
    static const int DRAW_TRIANGLES = 7;
    static const int DRAW_QUAD_STRIP = 8;
    static const int DRAW_QUADS = 9;
    static const int DRAW_POLYGON = 10;

    int getDrawMode() const { return m_nDrawMode; }
    void setDrawMode(int n) { m_nDrawMode = n; }
  };

  //////////////////////////////////////////////////

  /// Draw element class for fixed pipeline (or default shader)
  /// abstraction of VA/VBO implementation of OpenGL
  class GFX_API DrawElem : public AbstDrawElem
  {
    typedef AbstDrawElem super_t;

  public:

    DrawElem();
    virtual ~DrawElem();

    virtual bool vertex(int ind, const Vector4D &v) =0;

    virtual bool color(int ind, quint32 cc);

    virtual bool normal(int ind, const Vector4D &v);

    //void startPoints(int nsize);
    //void startLines(int nsize);
    //void startTriangles(int nsize);

    float getLineWidth() const { return m_fLineWidth; }
    void setLineWidth(float f) { m_fLineWidth = f; }

    quint32 getDefColor() const { return m_nDefColor; }
    void setDefColor(quint32 cc) { m_nDefColor = cc; }
    void setDefColor(const ColorPtr &col, qlib::uid_t nSceneID);

  protected:
    quint32 m_curcol;
    
  public:
    virtual void color2(const ColorPtr &col, qlib::uid_t nSceneID);
    virtual bool color2(int ind);
    
  private:
    /// line width/point size
    float m_fLineWidth;

    /// default color
    quint32 m_nDefColor;

  };
  
  /// Draw element with vertex and color
  class GFX_API DrawElemVC : public DrawElem
  {
    typedef DrawElem super_t;
    
  public:
    DrawElemVC();

    virtual ~DrawElemVC();

    struct Elem
    {
      qfloat32 x, y, z;
      qbyte r, g, b, a;
    };

    const Elem *getElems() const { return m_pData; }

    virtual const void *getData() const { return m_pData; }
    virtual size_t getElemSize() const { return sizeof(Elem); }

    virtual int getType() const { return VA_VC; }

    virtual void alloc(int nsize);

    virtual bool vertex(int ind, const Vector4D &v);

    inline void vertexfp(int ind, const qfloat32 *pcrd) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].x = pcrd[0];
      m_pData[ind].y = pcrd[1];
      m_pData[ind].z = pcrd[2];
    }

    inline void vertex3f(int ind, const Vector3F &v) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].x = v.x();
      m_pData[ind].y = v.y();
      m_pData[ind].z = v.z();
    }

    virtual bool color(int ind, quint32 cc);

    bool getVertex(int ind, Vector4D &v) const;

  private:
      Elem *m_pData;

  };

  /// Draw element with only vertex
  class GFX_API DrawElemV : public DrawElem
  {
    typedef DrawElem super_t;

  public:
    DrawElemV();

    virtual ~DrawElemV();

    struct Elem
    {
      qfloat32 x, y, z;
    };

    const Elem *getElems() const { return m_pData; }
    virtual const void *getData() const { return m_pData; }
    virtual size_t getElemSize() const { return sizeof(Elem); }

    virtual int getType() const { return VA_V; }

    virtual void alloc(int nsize);

    virtual bool vertex(int ind, const Vector4D &v);

    // virtual bool vertexfp(int ind, const qfloat32 *pcrd);
    inline void vertexfp(int ind, const qfloat32 *pcrd) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].x = pcrd[0];
      m_pData[ind].y = pcrd[1];
      m_pData[ind].z = pcrd[2];
    }
    inline void vertex3f(int ind, const Vector3F &v) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].x = v.x();
      m_pData[ind].y = v.y();
      m_pData[ind].z = v.z();
    }

    bool getVertex(int ind, Vector4D &v) const;

  private:
      Elem *m_pData;

  };

  /// Draw element with vertex, normal and color
  class GFX_API DrawElemVNC : public DrawElem
  {
    typedef DrawElem super_t;

  public:
    DrawElemVNC();

    virtual ~DrawElemVNC();

    struct Elem
    {
      qfloat32 x, y, z;
      qfloat32 nx, ny, nz;
      qbyte r, g, b, a;
    };

    Elem *m_pData;

    const Elem *getElems() const { return m_pData; }
    virtual const void *getData() const { return m_pData; }
    virtual size_t getElemSize() const { return sizeof(Elem); }

    virtual int getType() const { return VA_VNC; }

    virtual void alloc(int nsize);

    virtual bool vertex(int ind, const Vector4D &v);

    virtual bool color(int ind, quint32 cc);

    virtual bool normal(int ind, const Vector4D &v);

    inline void vertexfp(int ind, const qfloat32 *pcrd) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].x = pcrd[0];
      m_pData[ind].y = pcrd[1];
      m_pData[ind].z = pcrd[2];
    }

    inline void vertex3f(int ind, const Vector3F &v) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].x = v.x();
      m_pData[ind].y = v.y();
      m_pData[ind].z = v.z();
    }

    inline void normalfp(int ind, const qfloat32 *pcrd) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].nx = pcrd[0];
      m_pData[ind].ny = pcrd[1];
      m_pData[ind].nz = pcrd[2];
    }

    inline void normal3f(int ind, const Vector3F &v) {
      MB_ASSERT(ind>=0 && getSize()>ind);
      m_pData[ind].nx = v.x();
      m_pData[ind].ny = v.y();
      m_pData[ind].nz = v.z();
    }

    ////////
    // access methods
    bool getVertex(int ind, Vector4D &v) const;
    bool getNormal(int ind, Vector4D &v) const;

  };

  /// Draw element with vertex, normal, color, and index
  class GFX_API DrawElemVNCI : public DrawElemVNC
  {
    typedef DrawElemVNC super_t;

  public:
    typedef quint16 index_t;

    DrawElemVNCI();

    virtual ~DrawElemVNCI();

    //index_t *getIndexData() const { return m_pIndData; }
    virtual const void *getIndData() const { return m_pIndData; }

    virtual size_t getIndElemSize() const {
      return sizeof(index_t);
    }

    virtual size_t getIndSize() const {
      return m_nIndSize;
    }

    virtual int getType() const { return VA_VNCI; }

    /// allocate index buffer (for general use)
    void allocIndex(int ninds);

    void setIndex(int ind, index_t n1) {
      MB_ASSERT( ind <m_nIndSize);
      m_pIndData[ind] = n1;
    }

    /// start indexed triangles mode (shortcut method)
    void startIndexTriangles(int nverts, int nfaces) {
      //DrawElem::startTriangles(nverts);
      super_t::setDrawMode(DRAW_TRIANGLES);
      super_t::alloc(nverts);
      allocIndex(nfaces*3);
    }

    /// set face index for triangles mode (shortcut method)
    void setIndex3(int ind, index_t n1, index_t n2, index_t n3) {
      MB_ASSERT( (ind*3+2) <m_nIndSize);
      m_pIndData[ind*3 + 0] = n1;
      m_pIndData[ind*3 + 1] = n2;
      m_pIndData[ind*3 + 2] = n3;
    }

  private:
    int m_nIndSize;
    index_t *m_pIndData;


  };

  /// Draw element with vertex, normal, color, and index (32-bit)
  class GFX_API DrawElemVNCI32 : public DrawElemVNC
  {
    typedef DrawElemVNC super_t;

  public:
    typedef quint32 index_t;

    DrawElemVNCI32();

    virtual ~DrawElemVNCI32();

    // index_t *getIndexData() const { return m_pIndData; }
    virtual const void *getIndData() const { return m_pIndData; }

    virtual size_t getIndElemSize() const {
      return sizeof(index_t);
    }

    virtual size_t getIndSize() const {
      return m_nIndSize;
    }

    virtual int getType() const { return VA_VNCI32; }

    /// allocate index buffer (for general use)
    void allocIndex(int ninds);

    void setIndex(int ind, index_t n1) {
      MB_ASSERT( ind <m_nIndSize);
      m_pIndData[ind] = n1;
    }

    /// start indexed triangles mode (shortcut method)
    void startIndexTriangles(int nverts, int nfaces) {
      //DrawElem::startTriangles(nverts);
      super_t::setDrawMode(DRAW_TRIANGLES);
      super_t::alloc(nverts);
      allocIndex(nfaces*3);
    }

    /// set face index for triangles mode (shortcut method)
    void setIndex3(int ind, index_t n1, index_t n2, index_t n3) {
      MB_ASSERT( (ind*3+2) <m_nIndSize);
      m_pIndData[ind*3 + 0] = n1;
      m_pIndData[ind*3 + 1] = n2;
      m_pIndData[ind*3 + 2] = n3;
    }

    int getIndexSize() const { return m_nIndSize; }

  private:
    int m_nIndSize;
    index_t *m_pIndData;
  };

  //////////////////////////////////////////////////////
  
  class PixelBuffer;

  /// Draw element for pixel data (2D bitmap)
  class GFX_API DrawElemPix : public DrawElem
  {
    typedef DrawElem super_t;

  public:
    DrawElemPix();

    virtual ~DrawElemPix();

    virtual int getType() const;

    /// implemented but should not be used
    virtual void alloc(int nsize);
    virtual bool vertex(int ind, const Vector4D &v);

    // dummy??
    virtual const void *getData() const { return NULL; }
    virtual size_t getElemSize() const { return 0; }

    //////////

    /// Image data
    PixelBuffer *m_pPixBuf;
    
    Vector4D m_pos;
    quint32 m_color;

    void setup(const PixelBuffer &src, const Vector4D &pos, quint32 color);

  };

  //////////////////////////////////////////////////////

  /// Draw to file (wrapper for Mesh; compatible with DrawElemVNCI32) 
  class GFX_API DrawFileVNCI : public DrawElem
  {
    typedef DrawElem super_t;

  private:
    ColorPtr m_pCurCol;
    Mesh m_mesh;

  public:
    typedef quint32 index_t;

    DrawFileVNCI() {}

    virtual ~DrawFileVNCI() {}


    virtual bool vertex(int ind, const Vector4D &v) {
      m_mesh.setVertex2(ind, v);
      return true;
    }

    inline void vertex3f(int ind, const Vector3F &v) {
      vertex(ind, Vector4D(v.x(), v.y(), v.z()));
    }

    virtual bool normal(int ind, const Vector4D &v) {
      m_mesh.setNormal2(ind, v);
      return true;
    }

    inline void normal3f(int ind, const Vector3F &v) {
      normal(ind, Vector4D(v.x(), v.y(), v.z()));
    }

    virtual bool color(int ind, quint32 cc) {
      m_mesh.setColor2(ind, ColorPtr(MB_NEW SolidColor(cc)));
      return true;
    }
    virtual void color2(const ColorPtr &col, qlib::uid_t nSceneID) {
      m_pCurCol = col;
    }
    virtual bool color2(int ind) {
      m_mesh.setColor2(ind, m_pCurCol);
      return true;
    }


    virtual int getType() const { return VA_VNCI32; }

    virtual void invalidateCache() const {}

    virtual const void *getData() const {
      return NULL;
    }

    virtual size_t getElemSize() const {
      return 0;
    }

    virtual const void *getIndData() const { return NULL; }

    virtual size_t getIndElemSize() const {
      return sizeof(index_t);
    }

    virtual size_t getIndSize() const {
      return m_mesh.getFaceSize()*3;
    }


    /// allocate index buffer (for general use)
    virtual void alloc(int nsize) {
      m_mesh.initVerts(nsize);
    }

    void allocIndex(int ninds) {
      m_mesh.initFaces(ninds/3);
    }

    void setIndex(int ind, index_t n1) {
      m_mesh.setFace2(ind, n1);
    }

    /// start indexed triangles mode (shortcut method)
    void startIndexTriangles(int nverts, int nfaces) {
      setDrawMode(DRAW_TRIANGLES);
      alloc(nverts);
      allocIndex(nfaces*3);
    }

    /// set face index for triangles mode (shortcut method)
    void setIndex3(int ind, index_t n1, index_t n2, index_t n3) {
      //MB_ASSERT( (ind*3+2) <m_nIndSize);
      m_mesh.setFace(ind, n1, n2, n3);
    }

    int getSize() const { return m_mesh.getVertSize(); }
    //int getIndexSize() const {  }

    Mesh *getMesh() {
      return &m_mesh;
    }

  };

}

#endif

