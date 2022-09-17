
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
