/*
 *  Quaternion.h
 *
 *  Created by David Gavilan on 12/11/10.
 *  Copyright 2010 David Gavilan. All rights reserved.
 *
 */
#ifndef MATH_QUATERNION_H_
#define MATH_QUATERNION_H_

#include "Vector.h"
#include "math/Matrix.h"
#include "math/math_def.h"

// http://www.genesis3d.com/~kdtop/Quaternions-UsingToRepresentRotation.htm

MATH_NS_BEGIN

class Quat ;
inline Quat Inverse(const Quat& q);

// ===========================================================
/** @brief Class that represents a UNIT quaternion
 Note that only unit quaternions represent ROTATIONS!
 */
class Quat {
public:
    static const Quat ZERO;
    static const Quat UNIT;
public:
    // -----------------------------------------------------------
    // static methods
    // -----------------------------------------------------------
    static inline Quat CreateRotationAxis(const float angle, const Vector3& unitVector);
    
    // -----------------------------------------------------------
    // constructors
    // -----------------------------------------------------------
    Quat() :
    m_w(1), m_v(0,0,0)
    {}
    Quat(const float w, const Vector3& xyz) :
    m_w(w), m_v(xyz)
    {}
    Quat(const float w, const float x, const float y, const float z) :
    m_w(w), m_v(x,y,z)
    {}
    
    // -----------------------------------------------------------
    // getters
    // -----------------------------------------------------------
    inline const float GetW() const { return m_w; }
    inline const Vector3& GetXYZ() const { return m_v; }
    
    // -----------------------------------------------------------
    // setters
    // -----------------------------------------------------------
    inline Quat& SetW(const float w) { 
        m_w = w;
        return *this; 
    }
    
    inline Quat& SetXYZ(const Vector3& xyz) { 
        m_v = xyz;
        return *this; 
    }
    
    // -----------------------------------------------------------
    // functions
    // -----------------------------------------------------------
    inline float NormSqr() const {
        return m_w * m_w + m_v.GetX() * m_v.GetX() + m_v.GetY() * m_v.GetY() + m_v.GetZ() * m_v.GetZ();
    }
    inline float Norm() const {
        return sqrtf( NormSqr() );
    }
    inline Quat Normalize() const ;
	// Can be used the determine Quaternion neighbourhood
	inline float DotQ( const Quat& q ) const {
		return math::Dot(q.m_v,m_v)+ m_w * q.m_w;
	}    
    
    Matrix3 ToMatrix3() const;
    Matrix4 ToMatrix4() const;
    
    // -----------------------------------------------------------
    // operators
    // -----------------------------------------------------------
    /// avoid sums to preserve unit quats! use only for interpolation
    inline Quat& operator+=(const Quat& rhs) {
        m_w += rhs.m_w ;
        m_v += rhs.m_v ;
        return *this;
    }
    inline Quat operator+(const Quat& rhs) const {
        return Quat(*this) += rhs;
    }
    inline Quat& operator*=(const Quat& rhs) {
        float scalar = m_w * rhs.m_w - Dot(m_v, rhs.m_v) ;
        m_v = Cross(m_v, rhs.m_v) + m_w * rhs.m_v + rhs.m_w * m_v ;
        m_w = scalar;
        return *this;
    }
    inline Quat operator*(const Quat& rhs) const {
        return Quat(*this) *= rhs ;
    }
    /// scalar multiplication
    inline Quat& operator*=(const float rhs) {
        m_w *= rhs ;
        m_v *= rhs ;
        return *this ;
    }
    /// scalar multiplication
    inline Quat operator*(const float rhs) const {
        return Quat(*this) *= rhs ;
    }
    /// rotation of a vector by a UNIT quaternion
    inline Vector3 operator* (const Vector3 v) const {
        Quat p(0, v);
        p = (*this) * p * Inverse(*this);
        return p.GetXYZ();
    }
    
private:
    float	m_w ;	///< real part (scalar)
    Vector3	m_v ;	///< imaginary part (vector)
};


// ===========================================================
// static functions

// -----------------------------------------------------------
inline Quat Quat::CreateRotationAxis(const float angle, const Vector3& unitVector) {
    return Quat( cosf(0.5f * angle), sinf( 0.5f * angle ) * unitVector );
}

// -----------------------------------------------------------
/// Conjugate
inline Quat Conjugate(const Quat& q) {
    return Quat( q.GetW(), -q.GetXYZ() );
}
// -----------------------------------------------------------
/// Inverse
inline Quat Inverse(const Quat& q) {
    // assume it's a unit quaternion, so just Conjugate
    return Conjugate(q);
}
// -----------------------------------------------------------
/// Linear interpolation
inline Quat Lerp(const Quat& q1, const Quat& q2, float t) {
    return q1*(1.f-t) + q2*t; 
}
// -----------------------------------------------------------
/// Spherical linear interpolation
inline Quat Slerp(const Quat& q1, const Quat& q2, float t) {
    float w1, w2;
    
    float cosTheta = q1.DotQ(q2);
    float theta    = acosf(cosTheta);
    float sinTheta = sinf(theta);
    
    if( sinTheta > 0.001f )
    {
		w1 = float( sinf( (1.0f-t)*theta ) / sinTheta);
		w2 = float( sinf( t*theta) / sinTheta);
    } else {
		// CQuat a ~= CQuat b
		w1 = 1.0f - t;
		w2 = t;
    }    
    return q1*w1 + q2*w2; 
}

// -----------------------------------------------------------
/// similar rotations
inline bool IsClose( const Quat& a, const Quat&b, const float epsilon = 0.0001f ) {
    Vector3 diff = a.GetXYZ() - b.GetXYZ() ;
    float wdiff = a.GetW() - b.GetW() ;
    return IsClose( LengthSqr( diff ), epsilon ) && IsClose( wdiff * wdiff, epsilon );
}

/// normalize
inline Quat Quat::Normalize() const {
    // save some computation is is already normalized
    float r2 = NormSqr();
    if (fabsf(r2-1.0f)>NORM_SQR_ERROR_TOLERANCE) {
        float rInv = 1.f/sqrtf(r2);
        return Quat( rInv * m_w, rInv * m_v );
    }
    return (*this);
}


MATH_NS_END

#endif // MATH_QUATERNION_H_