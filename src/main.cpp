
#include <cllib>
#include <mlxlib>
#include "struct.hpp"
#include <ctime>
#include <sstream>
#include <atomic>
#include <unistd.h>
#include "cl/kernel.cl"

namespace config
{
    inline constexpr int width = 1000;
    inline constexpr int height = 1000;

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
int do_update(0);
bool mouse_press(false);

cllib::CLcontext context;
cllib::CLqueue queue;

std::vector<camera_t> cam_vec;
std::vector<sphere_t> sp_vec;
std::vector<plane_t> pl_vec;
std::vector<triangle_t> tr_vec;
std::vector<cone_t> cn_vec;
std::vector<cylinder_t> cy_vec;

std::vector<light_t> li_vec;

cllib::CLarray<camera_t, cllib::read_only_array> cameras;
cllib::CLarray<sphere_t, cllib::read_only_array> spheres;
cllib::CLarray<plane_t, cllib::read_only_array> planes;
cllib::CLarray<triangle_t, cllib::read_only_array> triangles;
cllib::CLarray<cone_t, cllib::read_only_array> cones;
cllib::CLarray<cylinder_t, cllib::read_only_array> cylinders;

cllib::CLarray<light_t, cllib::read_only_array> lights;

cllib::CLarray<unsigned int, cllib::write_only_array> canvas;

struct timespec fps_meter;

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
    if (not mouse_press)
        return ;
    int dx = x - mouse_pos.x;
    int dy = y - mouse_pos.y;
    look_direction.x = dx * config::horizontal_look_speed;
    look_direction.y = dy * config::vertical_look_speed;
    mouse_pos.x = x;
    mouse_pos.y = y;
}

void mousepress_hook(int button, int x, int y, void *)
{
    mouse_pos.x = x;
    mouse_pos.y = y;
    if (button == mlxlib::mouse::MOUSE_LEFT)
        mouse_press = true;
}

void mouserelease_hook(int button, void *)
{
    if (button == mlxlib::mouse::MOUSE_LEFT)
        mouse_press = false;
}

template <typename T>
void framehook(T *data) {
    static int fps_period = 0;
    static std::stringstream ss;

    if (do_update == 0 and not mouse_press)
        return ;

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
            sphere_t({0,-1,3}, 1, Color::red, 500, 0.2),
            sphere_t({2, 0, 4}, 1, Color::blue, 500, 0.2),
            sphere_t({-2, 0, 4}, 1, Color::green, 10, 0.2),
//            sphere_t({0, -5001, 0}, 5000, Color::yellow, 1000, 0.2),
//            sphere_t({2,2,2},0.05,Color::red,0.,0.),
//            sphere_t({3,2,1},0.05,Color::red,0.,0.),
//            sphere_t({3,3,3},0.05,Color::red,0.,0.),
//            sphere_t({0,2,10},0.1,Color::green,0.,0.),
//            sphere_t({1,2,10},0.1,Color::green,0.,0.),
//            sphere_t({-1,2,10},0.1,Color::green,0.,0.),
//            sphere_t({0,3,10},0.1,Color::blue,0.,0.),
//            sphere_t({0,1,10},0.1,Color::blue,0.,0.),
//            sphere_t({0,2,11},0.1,Color::red,0.,0.),
//            sphere_t({0,2,9},0.1,Color::red,0.,0.)
    };
    pl_vec = {
//            plane_t({0, -1, 0}, {0, 1, 0}, Color::yellow, 1000, 0.2)
    };
    tr_vec = {
//            triangle_t({2, 2, 2}, {3, 3, 3}, {3, 2, 1}, Color::grey, 10, 0.) // 1 2 3
//            triangle_t({5, 1, 0},{0, 1, 0},{2, 2, 2},   Color::grey, 300, 0.7) // 3 2 1
//            triangle_t({2, 2, 2}, {3, 3, 3}, {3, 2, 1}, Color::grey, 10, 0.) // 1 2 3
//            triangle_t({3, 3, 3},{2, 2, 2}, {3, 2, 1},  Color::grey, 10, 0.) // 3 1 2
    };
    cn_vec = {
            cone_t({0, 7, 5}, {-0.1,1,0.1}, 0.5, Color::purple, 500, 0.3)
    };
    cy_vec = {
//            cylinder_t({0, 0, 10}, {1, 0, 0}, 1, Color::yellow, 0, 0)
    };

    cam_vec = {
            camera_t({0, 0, -1}, {0, 0.4, 1})
    };

    li_vec = {
            ambient_t(0.1, Color::white),
            point_t(0.6, {2, 7, 1}, Color::white),
            direct_t(0.2, {1, 3, 4}, Color::white)
    };

//    FLOAT3 m[3];
//    FLOAT3 a = {1, 0, 0};
//    FLOAT3 b = {0, 1, 0};
//    set_rotation_matrix(m, a, b);
//    std::cout << m[0].x << ' ' << m[0].y << ' ' << m[0].z << std::endl;
//    std::cout << m[1].x << ' ' << m[1].y << ' ' << m[1].z << std::endl;
//    std::cout << m[2].x << ' ' << m[2].y << ' ' << m[2].z << std::endl;
//    FLOAT3 v = rotate_vector(a, m);
//    std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;

    spheres = cllib::CLarray<sphere_t, cllib::read_only_array>(sp_vec, context, queue);
    planes = cllib::CLarray<plane_t, cllib::read_only_array>(pl_vec, context, queue);
    triangles = cllib::CLarray<triangle_t, cllib::read_only_array>(tr_vec, context, queue);
    cones = cllib::CLarray<cone_t, cllib::read_only_array>(cn_vec, context, queue);
    cylinders = cllib::CLarray<cylinder_t, cllib::read_only_array>(cy_vec, context, queue);

    cameras = cllib::CLarray<camera_t, cllib::read_only_array>(cam_vec, context, queue);
    lights = cllib::CLarray<light_t, cllib::read_only_array>(li_vec, context, queue);

    canvas = cllib::CLarray<unsigned int, cllib::write_only_array>(config::width * config::height, context);
    cllib::CLkernel kernel(program, {config::width, config::height});

    kernel.reset_args();
    kernel.set_next_arg(canvas);
    kernel.set_next_arg(spheres);
    kernel.set_next_arg(planes);
    kernel.set_next_arg(triangles);
    kernel.set_next_arg(cones);
    kernel.set_next_arg(cylinders);

    kernel.set_next_arg(lights);
    kernel.set_next_arg(cameras);

    kernel.set_next_arg(static_cast<int>(spheres.size()));
    kernel.set_next_arg(static_cast<int>(planes.size()));
    kernel.set_next_arg(static_cast<int>(triangles.size()));
    kernel.set_next_arg(static_cast<int>(cones.size()));
    kernel.set_next_arg(static_cast<int>(cylinders.size()));

    kernel.set_next_arg(static_cast<int>(lights.size()));
    kernel.set_next_arg(static_cast<int>(cameras.size()));
    kernel.set_next_arg(config::width);
    kernel.set_next_arg(config::height);

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
//    win.add_hook(enter_hook, mlxlib::events::mouse_enter_window, nullptr, mlxlib::masks::mouse_enter_window);
    win.add_hook(exit, mlxlib::events::window_close, nullptr);
    win.add_hook(mousepress_hook, mlxlib::events::mouse_press, nullptr, mlxlib::masks::mouse_press);
    win.add_hook(mouserelease_hook, mlxlib::events::mouse_release, nullptr, mlxlib::masks::mouse_release);
    win.add_loop_hook(framehook, &data);

    img.fill(pixel_data);

    win.put_image(img);
    win.event_loop();
}