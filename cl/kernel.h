
#ifndef CL_KERNEL_H
# define CL_KERNEL_H

# define as_sphere(obj_ptr) cstatic_cast(sphere_ptr, obj_ptr)
# define as_plane(obj_ptr) cstatic_cast(plane_ptr, obj_ptr)
# define as_triangle(obj_ptr) cstatic_cast(triangle_ptr, obj_ptr)
# define as_cone(obj_ptr) cstatic_cast(cone_ptr, obj_ptr)
# define as_cylinder(obj_ptr) cstatic_cast(cylinder_ptr, obj_ptr)

# define get_obj_color(obj_ptr) (as_sphere(obj_ptr)->color)
# define get_obj_specular(obj_ptr) (as_sphere(obj_ptr)->specular)
# define get_obj_reflective(obj_ptr) (as_sphere(obj_ptr)->reflective)
# define get_obj_refractive(obj_ptr) (as_sphere(obj_ptr)->refractive)
# define get_obj_transparency(obj_ptr) (as_sphere(obj_ptr)->transparency)

# define BLACK ASSIGN_FLOAT3(0., 0., 0.)

# ifndef __CPP
#  define CPP_UNUSED
#  define CPP_INLINE
#  define ASSIGN_FLOAT3(__x, __y, __z) (FLOAT3)(__x, __y, __z)
#  define ASSIGN_COMPLEX(__real, __imag) (COMPLEX)(__real, __imag)
#  define ASSIGN_SCENE(...) (scene_t){__VA_ARGS__}
#  define cstatic_cast(__type, __var) ((__type)(__var))

#  define SPHRERE_PARAMS
#  define PLANE_PARAMS
#  define TRIANGLE_PARAMS
#  define CONE_PARAMS
#  define CYLINDER_PARAMS
#  define TORUS_PARAMS
#  define LIGHT_PARAMS
#  define CAMERA_PARAMS
# else
#  include <linalg.hpp>
#  define __constant
#  define __global
#  define __kernel
#  define CPP_UNUSED __attribute__((unused))
#  define CPP_INLINE inline
#  define NULLPTR nullptr
#  define ASSIGN_FLOAT3(__x, __y, __z) FLOAT3({{__x,__y, __z}})
#  define ASSIGN_COMPLEX(__real, __imag) {{__real, __imag}}
#  define ASSIGN_SCENE(...) {__VA_ARGS__}
#  define cstatic_cast(__type, __var) static_cast<__type>(__var)
#  define RTX_EMISSION
#  define RTX_DIRECT
#  define RTX_TRANSPARENCY
#  define RTX_REFRACTIVE

#  define SPHERE_PARAMS     \
    FL3_CREF position,      \
    FLOAT radius,           \
    FL3_CREF color,         \
    uint32_t specular,      \
    FLOAT reflective,       \
    FLOAT refractive,       \
    FLOAT transparency,     \
    FLOAT emission

#  define PLANE_PARAMS      \
    FL3_CREF point,         \
    FL3_CREF normal,        \
    FL3_CREF color,         \
    uint32_t specular,      \
    FLOAT reflective,       \
    FLOAT refractive,       \
    FLOAT transparency

#  define TRIANGLE_PARAMS   \
    FL3_CREF p1,            \
    FL3_CREF p2,            \
    FL3_CREF p3,            \
    FL3_CREF color,         \
    uint32_t specular,      \
    FLOAT reflective,       \
    FLOAT refractive,       \
    FLOAT transparency,     \
    FLOAT emission

#  define CONE_PARAMS       \
    FL3_CREF position,      \
    FL3_CREF direction,     \
    FLOAT width,            \
    FLOAT gamma,            \
    FL3_CREF color,         \
    uint32_t specular,      \
    FLOAT reflective,       \
    FLOAT refractive,       \
    FLOAT transparency

#  define CYLINDER_PARAMS   \
    FL3_CREF position,      \
    FL3_CREF direction,     \
    FLOAT radius,           \
    FLOAT height,           \
    FL3_CREF color,         \
    uint32_t specular,      \
    FLOAT reflective,       \
    FLOAT refractive,       \
    FLOAT transparency

#  define TORUS_PARAMS      \
    FL3_CREF position,      \
    FL3_CREF normal,        \
    FLOAT r,                \
    FLOAT R,                \
    FL3_CREF color,         \
    uint32_t specular,      \
    FLOAT reflective,       \
    FLOAT refractive,       \
    FLOAT transparency

#  define LIGHT_PARAMS      \
    light_type_t type,      \
    FLOAT intensity,        \
    FL3_CREF color,         \
    FL3_CREF vec

#  define CAMERA_PARAMS     \
    FL3_CREF pos,           \
    FL3_CREF dir

CPP_INLINE
uint32_t get_global_id(uint32_t) { return {}; }

using rtx::linalg::length;
using rtx::linalg::normalize;
using rtx::linalg::dot;
using rtx::linalg::cross;

using std::isnan;
using std::isinf;
using std::max;
using std::min;

# endif/* __CPP */

#endif /* CL_KERNEL_H */
