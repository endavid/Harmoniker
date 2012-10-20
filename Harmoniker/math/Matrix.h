/*
 *  Matrix.h
 *
 *  Created by David Gavilan on 10/12/09.
 *  Copyright 2009 David Gavilan. All rights reserved.
 *
 */
#ifndef MATH_MATRIX_H_
#define MATH_MATRIX_H_

#include "Vector.h"
#include "math_def.h"

#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
// vDSP functions
#include <Accelerate/Accelerate.h>
#endif

// http://www.parashift.com/c++-faq-lite/operator-overloading.html#faq-13.10

MATH_NS_BEGIN

class Matrix3 {
public:
    static inline const Matrix3 Identity();
public:
    Matrix3(const float value)  {
        m_col[0] = Vector3(value);
        m_col[1] = Vector3(value);
        m_col[2] = Vector3(value);
    }
    
    inline const Vector3& GetCol(uint32_t col) const {
        return m_col[col];
    }
    inline void SetCol(uint32_t col, const Vector3& v) {
        m_col[col] = v ;
    }
    /// m(row,col) = f;
    inline float& operator() (uint32_t row, uint32_t col) {
        return m_col[col](row);
    }
    /// float f = m(row,col);
    inline float  operator() (uint32_t row, uint32_t col) const {
        return m_col[col](row);
    }
    /// operators
    inline Matrix3 operator*(const Matrix3& rhs) const;
    inline const float* GetAsArray() const;
    inline float* GetAsArray();
    
private:
    Vector3	m_col[3];
};


class Matrix4 {
public:
    static inline const Matrix4 Identity();
public:
    Matrix4() {
        m_col[0] = Vector4(0);
        m_col[1] = Vector4(0);
        m_col[2] = Vector4(0);
        m_col[3] = Vector4(0);
    }
    Matrix4(const float value)  {
        m_col[0] = Vector4(value);
        m_col[1] = Vector4(value);
        m_col[2] = Vector4(value);
        m_col[3] = Vector4(value);
    }
    
    inline const Vector4& GetCol(uint32_t col) const {
        return m_col[col];
    }
    inline void SetCol(uint32_t col, const Vector4& v) {
        m_col[col] = v ;
    }
    /// m(row,col) = f;
    inline float& operator() (uint32_t row, uint32_t col) {
        return m_col[col](row);
    }
    /// float f = m(row,col);
    inline float  operator() (uint32_t row, uint32_t col) const {
        return m_col[col](row);
    }
    /// operators
    inline Matrix4 operator*(const Matrix4& rhs) const;
    inline Vector4 operator*(const Vector4& rhs) const;
    inline const float* GetAsArray() const;
    inline float* GetAsArray();

    // rather don't use. Define View matrices using Transform.
    //static Matrix4 LookAt(const Vector3& position,const Vector3& target,const Vector3& up);

    
    // -----------------------------------------------------------------------
    // Projection Matrices
    //  Notice that the only way to represent projections is through matrices
    //  For other 3D transformations, better use the Transform class.
    // -----------------------------------------------------------------------
    static Matrix4 CreateFrustum(float left,float right,float bottom,float top,float near,float far);
    static Matrix4 CreateOrtho(float left,float right,float bottom,float top, float near,float far);
    static Matrix4 Perspective(float fov,float near,float far,float aspectRatio);

    
private:
    Vector4	m_col[4];
};


// ================================================================
// inline functions

inline const Matrix3 Matrix3::Identity() {
    Matrix3 m(0);
    m.m_col[0].SetX(1.f);
    m.m_col[1].SetY(1.f);
    m.m_col[2].SetZ(1.f);
    return m;
}

/// Matrix multiplication
inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const {
    Matrix3 m(0);
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    vDSP_mmul(const_cast<float*>(rhs.GetAsArray()), 1, const_cast<float*>(GetAsArray()), 1, m.GetAsArray(), 1, 3, 3, 3);
#else
    m(0,0) = (*this)(0,0)*rhs(0,0)+(*this)(0,1)*rhs(1,0)+(*this)(0,2)*rhs(2,0);
    m(0,1) = (*this)(0,0)*rhs(0,1)+(*this)(0,1)*rhs(1,1)+(*this)(0,2)*rhs(2,1);
    m(0,2) = (*this)(0,0)*rhs(0,2)+(*this)(0,1)*rhs(1,2)+(*this)(0,2)*rhs(2,2);
    m(1,0) = (*this)(1,0)*rhs(0,0)+(*this)(1,1)*rhs(1,0)+(*this)(1,2)*rhs(2,0);
    m(1,1) = (*this)(1,0)*rhs(0,1)+(*this)(1,1)*rhs(1,1)+(*this)(1,2)*rhs(2,1);
    m(1,2) = (*this)(1,0)*rhs(0,2)+(*this)(1,1)*rhs(1,2)+(*this)(1,2)*rhs(2,2);
    m(2,0) = (*this)(2,0)*rhs(0,0)+(*this)(2,1)*rhs(1,0)+(*this)(2,2)*rhs(2,0);
    m(2,1) = (*this)(2,0)*rhs(0,1)+(*this)(2,1)*rhs(1,1)+(*this)(2,2)*rhs(2,1);
    m(2,2) = (*this)(2,0)*rhs(0,2)+(*this)(2,1)*rhs(1,2)+(*this)(2,2)*rhs(2,2);
#endif
    
    return m;
}


