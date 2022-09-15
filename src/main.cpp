
#include "cllib"
#include "mlxlib"
#include "struct.hpp"
#include <ctime>
#include <sstream>
#include <atomic>
#include <unistd.h>

namespace config
{
    inline constexpr int width = 800;
    inline constexpr int height = 800;

    const char *fname = "../cl/kernel.cl";
    int current_camera = 0;
    inline constexpr FLOAT forward_move_step = 0.1;
    inline constexpr FLOAT side_move_speed = 0.05;
    inline constexpr FLOAT vertical_move_speed = 0.05;
    inline constexpr FLOAT vertical_look_speed = 0.005;
    inline constexpr FLOAT horizontal_look_speed = 0.005;
}

FLOAT3 move_direction;
FLOAT2 look_direction;
cl_int2 mouse_pos;
int started(0), ended(0);
int do_update(0);
bool is_updating(false);

cllib::CLcontext context;
cllib::CLqueue queue;

std::vector<camera_t> cam_vec;
std::vector<sphere_t> sp_vec;
std::vector<ambient_t> amb_vec;
std::vector<point_t> pt_vec;
std::vector<direct_t> dir_vec;

cllib::CLarray<sphere_t> spheres;
cllib::CLarray<camera_t> cameras;
cllib::CLarray<ambient_t> ambients;
cllib::CLarray<point_t> points;
cllib::CLarray<direct_t> directs;

cllib::CLarray<unsigned int> canvas;

struct timespec fps_meter;

