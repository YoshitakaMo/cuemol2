//
// Vector class with 2 dimension real value
//

#ifndef __QLIB_VECTOR_2D_HPP__
#define __QLIB_VECTOR_2D_HPP__

#include "qlib.hpp"
//#include "VectorND.hpp"
#include "Vector2T.hpp"

namespace qlib {

  class Vector2D : public Vector2T<qfloat64>
  {
  public:
    typedef Vector2T<qfloat64> super_t;
    
  public:
    typedef qfloat64 value_type;

    ///////////////////////////////////////////////////
    // constructors

  public:

    /// Default constructor
    Vector2D()
      : super_t()
    {
    }

    /// Constructor without initialization
    Vector2D(int a, detail::no_init_tag b)
      : super_t(a, b)
    {
    }

    /// copy constructor
    Vector2D(const Vector2D &arg)
      : super_t(arg)
    {
    }

    /// construction from ptr
    explicit
    Vector2D(const value_type *parg)
      : super_t(parg)
    {
    }

    /// Implicit conversion
    Vector2D(const super_t &arg)
      : super_t(arg)
    {
    }

    /// Implicit conversion
    Vector2D(const super_t::super_t &arg)
      : super_t(arg)
    {
    }

    /// Cconversion
    template <typename _ArgType>
    explicit
    Vector2D(const Vector2T<_ArgType> &arg)
      : super_t(arg)
    {
    }

    Vector2D(value_type ax,value_type ay)
      : super_t(ax, ay)
    {
    }
  };

#if 0
  class Vector2D : public VectorND<2, LReal>
  {
  public:
    typedef VectorND<2, LReal> super_t;
    
  public:
    /////////////////
    // constructors

    /// Default constructor
    Vector2D()
    {
      super_t::zero();
    }

    /// Constructor without initialization
    explicit
    Vector2D(int, detail::no_init_tag)
    {
    }

    /// copy constructor
    Vector2D(const Vector2D &arg)
         : super_t(arg)
    {
    }

    /// Implicit conversion
    Vector2D(const super_t &arg)
         : super_t(arg)
    {
    }

    Vector2D(double ax,double ay)
    {
      super_t::ai(1) = ax;
      super_t::ai(2) = ay;
    }

  public:

    inline double x() const { return super_t::ai(1); }
    inline double y() const { return super_t::ai(2); }

    inline double &x() { return super_t::ai(1); }
    inline double &y() { return super_t::ai(2); }

    //////////////////////////////////////////
    // methods

    void set(double ax, double ay) {
      ai(1) = ax;
      ai(2) = ay;
    }

    inline double cross(const Vector2D &av) const {
      return x() * av.y() - y() * av.x();
    }

  };
#endif

  ///////////////////////////////////////////////
  // Definitions of non-member binary operators

  inline Vector2D operator+(const Vector2D &p1,const Vector2D &p2)
  {
    return p1.add(p2);
  }

  inline Vector2D operator-(const Vector2D &p1,const Vector2D &p2)
  {
    return p1.sub(p2);
  }

  inline bool operator==(const Vector2D &p1,const Vector2D &p2)
  {
    return p1.equals(p2);
  }

} // namespace qlib

#endif

