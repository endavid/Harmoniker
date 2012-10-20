//
//  SphericalHarmonics.h
//  Harmoniker
//
//  Created by David Gavilan on 9/2/12.
//  Copyright (c) 2012 David Gavilan. All rights reserved.
//


#ifndef MATH_SPHERICAL_HARMONICS_H_
#define MATH_SPHERICAL_HARMONICS_H_

#include "math/Common.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Spherical.h"

MATH_NS_BEGIN

/** SphericalHarmonics Samples */
struct SHSample {
    Spherical sph;
    Vector3 vec;
    double *coeff;
};

/**
 *  This class computes the Spherical Harmonics of an image
 */
class SphericalHarmonics {
public:
    /// Polar function
    typedef Vector3 (*polarFn)(double theta, double phi);
    
public:
    SphericalHarmonics(int numBands = 3, int numSamplesSqr = 100);
    ~SphericalHarmonics();
    
    // -----------------------------------------------------------
    // getters
    // -----------------------------------------------------------
    inline int GetNumBands() const { return m_numBands; }
    inline int GetNumCoeffs() const { return m_numCoeffs; }
    inline const Vector3* GetCoeffs() const { return m_pCoeffs; }
    
    // projects a polar function and computes the SH Coeffs
    Vector3* ProjectPolarFn(polarFn fn);
    // given a normal vector, retrieves the irradiance value
    Vector3 GetIrradianceApproximation(const Vector3& normal);
    
    // Associated Legendre Polynomial
    static double P(int l,int m,double x);
    // renormalization constant for SH function
    static double K(int l, int m);
    static double SH(int l, int m, double theta, double phi);
    
private:
    void setupSphericalSamples(SHSample samples[], int sqrt_n_samples);
    void computeIrradianceApproximationMatrices();
    
private:
    SHSample*   m_pSamples;         ///< SHSamples
    int         m_numBands;         ///< Number of bands
    int         m_numCoeffs;        ///< Number of coeffs
    int         m_numSamples;       ///< Number of samples
    Vector3*    m_pCoeffs;          ///< SH Coefficients (result)
    Matrix4     m_mIrradiance[3];   ///< Matrices used to approximate irradiance
    
}; // SphericalHarmonics

MATH_NS_END


#endif // MATH_SPHERICAL_HARMONICS_H_
