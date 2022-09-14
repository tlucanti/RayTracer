
#ifndef STRUCT_HPP
# define STRUCT_HPP

# include "cllib"
# ifdef __APPLE__
#  include <math.h>
#  define sincos __sincos
# endif /* __APPLE__ */

# define PACKED __attribute__((packed))
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

typedef struct sphere_s
{
    FLOAT3   center;
    FLOAT       radius;
    FLOAT3        color;
    int       specular;
    FLOAT       reflective;

    sphere_s(FLOAT3 center, FLOAT radius, FLOAT3 color, int specular, FLOAT reflective)
            : center(center), radius(radius), color(color), specular(specular), reflective(reflective)
    {}
} PACKED sphere_t;

typedef struct camera_s
{
    FLOAT3   position;
    FLOAT3   direction;

    FLOAT       alpha;
    FLOAT       theta;
    FLOAT3   rotate_matrix[3];

    camera_s(FLOAT3 position, FLOAT3 d)
            : position(position)
    {
        FLOAT length = 1.0f / sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
        direction = {d.x * length, d.y * length, d.z * length};

        alpha = atan2(d.x, d.z);
        if (std::isinf(alpha) or std::isnan(alpha))
            alpha = 0;

        theta = atan2(d.y, d.z * d.z + d.x * d.x);
        if (std::isinf(theta) or std::isnan(theta))
            theta = 0;
        recompute_matrix();
    }

    void recompute_matrix()
    {
        FLOAT	sin_alpha;
        FLOAT   cos_alpha;
        FLOAT   sin_theta;
        FLOAT   cos_theta;

        sincos(alpha, &sin_alpha, &cos_alpha);
        sincos(theta, &sin_theta, &cos_theta);

        rotate_matrix[0] = {cos_alpha, sin_alpha * sin_theta, sin_alpha * cos_theta};
        rotate_matrix[1] = {0, cos_theta, sin_theta};
        rotate_matrix[2] = {-sin_alpha, sin_theta * cos_alpha, cos_alpha * cos_theta};
    }
} PACKED camera_t;

typedef struct ambient_s
{
    FLOAT   intensity;
    FLOAT3    color;

    ambient_s(FLOAT intensity, FLOAT3 color)
            : intensity(intensity), color(color)
    {}
} PACKED ambient_t;

typedef struct point_s
{
    FLOAT3   position;
    FLOAT       intensity;
    FLOAT3        color;

    point_s(FLOAT3 position, FLOAT intencity, FLOAT3 color)
            : position(position), intensity(intencity), color(color)
    {}
} PACKED point_t;

typedef struct direct_s
{
    FLOAT3   direction;
    FLOAT       intensity;
    FLOAT3        color;

    direct_s(FLOAT3 d, FLOAT intensity, FLOAT3 color)
            : intensity(intensity), color(color)
    {
        FLOAT length = 1.0f / sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        direction = {d.x * length, d.y * length, d.z * length};
    }
} PACKED direct_t;

#endif /* STRUCT_HPP */
