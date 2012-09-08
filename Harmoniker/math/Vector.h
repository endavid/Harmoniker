/** @file Vector.h
 @author David Gavilan
 @date 9/19/09.
 */
//	Copyright 2009 David Gavilan. All rights reserved.
#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_

#include <math.h>
#include <stdint.h>
#include "math/Common.h"
#include "math/math_def.h"

// http://en.wikibooks.org/wiki/C++_Programming/Operators/Operator_Overloading

MATH_NS_BEGIN

// -----------------------------------------------------------
class Vector2 {
public:
    static const Vector2 ZERO;
public:
    Vector2(const float x, const float y) : 
    m_x(x), m_y(y)
    {}
    Vector2(const float value) :
    m_x(value), m_y(value)
    {}
    Vector2() :
    m_x(0), m_y(0)
    {}
    
    // getters
    inline const float GetX() const { return m_x; }
    inline const float GetY() const { return m_y; }
    
    // setters
    inline Vector2& SetX(const float x) { 
        m_x = x;
        return *this; 
    }
    
    inline Vector2& SetY(const float y) { 
        m_y = y;
        return *this; 
    }
    
    inline Vector2 Normalize() const ;
    
    // -----------------------------------------------------------
    // functions
    // -----------------------------------------------------------
    /// returns the angle formed by the axis
    inline float Atan() const {
        float a = atan2f(m_y, m_x);
        return a < 0.f ? PI2 + a : a ;
    }
    
    // -----------------------------------------------------------
    // operators
    // -----------------------------------------------------------
    inline Vector2& operator=(const float value) {
        m_x = m_y = value;
        return *this;
    }
    inline Vector2& operator+=(const Vector2& rhs) {
        m_x += rhs.GetX();
        m_y += rhs.GetY();
        return *this;
    }
    inline Vector2 operator+(const Vector2& rhs) const {
        return Vector2(*this) += rhs;
    }
    inline Vector2 operator-=(const Vector2& rhs) {
        m_x -= rhs.GetX();
        m_y -= rhs.GetY();
        return *this;
    }
    inline Vector2 operator-(const Vector2& rhs) const {
        return Vector2(*this) -= rhs;
    }
    inline Vector2& operator*=(const float rhs) {
        m_x *= rhs ;
        m_y *= rhs ;
        return *this ;
    }
    inline Vector2 operator*(const float rhs) const {
        return Vector2(*this) *= rhs ;
    }
    
    
private:
    float	m_x;
    float	m_y;
};

// -----------------------------------------------------------
class Vector3 {
public:
    static const Vector3 ZERO;
    
public:
    Vector3(const float x, const float y, const float z) : 
    m_x(x), m_y(y), m_z(z)
    {}
    Vector3(const float value) :
    m_x(value), m_y(value), m_z(value)
    {}
    Vector3() :
    m_x(0), m_y(0), m_z(0)
    {}
    
    // getters
    inline const float GetX() const { return m_x; }
    inline const float GetY() const { return m_y; }
    inline const float GetZ() const { return m_z; }
    
    // setters
    inline Vector3& SetX(const float x) { 
        m_x = x;
        return *this; 
    }
    
    inline Vector3& SetY(const float y) { 
        m_y = y;
        return *this; 
    }
    
    inline Vector3& SetZ(const float z) { 
        m_z = z;
        return *this; 
    }
    
    inline Vector3 Normalize() const ;
    
    // -----------------------------------------------------------
    // operators
    // -----------------------------------------------------------
    inline Vector3& operator=(const float value) {
        m_x = m_y = m_z = value;
        return *this;
    }
    inline Vector3& operator+=(const Vector3& rhs) {
        m_x += rhs.GetX();
        m_y += rhs.GetY();
        m_z += rhs.GetZ();
        return *this;
    }
    inline Vector3 operator+(const Vector3& rhs) const {
        return Vector3(*this) += rhs;
    }
    inline Vector3 operator-=(const Vector3& rhs) {
        m_x -= rhs.GetX();
        m_y -= rhs.GetY();
        m_z -= rhs.GetZ();
        return *this;
    }
    inline Vector3 operator-(const Vector3& rhs) const {
        return Vector3(*this) -= rhs;
    }
    inline Vector3& operator*=(const float rhs) {
        m_x *= rhs ;
        m_y *= rhs ;
        m_z *= rhs ;
        return *this ;
    }
    inline Vector3 operator*(const float rhs) const {
        return Vector3(*this) *= rhs ;
    }
    inline Vector3 operator-() const {
        return Vector3(-m_x, -m_y, -m_z);
    }
    /// v(0) = 0;
    inline float& operator() (uint32_t row) {
        return *(&m_x + row);
    }
    /// x = v(0);
    inline float  operator() (uint32_t row) const {
        return *(&m_x + row);
    }
    
