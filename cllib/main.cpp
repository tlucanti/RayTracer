
#include "CLlib.hpp"
#include <iostream>
#include <tuple>

template <size_t argc=0, class ...type>
typename std::enable_if<argc == sizeof...(type), void>::type __print(const std::tuple<type...> &t)
{}

template <size_t argc=0, class ...type>
typename std::enable_if<argc < sizeof...(type), void>::type __print(const std::tuple<type...> &t)
{
    std::cout << "num: " << argc << ", el: " << std::get<argc>(t) << std::endl;
    __print<argc + 1, type...>(t);
}

template <class... type>
void print(const type &...arg)
{
    __print(std::tuple<type...>(arg...));
}

int main()
{
    print(1, 2.1, 2.2f, (char *)"lol", (char *)"kek", 'f');

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
        throw ;
    }
    cllib::CLkernel kernel(program);

    cllib::CLarray<float> array1({1, 2, 3, 4, 5}, context, queue);
    cllib::CLarray<float> array2({10,20,30,40,50}, context, queue);
    cllib::CLarray<float> array3(5, context, queue);

    kernel.set_arg(0, array1);
    kernel.set_arg(1, array2);
    kernel.set_arg(2, array3);
    kernel.run(queue);
}