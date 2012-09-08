/*
 *  Matrix.cpp
 *
 *  Created by David Gavilan on 10/12/09.
 *  Copyright 2009 David Gavilan. All rights reserved.
 *
 */

#include "Matrix.h"

MATH_NS_BEGIN

Matrix4 Matrix4::CreateFrustum(float left,float right,float bottom,float top,float near,float far)
{
    Matrix4 m(0);
    
#if 0
    m(0,0) = 2 * near / (right - left); 
    m(1,1) = 2 * near / (top - bottom); 
    m(2,0) = (right + left) / (right - left); 
    m(2,1) = (top + bottom) / (top - bottom); 
    m(2,2) = - (far + near) / (far - near); 
    m(2,3) = -1.f;
    m(3,2) = -2 * far * near / (far - near); 
#else
    m(0,0) = 2 * near / (right - left); 
    m(1,1) = 2 * near / (top - bottom); 
    m(0,2) = (right + left) / (right - left); 
    m(1,2) = (top + bottom) / (top - bottom); 
    m(2,2) = - (far + near) / (far - near); 
    m(3,2) = -1.f;
    m(2,3) = -2 * far * near / (far - near); 
#endif
    return m;
}
Matrix4 Matrix4::CreateOrtho(float left,float right,float bottom,float top, float near,float far)
{
    Matrix4 m(0);

    m(0,0) = 2 / (right - left);
    m(0,3) = (right + left) / (right - left);
    m(1,1) = 2 / (top - bottom);
    m(1,3) = (top + bottom) / (top - bottom);
    m(2,2) = -2 / (far - near);
    m(2,3) = (far + near) / (far - near);
    m(3,3) = 1.f;
    return m;
}
Matrix4 Matrix4::Perspective(float fov,float near,float far,float aspectRatio)
{
    float size = near * tanf(0.5f*DegToRad(fov)); 
    return CreateFrustum(-size, size, -size / aspectRatio,
                        size / aspectRatio, near, far);
}


MATH_NS_END