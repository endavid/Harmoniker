//
//  Color.h
//  Harmoniker
//
//  Created by David Gavilan on 10/21/12.
//  Copyright (c) 2012 vid Games. All rights reserved.
//

#ifndef GFX_COLOR_H_
#define GFX_COLOR_H_

#include "gfx/gfx_def.h"
#include "math/Vector.h"

GFX_NS_BEGIN

// -----------------------------------------------------------
/**
 *  Class that encapsulates a Vector4 for the representation of colors.
 */
class Color {
public:
    static const double SRGB_GAMMA;

    static const Color WHITE;
    static const Color BLACK;
    static const Color CLEAR;
    
    enum ColorSpace {
        COLORSPACE_SRGB = 0,
        COLORSPACE_RGB,
        // COLORSPACE_XYZ
    };
    
public:
    /** Creates a color in sRGB color space */
    Color(const float r, const float g, const float b, const float a) :
    m_v(r,g,b,a),
    m_cs(COLORSPACE_SRGB)
    {}
    
    /** Creates a color in sRGB color space */
    Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255)
    : m_v(r,g,b,a)
    , m_cs(COLORSPACE_SRGB)
    {
        m_v = (1.f/255.f) * m_v ;
    }
    
    
    /** Creates a color in the given color space */
    Color(const ColorSpace cs, const math::Vector4& colordata) :
    m_v(colordata),
    m_cs(cs)
    {}
    
    // getters
    inline const float GetR() const { return m_v.GetX(); }
    inline const float GetG() const { return m_v.GetY(); }
    inline const float GetB() const { return m_v.GetZ(); }
    inline const float GetA() const { return m_v.GetW(); }

    inline math::Vector3 ToVector3() const { return m_v.GetXYZ(); }
    inline math::Vector4 ToVector4() const { return m_v; }


    Color ChangeColorSpace(const ColorSpace cs);
    
private:
    math::Vector4   m_v;
    ColorSpace      m_cs;
};


GFX_NS_END

#endif /* defined(GFX_COLOR_H_) */