    inline const float* GetAsArray() const { return &m_x; }
    inline float* GetAsArray() { return &m_x; }
    
private:
    float	m_x;
    float	m_y;
    float	m_z;
}; // Vector3



// -----------------------------------------------------------


class Vector4 {
public:
    static const Vector4 ZERO;
    
public:
    Vector4(const float x, const float y, const float z, const float w) : 
    m_x(x), m_y(y), m_z(z), m_w(w)
    {}
    Vector4(const Vector3& xyz, const float w) :
    m_x(xyz.GetX()), m_y(xyz.GetY()), m_z(xyz.GetZ()), m_w(w)
    {}
    Vector4(const float value) : 
    m_x(value), m_y(value), m_z(value), m_w(value)
    {}
    Vector4() :
    m_x(0), m_y(0), m_z(0), m_w(0)
    {}
    
    // getters
    inline const float GetX() const { return m_x; }
    inline const float GetY() const { return m_y; }
    inline const float GetZ() const { return m_z; }
    inline const float GetW() const { return m_w; }
    inline Vector3 GetXYZ() const { return Vector3(m_x,m_y,m_z); }
    
    // setters
    inline Vector4& SetX(const float x) { 
        m_x = x;
        return *this; 
    }
    
    inline Vector4& SetY(const float y) { 
        m_y = y;
        return *this; 
    }
    
    inline Vector4& SetZ(const float z) { 
        m_z = z;
        return *this; 
    }
    
    inline Vector4& SetW(const float w) { 
        m_w = w;
        return *this; 
    }
    
    // operators
    inline Vector4& operator=(const float value) {
        m_x = m_y = m_z = m_w = value;
        return *this;
    }
    inline Vector4& operator+=(const Vector4& rhs) {
        m_x += rhs.GetX();
        m_y += rhs.GetY();
        m_z += rhs.GetZ();
        m_w += rhs.GetW();
        return *this;
    }
    inline Vector4 operator+(const Vector4& rhs) const {
        return Vector4(*this) += rhs;
    }
    inline Vector4& operator-=(const Vector4& rhs) {
        m_x -= rhs.GetX();
        m_y -= rhs.GetY();
        m_z -= rhs.GetZ();
        m_w -= rhs.GetW();
        return *this;
    }
    inline Vector4 operator-(const Vector4& rhs) const {
        return Vector4(*this) -= rhs;
    }
    /// v(0) = 0;
    inline float& operator() (uint32_t row) {
        return *(&m_x + row);
    }
    /// x = v(0);
    inline float  operator() (uint32_t row) const {
        return *(&m_x + row);
    }
    
    inline const float* GetAsArray() const { return &m_x; }
    inline float* GetAsArray() { return &m_x; }
    
private:
    float	m_x;
    float	m_y;
    float	m_z;
    float	m_w;	
};


// static functions
// -----------------------------------------------------------

