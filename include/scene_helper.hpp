
#ifndef SCENE_HELPER_HPP
# define SCENE_HELPER_HPP

# include "tracer.cl"
# include "marcher.cl"
# include "tracing_base.cl"

void_ptr object_at_pos(int x, int y, obj_type_t *obj_type)
{
    y = static_cast<int>(rtx::config::height) - y;
    camera_t &cur_cam = rtx::objects::cam_vec.at(rtx::config::current_camera);
    FLOAT3 direction = vec3_from_pixpos(
        static_cast<uint32_t>(x),
        static_cast<uint32_t>(y),
        rtx::config::width, rtx::config::height
    );
    direction = rotate_vector(direction, cur_cam.rotate_matrix);
    FLOAT3 dummy_vec;

    if (rtx::config::tracer_type == RTX_RAY_TRACER) {
        FLOAT dummy_float;

        return closest_intersection_rtx(
            &rtx::scene::local_scene,
            cur_cam.position,
            direction,
            EPS,
            INFINITY,
            &dummy_float,
            obj_type,
            &dummy_vec
        );
    }
    else if (rtx::config::tracer_type == RTX_RAY_MARCHER)
        return closest_intersection_rmc(
            &rtx::scene::local_scene,
            cur_cam.position,
            direction,
            &dummy_vec,
            obj_type
        );
    else
        throw std::runtime_error("invalid tracer_type");
}

#endif /* SCENE_HELPER_HPP */
