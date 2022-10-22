
#ifndef CL_MARCHER
# define CL_MARCHER

# ifndef __CPP
#  include "kernel_common.h"
# endif /* __CPP */
# include "kernel.h"
# include "objects.h"
# include "algo.h"
# include "inc.h"

# define as_sphere(obj_ptr) cstatic_cast(sphere_ptr, obj_ptr)

# define get_obj_color(obj_ptr) (as_sphere(obj_ptr)->color)

# define BLACK ASSIGN_FLOAT3(0., 0., 0.)
# define RTX_RECURSION_DEPTH 64

CPP_UNUSED CPP_INLINE
FLOAT sphere_dsf(
        FLOAT3 point,
        sphere_ptr sp
    )
{
    FLOAT d = length(point - sp->position);
    return d - sp->radius;
}

CPP_UNUSED CPP_INLINE
FLOAT plane_dsf(
        FLOAT3 point,
        plane_ptr pl
    )
{
    FLOAT3 co = pl->point - point;
    return fabs(dot(pl->normal, co));
}

CPP_UNUSED CPP_INLINE
FLOAT scene_dsf(scene_ptr scene, FLOAT3 point, void_ptr *closest_obj)
{
    FLOAT closest_t = INFINITY;

    for (uint32_t i=0; i < scene->spheres_num; ++i)
    {
        FLOAT t = sphere_dsf(point, scene->spheres + i);

        if (t < closest_t)
        {
            closest_t = t;
            *closest_obj = scene->spheres + i;
        }
        if (t < EPS)
            break ;
    }
    for (uint32_t i=0; i < scene->planes_num; ++i)
    {
        FLOAT t = plane_dsf(point, scene->planes + i);

        if (t < closest_t)
        {
            closest_t = t;
            *closest_obj = scene->planes + i;
        }
        if (t < EPS)
            break ;
    }
    return closest_t;
}

CPP_UNUSED CPP_INLINE
FLOAT shadow_dsf(scene_ptr scene, FLOAT3 point)
{
    FLOAT closest_t = INFINITY;

    for (uint32_t i=0; i < scene->spheres_num; ++i)
    {
        closest_t = fmin(closest_t, sphere_dsf(point, scene->spheres + i));
        if (closest_t < EPS)
            break ;
    }
    for (uint32_t i=0; i < scene->planes_num; ++i)
    {
        closest_t = fmin(closest_t, plane_dsf(point, scene->planes + i));
        if (closest_t < EPS)
            break ;
    }
    return closest_t;
}

CPP_UNUSED CPP_INLINE
FLOAT3 get_normal(scene_ptr scene, FLOAT3 point)
{
    FLOAT3 d1 = ASSIGN_FLOAT3(EPS, 0., 0.);
    FLOAT3 d2 = ASSIGN_FLOAT3(0., EPS, 0.);
    FLOAT3 d3 = ASSIGN_FLOAT3(0., 0., EPS);

    FLOAT3 v1 = ASSIGN_FLOAT3(
        shadow_dsf(scene, point + d1),
        shadow_dsf(scene, point + d2),
        shadow_dsf(scene, point + d3)
    );

    FLOAT3 v2 = ASSIGN_FLOAT3(
        shadow_dsf(scene, point - d1),
        shadow_dsf(scene, point - d2),
        shadow_dsf(scene, point - d3)
    );

    return normalize(v1 - v2);
//    return (v1 - v2) * (0.5 / EPS);
}

CPP_UNUSED CPP_INLINE
void_ptr closest_intersection(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT3 *__restrict point_ptr
    )
{
    void_ptr closest_obj = NULLPTR;

    for (uint32_t iter = 0; iter < RTX_RECURSION_DEPTH; ++iter)
    {
        FLOAT t = scene_dsf(scene, camera, &closest_obj);

        if (t < EPS)
        {
            *point_ptr = camera;
            return closest_obj;
        }
        else if (t > FARAWAY)
            break ;
        camera = camera + direction * t;
    }
    return NULLPTR;
}

