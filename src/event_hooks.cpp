
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
    static void_ptr selected_obj = nullptr;
    static obj_type_t selected_type;
    static cl_int2 sel_c;
    static cl_int2 sel_ox;
    static cl_int2 sel_oy;
    static cl_int2 sel_oz;
}

static inline void _draw_line_low(int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;

    for (int x=x0; x <= x1; ++x) {
        rtx::data::img->put_pixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y), color);
        rtx::data::img->put_pixel(static_cast<uint32_t>(x + 1), static_cast<uint32_t>(y + 1), color);
        rtx::data::img->put_pixel(static_cast<uint32_t>(x - 1), static_cast<uint32_t>(y - 1), color);
        if (D > 0) {
            y = y + yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2 * dy;
        }
    }
}

static inline void _draw_line_high(int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;

    for (int y=y0; y <= y1; ++y) {
        rtx::data::img->put_pixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y), color);
        rtx::data::img->put_pixel(static_cast<uint32_t>(x + 1), static_cast<uint32_t>(y + 1), color);
        rtx::data::img->put_pixel(static_cast<uint32_t>(x - 1), static_cast<uint32_t>(y - 1), color);
        if (D > 0) {
            x = x + xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2 * dx;
        }
    }
}

static inline void _draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
        if (x0 > x1) {
            _draw_line_low(x1, y1, x0, y0, color);
        } else {
            _draw_line_low(x0, y0, x1, y1, color);
        }
    } else {
        if (y0 > y1) {
            _draw_line_high(x1, y1, x0, y0, color);
        } else {
            _draw_line_high(x0, y0, x1, y1, color);
        }
    }
}

static inline void draw_line(const cl_int2 &start, const cl_int2 &end, uint32_t color)
{
    const int w = static_cast<int>(rtx::config::width) - 2;
    const int h = static_cast<int>(rtx::config::height) - 2;
    _draw_line(
        std::max(std::min(start.x, w), 1),
        std::max(std::min(start.y, h), 1),
        std::max(std::min(end.x, w), 1),
        std::max(std::min(end.y, h), 1),
        color
    );
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
    else if (button == mlxlib::mouse::MOUSE_RIGHT) {
        move_params::selected_obj = object_at_pos(x, y, &move_params::selected_type);
        move_params::do_update = true;
    }
}

static void update_sel_obj_center(void_ptr obj)
{
    FLOAT3 center;
    camera_t &cur_cam = rtx::objects::cam_vec.at(rtx::config::current_camera);

    switch (move_params::selected_type)
    {
        case SPHERE: center = as_sphere(obj)->position; break ;

        case BOX: center = as_box(obj)->position; break ;
        default: center = {{0., 0., 0.}};
    }
    cur_cam.recompute_reverse_matrix();

    FLOAT3 oc = center - cur_cam.position;
    FLOAT3 ox = oc + ASSIGN_FLOAT3(1., 0., 0.);
    FLOAT3 oy = oc + ASSIGN_FLOAT3(0., 1., 0.);
    FLOAT3 oz = oc + ASSIGN_FLOAT3(0., 0., 1.);
    rtx::linalg::normalize_ref(oc);
    rtx::linalg::normalize_ref(ox);
    rtx::linalg::normalize_ref(oy);
    rtx::linalg::normalize_ref(oz);

    FLOAT3 direction = rotate_vector(oc, cur_cam.reverse_rotate_matrix);
    ox = rotate_vector(ox, cur_cam.reverse_rotate_matrix);
    oy = rotate_vector(oy, cur_cam.reverse_rotate_matrix);
    oz = rotate_vector(oz, cur_cam.reverse_rotate_matrix);

    pixpos_from_vec3(
        direction,
        &move_params::sel_c.x,
        &move_params::sel_c.y,
        static_cast<int>(rtx::config::width),
        static_cast<int>(rtx::config::height)
    );
    pixpos_from_vec3(
            ox,
            &move_params::sel_ox.x,
            &move_params::sel_ox.y,
            static_cast<int>(rtx::config::width),
            static_cast<int>(rtx::config::height)
    );
    pixpos_from_vec3(
            oy,
            &move_params::sel_oy.x,
            &move_params::sel_oy.y,
            static_cast<int>(rtx::config::width),
            static_cast<int>(rtx::config::height)
    );
    pixpos_from_vec3(
            oz,
            &move_params::sel_oz.x,
            &move_params::sel_oz.y,
            static_cast<int>(rtx::config::width),
            static_cast<int>(rtx::config::height)
    );

    move_params::do_update = true;

//        std::cout << "released mouse at: (" << x << ", " << y << ")\n";
//    std::cout << "hit object: " << obj << " (" << obj_type << ")\n";
//    std::cout << "object center: (" << move_params::center_pos.x << ", " << move_params::center_pos.y << ")\n";
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
    rtx::data::win->put_string(ss.str(), 10, 15);

    if (move_params::selected_obj != nullptr)
    {
        update_sel_obj_center(move_params::selected_obj);
        draw_line(move_params::sel_c, move_params::sel_ox, mlxlib::color::red);
        draw_line(move_params::sel_c, move_params::sel_oy, mlxlib::color::green);
        draw_line(move_params::sel_c, move_params::sel_oz, mlxlib::color::blue);
    }
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
    move_params::look_direction.x = 0;
    move_params::look_direction.y = 0;
}
