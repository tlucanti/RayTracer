
#include <objects.h>
#include <linalg.hpp>

// -----------------------------------------------------------------------------
sphere_s::sphere_s(
        const FLOAT3 &position,
        FLOAT radius,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        radius(radius),
        position(position)
{}

// -----------------------------------------------------------------------------
plane_s::plane_s(
        const FLOAT3 &point,
        const FLOAT3 &normal,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        point(point),
        normal(normal)
{
    rtx::linalg::normalize_ref(this->normal);
}

// -----------------------------------------------------------------------------
triangle_s::triangle_s(
        const FLOAT3 &p1,
        const FLOAT3 &p2,
        const FLOAT3 &p3,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        a(p1),
        b(p2),
        c(p3)
{
    normal = rtx::linalg::cross(p2 - p1, p3 - p1);
}

// -----------------------------------------------------------------------------
cone_s::cone_s(
        const FLOAT3 &position,
        const FLOAT3 &direction,
        FLOAT width,
        FLOAT gamma,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        width(width),
        gamma(gamma),
        position(position),
        direction(direction)
{
    rtx::linalg::normalize_ref(this->direction);
    rtx::linalg::set_rotation_matrix(this->matr, this->direction, {{0, 0, 1}});
}

// -----------------------------------------------------------------------------
cylinder_s::cylinder_s(
        const FLOAT3 &position,
        const FLOAT3 &direction,
        FLOAT radius,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective
   ) :
        color(color),
        specular(specular),
        reflective(reflective),
        radius(radius),
        position(position),
        direction(direction)
{
    rtx::linalg::normalize_ref(this->direction);
}

// -----------------------------------------------------------------------------
torus_s::torus_s(
        const FLOAT3 &position,
        const FLOAT3 &normal,
        FLOAT r,
        FLOAT R,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        r(r),
        R(R),
        position(position),
        normal(normal)
{
    rtx::linalg::normalize_ref(this->normal);
}

// -----------------------------------------------------------------------------
light_s::light_s(
        light_type_t type,
        FLOAT intensity,
        const FLOAT3 &color,
        const FLOAT3 &vec
    ) :
        type(type),
        intensity(intensity),
        color(color),
        direction(vec)
{
    if (type != AMBIENT and type != DIRECT and type != POINT)
        throw std::runtime_error("unknown light type");
}

// -----------------------------------------------------------------------------
camera_s::camera_s(
        const FLOAT3 &pos, const FLOAT3 &dir
    ) :
        position(pos),
        direction(dir),
        rotate_matrix(),
        alpha(),
        theta()
{
    rtx::linalg::normalize_ref(direction);
    rtx::linalg::compute_angles(dir, alpha, theta);
    rtx::linalg::compute_matrix(rotate_matrix, alpha, theta);
}

// -----------------------------------------------------------------------------
void camera_s::recompute_matrix()
{
    rtx::linalg::compute_matrix(rotate_matrix, alpha, theta);
}
