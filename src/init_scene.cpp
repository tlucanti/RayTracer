
#include <fstream>
#include <json>
#include <rtx.hpp>
#include <exception.hpp>

namespace flags
{
    inline static bool config = false;
    inline static bool cameras = false;
    inline static bool spheres = false;
}

template <uintmax_t mod=10000000001191, uintmax_t prime=257>
inline constexpr uintmax_t poly_hash(const char *str, size_t size, size_t idx)
{
    if (size == 0)
        return 0;
    else if (size == 1)
        return str[0];
    else if (idx < size)
        return (
            str[idx]
            + poly_hash<mod, prime>(str, size, idx + 1) * prime % mod
        ) % mod;
    return str[idx];
}

typedef enum
{
    INT_TYPE,
    NUM_TYPE,
    STRING_TYPE,
    VEC3_TYPE,
    COLOR_TYPE,

    OBJECT_TYPE,
    ARRAY_TYPE
} item_type;

inline constexpr uintmax_t hash(const std::string &str)
{
    return poly_hash(str.c_str(), str.size(), 0);
}

inline constexpr uintmax_t operator ""_hash(const char *str, size_t len)
{
    return poly_hash(str, len, 0);
}

template <typename type_T>
static std::string str(const type_T &t)
{
    return (std::stringstream() << '`' << t << '`').str();
}

static FLOAT3 to_vec3(const nlohmann::json &v)
{
    return (FLOAT3){v.at(0), v.at(1), v.at(2)};
}

static void parse_assert(bool cond, const std::string &str)
{
    if (not cond)
        rtx::Error("parser", str);
}

static void parse_warn(bool cond, const std::string &str)
{
    if (not cond)
        rtx::Warning("parser", str);
}

static void parse_notify(bool cond, const std::string &str)
{
    if (not cond)
        rtx::Info("parser", str);
}

static void parse_ok(bool cond, const std::string &str)
{
    if (cond)
        rtx::Ok("parser", str);
}

template <class value_type>
static void parse_range_assert(const std::string &name, const nlohmann::json &value, const value_type &lower, const value_type &upper)
{
    parse_assert(
        value >= lower and value <= upper,
        name + rtx::C[str(value)] + " should be in range "
        + rtx::Color::yellow + std::to_string(lower) + ":" + std::to_string(upper) + rtx::Color::reset
    );
}

static void parse_positive_assert(const std::string &name, const nlohmann::json &value)
{
    parse_assert(
        value > 0,
        name + rtx::C[str(value)] + " should be in positive"
    );
}

template <class value_type>
static void parse_redefinition_notify(const std::string &name, bool cond, const nlohmann::json &value, const value_type &current)
{
    parse_notify(
        not cond,
        rtx::Y[name] + " is already set to " + rtx::C[str(current)] + ", redefining to "
            + rtx::C[str(value)]
    );
}

static void parse_type_assert(const std::string &name, const nlohmann::json &value, item_type type)
{
    std::string msg = " value for " + rtx::Y[name] + ", got: " + rtx::C[str(value)];
    switch (type)
    {
        case INT_TYPE: parse_assert(value.is_number_integer(), "expected integer number"); break ;
        case NUM_TYPE: parse_assert(value.is_number(), "expected number" + msg); break ;
        case STRING_TYPE: parse_assert(value.is_string(), "expected string " + msg); break ;
        case COLOR_TYPE: parse_assert(value.is_string(), "expected color name/hex color code" + msg); break ;
        case VEC3_TYPE:
            parse_assert(value.is_array(), "expected vector3" + msg);
            parse_assert(value.size() == 3, "expected vector of size 3" + msg);
            break ;
        case OBJECT_TYPE: parse_assert(value.is_object(), "expected dict type for " + rtx::Y[name]); break ;
        case ARRAY_TYPE: parse_assert(value.is_array(), "expected array type for " + rtx::Y[name]); break ;
        default: throw std::runtime_error(
            rtx::R["[INTERNAL ERROR]: init_scene::parse_type_assert: unexpected item_type " + str(type)]);
    }
}

static void parse_unknown_notify(const std::string &name)
{
    parse_notify(true, "unknown field " + rtx::C[str(name)] + " (ignoring)");
}

template <class value_type>
static void parse_undefined_warn_set(const std::string &name, bool cond, const value_type &value, value_type &ref)
{
    parse_warn(cond, "value " + rtx::C[name] + " undefined, defaulting to " + rtx::Y[std::to_string(value)]);
    if (not cond)
        ref = value;
}

static void parse_undefined_assert(const std::string &name, bool cond)
{
    parse_assert(cond, "value " + rtx::C[name] + " undefined");
}

static void parse_vec3_singular_assert(const std::string &name, const FLOAT3 &vec)
{
    parse_assert(abs(rtx::linalg::dot(vec, vec)) > EPS, name + " is singular vector");
}

