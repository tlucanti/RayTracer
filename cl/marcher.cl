
#ifndef MARCHER_CL
# define MARCHER_CL

# ifndef __CPP
#  include "kernel_common.h"
# endif /* __CPP */
# include "kernel.h"
# include "objects.h"
# include "algo.h"
# include "inc.h"
# include "tracing_base.cl"

# define as_sphere(obj_ptr) cstatic_cast(sphere_ptr, obj_ptr)

# define get_obj_color(obj_ptr) (as_sphere(obj_ptr)->color)

# define BLACK ASSIGN_FLOAT3(0., 0., 0.)
# define RTX_RECURSION_DEPTH 32
# define HIT_DISTANCE 0.01
# define MISS_DISTANCE 100

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
FLOAT box_dfs(
        FLOAT3 point,
        box_ptr box
    )
{
    FLOAT3 co = box->position - point;
    return length(fmax(fabs(co) - box->sides, 0.));
}

# define update_closest_t(__dsf_func, __obj_name)       \
do {                                                    \
    FLOAT t = __dsf_func(point, __obj_name + i);        \
                                                        \
    if (t < closest_t)                                  \
    {                                                   \
        closest_t = t;                                  \
        *closest_obj = __obj_name + i;                  \
    }                                                   \
} while (false)

CPP_UNUSED CPP_INLINE
FLOAT scene_dsf(scene_ptr scene, FLOAT3 point, void_ptr *closest_obj)
{
    FLOAT closest_t = INFINITY;

    for (uint32_t i=0; i < scene->spheres_num; ++i)
    {
        update_closest_t(sphere_dsf, scene->spheres);
        if (closest_t < HIT_DISTANCE)
            break ;
    }
    for (uint32_t i=0; i < scene->planes_num; ++i)
    {
        update_closest_t(plane_dsf, scene->planes);
        if (closest_t < HIT_DISTANCE)
            break ;
    }
    for (uint32_t i=0; i < scene->boxes_num; ++i)
    {
        update_closest_t(box_dfs, scene->boxes);
        if (closest_t < HIT_DISTANCE)
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
        if (closest_t < HIT_DISTANCE)
            break ;
    }
    for (uint32_t i=0; i < scene->planes_num; ++i)
    {
        closest_t = fmin(closest_t, plane_dsf(point, scene->planes + i));
        if (closest_t < HIT_DISTANCE)
            break ;
    }
    for (uint32_t i=0; i < scene->boxes_num; ++i)
    {
        closest_t = fmin(closest_t, box_dfs(point, scene->boxes + i));
        if (closest_t < HIT_DISTANCE)
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

        if (t < HIT_DISTANCE)
        {
            *point_ptr = camera;
            return closest_obj;
        }
        else if (t > MISS_DISTANCE)
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

# define __VERSION 5
#endif /* MARCHER_CL */
