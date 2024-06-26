
#include <rtx.hpp>
#include <exception.hpp>

void rtx::init_gpu()
{
	std::cout << "nr platforms: " << cllib::get_platforms().size() << '\n';
    auto platform = std::move(cllib::get_platforms().at(0));
    	std::cout << "nr devices: " << platform.get_devices().size() << '\n';
    auto device = std::move(platform.get_devices().at(0));

    rtx::data::context = new cllib::CLcontext(device);
    rtx::data::queue = new cllib::CLqueue(*rtx::data::context, device);

    cllib::CLprogram program;
    try {
        program = cllib::CLprogram(4, std::ifstream(rtx::config::kernel_file), rtx::config::kernel_name, *rtx::data::context);
    } catch (std::invalid_argument &e) {
        rtx::Error("gpuinit", "cannot open file "_W + rtx::Y[rtx::config::kernel_file]);
        exit(1);
    }
    std::string flags = "-D__OPENCL -I" + std::string(rtx::config::cl_dir);
    if (rtx::config::emission)
        flags += " -DRTX_EMISSION ";
    if (rtx::config::blinding)
        flags += " -DRTX_DIRECT ";
    if (rtx::config::transparency)
        flags += " -DRTX_TRANSPARENCY ";
    if (rtx::config::refractive)
        flags += " -DRTX_REFRACTIVE ";
    if (rtx::config::tracer_type == RAY_TRACER_TYPE)
        flags += " -DRTX_RAY_TRACER ";
    else if (rtx::config::tracer_type == RAY_MARCHER_TYPE)
        flags += " -DRTX_RAY_MARCHER ";
    program.compile(device, true, flags.c_str());

//    cllib::CLprogram blur_program(4, std::ifstream(rtx::config::kernel_fname), "blur_convolution", *rtx::data::context);
//    blur_program.compile(device, true, flags.c_str());

    auto w = static_cast<size_t>(rtx::config::width);
    auto h = static_cast<size_t>(rtx::config::height);
    rtx::data::kernel = new cllib::CLkernel(program, {w, h});
//    rtx::data::blur_kernel = new cllib::CLkernel(blur_program, {static_cast<size_t>(rtx::config::width), static_cast<size_t>(rtx::config::height)});
}
