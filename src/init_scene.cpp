
#include <rtx.hpp>

void rtx::refill_spheres()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::sp_vec.data()),
        rtx::objects::sp_vec.size() * sizeof(sphere_t),
        *rtx::data::queue,
        true,
        rtx::objects::sphere_offset
    );
}

void rtx::refill_planes()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::pl_vec.data()),
        rtx::objects::pl_vec.size() * sizeof(plane_t),
        *rtx::data::queue,
        true,
        rtx::objects::plane_offset
    );
}

void rtx::refill_triangles()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::tr_vec.data()),
        rtx::objects::tr_vec.size() * sizeof(triangle_t),
        *rtx::data::queue,
        true,
        rtx::objects::triangle_offset
    );
}

void rtx::refill_cones()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::cn_vec.data()),
        rtx::objects::cn_vec.size() * sizeof(cone_t),
        *rtx::data::queue,
        true,
        rtx::objects::cone_offset
    );
}

void rtx::refill_cylinders()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::cy_vec.data()),
        rtx::objects::cy_vec.size() * sizeof(cylinder_t),
        *rtx::data::queue,
        true,
        rtx::objects::cylinder_offset
    );
}

void rtx::refill_torus()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::to_vec.data()),
        rtx::objects::to_vec.size() * sizeof(torus_t),
        *rtx::data::queue,
        true,
        rtx::objects::torus_offset
    );
}

void rtx::refill_boxes()
{
    rtx::scene::figures.fill(
        reinterpret_cast<const unsigned char *>(rtx::objects::box_vec.data()),
        rtx::objects::box_vec.size() * sizeof(box_t),
        *rtx::data::queue,
        true,
        rtx::objects::box_offset
    );
}

template <class value_type, class ptr_type>
static void add_figures_next(
        std::vector<unsigned char> &obj_vec,
        const std::vector<value_type> &fig_vec,
        ptr_type local_obj_ptr,
        uint32_t *obj_cnt,
        size_t *offset_ptr
)
{
    size_t new_size = fig_vec.size() * sizeof(value_type);
    size_t old_size = obj_vec.size() * sizeof(unsigned char);

    obj_vec.resize(old_size + new_size);
    std::memcpy(obj_vec.data() + old_size, fig_vec.data(), new_size);
    *local_obj_ptr = fig_vec.data();
    *obj_cnt = static_cast<uint32_t>(fig_vec.size());
    *offset_ptr = old_size;
}

void rtx::init_scene()
{
    std::vector<unsigned char> fig_vec;
    scene_t &local_scene = rtx::scene::local_scene;
    dsf_buff_item_t empty_item {0, nullptr};

    add_figures_next(fig_vec, rtx::objects::sp_vec, &local_scene.spheres, &local_scene.spheres_num, &rtx::objects::sphere_offset);
    add_figures_next(fig_vec, rtx::objects::pl_vec, &local_scene.planes, &local_scene.planes_num, &rtx::objects::plane_offset);
    add_figures_next(fig_vec, rtx::objects::tr_vec, &local_scene.triangles, &local_scene.triangles_num, &rtx::objects::triangle_offset);
    add_figures_next(fig_vec, rtx::objects::cn_vec, &local_scene.cones, &local_scene.cones_num, &rtx::objects::cone_offset);
    add_figures_next(fig_vec, rtx::objects::cy_vec, &local_scene.cylinders, &local_scene.cylinders_num, &rtx::objects::cylinder_offset);
    add_figures_next(fig_vec, rtx::objects::to_vec, &local_scene.torus, &local_scene.torus_num, &rtx::objects::torus_offset);
    add_figures_next(fig_vec, rtx::objects::box_vec, &local_scene.boxes,  &local_scene.boxes_num, &rtx::objects::box_offset);

    rtx::scene::figures = cllib::CLarray<unsigned char, cllib::read_only_array>(fig_vec, *rtx::data::context, *rtx::data::queue);
    if (rtx::scene::union_cnt > 0) {
        rtx::scene::dfs_buff = cllib::CLarray<dsf_buff_item_t, cllib::read_write_array>(
                static_cast<size_t>(rtx::scene::union_cnt), *rtx::data::context);
        rtx::scene::dfs_buff.memset(empty_item, *rtx::data::queue);
    }

    rtx::scene::cameras = cllib::CLarray<camera_t, cllib::read_only_array>(rtx::objects::cam_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::lights = cllib::CLarray<light_t, cllib::read_only_array>(rtx::objects::li_vec, *rtx::data::context, *rtx::data::queue);

    rtx::scene::canvas = cllib::CLarray<uint32_t, cllib::write_only_array>(rtx::config::width * rtx::config::height, *rtx::data::context);

//    rtx::scene::canvas2 = cllib::CLarray<uint32_t, cllib::read_write_array>((rtx::config::width) * (rtx::config::height), *rtx::data::context);
//    rtx::scene::distances = cllib::CLarray<FLOAT, cllib::read_write_array>((rtx::config::width) * (rtx::config::height), *rtx::data::context);
//    rtx::scene::canvas.memset(0, *rtx::data::queue);
//    rtx::scene::canvas2.memset(0, *rtx::data::queue);
//    rtx::scene::distances.memset(0., *rtx::data::queue);
}