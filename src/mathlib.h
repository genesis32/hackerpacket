#ifndef MATHLIB_H
#define MATHLIB_H

#include <math.h>

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

static const double my_epsilon = 1.0e-05f;
static const double my_twopi   = 2 * M_PI;

inline float my_cos(float angle){
	return(float) cos(M_PI * ((float) angle / 180));
}

inline float my_sin(float angle){
	return(float) sin(M_PI * ((float) angle / 180));
}

#endif