
#ifndef TRACING_BASE_CL
# define TRACING_BASE_CL

# define set_scene_fig_next(__field, __type, __cnt, __base_ptr, __shift_var) \
do {                                                                        \
    __field = creinterpret_cast(                                            \
        __constant const __type *__restrict,                                \
        __base_ptr + __shift_var                                            \
    );                                                                      \
    __shift_var += __cnt * sizeof(__type);                                  \
} while (false)

# ifdef RTX_RAY_TRACER
#  define trace_ray trace_ray_rtx
# elif defined(RTX_RAY_MARCHER)
#  define trace_ray trace_ray_rmc
# else
    CPP_INLINE FLOAT3 trace_ray(scene_ptr, FLOAT3, FLOAT3)
    {
        return ASSIGN_FLOAT3(255., 255., 255.);
    }
# endif

CPP_INLINE
FLOAT3 vec3_from_pixpos(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    return normalize(ASSIGN_FLOAT3(
        x / cstatic_cast(FLOAT, width) - 0.5,
        y / cstatic_cast(FLOAT, height) - 0.5,
        1.
    ));
}

CPP_INLINE
void pixpos_from_vec3(FLOAT3 vec, uint32_t *x, uint32_t *y, uint32_t width, uint32_t height)
{
    *x = cstatic_cast(uint32_t, (vec.x + 0.5) * width);
    *y = cstatic_cast(uint32_t, (vec.y + 0.5) * height);
}

CPP_UNUSED CPP_INLINE
__kernel void tracer_kernel(
        __global uint32_t *canvas,

        byte_ptr figures,

        light_ptr lights,
        camera_ptr cameras,

        const uint32_t spheres_num,
        const uint32_t planes_num,
        const uint32_t triangles_num,
        const uint32_t cones_num,
        const uint32_t cylinders_num,
        const uint32_t torus_num,
        const uint32_t boxes_num,

        const uint32_t lights_num,
        const uint32_t cameras_num,

        const uint32_t width,
        const uint32_t height
    )
{
    const uint32_t x = get_global_id(0);
    const uint32_t y = get_global_id(1);

    scene_t scene = {
            .lights = lights,
            .cameras = cameras,
            .spheres_num = spheres_num,
            .planes_num = planes_num,
            .triangles_num = triangles_num,
            .cones_num = cones_num,
            .cylinders_num = cylinders_num,
            .torus_num = torus_num,
            .boxes_num = boxes_num,
            .lights_num = lights_num,
            .cameras_num = cameras_num
    };
    size_t shift = 0;

    set_scene_fig_next(scene.spheres, sphere_t, spheres_num, figures, shift);
    set_scene_fig_next(scene.planes, plane_t, planes_num, figures, shift);
    set_scene_fig_next(scene.triangles, triangle_t, triangles_num, figures, shift);
    set_scene_fig_next(scene.cones, cone_t, cones_num, figures, shift);
    set_scene_fig_next(scene.cylinders, cylinder_t, cylinders_num, figures, shift);
    set_scene_fig_next(scene.torus, torus_t, torus_num, figures, shift);
    set_scene_fig_next(scene.boxes, box_t, boxes_num, figures, shift);

    FLOAT3 vec = vec3_from_pixpos(x, y, width, height);
    vec = rotate_vector(vec, cameras[0].rotate_matrix);
    //    FLOAT distance;
    const FLOAT3 color = trace_ray(
            &scene,
            cameras[0].position,
            vec
    );

    uint32_t pix_pos;

    pix_pos = (height - y - 1u) * (width) + x;
    const uint32_t int_color =
            cstatic_cast(uint32_t, color.x) << 16u
            | cstatic_cast(uint32_t, color.y) << 8u
            | cstatic_cast(uint32_t, color.z);
    canvas[pix_pos] = int_color;
}

CPP_UNUSED CPP_INLINE
__kernel void blur_convolution(
        __global uint32_t *input,
        __global uint32_t *output,
        __global FLOAT *distances,

        const int32_t width,
        const int32_t height
)
{
    const int32_t z = cstatic_cast(int, get_global_id(0));
    const int32_t y = cstatic_cast(int, get_global_id(1));
    const int pix_pos = (height - y - 1) * (width) + z;

    FLOAT3 convolution_val = ASSIGN_FLOAT3(0., 0., 0.);
    int conv_size = cstatic_cast(int, fmin(distances[pix_pos], 20.));
    int conv_step = conv_size / 10;
    int cnt = 0;

    for (int i=-conv_size; i <= conv_size; i+=conv_step) {
        for (int j=-conv_size; j <= conv_size; j+=conv_step) {
            int column = z + j;
            int row = height - y - 1 + i;

            row = max(0, row);
            row = min(height - 1, row);
            column = max(0, column);
            column = min(width - 1, column);

            const int pos = row * width + column;
            convolution_val.z += input[pos] & 0xFF;
            convolution_val.y += (input[pos] >> 8u) & 0xFF;
            convolution_val.x += (input[pos] >> 16u) & 0xFF;
            ++cnt;
        }
    }
    convolution_val *= 1. / cnt;
    const uint32_t blur_val =
            cstatic_cast(uint32_t, convolution_val.x) << 16u
            | cstatic_cast(uint32_t, convolution_val.y) << 8u
            | cstatic_cast(uint32_t, convolution_val.z);
    output[pix_pos] = blur_val;
}

#endif /* TRACING_BASE_CL */
