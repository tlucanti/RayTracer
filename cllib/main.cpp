
#include "CLlib.hpp"
#include <iostream>

#include <tuple>
#include <utility>
#include <iostream>

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
print(const Tp &...t)
{ }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
print(const Tp &...t)
{
    int i=0;
    for (const auto & a : {t...})
    {
        std::cout << i++ << ' ' << a.__get_buffer() << std::endl;
    }
}

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
    try {
        program.compile(device);
    } catch (cllib::CLexception &e) {
        std::cout << program.builder.get_program_log() << std::endl;
        std::cout << cllib::sources::addf.get_program_source() << std::endl;
    }
    cllib::CLkernel(program, "addf");

    cllib::CLarray<float> array1({1, 2, 3, 4, 5}, context, queue);
    cllib::CLarray<float> array2({10,20,30,40,50}, context, queue);
    cllib::CLarray<float> array3(5, context, queue);

    print(array1, array2, array3);
//    kernel.set_args(array1, array2, array3);
}