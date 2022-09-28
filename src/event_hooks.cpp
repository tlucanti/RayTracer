
#include <rtx.hpp>
#include <linalg.hpp>
#include <event_hooks.hpp>

#include <sstream>
#include <mlxlib>

namespace move_params
{
    static FLOAT3 move_direction;
    static FLOAT2 look_direction;
    static cl_int2 mouse_pos;
    static int do_update(0);
    static bool mouse_press(false);
    static struct timespec fps_meter;
}

void rtx::hooks::keypress_hook(int keycode, void *)
{
    switch (keycode)
    {
        case mlxlib::keys::KEY_W: move_params::move_direction.z = rtx::config::forward_move_step; break ;
        case mlxlib::keys::KEY_S: move_params::move_direction.z = -rtx::config::forward_move_step; break ;
        case mlxlib::keys::KEY_A: move_params::move_direction.x = -rtx::config::side_move_speed; break ;
        case mlxlib::keys::KEY_D: move_params::move_direction.x = rtx::config::side_move_speed; break ;
        case mlxlib::keys::KEY_SPACE: move_params::move_direction.y = rtx::config::vertical_move_speed; break ;
        case mlxlib::keys::KEY_LCTRL:
        case mlxlib::keys::KEY_RCTRL: move_params::move_direction.y = -rtx::config::vertical_move_speed; break ;
        case mlxlib::keys::KEY_UP: move_params::look_direction.y = -rtx::config::vertical_look_speed; break ;
        case mlxlib::keys::KEY_DOWN: move_params::look_direction.y = rtx::config::vertical_look_speed; break ;
        case mlxlib::keys::KEY_LEFT: move_params::look_direction.x = -rtx::config::horizontal_look_speed; break ;
        case mlxlib::keys::KEY_RIGHT: move_params::look_direction.x = rtx::config::horizontal_look_speed; break ;
        case mlxlib::keys::KEY_ESCAPE: exit(0);
        default: return ;
    }
    move_params::do_update += 1;
}

void rtx::hooks::keyrelease_hook(int keycode, void *)
{
    switch (keycode)
    {
        case mlxlib::keys::KEY_W:
        case mlxlib::keys::KEY_S: move_params::move_direction.z = 0; break ;
        case mlxlib::keys::KEY_A:
        case mlxlib::keys::KEY_D: move_params::move_direction.x = 0; break ;
        case mlxlib::keys::KEY_SPACE:
        case mlxlib::keys::KEY_LCTRL:
        case mlxlib::keys::KEY_RCTRL: move_params::move_direction.y = 0; break ;
        case mlxlib::keys::KEY_UP:
        case mlxlib::keys::KEY_DOWN: move_params::look_direction.y = 0; break ;
        case mlxlib::keys::KEY_LEFT:
        case mlxlib::keys::KEY_RIGHT: move_params::look_direction.x = 0; break ;
        default: return ;
    }
    move_params::do_update -= 1;
}

void rtx::hooks::mouse_hook(int x, int y, void *)
{
    if (not move_params::mouse_press)
        return ;
    int dx = x - move_params::mouse_pos.x;
    int dy = y - move_params::mouse_pos.y;
    move_params::look_direction.x = dx * rtx::config::horizontal_look_speed;
    move_params::look_direction.y = dy * rtx::config::vertical_look_speed;
    move_params::mouse_pos.x = x;
    move_params::mouse_pos.y = y;
}

void rtx::hooks::mousepress_hook(int button, int x, int y, void *)
{
    move_params::mouse_pos.x = x;
    move_params::mouse_pos.y = y;
    if (button == mlxlib::mouse::MOUSE_LEFT)
        move_params::mouse_press = true;
}

void rtx::hooks::mouserelease_hook(int button, void *)
{
    if (button == mlxlib::mouse::MOUSE_LEFT)
        move_params::mouse_press = false;
}

void rtx::hooks::framehook(void *)
{
    static int fps_period = 0;
    static std::stringstream ss;

    if (move_params::do_update == 0 and not move_params::mouse_press)
        return ;

    ++fps_period;
    camera_t &cur_cam = rtx::objects::cam_vec.at(rtx::config::current_camera);
    cur_cam.alpha += move_params::look_direction.x;
    cur_cam.theta += move_params::look_direction.y;
    cur_cam.recompute_matrix();
    rtx::linalg::add_rotated_vec(
        cur_cam.position,
        cur_cam.rotate_matrix,
        move_params::move_direction
    );

    rtx::scene::cameras.fill(rtx::objects::cam_vec, *rtx::data::queue);
    rtx::data::kernel->run(*rtx::data::queue, false);
    rtx::scene::canvas.dump(rtx::data::img->raw_pixel_data(), *rtx::data::queue);
    rtx::data::win->put_image(*rtx::data::img);
    if (fps_period == 30)
    {
        double fps = static_cast<double>(move_params::fps_meter.tv_sec) + static_cast<double>(move_params::fps_meter.tv_nsec) * 1e-9;
        clock_gettime(CLOCK_MONOTONIC, &move_params::fps_meter);
        fps -= static_cast<double>(move_params::fps_meter.tv_sec) + static_cast<double>(move_params::fps_meter.tv_nsec) * 1e-9;
        ss.str("");
        ss << "fps: " << -30. / fps;
        fps_period = 0;
    }
    rtx::data::win->put_string(ss.str(), 10, 15);
    move_params::look_direction.x = 0;
    move_params::look_direction.y = 0;
}
