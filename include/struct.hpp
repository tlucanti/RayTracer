
#ifndef STRUCT_HPP
# define STRUCT_HPP

# include <cllib>
# ifdef __APPLE__
#  include <math.h>
#  define sincos __sincos
# endif /* __APPLE__ */

# include <cmath>

typedef double FLOAT;
typedef cl_double3 FLOAT3;
typedef cl_double2 FLOAT2;

FLOAT3 operator +(const FLOAT3 &a, const FLOAT3 &b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

FLOAT3 operator -(const FLOAT3 &a)
{
    return {-a.x, -a.y, -a.z, -a.w};
}

FLOAT3 operator -(const FLOAT3 &a, const FLOAT3 &b)
{
    return a + (-b);
}

FLOAT3 operator *(const FLOAT3 &a, FLOAT t)
{
    return {a.x * t + a.y * t, a.z * t, a.w * t};
}

FLOAT3 operator +=(FLOAT3 &a, const FLOAT3 &b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

FLOAT3 cross(const FLOAT3 &a, const FLOAT3 &b)
{
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

namespace Color
{
    const FLOAT3 white = {255, 255, 255};
    const FLOAT3 black = {0, 0, 0};
    const FLOAT3 red = {255, 0, 0};
    const FLOAT3 green = {0, 255, 0};
    const FLOAT3 blue = {0, 0, 255};
    const FLOAT3 cyan = {0, 255, 255};
    const FLOAT3 magenta = {255, 0, 255};
    const FLOAT3 yellow = {255, 255, 0};
    const FLOAT3 purple = magenta;

    const FLOAT3 grey = {100, 100, 100};
}

FLOAT det(const FLOAT3 &v1, const FLOAT3 &v2, const FLOAT3 &v3)
{
    const FLOAT a = v1.x, b = v1.y, c = v1.z,
            d = v2.x, e = v2.y, f = v2.z,
            g = v3.x, h = v3.y, i = v3.z;
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

FLOAT dot(const FLOAT3 &v1, const FLOAT3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void normalize_ref(FLOAT3 &vec)
{
    FLOAT ln = 1. / sqrt(dot(vec, vec));
    vec.x *= ln;
    vec.y *= ln;
    vec.z *= ln;
}

#endif /* STRUCT_HPP */
