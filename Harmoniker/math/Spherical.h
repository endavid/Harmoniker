/** @file Spherical.h
 @author David Gavilan
 @date 6/28/09.
 */
//	Copyright 2009 David Gavilan. All rights reserved.
#ifndef MATH_SPHERICAL_H_
#define MATH_SPHERICAL_H_

#include <math.h>
#include "Vector.h"
#include "math/math_def.h"

MATH_NS_BEGIN		

/** @brief Spherical Coordinates
 *
 *  About Spherical Coordinates: http://en.wikipedia.org/wiki/Spherical_coordinates
 * θ = Inclination {0,π}
 * φ = Azimuth {0,2π}
 *
 */
class Spherical {
public:
    
    // -----------------------------------------------------------
    // constructors
    // -----------------------------------------------------------		
    Spherical(const float& radialDistance, const float& inclination, const float& azimuth) :
    m_r(radialDistance),
    m_theta(inclination),
    m_phi(azimuth)
    {}
    
    // -----------------------------------------------------------
    // getters
    // -----------------------------------------------------------		
    inline const float GetRadialDistance() const;
    inline const float GetInclination() const;
    inline const float GetAzimuth() const;
    
    
    /// @todo define this as a casting function so we can do: Vector3 p = (Vector3)spherical_variable;
    inline Vector3 ToVector3() const ;
    
private:
    float m_r;		///< radial distance
    float m_theta;	///< inclination, in radians
    float m_phi;	///< azimuth, in radians
    
}; // class Spherical


// ===================================================================
// inline functions
float const Spherical::GetRadialDistance() const { return m_r; }
float const Spherical::GetInclination() const { return m_theta; }
float const Spherical::GetAzimuth() const { return m_phi; }

inline Vector3 Spherical::ToVector3() const {
    Vector3 v(m_r * sin(m_theta) * sin(m_phi),
              m_r * cos(m_theta),
              m_r * sin(m_theta) * cos(m_phi) );
    return v;
}

MATH_NS_END

#endif // MATH_SPHERICAL_H_

