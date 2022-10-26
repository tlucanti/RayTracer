
#include <rtx.hpp>
#include <linalg.hpp>
#include <event_hooks.hpp>
#include <exception.hpp>
#include <scene_helper.hpp>

#include <sstream>
#include <mlxlib>

#define as_mutable_sphere(obj_ptr) static_cast<sphere_t *>(obj_ptr)
#define as_mutable_triangle(obj_ptr) static_cast<triangle_t *>(obj_ptr)
//...
#define as_mutable_box(obj_ptr) static_cast<box_t *>(obj_ptr)

#define get_mutable_obj_position(obj_ptr) (as_mutable_sphere(obj_ptr)->position)

typedef enum
{
    NONE_DIR,
    AXIS_OX,
    AXIS_OY,
    AXIS_OZ
} move_direction_t;

namespace move_params
{
    static FLOAT3 move_direction;
    static COMPLEX look_direction;
    static cl_int2 mouse_pos;
    static int do_update(0);
    static bool mouse_press(false);
    static struct timespec fps_meter;
    static void *selected_obj = nullptr;
    static obj_type_t selected_type;
    static cl_int2 sel_c;
    static cl_int2 sel_ox;
    static cl_int2 sel_oy;
    static cl_int2 sel_oz;
    static FLOAT sel_obj_dist;
    static move_direction_t selected_move_dir {NONE_DIR};
}

static inline int trunc(int x, int start, int end) {
    return std::min(std::max(x, start), end);
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
        trunc(start.x,  1, w),
        trunc(start.y, 1, h),
        trunc(end.x, 1, w),
        trunc(end.y, 1, h),
        color
    );
}

static inline void draw_rect(const cl_int2 &center, int side, uint32_t color)
{
    const int x_start = trunc(center.x - side, 0, static_cast<int>(rtx::config::width));
    const int x_end = trunc(center.x + side, 0, static_cast<int>(rtx::config::width));
    const int y_start = trunc(center.y - side, 0, static_cast<int>(rtx::config::height));
    const int y_end = trunc(center.y + side, 0, static_cast<int>(rtx::config::height));

    for (int x=x_start; x < x_end; ++x) {
        for (int y=y_start; y < y_end; ++y) {
            rtx::data::img->put_pixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y), color);
        }
    }
}

static inline void draw_circle(const cl_int2 &center, int radius, uint32_t color)
{
    const int w = static_cast<int>(rtx::config::width) - 1;
    const int h = static_cast<int>(rtx::config::height) - 1;

    for (int x = trunc(center.x - radius, 0, w) ; x <= trunc(center.x + radius, 0, w); ++x) {
        for (int y = trunc(center.y - radius, 0, h); y < trunc(center.y + radius, 0, h); ++y) {
            if (x + y < radius)
                rtx::data::img->put_pixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y), color);
        }
    }
}

static inline bool point_near(const cl_int2 &point, const cl_int2 &near, int gap)
{
    return std::abs(point.x - near.x) <= gap and std::abs(point.y - near.y) <= gap;
}

