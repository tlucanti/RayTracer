
#include "CLlib.hpp"
#include <iostream>

int main()
{
    auto platforms = cllib::get_platforms();
    cllib::CLplatform &platform = platforms.at(0);
    std::cout << "platform: " << platform.get_platform_name() << std::endl;

    auto devices = platform.get_devices(cllib::CLdevice::gpu_type, 1);
    cllib::CLdevice device = devices.at(0);
    std::cout << "device: " << device.get_device_name() << std::endl;
    std::cout << "builtin kernels: " << device.get_device_builtin_kernels() << std::endl;

    cllib::CLcontext context(device);
    cllib::CLqueue queue(context, device);
    cllib::CLprogram program(cllib::sources::addf, context);
    program.compile(device);
    cllib::CLkernel(program, "addf");

    cllib::CLarray<float>({1, 2, 3, 4, 5}, context, queue);
    cllib::CLarray<float>({10,20,30,40,50}, context, queue);
    cllib::CLarray<float>(5, context, queue);

    kernel.set_args()
}