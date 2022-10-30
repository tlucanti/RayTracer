
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

# ifdef RTX_RAY_TRACER
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
# elif defined(RTX_RAY_MARCHER)
CPP_UNUSED
void_ptr closest_intersection(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT3 *__restrict point_ptr
);
# endif

CPP_UNUSED
bool shadow_intersection(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end
);

CPP_UNUSED
FLOAT3 compute_lightning_rtx(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        uint32_t specular,
        FLOAT3 *specular_computed
);

CPP_UNUSED
FLOAT3 compute_lightning_rmc(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        uint32_t specular,
        FLOAT3 *specular_computed
);

CPP_UNUSED
FLOAT3 trace_ray_rtx(
        scene_ptr scene,
        FLOAT3 point,
        FLOAT3 direction
);

CPP_UNUSED
FLOAT3 trace_ray_rmc(
        scene_ptr scene,
        FLOAT3 point,
        FLOAT3 direction
);

CPP_UNUSED
__kernel void tracer_kernel(
        __global uint32_t *canvas,
        ENABLE_IF_MARCHER(__global dsf_buff_item_t *dsf_buffer COMA)
        ENABLE_IF_MARCHER(int32_t dsf_buffer_size COMA)

        byte_ptr figures,

        light_ptr lights,
        camera_ptr cameras,

        uint32_t spheres_num,
        uint32_t planes_num,
        uint32_t triangles_num,
        uint32_t cones_num,
        uint32_t cylinders_num,
        uint32_t torus_num,
        uint32_t boxes_num,

        uint32_t lights_num,
        uint32_t cameras_num,

        uint32_t width,
        uint32_t height
);

#endif /* CL_INC_H */
