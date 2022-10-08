
#include <objects.hpp>
#include <linalg.hpp>

// -----------------------------------------------------------------------------
sphere_s::sphere_s(
        const FLOT3 &position,
        FLOAT radius,
        const FLOT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        position(position),
        radius(radius),
        color(color),
        specular(specular),
        reflective(reflective),
        _padding0(),
        _padding1()
{}

// -----------------------------------------------------------------------------
plane_s::plane_s(
        const FLOT3 &point,
        const FLOT3 &normal,
        const FLOT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        point(point),
        normal(normal),
        color(color),
        specular(specular),
        reflective(reflective)
{
    rtx::linalg::normalize_ref(this->normal);
}

// -----------------------------------------------------------------------------
triangle_s::triangle_s(
        const FLOT3 &p1,
        const FLOT3 &p2,
        const FLOT3 &p3,
        const FLOT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        a(p1),
        b(p2),
        c(p3),
        color(color),
        specular(specular),
        reflective(reflective)
{
    normal = rtx::linalg::cross(p2 - p1, p3 - p1);
}

// -----------------------------------------------------------------------------
cone_s::cone_s(
        const FLOT3 &position,
        const FLOT3 &direction,
        FLOAT width,
        FLOAT gamma,
        const FLOT3 &color,
        uint32_t specular,
        FLOAT reflective
    ) :
        position(position),
        direction(direction),
        width(width),
        gamma(gamma),
        color(color),
        specular(specular),
        reflective(reflective)
{
    rtx::linalg::normalize_ref(this->direction);
    rtx::linalg::set_rotation_matrix(this->matr, this->direction, {0, 0, 1});
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
        position(position),
        direction(direction),
        radius(radius),
        color(color),
        specular(specular),
        reflective(reflective)
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
        position(position),
        normal(normal),
        r(r),
        R(R),
        color(color),
        specular(specular),
        reflective(reflective)
{
    rtx::linalg::normalize_ref(this->normal);
}

// -----------------------------------------------------------------------------
light_s::light_s(
        light_type_t type,
        FLOAT intensity,
        const FLOAT3 &color,
        const FLOAT3 &vec={}
    ) :
        type(type),
        intensity(intensity),
        color(color),
        direction(vec)
{
    if (type != AMBIENT and type != DIRECT and type != POINT)
        throw std::runtime_error("unknown light type");
}
