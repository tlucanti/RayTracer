
#include "cllib"
#include "mlxlib"

#include <fstream>
#include <cmath>

const char *fname = "../cl/kernel.cl";

#define PACKED __attribute__((packed))

namespace Color
{
    const cl_float3 white = {255, 255, 255};
    const cl_float3 black = {0, 0, 0};
    const cl_float3 red = {255, 0, 0};
    const cl_float3 green = {0, 255, 0};
    const cl_float3 blue = {0, 0, 255};
    const cl_float3 cyan = {0, 255, 255};
    const cl_float3 magenta = {255, 0, 255};
    const cl_float3 yellow = {255, 255, 0};
    const cl_float3 purple = magenta;
}

typedef struct sphere_s
{
    cl_float3   center;
    float       radius;
    cl_float3        color;
    int       specular;
    float       reflective;

    sphere_s(cl_float3 center, float radius, cl_float3 color, int specular, float reflective)
        : center(center), radius(radius), color(color), specular(specular), reflective(reflective)
    {}
} PACKED sphere_t;

typedef struct camera_s
{
    cl_float3   position;
    cl_float3   direction;

    camera_s(cl_float3 position, cl_float3 d)
        : position(position)
    {
        float length = 1.0f / sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        direction = {d.x * length, d.y * length, d.z * length};
    }
} PACKED camera_t;

typedef struct ambient_s
{
    float   intensity;
    cl_float3    color;

    ambient_s(float intensity, cl_float3 color)
        : intensity(intensity), color(color)
    {}
} PACKED ambient_t;

typedef struct point_s
{
    cl_float3   position;
    float       intensity;
    cl_float3        color;

    point_s(cl_float3 position, float intencity, cl_float3 color)
        : position(position), intensity(intencity), color(color)
    {}
} PACKED point_t;

typedef struct direct_s
{
    cl_float3   direction;
    float       intensity;
    cl_float3        color;

    direct_s(cl_float3 d, float intensity, cl_float3 color)
        : intensity(intensity), color(color)
    {
        float length = 1.0f / sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        direction = {d.x * length, d.y * length, d.z * length};
    }
} PACKED direct_t;

int main()
{
    auto platform = cllib::get_platforms().at(0);
    auto device = platform.get_devices().at(0);

    cllib::CLcontext context(device);
    cllib::CLqueue queue(context, device);

    cllib::CLprogram program(4, std::ifstream(fname), "ray_tracer", context);
    program.compile(device, true, "-D__OPENCL");

    std::vector<sphere_t> sp_vec = {
            sphere_t({0,-1,3}, 1, Color::red, 500, 0.2f),
            sphere_t({2, 0, 4}, 1, Color::blue, 500, 0.2f),
            sphere_t({-2, 0, 4}, 1, Color::green, 10, 0.2f),
            sphere_t({0, -5001, 0}, 5000, Color::yellow, 1000, 0.2f)
    };
    std::vector<camera_t> cam_vec = {
            camera_t({0, 0, 0}, {0, 0, 1})
    };

    std::vector<ambient_t> amb_vec = {
            ambient_t(0.2, Color::white)
    };
    std::vector<point_t> pt_vec = {
            point_t({2, 1, 0}, 0.6, Color::white)
    };
    std::vector<direct_t> dir_vec = {
            direct_t({1, 4, 4}, 0.2, Color::white)
    };

    cllib::CLarray<sphere_t> spheres(sp_vec, context, queue);
    cllib::CLarray<camera_t> cameras(cam_vec, context, queue);

    cllib::CLarray<ambient_t> ambients(amb_vec, context, queue);
    cllib::CLarray<point_t> points(pt_vec, context, queue);
    cllib::CLarray<direct_t> directs(dir_vec, context, queue);

    const int width = 1000;
    const int height = 1000;

    cllib::CLarray<unsigned int> canvas(width * height, context, queue);
    cllib::CLkernel kernel(program, {width, height});

    kernel.set_arg(0, canvas);
    kernel.set_arg(1, spheres);
    kernel.set_arg(2, ambients);
    kernel.set_arg(3, points);
    kernel.set_arg(4, directs);
    kernel.set_arg(5, cameras);
    kernel.set_arg(6, static_cast<int>(spheres.size()));
    kernel.set_arg(7, static_cast<int>(ambients.size()));
    kernel.set_arg(8, static_cast<int>(points.size()));
    kernel.set_arg(9, static_cast<int>(directs.size()));
    kernel.set_arg(10, static_cast<int>(cameras.size()));
    kernel.set_arg(11, width);
    kernel.set_arg(12, height);

    kernel.run(queue);
    auto pixel_data = canvas.dump(queue);

    mlxlib::MLXcore core;
    mlxlib::MLXwindow win(core, width, height);
    mlxlib::MLXimage img(core, width, height);

    img.fill(pixel_data);

    win.put_image(img);
    win.event_loop();
}