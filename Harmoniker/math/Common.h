//	Copyright 2012 David Gavilan. All rights reserved.

/** @file Common.h
	@author David Gavilan
	@date 3/5/12
 */


#ifndef MATH_COMMON_H_
#define MATH_COMMON_H_ 

#include <stdlib.h>
#include "math/math_def.h"

MATH_NS_BEGIN

static const float PI = 3.1415926535897932384626433832795f ;
static const float PI_2 = 0.5f * PI ;
static const float PI2 = 2.f * PI ;
static const float PI_INV = 1.f / PI;
static const float NORM_SQR_ERROR_TOLERANCE = 0.001f;

/// Converts angle in degrees to radians
inline float DegToRad(const float angle) {
    return angle * (PI/180.f);
}
/// Gets the sign of a number
inline float Sign(const float n) {
    return (n>=0.f)?1.f:-1.f;
}
/// Max
inline float Max(const float a, const float b) {
    return (a>=b)?a:b;
}
/// Min
inline float Min(const float a, const float b) {
    return (a<=b)?a:b;
}
/// Random number between 0 and 1
inline float Randf() {
    return (random()%1001) / 1000.0f;
}
/// Factorial
double Factorial(int n);

MATH_NS_END


#endif