void parse_config(const nlohmann::json &res)
{
    bool width = false;
    bool height = false;
    bool fovx = false;
    bool fovy = false;
    bool move_speed = false;
    bool look_speed = false;

    parse_type_assert("config", res, OBJECT_TYPE);
    for (const auto &item : res.items())
    {
        switch (hash(item.key()))
        {
            case ("screen_width"_hash): {
                std::string name = "window resolution width";
                parse_type_assert(name, item.value(), INT_TYPE);
                parse_positive_assert(name, item.value());
                parse_redefinition_notify(name, width, item.value(), rtx::config::width);
                rtx::config::width = item.value();
                width = true;
                break ;
            }
            case ("screen_height"_hash): {
                std::string name = "window resolution height";
                parse_type_assert(name, item.value(), INT_TYPE);
                parse_positive_assert(name, item.value());
                parse_redefinition_notify(name, height, item.value(), rtx::config::height);
                rtx::config::height = item.value();
                height = true;
                break ;
            }
            case ("fovx"_hash): {
                std::string name = "fov x";
                parse_type_assert(name, item.value(), NUM_TYPE);
                parse_range_assert(name, item.value(), 0, 180);
                parse_redefinition_notify(name, fovx, item.value(), rtx::config::fovx);
                rtx::config::fovx = item.value();
                fovx = true;
                break ;
            }
            case ("fovy"_hash): {
                std::string name = "fov y";
                parse_type_assert(name, item.value(), NUM_TYPE);
                parse_range_assert(name, item.value(), 0, 180);
                parse_redefinition_notify(name, fovy, item.value(), rtx::config::fovy);
                rtx::config::fovy = item.value();
                fovy = true;
                break ;
            }
            case ("move_speed"_hash): {
                std::string name = "move speed";
                parse_type_assert(name, item.value(), NUM_TYPE);
                parse_redefinition_notify(name, move_speed, item.value(), rtx::config::forward_move_step);
                rtx::config::forward_move_step = item.value();
                move_speed = true;
                break ;
            }
            case ("look_speed"_hash): {
                std::string name = "mouse sensitivity";
                parse_type_assert(name, item.value(), NUM_TYPE);
                parse_redefinition_notify(name, look_speed, item.value(), rtx::config::horizontal_look_speed);
                rtx::config::horizontal_look_speed = item.value();
                look_speed = true;
                break ;
            }
            default:
                parse_unknown_notify(item.key());
        }
    }
    parse_undefined_warn_set("window resolution width", width, 800, rtx::config::width);
    parse_undefined_warn_set("window resolution height", height, 600, rtx::config::height);
    parse_undefined_warn_set("fov x", fovx, 90., rtx::config::fovx);
    parse_undefined_warn_set("fov y", fovy, 90., rtx::config::fovy);
    parse_undefined_warn_set("move speed", move_speed, 0.1, rtx::config::forward_move_step);
    rtx::config::forward_move_step = rtx::config::forward_move_step / 2.;
    rtx::config::forward_move_step = rtx::config::forward_move_step / 2.;
    parse_undefined_warn_set("mouse sensitivity", look_speed, 0.005, rtx::config::vertical_look_speed);
    rtx::config::horizontal_look_speed = rtx::config::vertical_look_speed;
    parse_ok(true, "scene::config parsed")
}

void parse_cameras_single(const nlohmann::json &camera)
{
    parse_type_assert("cameras", camera, OBJECT_TYPE);
    FLOAT3 position = {0, 0, 0};
    FLOAT3 direction = {0, 0, 1};
    bool got_position = false;
    bool got_direction = false;
    for (const auto &item : camera.items())
    {
        switch (hash(item.key()))
        {
            case ("position"_hash): {
                std::string name = "camera position";
                parse_type_assert(name, item.value(), VEC3_TYPE);
                parse_redefinition_notify(name, got_position, item.value(), position);
                position = to_vec3(item.key());
                got_position = true;
                break ;
            }
            case ("direction"_hash): {
                std::string name = "camera direction";
                parse_type_assert(name, item.value(), VEC3_TYPE);
                parse_redefinition_notify(name, got_direction, item.value(), direction);
                direction = to_vec3(item.key());
                parse_vec3_singular_assert(name, direction);
                got_direction = true;
                break ;
            }
            default:
                parse_unknown_notify(item.key());
        }
    }
    parse_undefined_assert("camera position", got_position);
    parse_undefined_assert("camera direction", got_direction);
}

void parse_cameras(const nlohmann::json &cameras)
{
    parse_type_assert("cameras array", cameras, ARRAY_TYPE);
    for (const auto &item : cameras)
        parse_cameras_single(item);
}

void parse_spheres(const nlohmann::json &spheres)
{

}

void parse_planes(const nlohmann::json &planes)
{

}

void parse_triangles(const nlohmann::json &triangles)
{

}

void parse_cones(const nlohmann::json &cones)
{

}

void parse_cylinders(const nlohmann::json &cylinders)
{

}

