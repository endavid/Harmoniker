/*
 *  Transform.cpp
 *
 *  Created by David Gavilan on 7/4/10.
 *  Copyright 2010 David Gavilan. All rights reserved.
 *
 */

#include "Transform.h"

MATH_NS_BEGIN

// ---------------------------------------------------------
/// Converts Transform to Matrix4
Matrix4 Transform::ToMatrix4() const
{
    Matrix4 m = m_rotation.ToMatrix4();
    const Vector4 translation = Vector4( m_position, 1.f );
    m.SetCol(0, m_size.GetX() * m.GetCol(0));
    m.SetCol(1, m_size.GetY() * m.GetCol(1));
    m.SetCol(2, m_size.GetZ() * m.GetCol(2));    
    m.SetCol(3, translation);
    return m ;
}

// ---------------------------------------------------------
/// Converts Transform to Matrix4
Matrix4 TransformRT::ToMatrix4() const
{
    Matrix4 m = m_rotation.ToMatrix4();
    const Vector4 translation = Vector4( m_position, 1.f );
    m.SetCol(0, m.GetCol(0));
    m.SetCol(1, m.GetCol(1));
    m.SetCol(2, m.GetCol(2));
    m.SetCol(3, translation);
    return m ;
}

MATH_NS_END