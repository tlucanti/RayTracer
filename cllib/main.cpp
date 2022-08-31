
#include "CLlib.hpp"

int main()
{
    auto platforms = cllib::get_platforms();
    cllib::CLplatform platform = platforms.at(0);

    auto devices = platform.get_devices();
    cllib::CLdevice device = devices.at(0);

    cllib::CLcontext context(device);
    cllib::CLqueue queue(context, device);
    cllib::CLprogram program(cllib::sources::addf, context);
    program.compile(device);
    cllib::CLkernel(program, "add");
}