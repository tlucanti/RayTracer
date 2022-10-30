
#include <objects.h>
#include <linalg.hpp>

// -----------------------------------------------------------------------------
sphere_s::sphere_s(
        const FLOAT3 &position,
        FLOAT radius,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        position(position),
        negative(negative),
        union_num(union_num),
        emission(emission),
        radius(radius)
{}

// -----------------------------------------------------------------------------
sphere_s::sphere_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    position(),
    negative(),
    union_num(),
    emission(),
    radius()
{}

// -----------------------------------------------------------------------------
plane_s::plane_s(
        const FLOAT3 &position,
        const FLOAT3 &normal,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        position(position),
        negative(negative),
        union_num(union_num),
        emission(emission),
        normal(normal)
{
    rtx::linalg::normalize_ref(this->normal);
}

// -----------------------------------------------------------------------------
plane_s::plane_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    position(),
    negative(),
    union_num(),
    emission(),
    normal()
{}

// -----------------------------------------------------------------------------
triangle_s::triangle_s(
        const FLOAT3 &p1,
        const FLOAT3 &p2,
        const FLOAT3 &p3,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        negative(negative),
        union_num(union_num),
        emission(emission),
        a(p1),
        b(p2),
        c(p3)
{
    a.y += 0.01;
    normal = rtx::linalg::cross(b - a, c - a);
    rtx::linalg::normalize_ref(normal);
}

// -----------------------------------------------------------------------------
triangle_s::triangle_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    negative(),
    union_num(),
    emission(),
    a(),
    b(),
    c()
{}

// -----------------------------------------------------------------------------
cone_s::cone_s(
        const FLOAT3 &position,
        const FLOAT3 &direction,
        FLOAT width,
        FLOAT gamma,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        position(position),
        negative(negative),
        union_num(union_num),
        emission(emission),
        width(width),
        gamma(gamma),
        direction(direction)
{
    rtx::linalg::normalize_ref(this->direction);
    rtx::linalg::set_rotation_matrix(this->matr, this->direction, {{0, 0, 1}});
}

// -----------------------------------------------------------------------------
cone_s::cone_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    position(),
    negative(),
    union_num(),
    emission(),
    width(),
    gamma(),
    direction()
{}

// -----------------------------------------------------------------------------
cylinder_s::cylinder_s(
        const FLOAT3 &position,
        const FLOAT3 &direction,
        FLOAT radius,
        FLOAT height,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
   ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        position(position),
        negative(negative),
        union_num(union_num),
        emission(emission),
        radius(radius),
        height(height),
        direction(direction)
{
    rtx::linalg::normalize_ref(this->direction);
}

// -----------------------------------------------------------------------------
cylinder_s::cylinder_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    position(),
    negative(),
    union_num(),
    emission(),
    radius(),
    height(),
    direction()
{}

// -----------------------------------------------------------------------------
torus_s::torus_s(
        const FLOAT3 &position,
        const FLOAT3 &normal,
        FLOAT r,
        FLOAT R,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        position(position),
        negative(negative),
        union_num(union_num),
        emission(emission),
        r(r),
        R(R),
        normal(normal)
{
    rtx::linalg::normalize_ref(this->normal);
    rtx::linalg::set_rotation_matrix(this->matr, this->normal, {{0, 0, 1}});
}

// -----------------------------------------------------------------------------
torus_s::torus_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    position(),
    negative(),
    union_num(),
    emission(),
    r(),
    R(),
    normal()
{}

// -----------------------------------------------------------------------------
box_t::box_s(
        const FLOAT3 &position,
        const FLOAT3 &sides,
        const FLOAT3 &color,
        uint32_t specular,
        FLOAT reflective,
        FLOAT refractive,
        FLOAT transparency,
        FLOAT emission,
        uint32_t negative,
        int32_t union_num
    ) :
        color(color),
        specular(specular),
        reflective(reflective),
        refractive(refractive),
        transparency(transparency),
        position(position),
        negative(negative),
        union_num(union_num),
        emission(emission),
        sides(sides)
{}

// -----------------------------------------------------------------------------
box_t::box_s() :
    color(),
    specular(),
    reflective(),
    refractive(),
    transparency(),
    position(),
    negative(),
    union_num(),
    emission(),
    sides()
{}

// -----------------------------------------------------------------------------
light_s::light_s(
        light_type_t type,
        FLOAT intensity,
        const FLOAT3 &color,
        const FLOAT3 &vec
    ) :
        color(color * intensity * (1. / 255.)),
        direction(vec),
        type(type)
{
    if (type != AMBIENT and type != DIRECT and type != POINT)
        throw std::runtime_error("unknown light type");
    if (type == DIRECT)
        rtx::linalg::normalize_ref(this->direction);
}

// -----------------------------------------------------------------------------
camera_s::camera_s(
        const FLOAT3 &pos,
        const FLOAT3 &dir
    ) :
        position(pos),
        direction(dir),
        rotate_matrix(),
        reverse_rotate_matrix(),
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
//    rtx::linalg::compute_matrix_gamma_abg(rotate_matrix, {{alpha, 0., theta}});
    rtx::linalg::compute_matrix(rotate_matrix,alpha, theta);
    }

// -----------------------------------------------------------------------------
void camera_s::recompute_reverse_matrix()
{
    rtx::linalg::inv(rotate_matrix, reverse_rotate_matrix);
}
