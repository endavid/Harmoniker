//
//  Color.cpp
//  Harmoniker
//
//  Created by David Gavilan on 10/21/12.
//  Copyright (c) 2012 vid Games. All rights reserved.
//

#include "Color.h"

GFX_NS_BEGIN

// static constants
const double Color::SRGB_GAMMA = 2.4;
const Color Color::WHITE(1.f,1.f,1.f,1.f);
const Color Color::BLACK(0.f,0.f,0.f,1.f);
const Color Color::CLEAR(0.f,0.f,0.f,0.f);


Color Color::ChangeColorSpace(const vd::gfx::Color::ColorSpace cs)
{
    Color c(*this); // notice that alpha is not changed
    
    // same color space
    if (cs == m_cs) return c;
    
    if (m_cs == COLORSPACE_SRGB) { // first convert to linear RGB
        // SRGB -> RGB
        for (int i=0; i<3; ++i) {
            if ( m_v(i) > 0.04045 ) c.m_v(i) =(float)pow((m_v(i)+0.055)/1.055, SRGB_GAMMA);
            else c.m_v(i) = (m_v(i) / 12.92f);
        }
    }
    
    // RGB to other spaces
    switch (cs) {
        case COLORSPACE_SRGB:
            // RGB -> SRGB
            for (int i=0; i<3; ++i) {
                if ( c.m_v(i) > 0.00304f )
                    c.m_v(i) = (float)(1.055 * pow(c.m_v(i),(1.0/SRGB_GAMMA)) - 0.055);
                else
                    c.m_v(i) = (float)(12.92 * c.m_v(i));
            }
            break;
            
        default:
            // unknown conversion
            break;
    }
    
    return c;
    
}

GFX_NS_END
