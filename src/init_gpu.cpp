
#include <rtx.hpp>

void rtx::init_gpu()
{
    auto platform = cllib::get_platforms().at(0);
    auto device = platform.get_devices().at(0);

    rtx::data::context = new cllib::CLcontext(device);
    rtx::data::queue = new cllib::CLqueue(*rtx::data::context, device);

    cllib::CLprogram program(4, std::ifstream(rtx::config::kernel_fname), "ray_tracer", *rtx::data::context);
    program.compile(device, true, "-D__OPENCL");

    rtx::data::kernel = new cllib::CLkernel(program, {rtx::config::width, rtx::config::height});
}
