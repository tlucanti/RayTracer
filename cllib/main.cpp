
#include "CLlib.hpp"

int main()
{
    cllib::CLplatform platform;
    cllib::CLdevice device(platform, cllib::gpu_type);
    cllib::CLcontext context(device);
    cllib::CLqueue queue(context, device);
    cllib::CLprogram program(cllib::sources::addf, context);
    program.compile(device);
    cllib::CLkernel(program, "add");
}