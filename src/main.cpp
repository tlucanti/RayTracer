
#include <thread>
#include <rtx.hpp>
#include <cl/marcher.cl>
#include <thread>
#include <exception.hpp>
#include <utils.hpp>

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

    add_figures_next(fig_vec, rtx::objects::sp_vec, &local_scene.spheres, &local_scene.spheres_num, &rtx::objects::sphere_offset);
    add_figures_next(fig_vec, rtx::objects::pl_vec, &local_scene.planes, &local_scene.planes_num, &rtx::objects::plane_offset);
    add_figures_next(fig_vec, rtx::objects::tr_vec, &local_scene.triangles, &local_scene.triangles_num, &rtx::objects::triangle_offset);
    add_figures_next(fig_vec, rtx::objects::cn_vec, &local_scene.cones, &local_scene.cones_num, &rtx::objects::cone_offset);
    add_figures_next(fig_vec, rtx::objects::cy_vec, &local_scene.cylinders, &local_scene.cylinders_num, &rtx::objects::cylinder_offset);
    add_figures_next(fig_vec, rtx::objects::to_vec, &local_scene.torus, &local_scene.torus_num, &rtx::objects::torus_offset);
    add_figures_next(fig_vec, rtx::objects::box_vec, &local_scene.boxes,  &local_scene.boxes_num, &rtx::objects::box_offset);

    rtx::scene::figures = cllib::CLarray<unsigned char, cllib::read_only_array>(fig_vec, *rtx::data::context, *rtx::data::queue);

    rtx::scene::cameras = cllib::CLarray<camera_t, cllib::read_only_array>(rtx::objects::cam_vec, *rtx::data::context, *rtx::data::queue);
    rtx::scene::lights = cllib::CLarray<light_t, cllib::read_only_array>(rtx::objects::li_vec, *rtx::data::context, *rtx::data::queue);

    rtx::scene::canvas = cllib::CLarray<uint32_t, cllib::write_only_array>(rtx::config::width * rtx::config::height, *rtx::data::context);

//    rtx::scene::canvas2 = cllib::CLarray<uint32_t, cllib::read_write_array>((rtx::config::width) * (rtx::config::height), *rtx::data::context);
//    rtx::scene::distances = cllib::CLarray<FLOAT, cllib::read_write_array>((rtx::config::width) * (rtx::config::height), *rtx::data::context);
//    rtx::scene::canvas.memset(0, *rtx::data::queue);
//    rtx::scene::canvas2.memset(0, *rtx::data::queue);
//    rtx::scene::distances.memset(0., *rtx::data::queue);
}

void rtx::init_kernel()
{
    cllib::CLkernel &kernel = *rtx::data::kernel;
    kernel.reset_args();
    kernel.set_next_arg(rtx::scene::canvas);
//    kernel.set_next_arg(rtx::scene::distances);

    kernel.set_next_arg(rtx::scene::figures);

    kernel.set_next_arg(rtx::scene::lights);
    kernel.set_next_arg(rtx::scene::cameras);

    kernel.set_next_arg(static_cast<int>(rtx::objects::sp_vec.size()));
    kernel.set_next_arg(static_cast<int>(rtx::objects::pl_vec.size()));
    kernel.set_next_arg(static_cast<int>(rtx::objects::tr_vec.size()));
    kernel.set_next_arg(static_cast<int>(rtx::objects::cn_vec.size()));
    kernel.set_next_arg(static_cast<int>(rtx::objects::cy_vec.size()));
    kernel.set_next_arg(static_cast<int>(rtx::objects::to_vec.size()));
    kernel.set_next_arg(static_cast<int>(rtx::objects::box_vec.size()));

    kernel.set_next_arg(static_cast<int>(rtx::scene::lights.size()));
    kernel.set_next_arg(static_cast<int>(rtx::scene::cameras.size()));
    kernel.set_next_arg(rtx::config::width);
    kernel.set_next_arg(rtx::config::height);
}

void init_blur_kernel(cllib::CLkernel &blur_kernel)
{
    blur_kernel.reset_args();
    blur_kernel.set_next_arg(rtx::scene::canvas);
//    blur_kernel.set_next_arg(rtx::scene::canvas2);
//    blur_kernel.set_next_arg(rtx::scene::distances);
    blur_kernel.set_next_arg(rtx::config::width);
    blur_kernel.set_next_arg(rtx::config::height);
}

NORET void rtx::collapse(int status)
{
    delete rtx::data::win;
    delete rtx::data::img;
    delete rtx::data::kernel;
    delete rtx::data::context;
    delete rtx::data::queue;
//    delete rtx::data::blur_kernel;
    exit(status);
}

static inline std::string _cy(const char *s)
{
    return rtx::C["`"_s + s + "`"_s];
}

