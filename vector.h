#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef double Float;
#define FLOAT_SCANF_FORMAT "%lf"
#define FLOAT_PRINTF_FORMAT "%.10g"

typedef Float Vector[3];

static inline Float vector_lengthsq(const Vector v)
{
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

static inline void vector_mul(Vector dest, Float k, const Vector v)
{
	for(int i = 0; i < 3; i++)
		dest[i] = k * v[i];
}

static inline void vector_add(Vector dest, const Vector v1, const Vector v2)
{
	for(int i = 0; i < 3; i++)
		dest[i] = v1[i] + v2[i];
}

static inline void vector_sub(Vector dest, const Vector v1, const Vector v2)
{
	for(int i = 0; i < 3; i++)
		dest[i] = v1[i] - v2[i];
}

#endif
