
#include "cllib"
#include "mlxlib"

#include <fstream>

const char *fname = "../cl/kernel.cl";

#define PACKED __attribute__((packed))

typedef struct sphere_s
{
    cl_float3   center;
    float       radius;
    int         color;
} PACKED sphere_t;

typedef struct camera_s
{
    cl_float3   center;
    cl_float3   direction;
} PACKED camera_t;

int main()
{
    auto platform = cllib::get_platforms().at(0);
    auto device = platform.get_devices().at(0);

    cllib::CLcontext context(device);
    cllib::CLqueue queue(context, device);

    cllib::CLprogram program(4, std::ifstream(fname), "ray_tracer", context);
    program.compile(device, true);

    std::vector<sphere_t> sp_vec = {
        {{0,-1,3}, 1, 0xFF0000},
        {{2, 0, 4}, 1, 0x000FF},
        {{-2, 0, 4}, 1, 0x00FF00}
    };
    std::vector<camera_t> cam_vec = {
        {{0, 0, 0}, {0, 0, 0}}
    };

    cllib::CLarray<sphere_t> spheres(sp_vec, context, queue);
    cllib::CLarray<camera_t> cameras(cam_vec, context, queue);

    int spheres_num = sp_vec.size();
    int cameras_num = cam_vec.size();

    const int width = 1920;
    const int height = 1080;

    cllib::CLarray<unsigned int> canvas(width * height, context, queue);
    cllib::CLkernel kernel(program, {width, height});

    kernel.set_arg(0, canvas);
    kernel.set_arg(1, spheres);
    kernel.set_arg(2, spheres_num);
    kernel.set_arg(3, cameras);
    kernel.set_arg(4, cameras_num);
    kernel.set_arg(5, width);
    kernel.set_arg(6, height);

    kernel.run(queue);
    auto pixel_data = canvas.dump(queue);

    mlxlib::MLXcore core;
    mlxlib::MLXwindow win(core, width, height);
    mlxlib::MLXimage img(core, width, height);

    img.fill(pixel_data);

    win.put_image(img);
    win.event_loop();
}