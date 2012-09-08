//
//  Common.cpp
//  Harmoniker
//
//  Created by David Gavilan on 9/2/12.
//  Copyright (c) 2012 David Gavilan. All rights reserved.
//

#include "math/Common.h"

MATH_NS_BEGIN

#define FACTORIAL_CACHE_NUM 33

namespace {
    bool _isFactorialCached = false;
    double _factorialCache[FACTORIAL_CACHE_NUM];
}

// Factorial of a number with a cache
double Factorial(int n)
{
    if (n < 2) return 1;
    if (n - 2 < FACTORIAL_CACHE_NUM) {
        if (!_isFactorialCached) {
            // init cache
            for (int c=0;c<FACTORIAL_CACHE_NUM;++c) {
                double r = 1;
                for (int i = 2; i <= c+2; ++i) {
                    r *= i;
                }
                _factorialCache[c] = r;
            }
        }
        return _factorialCache[n-2];
    } else {
        double r = 1;
        for (int i = 2; i <= n; ++i) {
            r *= i;
        }
        return r;
    }
}

MATH_NS_END