FLOAT dot(const FLOAT3 &v1, const FLOAT3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void add_rotated_vec(FLOAT3 &vec, const FLOAT3 matrix[3], FLOAT3 dir)
{
    vec.x += dot(matrix[0], dir);
    vec.y += dot(matrix[1], dir);
    vec.z += dot(matrix[2], dir);
}

void keypress_hook(int keycode, void *)
{
    switch (keycode)
    {
        case mlxlib::keys::KEY_W: move_direction.z = config::forward_move_step; break ;
        case mlxlib::keys::KEY_S: move_direction.z = -config::forward_move_step; break ;
        case mlxlib::keys::KEY_A: move_direction.x = -config::side_move_speed; break ;
        case mlxlib::keys::KEY_D: move_direction.x = config::side_move_speed; break ;
        case mlxlib::keys::KEY_SPACE: move_direction.y = config::vertical_move_speed; break ;
        case mlxlib::keys::KEY_LCTRL:
        case mlxlib::keys::KEY_RCTRL: move_direction.y = -config::vertical_move_speed; break ;
        case mlxlib::keys::KEY_UP: look_direction.y = -config::vertical_look_speed; break ;
        case mlxlib::keys::KEY_DOWN: look_direction.y = config::vertical_look_speed; break ;
        case mlxlib::keys::KEY_LEFT: look_direction.x = -config::horizontal_look_speed; break ;
        case mlxlib::keys::KEY_RIGHT: look_direction.x = config::horizontal_look_speed; break ;
        case mlxlib::keys::KEY_ESCAPE: exit(0);
        default: return ;
    }
    do_update += 1;
}

void keyrelease_hook(int keycode, void *)
{
    switch (keycode)
    {
        case mlxlib::keys::KEY_W:
        case mlxlib::keys::KEY_S: move_direction.z = 0; break ;
        case mlxlib::keys::KEY_A:
        case mlxlib::keys::KEY_D: move_direction.x = 0; break ;
        case mlxlib::keys::KEY_SPACE:
        case mlxlib::keys::KEY_LCTRL:
        case mlxlib::keys::KEY_RCTRL: move_direction.y = 0; break ;
        case mlxlib::keys::KEY_UP:
        case mlxlib::keys::KEY_DOWN: look_direction.y = 0; break ;
        case mlxlib::keys::KEY_LEFT:
        case mlxlib::keys::KEY_RIGHT: look_direction.x = 0; break ;
        default: return ;
    }
    do_update -= 1;
}

void mouse_hook(int x, int y, void *)
{
    int dx = x - mouse_pos.x;
    int dy = y - mouse_pos.y;
    look_direction.x = dx * config::horizontal_look_speed;
    look_direction.y = dy * config::vertical_look_speed;
//    if (dx < 5)
//        look_direction.x /= 5;
//    if (dy < 5)
//        look_direction.y /= 5;
    mouse_pos.x = x;
    mouse_pos.y = y;
}

void enter_hook(int x, int y, void *)
{
    mouse_pos.x = x;
    mouse_pos.y = y;
}

template <typename T>
void framehook(T *data) {
    static int fps_period = 0;
    static std::stringstream ss;

//    if (not do_update)
//        return ;

    ++fps_period;
    camera_t &cur_cam = cam_vec.at(config::current_camera);
    cur_cam.alpha += look_direction.x;
    cur_cam.theta += look_direction.y;
    cur_cam.recompute_matrix();
    add_rotated_vec(
        cur_cam.position,
        cur_cam.rotate_matrix,
        move_direction
    );

    cameras.fill(cam_vec, queue);
    data->kernel.run(queue, false);
    canvas.dump(data->img.raw_pixel_data(), queue);
    data->win.put_image(data->img);
    if (fps_period == 30)
    {
        double fps = static_cast<double>(fps_meter.tv_sec) + static_cast<double>(fps_meter.tv_nsec) * 1e-9;
        clock_gettime(CLOCK_MONOTONIC, &fps_meter);
        fps -= static_cast<double>(fps_meter.tv_sec) + static_cast<double>(fps_meter.tv_nsec) * 1e-9;
        ss.str("");
        ss << "fps: " << -30. / fps;
        fps_period = 0;
    }
    data->win.put_string(ss.str(), 10, 15);
    look_direction.x = 0;
    look_direction.y = 0;
}

int main()
{
    auto platform = cllib::get_platforms().at(0);
    auto device = platform.get_devices().at(0);

    context = cllib::CLcontext(device);
    queue = cllib::CLqueue(context, device);

    cllib::CLprogram program(4, std::ifstream(config::fname), "ray_tracer", context);
    program.compile(device, true, "-D__OPENCL");

    sp_vec = {
            sphere_t({0,-1,3}, 1, Color::red, 500, 0.2f),
            sphere_t({2, 0, 4}, 1, Color::blue, 500, 0.2f),
            sphere_t({-2, 0, 4}, 1, Color::green, 10, 0.2f),
            sphere_t({0, -5001, 0}, 5000, Color::yellow, 1000, 0.2f)
    };
    cam_vec = {
            camera_t({0, 0, -1}, {0, 0, 1})
    };

    amb_vec = {
            ambient_t(0.2, Color::white)
    };
    pt_vec = {
            point_t({2, 1, 0}, 0.6, Color::white)
    };
    dir_vec = {
            direct_t({1, 4, 4}, 0.2, Color::white)
    };

    spheres = cllib::CLarray<sphere_t> (sp_vec, context, queue);
    cameras = cllib::CLarray<camera_t>(cam_vec, context, queue);

    ambients = cllib::CLarray<ambient_t>(amb_vec, context, queue);
    points = cllib::CLarray<point_t>(pt_vec, context, queue);
    directs = cllib::CLarray<direct_t>(dir_vec, context, queue);

    canvas = cllib::CLarray<unsigned int>(config::width * config::height, context, queue);
    cllib::CLkernel kernel(program, {config::width, config::height});

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
    kernel.set_arg(11, config::width);
    kernel.set_arg(12, config::height);

    clock_gettime(CLOCK_MONOTONIC, &fps_meter);
    kernel.run(queue);
    auto pixel_data = canvas.dump(queue);

    mlxlib::MLXcore core;
    mlxlib::MLXwindow win(core, config::width, config::height);
    mlxlib::MLXimage img(core, config::width, config::height);

    struct {
        mlxlib::MLXwindow &win;
        mlxlib::MLXimage &img;
        cllib::CLkernel &kernel;
    } data = {win, img, kernel};

//    win.add_keyhook(keypress_hook);
    win.add_hook(keypress_hook, mlxlib::events::key_press, nullptr, mlxlib::masks::key_press);
    win.add_hook(keyrelease_hook, mlxlib::events::key_release, nullptr, mlxlib::masks::key_release);
    win.add_hook(mouse_hook, mlxlib::events::mouse_move, nullptr, mlxlib::masks::mouse_motion);
    win.add_hook(enter_hook, mlxlib::events::mouse_enter_window, nullptr, mlxlib::masks::mouse_enter_window);
    win.add_hook(exit, mlxlib::events::window_close, nullptr);
    win.add_loop_hook(framehook, &data);

    img.fill(pixel_data);

    win.put_image(img);
    win.event_loop();
}