static inline bool endthwith(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void argparse(int argc, char **argv)
{
    bool got_scene = false;
    bool got_tracer = false;

    --argc;
    int i=1;
    while (i <= argc) {
        switch (rtx::hash(argv[i])) {
            case "--scene"_hash: {
                got_scene = true;
                if (i == argc) {
                    rtx::Error("argparse", rtx::W["expected scene name after " + rtx::C["`--scene`"] + " option"_W]);
                    exit(1);
                } else {
                    ++i;
                    if (not endthwith(argv[i], ".json")) {
                        rtx::Error("argparse", "scene name should have "_W + ".json"_Y + " extension"_W);
                        exit(1);
                    }
                    rtx::config::scene_fname = argv[i];
                }
                break ;
            }
            case "--tracer"_hash: {
                got_tracer = true;
                if (i == argc) {
                    rtx::Error("argparse", "expected tracer name after "_W + "`--tracer`"_C + "option"_W);
                    exit(1);
                } else {
                    ++i;
                    switch (rtx::hash(argv[i])) {
                        case ("tracer"_hash):
                        case ("tracing"_hash):
                        case ("rtx"_hash): {
                            rtx::config::tracer_type = RTX_RAY_TRACER;
                            rtx::config::kernel_file = rtx::config::tracer_fname;
                            break ;
                        }
                        case ("marcher"_hash):
                        case ("marching"_hash):
                        case ("rmc"_hash): {
                            rtx::config::tracer_type = RTX_RAY_MARCHER;
                            rtx::config::kernel_file = rtx::config::marcher_fname;
                            break ;
                        }
                        default: {
                            rtx::Error("argparse", "unknown "_W + "tracer"_Y + " option "_W + _cy(argv[i]));
                            exit(1);
                        }
                    }
                    break ;
                }
            }
            case "--help"_hash: {
                rtx::Info("help", "ray tracer program with GPU computing"_W);
                rtx::Info("help", "");
                rtx::Info("help", "program arguments:"_W);
                rtx::Info("help", "    " + "--tracer"_P + " [TRACER]"_C + " choose tracing algorithm"_W);
                rtx::Info("help", "        " + "[TRACER]"_C + " can be one of:"_W);
                rtx::Info("help", "            " + "tracer"_Y + "/"_W + "tracing"_Y + "/"_W + "rtx"_Y + ": classical ray tracing algorithm"_W);
                rtx::Info("help", "            " + "marcher"_Y + "/"_W + "marching"_Y + "/"_W + "rmc"_Y + ": ray marching algorithm"_W);
                rtx::Info("help", "    " + "--scene"_P + " [PATH]"_C + " scene configuration file"_W);
                rtx::Info("help", "        " + "[PATH]"_C + " is absolute or relative path to "_W + ".json"_Y + " file"_W);
                rtx::Info("help", "");
                rtx::Info("help", "tlucanti (c) https://github.com/tlucanti/RayTracer"_W);
                exit(0);
            }
            default: {
                rtx::Error("argparse", "unknown option "_W + _cy(argv[i]));
                exit(1);
            }
        }
        ++i;
    }
    if (not (got_scene || got_tracer)) {
        rtx::Error("argparse", "parameters expected, run with option "_W + "--help"_C);
        exit(1);
    }
    if (not got_scene)
        rtx::Error("argparse", "expected scene name with option "_W + "--scene"_C);
    if (not got_tracer)
        rtx::Error("argparse", "expected tracer algorithm with option "_W + "--tracer"_C);
    if (not (got_scene && got_tracer))
        exit(1);
}

int main(int argc, char **argv)
{
    camera_t cam({{0., 0., 0.}}, {{1., 1., 1.}});
    std::cout << "alpha: " << cam.alpha << ", theta: " << cam.theta << std::endl;
    cam.recompute_matrix();
    FLOAT3 dir = {{1, 0, 0}};
    std::cout << "origin: " << dir << std::endl;
    FLOAT3 d2 = rotate_vector(dir, cam.rotate_matrix);
    std::cout << "rotated: " << d2 << std::endl;
    cam.recompute_reverse_matrix();
    FLOAT3 d3 = rotate_vector(d2, cam.reverse_rotate_matrix);
    std::cout << "rotated back: " << d3 << std::endl;
//    return 0;

    argparse(argc, argv);
    putenv(const_cast<char *>("CUDA_CACHE_DISABLE=1"));

#ifndef __DEBUG
    try {
#endif
        rtx::parse_scene();
        rtx::init_mlx();
        rtx::init_gpu();
//        std::thread gpu_initializer(rtx::init_gpu);
//        std::thread mlx_initializer(rtx::init_mlx);
//        gpu_initializer.join();
//        mlx_initializer.join();
//    return 0;
#ifndef __DEBUG
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
//        std::cout << "-------------------------------------\n";
//        std::cout << "aborting program\n";
        return 1;
    }
#endif

    rtx::init_scene();
    rtx::init_kernel();
//    init_blur_kernel(*rtx::data::blur_kernel);

    rtx::data::kernel->run(*rtx::data::queue);
//    rtx::data::blur_kernel->run(*rtx::data::queue);

    auto pixel_data = rtx::scene::canvas.dump(*rtx::data::queue);

    rtx::data::img->fill(pixel_data);
    rtx::data::win->put_image(*rtx::data::img);
    rtx::data::win->event_loop();
}