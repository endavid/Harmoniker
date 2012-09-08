/*
 *  Quaternion.cpp
 *
 *  Created by David Gavilan on 12/11/10.
 *  Copyright 2010 David Gavilan. All rights reserved.
 *
 */

#include "Quaternion.h"

namespace vd {
namespace math {
	
	/// Returns a rotation matrix (column major, p' = M * p)
	Matrix3 Quat::ToMatrix3() const
	{
		const float w = m_w ;
		const float x = m_v.GetX() ;
		const float y = m_v.GetY() ;
		const float z = m_v.GetZ() ;
		const float w2 = w * w ;
		const float x2 = x * x ;
		const float y2 = y * y ;
		const float z2 = z * z ;
		Matrix3 m(0);
		
		m(0,0) = w2 + x2 - y2 - z2 ;
		m(0,1) = 2*x*y - 2*w*z ;
		m(0,2) = 2*x*z + 2*w*y ;
		m(1,0) = 2*x*y + 2*w*z ;
		m(1,1) = w2 - x2 + y2 - z2 ;
		m(1,2) = 2*y*z - 2*w*x ;
		m(2,0) = 2*x*z - 2*w*y ;
		m(2,1) = 2*y*z + 2*w*x ;
		m(2,2) = w2 - x2 - y2 + z2 ;
		
		return m ;
	}
    
    /// Returns a rotation matrix (column major, p' = M * p)
	Matrix4 Quat::ToMatrix4() const
	{
		const float w = m_w ;
		const float x = m_v.GetX() ;
		const float y = m_v.GetY() ;
		const float z = m_v.GetZ() ;
		const float w2 = w * w ;
		const float x2 = x * x ;
		const float y2 = y * y ;
		const float z2 = z * z ;
		Matrix4 m(0);
		
		m(0,0) = w2 + x2 - y2 - z2 ;
		m(0,1) = 2*x*y - 2*w*z ;
		m(0,2) = 2*x*z + 2*w*y ;
		m(1,0) = 2*x*y + 2*w*z ;
		m(1,1) = w2 - x2 + y2 - z2 ;
		m(1,2) = 2*y*z - 2*w*x ;
		m(2,0) = 2*x*z - 2*w*y ;
		m(2,1) = 2*y*z + 2*w*x ;
		m(2,2) = w2 - x2 - y2 + z2 ;
		m(3,3) = w2 + x2 + y2 + z2 ; // = 1 if unit quaternion
		
		return m ;
	}
	
}
}