CPP_UNUSED CPP_INLINE
FLOAT3 compute_lightning(
        scene_ptr scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        uint32_t specular,
        FLOAT3 *specular_computed
    )
{
    FLOAT3 color = BLACK;
    FLOAT end;
    FLOAT3 light_vector;

    for (uint32_t i=0; i < scene->lights_num; ++i)
    {
        switch (scene->lights[i].type)
        {
            case AMBIENT:
                color += scene->lights[i].color;
                continue ;
            case DIRECT:
                light_vector = -scene->lights[i].direction;
                end = INFINITY;
                break ;
            case POINT:
                light_vector = scene->lights[i].position - point;
                end = length(light_vector);
                light_vector *= 1. / end;
                break ;
        }

        // diffuse lightning
        FLOAT normal_angle = dot(normal, light_vector);
        if (normal_angle > EPS)
            color += scene->lights[i].color * normal_angle;

        // specular light
        if (specular > 0)
        {
            FLOAT3 reflected = reflect_ray(light_vector, normal);
            FLOAT reflected_angle = dot(reflected, direction);
            if (reflected_angle < EPS)
                *specular_computed += scene->lights[i].color * pow(reflected_angle, specular);
        }
    }
    return color;
}

CPP_UNUSED CPP_INLINE
FLOAT3 trace_ray(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction
    )
{
    void_ptr closest_obj;
    FLOAT3 color = BLACK;
    FLOAT3 point;
    FLOAT3 normal;

    closest_obj = closest_intersection(scene, camera, direction, &point);

    if (closest_obj == NULLPTR)
        return color;

    normal = get_normal(scene, point);
    FLOAT3 specular_val = ASSIGN_FLOAT3(0., 0., 0.);
    FLOAT3 factor = compute_lightning(
        scene,
        point,
        normal,
        direction,
        get_obj_specular(closest_obj),
        &specular_val
    );

    color = get_obj_color(closest_obj) * factor;
    color += specular_val * 255.;
    color.x = fmin(255., color.x);
    color.y = fmin(255., color.y);
    color.z = fmin(255., color.z);
    return color;
}

CPP_UNUSED CPP_INLINE
__kernel void ray_marcher(
        __global uint32_t *canvas,

        sphere_ptr spheres,
        plane_ptr planes,
        triangle_ptr triangles,
        cone_ptr cones,
        cylinder_ptr cylinders,
        torus_ptr torus,

        light_ptr lights,
        camera_ptr cameras,

        const uint32_t spheres_num,
        const uint32_t planes_num,
        const uint32_t triangles_num,
        const uint32_t cones_num,
        const uint32_t cylinders_num,
        const uint32_t torus_num,

        const uint32_t lights_num,
        const uint32_t cameras_num,

        const uint32_t width,
        const uint32_t height
    )
{
    const FLOAT rheight = 1. / height;
    const FLOAT rwidth = 1. / width;
    const uint32_t z = get_global_id(0);
    const uint32_t y = get_global_id(1);

    const scene_t scene = ASSIGN_SCENE(
        spheres,
        planes,
        triangles,
        cones,
        cylinders,
        torus,

        lights,
        cameras,

        spheres_num,
        planes_num,
        triangles_num,
        cones_num,
        cylinders_num,
        torus_num,

        lights_num,
        cameras_num
    );

    FLOAT3 vec = ASSIGN_FLOAT3(
            (z - width / 2.) * rwidth,                                              // FIXME: open brakets (simplify)
            (y - height / 2.) * rheight,
            1
    );
    vec = normalize(vec);
    vec = rotate_vector(vec, cameras[0].rotate_matrix);
//    FLOAT distance;
    const FLOAT3 color = trace_ray(
        &scene,
        cameras[0].position,
        vec
    );

    uint32_t pix_pos;

    pix_pos = (height - y - 1u) * (width) + z;
    const uint32_t int_color =
            cstatic_cast(uint32_t, color.x) << 16u
            | cstatic_cast(uint32_t, color.y) << 8u
            | cstatic_cast(uint32_t, color.z);
    canvas[pix_pos] = int_color;
}

# define __VERSION 3
#endif /* CL_MARCHER */
