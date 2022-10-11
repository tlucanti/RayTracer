
#include <thread>
#include <rtx.hpp>
#include <cl/kernel.cl>

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

#include <json>
int main()
{
    putenv(const_cast<char *>("CUDA_CACHE_DISABLE=1"));

//    COMPLEX x1, x2;
//    cubic_complex_solve(-10/3., 14/3., 27/3., &x1, &x2);
//    std::cout << x1 << ' ' << x2 << std::endl;
//    cubic_complex_solve(-21.25, 122.75, 0, &x1, &x2);
//    std::cout << x1 << ' ' << x2 << std::endl;
//    std::cout << quartic_complex_solve(2, -41, -42, 360);
//    return 0;

//    test_ferrari();
//    return 0;

//    nlohmann::json a = R"({"a": 123, "a": 432})"_json;
//    std::cout << a["a"] << std::endl;
//    return 0;

//    std::cout << newton_cubic_solve(-2.3333, 0.2, 1.4, 0.6, -4.8) << std::endl;
//    std::cout << cubic_solve(2.1, -6.7, 10, -1.8) << std::endl;
//    std::cout << ferrari_solve(0.5, -5, 10, -6.3, 4.9) << std::endl;
//    return 0;

//    std::cout << newton_solve(0, 0.657082, -1.056095, 14.076677, -7.052426, 51.848435) << std::endl;
//    return 0;

//    try {
        rtx::parse_scene(rtx::config::scene_fname); // run in thread
//    return 0;
        rtx::init_gpu(); // run in thread
        rtx::init_mlx(); // run in thread
//    } catch (std::exception &e) {
//        std::cout << e.what() << std::endl;
//        std::cout << "-------------------------------------\n";
//        std::cout << "aborting program\n";
//        return 1;
//    }

    init_scene();
    init_kernel(*rtx::data::kernel);

    rtx::data::kernel->run(*rtx::data::queue);

    auto pixel_data = rtx::scene::canvas.dump(*rtx::data::queue);

    rtx::data::img->fill(pixel_data);
    rtx::data::win->put_image(*rtx::data::img);
    rtx::data::win->event_loop();

    rtx::collapse(0);
}