const float* Matrix3::GetAsArray() const {
    return m_col[0].GetAsArray();
}
float* Matrix3::GetAsArray() {
    return m_col[0].GetAsArray();
}

// --------------------------------------------------

inline const Matrix4 Matrix4::Identity() {
    Matrix4 m(0);
    m.m_col[0].SetX(1.f);
    m.m_col[1].SetY(1.f);
    m.m_col[2].SetZ(1.f);
    m.m_col[3].SetW(1.f);
    return m;
}

/// Matrix multiplication
inline Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
    Matrix4 m(0);
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    vDSP_mmul(const_cast<float*>(rhs.GetAsArray()), 1, const_cast<float*>(GetAsArray()), 1, m.GetAsArray(), 1, 4, 4, 4);
#else
    m(0,0) = (*this)(0,0)*rhs(0,0)+(*this)(0,1)*rhs(1,0)+(*this)(0,2)*rhs(2,0)+(*this)(0,3)*rhs(3,0);
    m(0,1) = (*this)(0,0)*rhs(0,1)+(*this)(0,1)*rhs(1,1)+(*this)(0,2)*rhs(2,1)+(*this)(0,3)*rhs(3,1);
    m(0,2) = (*this)(0,0)*rhs(0,2)+(*this)(0,1)*rhs(1,2)+(*this)(0,2)*rhs(2,2)+(*this)(0,3)*rhs(3,2);
    m(0,3) = (*this)(0,0)*rhs(0,3)+(*this)(0,1)*rhs(1,3)+(*this)(0,2)*rhs(2,3)+(*this)(0,3)*rhs(3,3);
    m(1,0) = (*this)(1,0)*rhs(0,0)+(*this)(1,1)*rhs(1,0)+(*this)(1,2)*rhs(2,0)+(*this)(1,3)*rhs(3,0);
    m(1,1) = (*this)(1,0)*rhs(0,1)+(*this)(1,1)*rhs(1,1)+(*this)(1,2)*rhs(2,1)+(*this)(1,3)*rhs(3,1);
    m(1,2) = (*this)(1,0)*rhs(0,2)+(*this)(1,1)*rhs(1,2)+(*this)(1,2)*rhs(2,2)+(*this)(1,3)*rhs(3,2);
    m(1,3) = (*this)(1,0)*rhs(0,3)+(*this)(1,1)*rhs(1,3)+(*this)(1,2)*rhs(2,3)+(*this)(1,3)*rhs(3,3);
    m(2,0) = (*this)(2,0)*rhs(0,0)+(*this)(2,1)*rhs(1,0)+(*this)(2,2)*rhs(2,0)+(*this)(2,3)*rhs(3,0);
    m(2,1) = (*this)(2,0)*rhs(0,1)+(*this)(2,1)*rhs(1,1)+(*this)(2,2)*rhs(2,1)+(*this)(2,3)*rhs(3,1);
    m(2,2) = (*this)(2,0)*rhs(0,2)+(*this)(2,1)*rhs(1,2)+(*this)(2,2)*rhs(2,2)+(*this)(2,3)*rhs(3,2);
    m(2,3) = (*this)(2,0)*rhs(0,3)+(*this)(2,1)*rhs(1,3)+(*this)(2,2)*rhs(2,3)+(*this)(2,3)*rhs(3,3);
    m(3,0) = (*this)(3,0)*rhs(0,0)+(*this)(3,1)*rhs(1,0)+(*this)(3,2)*rhs(2,0)+(*this)(3,3)*rhs(3,0);
    m(3,1) = (*this)(3,0)*rhs(0,1)+(*this)(3,1)*rhs(1,1)+(*this)(3,2)*rhs(2,1)+(*this)(3,3)*rhs(3,1);
    m(3,2) = (*this)(3,0)*rhs(0,2)+(*this)(3,1)*rhs(1,2)+(*this)(3,2)*rhs(2,2)+(*this)(3,3)*rhs(3,2);
    m(3,3) = (*this)(3,0)*rhs(0,3)+(*this)(3,1)*rhs(1,3)+(*this)(3,2)*rhs(2,3)+(*this)(3,3)*rhs(3,3);
#endif
    
    return m;
}

/// Vector multiplication
inline Vector4 Matrix4::operator*(const Vector4& rhs) const {
    Vector4 v(0);
    
    v(0) = (*this)(0,0)*rhs(0)+(*this)(0,1)*rhs(1)+(*this)(0,2)*rhs(2)+(*this)(0,3)*rhs(3);
    v(1) = (*this)(1,0)*rhs(0)+(*this)(1,1)*rhs(1)+(*this)(1,2)*rhs(2)+(*this)(1,3)*rhs(3);
    v(2) = (*this)(2,0)*rhs(0)+(*this)(2,1)*rhs(1)+(*this)(2,2)*rhs(2)+(*this)(2,3)*rhs(3);
    v(3) = (*this)(3,0)*rhs(0)+(*this)(3,1)*rhs(1)+(*this)(3,2)*rhs(2)+(*this)(3,3)*rhs(3);

    return v;
}

const float* Matrix4::GetAsArray() const {
    return m_col[0].GetAsArray();
}
float* Matrix4::GetAsArray() {
    return m_col[0].GetAsArray();
}



MATH_NS_END

#endif // MATH_MATRIX_H_