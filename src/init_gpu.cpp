
#include <rtx.hpp>

void rtx::init_gpu()
{
    auto platform = cllib::get_platforms().at(0);
    auto device = platform.get_devices().at(0);

    rtx::data::context = new cllib::CLcontext(device);
    rtx::data::queue = new cllib::CLqueue(*rtx::data::context, device);

    cllib::CLprogram program(4, std::ifstream(rtx::config::kernel_fname), "ray_tracer", *rtx::data::context);
    std::string flags = "-D__OPENCL -I" + std::string(rtx::config::cl_dir);
    if (rtx::config::emission)
        flags += " -DRTX_EMISSION ";
    program.compile(device, true, flags.c_str());

//    cllib::CLprogram blur_program(4, std::ifstream(rtx::config::kernel_fname), "blur_convolution", *rtx::data::context);
//    blur_program.compile(device, true, flags.c_str());

    auto w = static_cast<size_t>(rtx::config::width);
    auto h = static_cast<size_t>(rtx::config::height);
    rtx::data::kernel = new cllib::CLkernel(program, {w, h});
//    rtx::data::blur_kernel = new cllib::CLkernel(blur_program, {static_cast<size_t>(rtx::config::width), static_cast<size_t>(rtx::config::height)});
}
