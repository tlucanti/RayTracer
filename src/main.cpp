
#include <thread>

#include <rtx.hpp>

void init_scene()
{
    rtx::scene::spheres = cllib::CLarray<sphere_t, cllib::read_only_array>(rtx::objects::sp_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::planes = cllib::CLarray<plane_t, cllib::read_only_array>(rtx::objects::pl_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::triangles = cllib::CLarray<triangle_t, cllib::read_only_array>(rtx::objects::tr_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::cones = cllib::CLarray<cone_t, cllib::read_only_array>(rtx::objects::cn_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::cylinders = cllib::CLarray<cylinder_t, cllib::read_only_array>(rtx::objects::cy_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::torus = cllib::CLarray<torus_t, cllib::read_only_array>(rtx::objects::to_vec, *rtx::data::context, *rtx::data::queue);

    rtx::scene::cameras = cllib::CLarray<camera_t, cllib::read_only_array>(rtx::objects::cam_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::lights = cllib::CLarray<light_t, cllib::read_only_array>(rtx::objects::li_vec, *rtx::data::context, *rtx::data::queue);

    rtx::scene::canvas = cllib::CLarray<unsigned int, cllib::write_only_array>(rtx::config::width * rtx::config::height, *rtx::data::context);
}

void init_kernel(cllib::CLkernel &kernel)
{
    kernel.reset_args();
    kernel.set_next_arg(rtx::scene::canvas);
    kernel.set_next_arg(rtx::scene::spheres);
    kernel.set_next_arg(rtx::scene::planes);
    kernel.set_next_arg(rtx::scene::triangles);
    kernel.set_next_arg(rtx::scene::cones);
    kernel.set_next_arg(rtx::scene::cylinders);
    kernel.set_next_arg(rtx::scene::torus);

    kernel.set_next_arg(rtx::scene::lights);
    kernel.set_next_arg(rtx::scene::cameras);

    kernel.set_next_arg(static_cast<int>(rtx::scene::spheres.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::planes.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::triangles.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::cones.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::cylinders.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::torus.size()));

    kernel.set_next_arg(static_cast<int>(rtx::scene::lights.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::cameras.size()));
    kernel.set_next_arg(rtx::config::width);
    kernel.set_next_arg(rtx::config::height);
}

NORET void rtx::collapse(int status)
{
    delete rtx::data::win;
    delete rtx::data::img;
    delete rtx::data::kernel;
    delete rtx::data::context;
    delete rtx::data::queue;
    exit(status);
}

int main()
{
    std::cout << newton_cubic_solve(-2.3333, 0.2, 1.4, 0.6, -4.8) << std::endl;
    std::cout << cubic_solve(2.1, -6.7, 10, -1.8) << std::endl;
    std::cout << ferrari_solve(0.5, -5, 10, -6.3, 4.9) << std::endl;
//    return 0;

//    std::cout << newton_solve(0, 0.657082, -1.056095, 14.076677, -7.052426, 51.848435) << std::endl;
//    return 0;

    rtx::parse_scene(rtx::config::scene_fname); // run in thread
//    return 0;
    rtx::init_gpu(); // run in thread
    rtx::init_mlx(); // run in thread

    init_scene();
    init_kernel(*rtx::data::kernel);

    rtx::data::kernel->run(*rtx::data::queue);

    auto pixel_data = rtx::scene::canvas.dump(*rtx::data::queue);

    rtx::data::img->fill(pixel_data);
    rtx::data::win->put_image(*rtx::data::img);
    rtx::data::win->event_loop();

    rtx::collapse(0);
}