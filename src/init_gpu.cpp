
#include <rtx.hpp>

void rtx::init_gpu()
{
    auto platform = cllib::get_platforms().at(0);
    auto device = platform.get_devices().at(0);

    rtx::data::context = new cllib::CLcontext(device);
    rtx::data::queue = new cllib::CLqueue(*rtx::data::context, device);

    cllib::CLprogram program(4, std::ifstream(rtx::config::kernel_fname), "ray_tracer", *rtx::data::context);
    std::string flags = "-D__OPENCL -D__NOCACHE_123 -I" + std::string(rtx::config::cl_dir);
    program.compile(device, true, flags.c_str());

    rtx::data::kernel = new cllib::CLkernel(program, {static_cast<size_t>(rtx::config::width), static_cast<size_t>(rtx::config::height)});
}
