
#include <thread>
#include <rtx.hpp>
#include <cl/marcher.cl>
#include <thread>
#include <exception.hpp>
#include <utils.hpp>

void rtx::init_kernel()
{
    cllib::CLkernel &kernel = *rtx::data::kernel;
    kernel.reset_args();
    kernel.set_next_arg(rtx::scene::canvas);
    if (rtx::config::tracer_type == RAY_MARCHER_TYPE) {
        kernel.set_next_arg(rtx::scene::dfs_buff);
        kernel.set_next_arg(rtx::scene::union_cnt);
    }
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
                            rtx::config::tracer_type = RAY_TRACER_TYPE;
                            rtx::config::kernel_file = rtx::config::tracer_fname;
                            break ;
                        }
                        case ("marcher"_hash):
                        case ("marching"_hash):
                        case ("rmc"_hash): {
                            rtx::config::tracer_type = RAY_MARCHER_TYPE;
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