void parse_torus(const nlohmann::json &torus)
{

}

void parse_lights(const nlohmann::json &lights)
{

}

void rtx::parse_scene(const char *fname)
{
    std::ifstream stream(fname);
    if (not stream.is_open())
        throw std::runtime_error(std::string("cannot open `") + fname + "` file");
    nlohmann::json data = nlohmann::json::parse(stream);
    for (const auto &i : data.items())
    {
        std::cout << "data: " << i.key() << ' ' << i.value() << std::endl;
        switch(hash(i.key()))
        {
            case("resolution"_hash): parse_config(i.value()); break ;
            case("cameras"_hash): parse_cameras(i.value()); break ;
            case("spheres"_hash): parse_spheres(i.value()); break ;
            case("planes"_hash): parse_planes(i.value()); break ;
            case("triangles"_hash): parse_triangles(i.value()); break ;
            case("cones"_hash): parse_cones(i.value()); break ;
            case("cylinders"_hash): parse_cylinders(i.value()); break ;
            case("torus"_hash): parse_torus(i.value()); break ;
            case("lights"_hash): parse_lights(i.value()); break ;
            default: parse_unknown_notify(i.key());
        }
    }

    rtx::objects::sp_vec = {
            sphere_t({0 , 0, 0}, 0.2, rtx::color::white, 0, 0),
            sphere_t({3 , 2, 0}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({0,-1,3}, 1, Color::red, 500, 0.2),
            sphere_t({2, 0, 4}, 1, rtx::color::blue, 500, 0.2),
            sphere_t({-2, 0, 4}, 1, rtx::color::green, 10, 0.2),
            sphere_t({3,-1,0}, 1, rtx::color::red, 500, 0.2),
//            sphere_t({4, 0, 0}, 1, Color::blue, 500, 0.2),
//            sphere_t({4, 0, -2}, 1, Color::green, 10, 0.2),
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
    rtx::objects::pl_vec = {
//            plane_t({0, -1, 0}, {0, 1, 0}, Color::yellow, 1000, 0.2)
    };
    rtx::objects::tr_vec = {
//            triangle_t({2, 2, 2}, {3, 3, 3}, {3, 2, 1}, Color::grey, 10, 0.) // 1 2 3
//            triangle_t({5, 1, 0},{0, 1, 0},{2, 2, 2},   Color::grey, 300, 0.7) // 3 2 1
//            triangle_t({2, 2, 2}, {3, 3, 3}, {3, 2, 1}, Color::grey, 10, 0.) // 1 2 3
//            triangle_t({3, 3, 3},{2, 2, 2}, {3, 2, 1},  Color::grey, 10, 0.) // 3 1 2
    };
    rtx::objects::cn_vec = {
//            cone_t({0, 5, 7}, {0,1,0}, 0.5, {255, 149, 0}, 500, 0.3)
//            cone_t({0, 0, 0}, {1,1,1}, 1, 0, {255, 149, 0}, 500, 0.3)
    };
    rtx::objects::cy_vec = {
//            cylinder_t({0, 0, 10}, {1, 0, 0}, 1, Color::yellow, 0, 0)
    };
    rtx::objects::to_vec = {
            torus_t({0, 0, 0}, {0, 1, 0}, 0.5, 3, rtx::color::cyan, 0, 0)
    };

    rtx::objects::cam_vec = {
            camera_t({-5, 0, 0}, {1, 0, 0})
    };

    rtx::objects::li_vec = {
            ambient_t(0.1, rtx::color::white),
            point_t(0.6, rtx::color::white, {3, 2, 0}),
            direct_t(0.3, rtx::color::white, {1, 3, 4})
    };

//    FLOAT t, p;
//    FLOAT3 cam = {0, 0, 1};
//    FLOAT3 dir = {0, 1, 0};
//    cone_t *cn = &cn_vec.at(0);
//    t = intersect_cone(cam, dir, cn, &p);
//    std::cout << "t: " << t << " p: " << p << std::endl;
//    FLOAT3 point = cam + dir * t;
//    FLOAT3 o = cn->center + cn->direction * p;
//    FLOAT3 normal = point - o;
//    std::cout << "point: " << point << std::endl;
//    std::cout << "o: " << o << std::endl;
//    std::cout << "normal: " << normal << std::endl;
//    return 0;

//    FLOAT3 m[3];
//    FLOAT3 a = {1, 0, 0};
//    FLOAT3 b = {0, 1, 0};
//    set_rotation_matrix(m, a, b);
//    std::cout << m[0].x << ' ' << m[0].y << ' ' << m[0].z << std::endl;
//    std::cout << m[1].x << ' ' << m[1].y << ' ' << m[1].z << std::endl;
//    std::cout << m[2].x << ' ' << m[2].y << ' ' << m[2].z << std::endl;
//    FLOAT3 v = rotate_vector(a, m);
//    std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;

}
