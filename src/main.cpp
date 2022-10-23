
#include <thread>
#include <rtx.hpp>
#include <cl/marcher.cl>
#include <thread>
#include <exception.hpp>
#include <utils.hpp>

void rtx::init_scene()
{
    size_t full_size = \
            rtx::objects::sp_vec.size() * sizeof(sphere_t) +
            rtx::objects::pl_vec.size() * sizeof(plane_t) +
            rtx::objects::tr_vec.size() * sizeof(triangle_t) +
            rtx::objects::cn_vec.size() * sizeof(cone_t) +
            rtx::objects::cy_vec.size() * sizeof(cylinder_t) +
            rtx::objects::to_vec.size() * sizeof(torus_t);
    std::vector<unsigned char> fig_vec(full_size);
    size_t size=0;
    size_t ds = 0;
    ds = rtx::objects::sp_vec.size() * sizeof(sphere_t);
    std::memcpy(fig_vec.data() + size, rtx::objects::sp_vec.data(), ds);
    size += ds;

    ds = rtx::objects::pl_vec.size() * sizeof(plane_t);
    std::memcpy(fig_vec.data() + size, rtx::objects::pl_vec.data(), ds);
    size += ds;

    ds = rtx::objects::tr_vec.size() * sizeof(triangle_t);
    std::memcpy(fig_vec.data() + size, rtx::objects::tr_vec.data(), ds);
    size += ds;

    ds = rtx::objects::cn_vec.size() * sizeof(cone_t);
    std::memcpy(fig_vec.data() + size, rtx::objects::cn_vec.data(), ds);
    size += ds;

    ds = rtx::objects::cy_vec.size() * sizeof(cylinder_t);
    std::memcpy(fig_vec.data() + size, rtx::objects::cy_vec.data(), ds);
    size += ds;

    ds = rtx::objects::to_vec.size() * sizeof(torus_t);
    std::memcpy(fig_vec.data() + size, rtx::objects::to_vec.data(), ds);
    size += ds;

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
                            rtx::config::tracer = RTX_RAY_TRACER;
                            rtx::config::kernel_file = rtx::config::tracer_fname;
                            rtx::config::kernel_name = "ray_tracer";
                            break ;
                        }
                        case ("marcher"_hash):
                        case ("marching"_hash):
                        case ("rmc"_hash): {
                            rtx::config::tracer = RTX_RAY_MARCHER;
                            rtx::config::kernel_file = rtx::config::marcher_fname;
                            rtx::config::kernel_name = "ray_marcher";
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
    argparse(argc, argv);
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

    try {
        rtx::parse_scene();
        rtx::init_mlx();
        rtx::init_gpu();
//        std::thread gpu_initializer(rtx::init_gpu);
//        std::thread mlx_initializer(rtx::init_mlx);
//        gpu_initializer.join();
//        mlx_initializer.join();
//    return 0;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
//        std::cout << "-------------------------------------\n";
//        std::cout << "aborting program\n";
        return 1;
    }

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