inline Vector2 operator*(const float lhs, const Vector2& rhs) {
    return Vector2( lhs * rhs.GetX(), lhs * rhs.GetY() );
}
inline Vector3 operator*(const float lhs, const Vector3& rhs) {
    return Vector3( lhs * rhs.GetX(), lhs * rhs.GetY(), lhs * rhs.GetZ() );
}
inline Vector4 operator*(const float lhs, const Vector4& rhs) {
    return Vector4( lhs * rhs.GetX(), lhs * rhs.GetY(), lhs * rhs.GetZ(), lhs * rhs.GetW() );
}
// -----------------------------------------------------
/// dot product 
inline float Dot(const Vector4& lhs, const Vector4& rhs) {
    return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY() 
    + lhs.GetZ() * rhs.GetZ() + lhs.GetW() * lhs.GetW();
}
/// dot product 
inline float Dot(const Vector3& lhs, const Vector3& rhs) {
    return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY() 
    + lhs.GetZ() * rhs.GetZ();
}
/// dot product 
inline float Dot(const Vector2& lhs, const Vector2& rhs) {
    return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY() ;
}
// -----------------------------------------------------
/// multiply elements
inline Vector3 MulPerElem(const Vector3& lhs, const Vector3& rhs) {
    return Vector3( lhs.GetX() * rhs.GetX(), lhs.GetY() * rhs.GetY(), lhs.GetZ() * rhs.GetZ());
}
/// multiply elements
inline Vector4 MulPerElem(const Vector4& lhs, const Vector4& rhs) {
    return Vector4( lhs.GetX() * rhs.GetX(), lhs.GetY() * rhs.GetY(), lhs.GetZ() * rhs.GetZ(), lhs.GetW() * rhs.GetW() );
}
// -----------------------------------------------------
/// cross product
inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs) {
    // (a2b3 − a3b2, a3b1 − a1b3, a1b2 − a2b1)
    Vector3 perpendicular(
                          lhs.GetY() * rhs.GetZ() - lhs.GetZ() * rhs.GetY(),
                          lhs.GetZ() * rhs.GetX() - lhs.GetX() * rhs.GetZ(),
                          lhs.GetX() * rhs.GetY() - lhs.GetY() * rhs.GetX() ) ;
    return perpendicular;
}
// -----------------------------------------------------
/// square-length of a vector
inline float LengthSqr(const Vector4& lhs) {
    return Dot(lhs, lhs);
}
/// square-length of a vector
inline float LengthSqr(const Vector3& lhs) {
    return Dot(lhs, lhs);
}
/// square-length of a vector
inline float LengthSqr(const Vector2& lhs) {
    return Dot(lhs, lhs);
}

// -----------------------------------------------------
/// length of a vector
inline float Length(const Vector4& lhs) {
    return sqrtf( LengthSqr(lhs) );
}
/// length of a vector
inline float Length(const Vector3& lhs) {
    return sqrtf( LengthSqr(lhs) );
}
/// length of a vector
inline float Length(const Vector2& lhs) {
    return sqrtf( LengthSqr(lhs) );
}

// -----------------------------------------------------
/// normalize
inline Vector2 Vector2::Normalize() const {
    // save some computation is is already normalized
    float r2 = LengthSqr(*this);
    if (fabsf(r2-1.0f)>NORM_SQR_ERROR_TOLERANCE) {
        return ( (1.f/sqrtf(r2)) * (*this) );
    }
    return (*this);
}
/// normalize
inline Vector3 Vector3::Normalize() const {
    // save some computation is is already normalized
    float r2 = LengthSqr(*this);
    if (fabsf(r2-1.0f)>NORM_SQR_ERROR_TOLERANCE) {
        return ( (1.f/sqrtf(r2)) * (*this) );
    }
    return (*this);
}

// -----------------------------------------------------
/// similar/close numbers
inline bool IsClose( const float a, const float b, const float epsilon = 0.0001f ) {
    if ( fabsf( a - b ) < epsilon ) return true ;
    return false ;
}

/// similar vectors
inline bool IsClose( const Vector4& a, const Vector4&b, const float epsilon = 0.0001f ) {
    Vector4 diff = a - b ;
    return IsClose( LengthSqr( diff ), 0.f, epsilon );
}
/// similar vectors
inline bool IsClose( const Vector3& a, const Vector3&b, const float epsilon = 0.0001f ) {
    Vector3 diff = a - b ;
    return IsClose( LengthSqr( diff ), 0.f, epsilon );
}

// -----------------------------------------------------------
/// Inverse
inline Vector3 Inverse(const Vector3& v) {
    return Vector3( fabsf(v.GetX())>0.f ? 1.f/v.GetX() : 0.f,
                   fabsf(v.GetY())>0.f ? 1.f/v.GetY() : 0.f,
                   fabsf(v.GetZ())>0.f ? 1.f/v.GetZ() : 0.f );
}

// -----------------------------------------------------------
/// Inverse
inline Vector4 Inverse(const Vector4& v) {
    return Vector4( fabsf(v.GetX())>0.f ? 1.f/v.GetX() : 0.f,
                   fabsf(v.GetY())>0.f ? 1.f/v.GetY() : 0.f,
                   fabsf(v.GetZ())>0.f ? 1.f/v.GetZ() : 0.f,
                   fabsf(v.GetW())>0.f ? 1.f/v.GetW() : 0.f);
}

MATH_NS_END

#endif // MATH_VECTOR_H_