static inline void move_object(void *obj, obj_type_t type, move_direction_t dir, const cl_int2 &move_vec)
{
    FLOAT dx = static_cast<FLOAT>(move_vec.x) / static_cast<FLOAT>(rtx::config::width);
    FLOAT dy = static_cast<FLOAT>(move_vec.y) / static_cast<FLOAT>(rtx::config::height);
    FLOAT len = move_params::sel_obj_dist * sqrt(dx * dx + dy * dy);
    cl_int2 arrow_dir;

    switch (dir)
    {
        case (AXIS_OX): arrow_dir = move_params::sel_ox - move_params::sel_c; break ;
        case (AXIS_OY): arrow_dir = move_params::sel_oy - move_params::sel_c; break ;
        case (AXIS_OZ): arrow_dir = move_params::sel_oz - move_params::sel_c; break ;
        default: break ;
    }
    if (dot(arrow_dir, move_vec) < 0)
        len = -len;

    if (type != TRIANGLE) {
        switch (dir)
        {
            case (AXIS_OX): get_mutable_obj_position(obj).x += len; break ;
            case (AXIS_OY): get_mutable_obj_position(obj).y += len; break ;
            case (AXIS_OZ): get_mutable_obj_position(obj).z += len; break ;
            default: break ;
        }
    } else {
        FLOAT3 add;
        switch (dir)
        {
            case (AXIS_OX): add = ASSIGN_FLOAT3(len, 0., 0.); break ;
            case (AXIS_OY): add = ASSIGN_FLOAT3(0., len, 0.); break ;
            case (AXIS_OZ): add = ASSIGN_FLOAT3(0., 0., len); break ;
            default: break ;
        }
        as_mutable_triangle(obj)->a += add;
        as_mutable_triangle(obj)->b += add;
        as_mutable_triangle(obj)->c += add;
    }

    switch (type)
    {
        case (SPHERE): rtx::refill_spheres(); break ;
        case (PLANE): rtx::refill_planes(); break ;
        case (TRIANGLE): rtx::refill_triangles(); break ;
        case (CONE): rtx::refill_cones(); break ;
        case (CYLINDER): rtx::refill_cylinders(); break ;
        case (TOR): rtx::refill_torus(); break ;
        case (BOX): rtx::refill_boxes(); break ;
        default: break ;
    }
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
    int dx = x - move_params::mouse_pos.x;
    int dy = y - move_params::mouse_pos.y;
    move_params::mouse_pos.x = x;
    move_params::mouse_pos.y = y;
    if (move_params::mouse_press) {
        move_params::look_direction.x = dx * rtx::config::horizontal_look_speed;
        move_params::look_direction.y = dy * rtx::config::vertical_look_speed;
    }
    if (move_params::selected_move_dir != NONE_DIR) {
        move_object(move_params::selected_obj, move_params::selected_type, move_params::selected_move_dir, {{dx, dy}});
    }
}

void rtx::hooks::mousepress_hook(int button, int x, int y, void *)
{
    move_params::mouse_pos.x = x;
    move_params::mouse_pos.y = y;
    if (button == mlxlib::mouse::MOUSE_LEFT)
        move_params::mouse_press = true;
    else if (button == mlxlib::mouse::MOUSE_RIGHT) {
        if (move_params::selected_obj == nullptr)
            return ;
        if (point_near({{x, y}}, move_params::sel_ox, 10)) {
            move_params::selected_move_dir = AXIS_OX;
        } else if (point_near({{x, y}}, move_params::sel_oy, 10)) {
            move_params::selected_move_dir = AXIS_OY;
        } else if (point_near({{x, y}}, move_params::sel_oz, 10)) {
            move_params::selected_move_dir = AXIS_OZ;
        }
    }
}

void rtx::hooks::mouserelease_hook(int button, int x, int y, void *)
{
    if (button == mlxlib::mouse::MOUSE_LEFT)
        move_params::mouse_press = false;
    else if (button == mlxlib::mouse::MOUSE_RIGHT && move_params::selected_move_dir == NONE_DIR) {
        move_params::selected_obj = const_cast<void *>(object_at_pos(x, y, &move_params::selected_type));
        move_params::do_update = true;
    } else {
        move_params::selected_move_dir = NONE_DIR;
    }
}

static void update_sel_obj_center(void_ptr obj)
{
    FLOAT3 center;
    camera_t &cur_cam = rtx::objects::cam_vec.at(rtx::config::current_camera);

    if (move_params::selected_type != TRIANGLE)
        center = get_obj_position(obj);
    else
        center = as_triangle(obj)->a;
    cur_cam.recompute_reverse_matrix();

    FLOAT3 oc = center - cur_cam.position;
    move_params::sel_obj_dist = length(oc);
    FLOAT rlen = 1. / move_params::sel_obj_dist;
    oc *= rlen;

    FLOAT3 ox = oc + ASSIGN_FLOAT3(0.1, 0., 0.);
    FLOAT3 oy = oc + ASSIGN_FLOAT3(0., 0.1, 0.);
    FLOAT3 oz = oc + ASSIGN_FLOAT3(0., 0., 0.1);
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
        draw_rect(move_params::sel_ox, 10, mlxlib::color::red);
        draw_rect(move_params::sel_oy, 10, mlxlib::color::green);
        draw_rect(move_params::sel_oz, 10, mlxlib::color::blue);
        (void)draw_circle;
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
