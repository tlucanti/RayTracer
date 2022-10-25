
#include <rtx.hpp>
#include <linalg.hpp>
#include <event_hooks.hpp>
#include <exception.hpp>
#include <scene_helper.hpp>

#include <sstream>
#include <mlxlib>

namespace move_params
{
    static FLOAT3 move_direction;
    static COMPLEX look_direction;
    static cl_int2 mouse_pos;
    static int do_update(0);
    static bool mouse_press(false);
    static struct timespec fps_meter;
}

static void screenshot(const char *fname)
{
    uint64_t	f_size;
    uint64_t	header[7];

    std::ofstream fout(fname);
    if (not fout.is_open()) {
        rtx::Error("screen-capture", std::string("cannot open file `") + fname + "` for screenshot");
        return ;
    }
    bzero(header, 56);
    f_size = 14 + 40 + (rtx::config::width * rtx::config::height) * 4;
    rtx::config::height *= static_cast<unsigned int>(-1);
    header[0x00] = 0x4d42u | f_size << 16u;
    header[0x01] = 0x28000000380000u;
    header[0x02] = static_cast<uint64_t>(rtx::config::width << 16u) | static_cast<uint64_t>(rtx::config::height) << 48u;
    header[0x03] = static_cast<unsigned int>(rtx::config::height) >> 16u | 0x2000010000u;
    fout.write(reinterpret_cast<char *>(header), 56);
    rtx::config::height *= static_cast<unsigned int>(-1);
    fout.write(reinterpret_cast<char *>(rtx::data::img->raw_pixel_data()), (rtx::config::width * rtx::config::height) * 4);
    fout.close();
    rtx::Ok("screen-capture", "saved screenshot at ./scr.bmp");
}

static void reload_scene()
{
    rtx::objects::cam_vec.clear();
    rtx::objects::sp_vec.clear();
    rtx::objects::pl_vec.clear();
    rtx::objects::tr_vec.clear();
    rtx::objects::cn_vec.clear();
    rtx::objects::cy_vec.clear();
    rtx::objects::to_vec.clear();
    rtx::objects::li_vec.clear();

    rtx::parse_scene();
    rtx::init_scene();

    rtx::init_kernel();
}

void rtx::hooks::keypress_hook(int keycode, void *)
{
    (void)reload_scene;
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
        case mlxlib::keys::KEY_P: screenshot("../screenshot.bmp"); break ;
//        case mlxlib::keys::KEY_R: reload_scene(); break ;
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

void rtx::hooks::mouserelease_hook(int button, int x, int y, void *)
{
    if (button == mlxlib::mouse::MOUSE_LEFT)
        move_params::mouse_press = false;
    if (button == mlxlib::mouse::MOUSE_RIGHT)
    {
        obj_type_t obj_type;
        FLOAT3 center;
        camera_t &cur_cam = rtx::objects::cam_vec.at(rtx::config::current_camera);

        void_ptr obj = object_at_pos(x, y, &obj_type);
        if (obj == nullptr)
            return ;
        switch (obj_type)
        {
            case SPHERE: center = as_sphere(obj)->position; break ;

            case BOX: center = as_box(obj)->position; break ;
            default: center = {{0., 0., 0.}};
        }
        FLOAT3 oc = center - cur_cam.position;
        rtx::linalg::normalize_ref(oc);
        cur_cam.recompute_reverse_matrix();
        FLOAT3 direction = rotate_vector(oc, cur_cam.reverse_rotate_matrix);

        uint32_t cx, cy;
        pixpos_from_vec3(direction, &cx, &cy, rtx::config::width, rtx::config::height);

        std::cout << "released mouse at: (" << x << ", " << y << ")\n";
        std::cout << "hit object: " << obj << " (" << obj_type << ")\n";
        std::cout << "object center: (" << cx << ", " << cy << ")\n";
    }
}

void rtx::hooks::framehook(void *)
{
//    if (rtx::data::reloading.load(std::memory_order_seq_cst))
//        return ;
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
//    rtx::data::blur_kernel->run(*rtx::data::queue, true);
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
