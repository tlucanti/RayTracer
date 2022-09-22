
#ifndef STRUCT_HPP
# define STRUCT_HPP

# include <cllib>
# ifdef __APPLE__
#  include <math.h>
#  define sincos __sincos
# endif /* __APPLE__ */

# include <cmath>

# define EPS 1e-4
# define PI M_PI

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
    return {a.x * t, a.y * t, a.z * t, a.w * t};
}

FLOAT3 operator *(const FLOAT3 &a, const FLOAT3 &b)
{
    return {a.x * a.x, a.y * a.y, a.z * a.z};
}

FLOAT3 &operator +=(FLOAT3 &a, const FLOAT3 &b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

FLOAT3 &operator *=(FLOAT3 &a, FLOAT b)
{
    a.x *= b;
    a.y *= b;
    a.z *= b;
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

void compute_angles(const FLOAT3 &direction, FLOAT &alpha, FLOAT &theta)
{
    alpha = atan2(direction.x, direction.z);
    if (std::isinf(alpha) or std::isnan(alpha))
        alpha = 0;

    theta = -atan2(direction.y, (direction.z * direction.z + direction.x * direction.x));
    if (std::isinf(theta) or std::isnan(theta))
        theta = 0;
}

void compute_angles_gamma(const FLOAT3 &direction, FLOAT3 &atg)
{
    atg.x = atan2(direction.y, direction.z);
    if (std::isinf(atg.x) or std::isnan(atg.x))
        atg.x = 0;

    atg.y = atan2(direction.x, direction.z);
    if (std::isinf(atg.y) or std::isnan(atg.y))
        atg.y = 0;

    atg.z = atan2(direction.y, direction.x);
    if (std::isinf(atg.z) or std::isnan(atg.z))
        atg.z = 0;
}

void compute_matrix(FLOAT3 rotate_matrix[3], FLOAT alpha, FLOAT theta)
{
    FLOAT	sin_alpha;
    FLOAT   cos_alpha;
    FLOAT   sin_theta;
    FLOAT   cos_theta;

    sincos(alpha, &sin_alpha, &cos_alpha);
    sincos(theta, &sin_theta, &cos_theta);

    rotate_matrix[0] = {cos_alpha, sin_alpha * sin_theta, sin_alpha * cos_theta};
    rotate_matrix[1] = {0, cos_theta, -sin_theta};
    rotate_matrix[2] = {-sin_alpha, sin_theta * cos_alpha, cos_alpha * cos_theta};
}

void compute_matrix_gamma(FLOAT3 rotate_matrix[3], FLOAT3 atg)
{
    FLOAT  _sin[3];
    FLOAT  _cos[3];

    sincos(atg.x, _sin, _cos);
    sincos(atg.y, _sin + 1, _cos + 1);
    sincos(atg.z, _sin + 2, _cos + 2);
//     rotate_matrix[0] = {
//         _cos[0] * _cos[1],
//         _cos[0] * _sin[1] * _sin[2] - _sin[0] * _cos[2],
//         _cos[0] * _sin[1] * _cos[2] + _sin[0] * _sin[2]
//     };
//     rotate_matrix[1] = {
//         _sin[0] * _cos[1],
//         _sin[0] * _sin[1] * _sin[2] + _cos[0] * _cos[2],
//         _sin[0] * _sin[1] * _cos[2] - _cos[0] * _sin[2]
//     };
//     rotate_matrix[2] = {
//         -_sin[1],
//         _cos[1] * _sin[2],
//         _cos[1] * _cos[2]
//     };
	rotate_matrix[0] = {
		_cos[1] * _cos[2],
		_sin[0] * _sin[1] * _cos[2] - _cos[0] * _sin[2],
		_cos[0] * _sin[1] * _cos[2] + _sin[0] * _sin[2]
	};
	rotate_matrix[1] = {
		_cos[1] * _sin[2],
		_sin[0] * _sin[1] * _sin[2] + _cos[0] * _cos[2],
		_cos[0] * _sin[1] * _sin[2] - _sin[0] * _cos[2]
	};
	rotate_matrix[2] = {
		-_sin[1],
		_sin[0] * _cos[1],
		_cos[0] * _cos[1]
	};
}

void set_rotation_matrix(FLOAT3 *matrix, const FLOAT3 &v1, const FLOAT3 &v2)
{
    FLOAT3 k = cross(v1, v2);
    FLOAT cos_theta = dot(v1, v2);
    FLOAT sin_theta = sqrt(dot(k, k));
    if (abs(sin_theta) < EPS)
    {
        FLOAT factor = -2. * sin_theta + 1;
        matrix[0] = (FLOAT3){1, 0, 0} * factor;
        matrix[1] = (FLOAT3){0, 1, 0} * factor;
        matrix[2] = (FLOAT3){0, 0, 1} * factor;
        return ;
    }
    normalize_ref(k);
    FLOAT3 k2 = {k.x * k.x, k.y * k.y, k.z * k.z};

    std::cout << "k " << k.x << ' ' << k.y << ' ' << k.z << std::endl;
    std::cout << "k2 " << k2.x << ' ' << k2.y << ' ' << k2.z << std::endl;

    matrix[0] = {1, 0, 0};
    matrix[1] = {0, 1, 0};
    matrix[2] = {0, 0, 1};

    matrix[0] += (FLOAT3){0, -k.z, k.y} * sin_theta;
    matrix[1] += (FLOAT3){k.z, 0, -k.x} * sin_theta;
    matrix[2] += (FLOAT3){-k.y, k.x, 0} * sin_theta;

    matrix[0] += (FLOAT3){-k2.y - k2.z, k.x * k.y, k.x * k.z} * (1 - cos_theta);
    matrix[1] += (FLOAT3){k.x * k.y, -k2.x - k2.z, k.y * k.z} * (1 - cos_theta);
    matrix[2] += (FLOAT3){k.x * k.z, k.y * k.z, -k2.x - k2.y} * (1 - cos_theta);
}

#endif /* STRUCT_HPP */
