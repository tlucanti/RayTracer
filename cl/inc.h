
#ifndef CL_INC_H
# define CL_INC_H

# include "objects.h"
# include "kernel.h"

CPP_UNUSED
bool check_sphere(
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT3 position,
        FLOAT radius
);

CPP_UNUSED
FLOAT intersect_sphere(
        FLOAT3 camera,
        FLOAT3 direction,
        sphere_ptr sp
);

CPP_UNUSED
FLOAT intersect_plane(
        FLOAT3 camera,
        FLOAT3 direction,
        plane_ptr pl
);

CPP_UNUSED
FLOAT intersect_triangle(
        FLOAT3 camera,
        FLOAT3 direction,
        triangle_ptr tr
);

CPP_UNUSED
FLOAT intersect_cone(
        FLOAT3 camera,
        FLOAT3 direction,
        cone_ptr cn,
        FLOAT3 *param
);

CPP_UNUSED
FLOAT intersect_cylinder(
        FLOAT3 camera,
        FLOAT3 direction,
        cylinder_ptr cy
);

CPP_UNUSED
FLOAT intersect_torus(
        FLOAT3 camera,
        FLOAT3 direction,
        torus_ptr to,
        FLOAT3 *param
);

CPP_UNUSED
void_ptr closest_intersection(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end,
        FLOAT *closest_t_ptr,
        obj_type_t *closest_type_ptr,
        FLOAT3 *param
);

CPP_UNUSED
bool shadow_intersection(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end
);

CPP_UNUSED
FLOAT compute_lightning_single(
        FLOAT3 light_vector,
        FLOAT3 normal_vector,
        FLOAT3 direction,
        FLOAT light_intensity,
        uint32_t specular
);

CPP_UNUSED
FLOAT compute_lightning(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        uint32_t specular
);

CPP_UNUSED
FLOAT3 trace_ray(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 direction,
        uint32_t recursion_depth
);

CPP_UNUSED CPP_INLINE
__kernel void ray_tracer(
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
);

#endif /* CL_INC_H */
