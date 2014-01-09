#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <string.h>

typedef float Float;
#define FLOAT_SCANF_FORMAT "%f"
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

static inline void vector_copy(Vector dest, const Vector src)
{
	memcpy(dest, src, sizeof(Vector));
}

static inline void vector_normalize(Vector dest, const Vector vec)
{
	Float length = sqrt(vector_lengthsq(vec));
	for(int i = 0; i < 3; i++)
		dest[i] = vec[i] / length;
}

static inline void vector_cross(Vector dest, const Vector a, const Vector b)
{
    dest[0] = a[1] * b[2] - b[1] * a[2];
    dest[1] = a[2] * b[0] - b[2] * a[0];
    dest[2] = a[0] * b[1] - b[1] * a[0];
}

#endif
