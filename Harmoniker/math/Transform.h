/*
 *  Transform.h
 *
 *  Created by David Gavilan on 7/4/10.
 *  Copyright 2010 David Gavilan. All rights reserved.
 *
 */

#ifndef MATH_TRANSFORM_H_
#define MATH_TRANSFORM_H_

#include "math/Vector.h"
#include "math/Quaternion.h"
#include "math/Matrix.h"
#include "math/math_def.h"

MATH_NS_BEGIN

/// Transformation
class Transform {
public:
    // -----------------------------------------------------------
    // constructors
    // -----------------------------------------------------------		
    Transform() : // default: at origin, 0 rotation, size 1
    m_position( 0 ), m_size( 1 ), m_rotation( 1, 0, 0, 0 )
    {}
    
    // -----------------------------------------------------------
    // getters
    // -----------------------------------------------------------		
    inline const Vector3& GetPosition() const ;
    inline const Vector3& GetSize() const ;
    inline const Quat& GetRotation() const ;
    
    // -----------------------------------------------------------
    // setters
    // -----------------------------------------------------------		
    Transform& SetPosition( const math::Vector3& position ) {
        m_position = position ;
        return *this;
    }
    Transform& SetSize( const math::Vector3& size ) {
        m_size = size ;
        return *this ;
    }
    Transform& SetRotation( const float angle, const math::Vector3& axis ) {
        m_rotation = Quat::CreateRotationAxis(angle, axis) ;
        return *this ;
    }
    Transform& SetRotation( const Quat& quat ) {
        m_rotation = quat;
        return *this;
    }
    // -----------------------------------------------------------
    // functions
    // -----------------------------------------------------------
    Matrix4 ToMatrix4() const ;
    
    // -----------------------------------------------------------
    // operators
    // -----------------------------------------------------------
    /// applies transform to a vector
    inline Vector3 operator* (const Vector3 v) const {
        return m_position + m_rotation * MulPerElem(v,m_size) ;
    }
    
private:
    Vector3			m_position ;
    Vector3			m_size ;
    Quat			m_rotation ;
};

/// Transformation without scale (Rotation & Translation only)
class TransformRT {
public:
    // -----------------------------------------------------------
    // constructors
    // -----------------------------------------------------------		
    TransformRT() : // default: at origin, 0 rotation
    m_position( 0 ), m_rotation( 1, 0, 0, 0 )
    {}
    
    // -----------------------------------------------------------
    // getters
    // -----------------------------------------------------------		
    inline const Vector3& GetPosition() const ;
    inline const Quat& GetRotation() const ;
    
    // -----------------------------------------------------------
    // setters
    // -----------------------------------------------------------		
    TransformRT& SetPosition( const math::Vector3& position ) {
        m_position = position ;
        return *this;
    }
    TransformRT& SetRotation( const float angle, const math::Vector3& axis ) {
        m_rotation = Quat::CreateRotationAxis(angle, axis) ;
        return *this ;
    }
    TransformRT& SetRotation( const Quat& quat ) {
        m_rotation = quat;
        return *this;
    }
    // -----------------------------------------------------------
    // functions
    // -----------------------------------------------------------
    Matrix4 ToMatrix4() const ;
    
    // -----------------------------------------------------------
    // operators
    // -----------------------------------------------------------
    /// applies transform to a vector
    inline Vector3 operator* (const Vector3 v) const {
        return m_position + m_rotation * v ;
    }
    
private:
    Vector3			m_position ;
    Quat			m_rotation ;
};

// =========================================================================
// inline functions
inline const Vector3& Transform::GetPosition() const { return m_position ; }
inline const Vector3& Transform::GetSize() const { return m_size ; }
inline const Quat& Transform::GetRotation() const { return m_rotation; }
inline const Vector3& TransformRT::GetPosition() const { return m_position ; }
inline const Quat& TransformRT::GetRotation() const { return m_rotation; }

// ==========================================================================
// static functions

/// similar transforms
inline bool IsClose( const Transform& a, const Transform&b, const float epsilon = 0.0001f ) {
    return IsClose( a.GetPosition(), b.GetPosition(), epsilon ) 
    && IsClose( a.GetSize(), b.GetSize(), epsilon )
    && IsClose( a.GetRotation(), b.GetRotation(), epsilon );
}

// -----------------------------------------------------------
/// Inverse
inline Transform Inverse(const Transform& q) {
    return Transform().SetPosition(-q.GetPosition())
            .SetRotation(Inverse(q.GetRotation()))
            .SetSize(Inverse(q.GetSize()));
}
/// Inverse
inline TransformRT Inverse(const TransformRT& q) {
    return TransformRT().SetPosition(-q.GetPosition())
    .SetRotation(Inverse(q.GetRotation()));
}

MATH_NS_END

#endif