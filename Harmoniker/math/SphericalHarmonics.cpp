//
//  SphericalHarmonics.cpp
//  Harmoniker
//
//  Created by David Gavilan on 9/2/12.
//  Copyright (c) 2012 David Gavilan. All rights reserved.
//  Ref. "Spherical Harmonics Lighting: The Gritty Details", 
//      Robin Green
//

#include <stdlib.h>
#include "SphericalHarmonics.h"

MATH_NS_BEGIN

/** 
 * Constructor
 * @param numBands Number of Bands (default = 3)
 * @param numSamplesSqr Sqrt of number of samples (default = 500)
 */
SphericalHarmonics::SphericalHarmonics(int numBands, int numSamplesSqr)
: m_numBands(numBands)
, m_numCoeffs(numBands*numBands)
, m_numSamples(numSamplesSqr*numSamplesSqr)
{
    m_pSamples = (SHSample*)malloc(m_numSamples*sizeof(SHSample));
    for (int i=0;i<m_numSamples;++i) {
        m_pSamples[i].coeff = (double*)malloc(m_numCoeffs*sizeof(double));
    }
    m_pCoeffs = (Vector3*)malloc(m_numCoeffs*sizeof(Vector3));
    for (int i=0;i<m_numCoeffs;++i) {
        m_pCoeffs[i]=Vector3::ZERO;
    }
    setupSphericalSamples(m_pSamples, numSamplesSqr);

}

SphericalHarmonics::~SphericalHarmonics()
{
    for (int i=0;i<m_numSamples;++i) {
        free(m_pSamples[i].coeff);
    }
    free(m_pSamples);
    free(m_pCoeffs);
}


/// evaluate an Associated Legendre Polynomial P(l,m,x) at x
double SphericalHarmonics::P(int l,int m,double x)
{
    double pmm = 1.0;
    if(m>0) {
        double somx2 = sqrt((1.0-x)*(1.0+x)); 
        double fact = 1.0;
        for(int i=1; i<=m; i++) {
            pmm *= (-fact) * somx2;
            fact += 2.0; }
    }
    if(l==m) return pmm;
    double pmmp1 = x * (2.0*m+1.0) * pmm;
    if(l==m+1) return pmmp1;
    double pll = 0.0;
    for(int ll=m+2; ll<=l; ++ll) {
        pll = ( (2.0*ll-1.0)*x*pmmp1-(ll+m-1.0)*pmm ) / (ll-m);
        pmm = pmmp1;
        pmmp1 = pll;
    }
    return pll;
}

/// renormalization constant for SH function
double SphericalHarmonics::K(int l, int m)
{
    double temp = ((2.0*l+1.0)*Factorial(l-m)) / (4.0*PI*Factorial(l+m)); 
    return sqrt(temp);
}
/**
 * @return a point sample of a Spherical Harmonic basis function
 *  l is the band, range [0..N]
 *  m in the range [-l..l]
 *  theta in the range [0..Pi]
 *  phi in the range [0..2*Pi]
 */
double SphericalHarmonics::SH(int l, int m, double theta, double phi)
{

    const double sqrt2 = sqrt(2.0);
    if(m==0) return K(l,0)*P(l,m,cos(theta));
    else if(m>0) return sqrt2*K(l,m)*cos(m*phi)*P(l,m,cos(theta));
    else return sqrt2*K(l,-m)*sin(-m*phi)*P(l,-m,cos(theta));
}


/**
 * @brief Initializes the SHSamples
 * fill an N*N*2 array with uniformly distributed
 * samples across the sphere using jittered stratification
 */
void SphericalHarmonics::setupSphericalSamples(SHSample* samples, int sqrt_n_samples) {
    int i=0; // array index
    double oneoverN = 1.0/sqrt_n_samples;
    for(int a=0; a<sqrt_n_samples; a++) {
        for(int b=0; b<sqrt_n_samples; b++) {
            // generate unbiased distribution of spherical coords
            double x=(a+Randf())*oneoverN; //do not reuse results
            double y=(b+Randf())*oneoverN; //each sample must be random
            double theta = 2.0 * acos(sqrt(1.0 - x));
            double phi = 2.0 * PI * y;
            samples[i].sph = Spherical(1.0f, (float)theta, (float)phi);
            // convert spherical coords to unit vector
            samples[i].vec = samples[i].sph.ToVector3();
            // precompute all SH coefficients for this sample
            for(int l=0; l<m_numBands; ++l) {
                for(int m=-l; m<=l; ++m) {
                    int index = l*(l+1)+m;
                    samples[i].coeff[index] = SH(l,m,theta,phi);
                }
            }
            ++i; 
        }
    }
}

/**
 * Projects a polar function and computes the SH Coeffs
 * @param fn the Polar Function. If the polar function is an image, pass a function that retrieves (R,G,B) values from it given a spherical coordinate.
 */
Vector3* SphericalHarmonics::ProjectPolarFn(polarFn fn)
{
    const double weight = 4.0*PI;
    // for each sample
    for(int i=0; i<m_numSamples; ++i) {
        double theta = m_pSamples[i].sph.GetInclination();
        double phi   = m_pSamples[i].sph.GetAzimuth();
        for(int n=0; n<m_numCoeffs; ++n) {
            m_pCoeffs[n] += fn(theta,phi) * m_pSamples[i].coeff[n];
        }
    }
    // divide the result by weight and number of samples
    double factor = weight / m_numSamples;
    for(int i=0; i<m_numCoeffs; ++i) {
        m_pCoeffs[i] *= (float)factor;
    }
    return m_pCoeffs;
}



MATH_NS_END

