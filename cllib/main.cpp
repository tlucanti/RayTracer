
#include "CLlib.hpp"
#include <iostream>
#include <tuple>

template <size_t argc=0, class ...type>
typename std::enable_if<argc == sizeof...(type), void>::type
_print(const std::tuple<type...> &t)
{}

template <size_t argc=0, class ...type>
typename std::enable_if<argc < sizeof...(type), void>::type
_print(const std::tuple<type...> &t)
{
    std::cout << "num: " << argc << ", el: " << std::get<argc>(t) << std::endl;
    _print<argc + 1, type...>(t);
}

template <class... type>
void print(const type &...arg)
{
    _print(std::tuple<type...>(arg...));
}

const char *kernel2_code =
"__kernel void add_fl(__global const float *a, const float b, __global float *c)"
"{"
"    int i = get_global_id(0);"
"    c[i] = a[i] + b;"
"}";

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
//    cllib::CLprogram program(cllib::sources::addf, context);
    cllib::CLprogram program(3, kernel2_code, "add_fl", context);
    try {
        program.compile(device);
    } catch (cllib::CLexception &e) {
        std::cout << program.builder.get_program_log() << std::endl;
        std::cout << cllib::sources::addf.get_program_source() << std::endl;
        throw ;
    }

    cllib::CLarray<float> array1({1, 2, 3, 4, 5}, context, queue);
//    cllib::CLarray<float> array2({10,20,30,40,50}, context, queue);
    float arg2 = 0.5;
    cllib::CLarray<float> array3(5, context, queue);

    cllib::CLkernel kernel(program, array1.size());

    kernel.set_arg(0, array1);
    kernel.set_arg(1, arg2);
    kernel.set_arg(2, array3);

    kernel.run(queue);

    auto arr = array3.dump(queue);
    for (const auto &i : arr)
        std::cout << i << ' ';
    std::cout << std::endl;
}
