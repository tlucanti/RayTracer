
#include <fstream>
#include <json>
#include <rtx.hpp>
#include <exception.hpp>
#include <objects.hpp>

union Flags
{
    struct
    {
        unsigned spheres : 1;
        unsigned planes : 1;
        unsigned triangles : 1;
        unsigned hyperboloids : 1;
        unsigned cylinders : 1;
        unsigned : 0;
        unsigned config : 1;
        unsigned cameras : 1;
        unsigned torus : 1;
        unsigned lights : 1;
    };
    unsigned objects;
    unsigned long long all;
};

inline static Flags flags;

template <uintmax_t mod=10000000001191, uintmax_t prime=257>
inline constexpr uintmax_t poly_hash(const char *str, size_t size, size_t idx)
{
    if (size == 0)
        return 0;
    else if (size == 1)
        return static_cast<uintmax_t>(str[0]);
    else if (idx < size)
        return (
            static_cast<uintmax_t>(str[idx])
            + poly_hash<mod, prime>(str, size, idx + 1) * prime % mod
        ) % mod;
    return static_cast<uintmax_t>(str[idx]);
}

typedef enum
{
    INT_TYPE,
    NUM_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    VEC3_TYPE,
    COLOR_TYPE,
    HEX_TYPE,

    OBJECT_TYPE,
    ARRAY_TYPE
} item_type;

inline uintmax_t hash(const std::string &str)
{
    return poly_hash(str.c_str(), str.size(), 0);
}

inline constexpr uintmax_t operator ""_hash(const char *str, size_t len)
{
    return poly_hash(str, len, 0);
}

template <typename value_type>
static std::string str(const value_type &t)
{
    std::stringstream ss;
    ss << '`' << t << '`';
    return ss.str();
}

template <typename value_type>
static std::string to_string(const value_type &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

static FLOAT3 to_vec3(const nlohmann::json &v)
{
    return FLOAT3({{v[0], v[1], v[2]}});
}

static FLOAT3 to_col3(const std::string &str)
{
    std::stringstream ss;
    ss << std::hex << str;
    unsigned int i;
    ss >> i;
    return {{
        static_cast<double>((i >> 16u) & 0xFF),
        static_cast<double>((i >> 8u) & 0xFF),
        static_cast<double>(i & 0xFF)
    }};
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

static void parse_print(const std::string &str)
{
    std::cout << "[ ** ]: "_W + "parser"_P + ": "_W + str + rtx::Color::reset + "\n";
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
        case VEC3_TYPE:
            parse_assert(value.is_array(), "expected vector3" + msg);
            parse_assert(value.size() == 3, "expected vector of size 3" + msg);
            break ;
        case OBJECT_TYPE: parse_assert(value.is_object(), "expected dict type for " + rtx::Y[name]); break ;
        case ARRAY_TYPE: parse_assert(value.is_array(), "expected array type for " + rtx::Y[name]); break ;
        case COLOR_TYPE:
            parse_assert(value.is_array() or value.is_string(), "expected hex color/color name/rgb vector");
            parse_assert(not value.is_array() or value.size() == 3, "expected rgb vector of size 3");
            break ;
        case HEX_TYPE:
            parse_assert(std::string(value).size() == 7 and std::string(value).at(0) == '#', "hex color (like " + "#RRGGBB"_G + ")");
            break ;
        case BOOL_TYPE:
            parse_assert(value.is_boolean(), "expected boolean" +  msg); break ;
        default: throw std::runtime_error(
            rtx::R["[INTERNAL ERROR]: init_scene::parse_type_assert: unexpected item_type " + str(type)]);
    }
}

static void parse_unknown_notify(const std::string &name)
{
    parse_warn(false, "unknown field " + rtx::C[str(name)] + " (ignoring)");
}

template <class value_type>
static void parse_undefined_warn_set(const std::string &name, bool cond, const value_type &value, value_type &ref)
{
    parse_warn(cond, "value " + rtx::C[name] + " undefined, defaulting to " + rtx::Y[to_string(value)]);
    if (not cond)
        ref = value;
}

template <class value_type>
static void parse_undefined_info_set(const std::string &name, bool cond, const value_type &value, value_type &ref)
{
    parse_notify(cond, "value " + rtx::C[name] + " undefined, defaulting to " + rtx::Y[to_string(value)]);
    if (not cond)
        ref = value;
}

static void parse_undefined_assert(const std::string &name, bool cond)
{
    parse_assert(cond, "value " + rtx::C[name] + " undefined");
}

static void parse_extra_defined_warn(const std::string &name, bool cond, const std::string &msg)
{
    parse_assert(not cond, "value " + rtx::C[name] + " should not be defined " + msg);
}

static void parse_vec3_singular_assert(const std::string &name, const FLOAT3 &vec)
{
    parse_assert(rtx::linalg::dot(vec, vec) > EPS, name + " is singular vector");
}

template <class item_type>
static void parse_vec3_point(const std::string &name, const item_type &item, bool &flag, FLOAT3 &vec)
{
    parse_type_assert(name, item.value(), VEC3_TYPE);
    parse_redefinition_notify(name, flag, item.value(), vec);
    vec = to_vec3(item.value());
    flag = true;
}

template <class item_type>
static void parse_vec3_unit(const std::string &name, const item_type &item, bool &flag, FLOAT3 &unit)
{
    parse_vec3_point(name, item, flag, unit);
    parse_vec3_singular_assert(name, unit);
}

template <class item_type, class value_type>
static void parse_int_positive(const std::string &name, const item_type &item, bool &flag, value_type &value)
{
    parse_type_assert(name, item.value(), INT_TYPE);
    parse_positive_assert(name, item.value());
    value = static_cast<value_type>(item.value());
    flag = true;
}

template <class item_type>
static void parse_bool(const std::string &name, const item_type &item, bool &flag, bool &value)
{
    parse_type_assert(name, item.value(), BOOL_TYPE);
    value = static_cast<bool>(item.value());
    flag = true;
}

template <class item_type>
static void parse_float_positive(const std::string &name, const item_type &item, bool &flag, FLOAT &value)
{
    parse_type_assert(name, item.value(), NUM_TYPE);
    parse_positive_assert(name, item.value());
    value = static_cast<FLOAT>(item.value());
    flag = true;
}

template <class item_type>
static void parse_float_ranged(const std::string &name, const item_type &item, bool &flag, FLOAT &value, FLOAT lower, FLOAT upper)
{
    parse_type_assert(name, item.value(), NUM_TYPE);
    parse_range_assert(name, value, lower, upper);
    value = item.value();
    flag = true;
}

template <class item_type>
static void parse_float_unit(const std::string &name, const item_type &item, bool &flag, FLOAT &value)
{
    parse_type_assert(name, item.value(), NUM_TYPE);
    parse_range_assert(name, item.value(), 0., 1.);
    value = item.value();
    flag = true;
}

template <class item_type>
static void parse_float_any(const std::string &name, const item_type &item, bool &flag, FLOAT &value)
{
    parse_type_assert(name, item.value(), NUM_TYPE);
    value = item.value();
    flag = true;
}

template <class item_type>
static void parse_color(const std::string &name, const item_type &item, bool &flag, FLOAT3 &color)
{
    parse_type_assert(name, item.value(), COLOR_TYPE);

    if (item.value().is_array())
        color = to_vec3(item.value());
    else if (item.value().is_string())
    {
        switch(hash(std::string(item.value())))
        {
            case ("white"_hash): color = rtx::color::white; break ;
            case ("black"_hash): color = rtx::color::black; break ;
            case ("red"_hash): color = rtx::color::red; break ;
            case ("green"_hash): color = rtx::color::green; break ;
            case ("blue"_hash): color = rtx::color::blue; break ;
            case ("cyan"_hash): color = rtx::color::cyan; break ;
            case ("magenta"_hash): color = rtx::color::magenta; break ;
            case ("purple"_hash): color = rtx::color::purple; break ;
            case ("yellow"_hash): color = rtx::color::yellow; break ;
            case ("grey"_hash): color = rtx::color::grey; break ;
            case ("orange"_hash): color = rtx::color::orange; break ;
            default: {
                std::string st = item.value();
                bool cond = not st.empty() and st.at(0) == '#';
                if (cond)
                {
                    parse_type_assert(name, item.value(), HEX_TYPE);
                    color = to_col3(st.substr(1));
                } else
                    parse_assert(false, "unknown color " + rtx::C[str(std::string(item.value()))]);
                break ;
            };
        }
    }
    flag = true;
}

template <class item_type>
static void parse_light_type(const std::string &name, const item_type &item, bool &flag, light_type_t &type)
{
    parse_type_assert(name, item.value(), STRING_TYPE);
    switch (hash(std::string(item.value())))
    {
        case ("ambient"_hash): type = AMBIENT; break ;
        case ("point"_hash): type = POINT; break ;
        case ("direct"_hash): type = DIRECT; break ;
        default: parse_assert(false, "unknown " + name + ' ' + rtx::C[str(std::string(item.value()))]); return ;
    }
    flag = true;
}

static void parse_config(const nlohmann::json &res)
{
    bool width = false;
    bool height = false;
    bool fovx = false;
    bool fovy = false;
    bool move_speed = false;
    bool look_speed = false;
    bool emission = false;

    parse_type_assert("config", res, OBJECT_TYPE);
    for (const auto &item : res.items())
    {
        switch (hash(item.key()))
        {
            case ("width"_hash):
            case ("screen_width"_hash): parse_int_positive("window resolution width", item, width, rtx::config::width); break ;
            case ("height"_hash):
            case ("screen_height"_hash): parse_int_positive("window resolution height", item, height, rtx::config::height); break ;
            case ("fovx"_hash): parse_float_ranged("fov x", item, fovx, rtx::config::fovx, 0, 180); break ;
            case ("fovy"_hash): parse_float_ranged("fov x", item, fovy, rtx::config::fovy, 0, 180); break ;
            case ("move_speed"_hash): parse_float_positive("move speed", item, move_speed, rtx::config::forward_move_step); break ;
            case ("look_speed"_hash): parse_float_positive("mouse sensitivity", item, look_speed, rtx::config::horizontal_look_speed); break ;
            case ("emission"_hash): parse_bool("object emission", item, emission, rtx::config::emission); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }
    parse_undefined_warn_set("window resolution width", width, 1000u, rtx::config::width);
    parse_undefined_warn_set("window resolution height", height, 1000u, rtx::config::height);
    parse_undefined_warn_set("fov x", fovx, 90., rtx::config::fovx);
    parse_undefined_warn_set("fov y", fovy, 90., rtx::config::fovy);
    parse_undefined_warn_set("move speed", move_speed, 0.1, rtx::config::forward_move_step);
    rtx::config::side_move_speed = rtx::config::forward_move_step / 2.;
    rtx::config::vertical_move_speed = rtx::config::forward_move_step;
    parse_undefined_warn_set("mouse sensitivity", look_speed, 0.005, rtx::config::vertical_look_speed);
    rtx::config::horizontal_look_speed = rtx::config::vertical_look_speed;
    parse_undefined_warn_set("object emission", emission, false, rtx::config::emission);
    flags.config = true;
    parse_ok(true, "scene::config "_W + "parsed"_G);
}

static void parse_cameras_single(const nlohmann::json &camera)
{
    parse_type_assert("cameras", camera, OBJECT_TYPE);
    FLOAT3 position;
    FLOAT3 direction;
    bool got_position = false;
    bool got_direction = false;

    for (const auto &item : camera.items())
    {
        switch (hash(item.key()))
        {
            case ("position"_hash): parse_vec3_point("camera position", item, got_position, position); break ;
            case ("direction"_hash): parse_vec3_unit("camera direction", item, got_direction, direction); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }
    parse_undefined_assert("camera position", got_position);
    parse_undefined_assert("camera direction", got_direction);
    if (got_position && got_direction)
    {
        camera_t cam(position, direction);
        parse_print("added camera " + rtx::B["["
            + to_string(rtx::objects::cam_vec.size()) + "]"] + ": "
            + rtx::Orange[to_string(cam)]);
        rtx::objects::cam_vec.push_back(cam);
        flags.cameras = true;
    }
}

static void parse_cameras(const nlohmann::json &cameras)
{
    parse_type_assert("cameras array", cameras, ARRAY_TYPE);
    for (const auto &item : cameras)
        parse_cameras_single(item);
    parse_ok(true, "scene::cameras "_W + "parsed"_G);
}

static void parse_sphere_single(const nlohmann::json &sphere)
{
    parse_type_assert("sphere", sphere, OBJECT_TYPE);

    FLOAT3 color;
    uint32_t specular;
    FLOAT reflective;
    FLOAT refractive;
    FLOAT transparency;
    FLOAT3 position;
    FLOAT radius;
    FLOAT emission;

    bool got_color = false;
    bool got_specular = false;
    bool got_reflective = false;
    bool got_refractive = false;
    bool got_transparency = false;
    bool got_position = false;
    bool got_radius = false;
    bool got_emission = false;

    for (const auto &item : sphere.items())
    {
        switch (hash(item.key()))
        {
            case ("color"_hash): parse_color("sphere color", item, got_color, color); break ;
            case ("specular"_hash): parse_int_positive("sphere specular", item, got_specular, specular); break ;
            case ("reflective"_hash): parse_float_unit("sphere reflective", item, got_reflective, reflective); break ;
            case ("refractive"_hash): parse_float_unit("sphere refractive", item, got_refractive, refractive); break ;
            case ("transparency"_hash): parse_float_unit("sphere transparency", item, got_transparency, transparency); break ;
            case ("center"_hash):
            case ("position"_hash): parse_vec3_point("sphere position", item, got_position, position); break ;
            case ("radius"_hash): parse_float_positive("sphere radius", item, got_radius, radius); break ;
            case ("emission"_hash): parse_float_positive("sphere emission", item, got_emission, emission); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }
    parse_undefined_assert("sphere color", got_color);
    parse_undefined_info_set("sphere specular", got_specular, 0u, specular);
    parse_undefined_info_set("sphere reflective", got_reflective, 0., reflective);
    parse_undefined_info_set("sphere refractive", got_refractive, 0., refractive);
    parse_undefined_info_set("sphere transparency", got_transparency, 0., transparency);
    parse_undefined_info_set("sphere emission", got_emission, 0., emission);
    parse_undefined_assert("sphere position", got_position);
    parse_undefined_assert("sphere radius", got_radius);
    if (got_color && got_position && got_radius)
    {
        sphere_t sp(position, radius, color, specular, reflective, refractive, transparency, emission);
        parse_print("added sphere " + rtx::B["["
            + to_string(rtx::objects::sp_vec.size()) + "]"] + ": "
            + rtx::Orange[to_string(sp)]);
        rtx::objects::sp_vec.emplace_back(sp);
        flags.spheres = true;
    }
}

static void parse_spheres(const nlohmann::json &spheres)
{
    parse_type_assert("spheres array", spheres, ARRAY_TYPE);
    for (const auto &item : spheres)
        parse_sphere_single(item);
    parse_ok(true, "scene::spheres "_W + "parsed"_G);
}

static void parse_plane_single(const nlohmann::json &plane)
{
    parse_type_assert("plane", plane, OBJECT_TYPE);

    FLOAT3 color;
    uint32_t specular;
    FLOAT reflective;
    FLOAT3 point;
    FLOAT3 normal;

    bool got_color = false;
    bool got_specular = false;
    bool got_reflective = false;
    bool got_point = false;
    bool got_normal = false;

    for (const auto &item : plane.items())
    {
        switch (hash(item.key()))
        {
            case ("color"_hash): parse_color("plane color", item, got_color, color); break ;
            case ("specular"_hash): parse_int_positive("plane specular", item, got_specular, specular); break ;
            case ("reflective"_hash): parse_float_unit("plane reflective", item, got_reflective, reflective); break ;
            case ("position"_hash):
            case ("point"_hash): parse_vec3_point("plane point", item, got_point, point); break;
            case ("normal"_hash): parse_vec3_unit("plane normal", item, got_normal, normal); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }
    parse_undefined_assert("plane color", got_color);
    parse_undefined_info_set("plane specular", got_specular, 0u, specular);
    parse_undefined_info_set("plane reflective", got_reflective, 0., reflective);
    parse_undefined_assert("plane point", got_point);
    parse_undefined_assert("plane normal", got_normal);
    if (got_color && got_point && got_normal)
    {
        plane_t pl(point, normal, color, specular, reflective);
        parse_print("added plane " + rtx::B["["
            + to_string(rtx::objects::pl_vec.size()) + "]"] + ": "
            + rtx::Orange[to_string(pl)]);
        rtx::objects::pl_vec.emplace_back(pl);
        flags.planes = true;
    }
}

static void parse_planes(const nlohmann::json &planes)
{
    parse_type_assert("planes array", planes, ARRAY_TYPE);
    for (const auto &item : planes)
        parse_plane_single(item);
    parse_ok(true, "scene::planes "_W + "parsed"_G);
}

static void parse_triangle_single(const nlohmann::json &triangle)
{
    parse_type_assert("triangle", triangle, OBJECT_TYPE);

    FLOAT3 color;
    uint32_t specular;
    FLOAT reflective;
    FLOAT3 a;
    FLOAT3 b;
    FLOAT3 c;

    bool got_color = false;
    bool got_specular = false;
    bool got_reflective = false;
    bool got_a = false;
    bool got_b = false;
    bool got_c = false;

    for (const auto &item : triangle.items())
    {
        switch (hash(item.key()))
        {
            case ("color"_hash): parse_color("triangle color", item, got_color, color); break ;
            case ("specular"_hash): parse_int_positive("triangle specular", item, got_specular, specular); break ;
            case ("reflective"_hash): parse_float_unit("triangle reflective", item, got_reflective, reflective); break ;
            case ("a"_hash): parse_vec3_point("triangle point a", item, got_a, a); break ;
            case ("b"_hash): parse_vec3_point("triangle point b", item, got_b, b); break ;
            case ("c"_hash): parse_vec3_point("triangle point c", item, got_c, c); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }
    parse_undefined_assert("triangle color", got_color);
    parse_undefined_info_set("triangle specular", got_specular, 0u, specular);
    parse_undefined_info_set("triangle reflective", got_reflective, 0., reflective);
    parse_undefined_assert("triangle point a", got_a);
    parse_undefined_assert("triangle point b", got_b);
    parse_undefined_assert("triangle point c", got_c);
    if (got_color && got_a && got_b && got_c)
    {
        rtx::objects::tr_vec.emplace_back(a, b, c, color, specular, reflective);
        flags.triangles = true;
    }
}

static void parse_triangles(const nlohmann::json &triangles)
{
    parse_type_assert("triangles array", triangles, ARRAY_TYPE);
    for (const auto &item : triangles)
        parse_triangle_single(item);
    parse_ok(true, "scene::triangles "_W + "parsed"_G);
}

static void parse_hyperboloid_single(const nlohmann::json &hyper)
{
    parse_type_assert("hyperboloid", hyper, OBJECT_TYPE);

    FLOAT3 color;
    uint32_t specular;
    FLOAT reflective;
    FLOAT3 position;
    FLOAT3 direction;
    FLOAT width;
    FLOAT gamma;

    bool got_color = false;
    bool got_specular = false;
    bool got_reflective = false;
    bool got_position = false;
    bool got_direction = false;
    bool got_width = false;
    bool got_gamma = false;

    for (const auto &item : hyper.items())
    {
        switch (hash(item.key()))
        {
            case ("color"_hash): parse_color("hyperboloid color", item, got_color, color); break ;
            case ("specular"_hash): parse_int_positive("hyperboloid specular", item, got_specular, specular); break ;
            case ("reflective"_hash): parse_float_unit("hyperboloid reflective", item, got_reflective, reflective); break ;
            case ("position"_hash): parse_vec3_point("hyperboloid position", item, got_position, position); break ;
            case ("direction"_hash): parse_vec3_unit("hyperboloid direction", item, got_direction, direction); break ;
            case ("width"_hash): parse_float_unit("hyperboloid width", item, got_width, width); break ;
            case ("gamma"_hash): parse_float_any("hyperboloid gamma", item, got_gamma, gamma); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }
    parse_undefined_assert("hyperboloid color", got_color);
    parse_undefined_info_set("hyperboloid specular", got_specular, 0u, specular);
    parse_undefined_info_set("hyperboloid reflective", got_reflective, 0., reflective);
    parse_undefined_assert("hyperboloid position", got_position);
    parse_undefined_assert("hyperboloid direction", got_direction);
    parse_undefined_assert("hyperboloid width", got_width);
    parse_undefined_info_set("hyperboloid gamma", got_gamma, 0., gamma);
    if (got_color && got_position && got_direction && got_width)
    {
        rtx::objects::cn_vec.emplace_back(position, direction, width, gamma, color, specular, reflective);
        flags.hyperboloids = true;
    }
}

static void parse_hyperboloids(const nlohmann::json &cones)
{
    parse_type_assert("hyperboloids array", cones, ARRAY_TYPE);
    for (const auto &item : cones)
        parse_hyperboloid_single(item);
    parse_ok(true, "scene::hyperboloids "_W + "parsed"_G);
}

static void parse_cylinder_single(const nlohmann::json &cylinder)
{
    parse_type_assert("cylinder", cylinder, OBJECT_TYPE);

    FLOAT3 color;
    uint32_t specular;
    FLOAT reflective;
    FLOAT3 position;
    FLOAT3 direction;
    FLOAT radius;
    FLOAT height;

    bool got_color = false;
    bool got_specular = false;
    bool got_reflective = false;
    bool got_position = false;
    bool got_direction = false;
    bool got_radius = false;
    bool got_height = false;

    for (const auto &item : cylinder.items())
    {
        switch (hash(item.key()))
        {
            case ("color"_hash): parse_color("cylinder color", item, got_color, color); break ;
            case ("specular"_hash): parse_int_positive("cylinder specular", item, got_specular, specular); break ;
            case ("reflective"_hash): parse_float_unit("cylinder reflective", item, got_reflective, reflective); break ;
            case ("position"_hash): parse_vec3_point("cylinder position", item, got_position, position); break ;
            case ("direction"_hash): parse_vec3_unit("cylinder direction", item, got_direction, direction); break ;
            case ("radius"_hash): parse_float_positive("cylinder radius", item, got_radius, radius); break ;
            case ("height"_hash): parse_float_positive("cylinder height", item, got_height, height); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }

    parse_undefined_assert("cylinder color", got_color);
    parse_undefined_info_set("cylinder specular", got_specular, 0u, specular);
    parse_undefined_info_set("cylinder reflective", got_reflective, 0., reflective);
    parse_undefined_info_set("cylinder height", got_height, std::numeric_limits<double>::infinity(), height);
    parse_undefined_assert("cylinder position", got_position);
    parse_undefined_assert("cylinder direction", got_direction);
    parse_undefined_assert("cylinder radius", got_radius);
    if (got_color && got_position && got_direction && got_radius)
    {
        rtx::objects::cy_vec.emplace_back(position, direction, radius, height, color, specular, reflective);
        flags.cylinders = true;
    }
}

static void parse_cylinders(const nlohmann::json &cylinders)
{
    parse_type_assert("cylinders array", cylinders, ARRAY_TYPE);
    for (const auto &item : cylinders)
        parse_cylinder_single(item);
    parse_ok(true, "scene::cylinders "_W + "parsed"_G);
}

static void parse_torus_single(const nlohmann::json &torus)
{
    parse_type_assert("torus", torus, OBJECT_TYPE);

    FLOAT3 color;
    uint32_t specular;
    FLOAT reflective;
    FLOAT3 position;
    FLOAT3 normal;
    FLOAT inner;
    FLOAT outer;

    bool got_color = false;
    bool got_specular = false;
    bool got_reflective = false;
    bool got_position = false;
    bool got_normal = false;
    bool got_inner = false;
    bool got_outer = false;

    for (const auto &item : torus.items())
    {
        switch (hash(item.key()))
        {
            case ("color"_hash): parse_color("torus color", item, got_color, color); break ;
            case ("specular"_hash): parse_int_positive("torus specular", item, got_specular, specular); break ;
            case ("reflective"_hash): parse_float_unit("torus reflective", item, got_reflective, reflective); break ;
            case ("position"_hash): parse_vec3_point("torus position", item, got_position, position); break ;
            case ("normal"_hash): parse_vec3_unit("torus normal", item, got_normal, normal); break ;
            case ("inner_radius"_hash): parse_float_positive("torus inner radius", item, got_inner, inner); break ;
            case ("outer_radius"_hash): parse_float_positive("torus outer radius", item, got_outer, outer); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }

    parse_undefined_assert("torus color", got_color);
    parse_undefined_info_set("torus specular", got_specular, 0u, specular);
    parse_undefined_info_set("torus reflective", got_reflective, 0., reflective);
    parse_undefined_assert("torus position", got_position);
    parse_undefined_assert("torus normal", got_normal);
    parse_undefined_assert("torus inner radius", got_inner);
    parse_undefined_assert("torus outer radius", got_outer);
    if (got_color && got_position && got_normal && got_inner && got_outer)
    {
        rtx::objects::to_vec.emplace_back(position, normal, inner, outer, color, specular, reflective);
        flags.torus = true;
    }
}

static void parse_torus(const nlohmann::json &torus)
{
    parse_type_assert("torus array", torus, ARRAY_TYPE);
    for (const auto &item : torus)
        parse_torus_single(item);
    parse_ok(true, "scene::torus "_W + "parsed"_G);
}

static void parse_light_single(const nlohmann::json &light)
{
    parse_type_assert("light", light, OBJECT_TYPE);

    light_type_t type;
    FLOAT intensity;
    FLOAT3 color;
    FLOAT3 vec;

    bool got_type = false;
    bool got_intensity = false;
    bool got_color = false;
    bool got_position = false;
    bool got_direction = false;

    for (const auto &item : light.items())
    {
        switch (hash(item.key()))
        {
            case ("type"_hash): parse_light_type("light type", item, got_type, type); break ;
            case ("intensity"_hash): parse_float_unit("light intensity", item, got_intensity, intensity); break ;
            case ("color"_hash): parse_color("light color", item, got_color, color); break ;
            case ("position"_hash): parse_vec3_point("light position", item, got_position, vec); break ;
            case ("direction"_hash): parse_vec3_unit("light direction", item, got_direction, vec); break ;
            default: parse_unknown_notify(item.key()); break ;
        }
    }

    parse_undefined_assert("light type", got_type);
    parse_undefined_assert("light intensity", got_intensity);
    parse_undefined_info_set("light color", got_color, rtx::color::white, color);
    if (got_type && got_intensity)
    {
        switch (type)
        {
            case (AMBIENT): {
                parse_extra_defined_warn("light position", got_position, "with light type " + "`ambient`"_C);
                parse_extra_defined_warn("light direction", got_direction, "with light type " + "`ambient`"_C);
                rtx::objects::li_vec.push_back(ambient_t(intensity, color));
                break ;
            }
            case (POINT): {
                parse_undefined_assert("light position", got_position);
                parse_extra_defined_warn("light direction", got_direction, "with light type " + "`point`"_C);
                if (got_position and not got_direction)
                    rtx::objects::li_vec.push_back(point_t(intensity, color, vec));
                else
                    return ;
                break ;
            }
            case (DIRECT): {
                parse_extra_defined_warn("light position", got_position, "with light type " + "`direct`"_C);
                parse_undefined_assert("light direction", got_direction);
                if (not got_position and got_direction)
                    rtx::objects::li_vec.push_back(direct_t(intensity, color, vec));
                else
                    return ;
                break ;
            }
            default:throw std::runtime_error(
                rtx::R["[INTERNAL ERROR]: init_scene::parse_light_single: unexpected light type " + str(type)]);
        }
        parse_print("added light " + rtx::B["["
            + to_string(rtx::objects::li_vec.size() - 1) + "]"] + ": "
            + rtx::Orange[to_string(rtx::objects::li_vec.back())]);
        flags.lights = true;
    }
}

static void parse_lights(const nlohmann::json &lights)
{
    parse_type_assert("lights array", lights, ARRAY_TYPE);
    for (const auto &item : lights)
        parse_light_single(item);
    parse_ok(true, "scene::lights "_W + "parsed"_G);
}

void rtx::parse_scene()
{
//    rtx::data::reloading.store(true, std::memory_order_seq_cst);

    flags.all = 0ull;
    std::ifstream stream(rtx::config::scene_fname);
    if (not stream.is_open())
        throw Exception(std::string("cannot open `") + rtx::config::scene_fname + "` file");
    nlohmann::json data;
    try {
        data = nlohmann::json::parse(stream);
    } catch (nlohmann::json::exception &e) {
        throw Exception("json parser fall with exception: " + rtx::Y[e.what()]);
    }
    for (const auto &i : data.items())
    {
        switch(hash(i.key()))
        {
            case("config"_hash): parse_config(i.value()); break ;
            case("cameras"_hash): parse_cameras(i.value()); break ;
            case("spheres"_hash): parse_spheres(i.value()); break ;
            case("planes"_hash): parse_planes(i.value()); break ;
            case("triangles"_hash): parse_triangles(i.value()); break ;
            case("cones"_hash): parse_hyperboloids(i.value()); break ;
            case("cylinders"_hash): parse_cylinders(i.value()); break ;
            case("torus"_hash): parse_torus(i.value()); break ;
            case("lights"_hash): parse_lights(i.value()); break ;
            default: parse_unknown_notify(i.key());
        }
    }
    parse_notify(flags.config, "config parameters are not set, setting to default values");
    if (not flags.config) {
        rtx::config::width = 1000u;
        rtx::config::height = 1000u;
        rtx::config::forward_move_step = 0.1;
        rtx::config::side_move_speed = 0.05;
        rtx::config::vertical_move_speed = 0.1;
        rtx::config::vertical_look_speed = 0.005;
        rtx::config::horizontal_look_speed = 0.005;
    }
    parse_warn(flags.lights, "your scene don`t have any light sources, you will not see anything");
    parse_warn(flags.objects, "your scene don`t have any objects, you will not see anything");
    parse_assert(flags.cameras, "your scene don`t have any cameras");
    if (not flags.cameras)
        throw rtx::Exception("cannot start due to scene errors");

//    rtx::objects::sp_vec = {
//            sphere_t({0, 0, 0}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({0, 1., 0}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({0, 1.5, 0}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({0, 2., 0}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({0, 2.5, 0}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({3 , 2.0, 7}, 0.2, rtx::color::white, 0, 0),
//            sphere_t({0,-1,3}, 1, Color::red, 500, 0.2),
//            sphere_t({2, 0, 4}, 1, rtx::color::blue, 500, 0.2),
//            sphere_t({-2, 0, 4}, 1, rtx::color::green, 10, 0.2),
//            sphere_t({3,-1,0}, 1, rtx::color::red, 500, 0.2),
////            sphere_t({4, 0, 0}, 1, Color::blue, 500, 0.2),
////            sphere_t({4, 0, -2}, 1, Color::green, 10, 0.2),
////            sphere_t({0, -5001, 0}, 5000, Color::yellow, 1000, 0.2),
////            sphere_t({2,2,2},0.05,Color::red,0.,0.),
////            sphere_t({3,2,1},0.05,Color::red,0.,0.),
////            sphere_t({3,3,3},0.05,Color::red,0.,0.),
////            sphere_t({0,2,10},0.1,Color::green,0.,0.),
////            sphere_t({1,2,10},0.1,Color::green,0.,0.),
////            sphere_t({-1,2,10},0.1,Color::green,0.,0.),
////            sphere_t({0,3,10},0.1,Color::blue,0.,0.),
////            sphere_t({0,1,10},0.1,Color::blue,0.,0.),
////            sphere_t({0,2,11},0.1,Color::red,0.,0.),
////            sphere_t({0,2,9},0.1,Color::red,0.,0.)
//    };
//    rtx::objects::pl_vec = {
////            plane_t({0, -1, 0}, {0, 1, 0}, Color::yellow, 1000, 0.2)
//    };
//    rtx::objects::tr_vec = {
////            triangle_t({2, 2, 2}, {3, 3, 3}, {3, 2, 1}, Color::grey, 10, 0.) // 1 2 3
////            triangle_t({5, 1, 0},{0, 1, 0},{2, 2, 2},   Color::grey, 300, 0.7) // 3 2 1
////            triangle_t({2, 2, 2}, {3, 3, 3}, {3, 2, 1}, Color::grey, 10, 0.) // 1 2 3
////            triangle_t({3, 3, 3},{2, 2, 2}, {3, 2, 1},  Color::grey, 10, 0.) // 3 1 2
//    };
//    rtx::objects::cn_vec = {
////            cone_t({0, 5, 7}, {0,1,0}, 0.5, {255, 149, 0}, 500, 0.3)
////            cone_t({0, 0, 0}, {1,1,1}, 1, 0, {255, 149, 0}, 500, 0.3)
//    };
//    rtx::objects::cy_vec = {
////            cylinder_t({0, 0, 10}, {1, 0, 0}, 1, Color::yellow, 0, 0)
//    };
//    rtx::objects::to_vec = {
//            torus_t({0, 0, 0}, {0, 1, 0}, 0.5, 3, rtx::color::cyan, 0, 0)
//    };
//
//    rtx::objects::cam_vec = {
//            camera_t({-5, 0, 0}, {1, 0, 0})
//    };
//
//    rtx::objects::li_vec = {
//            ambient_t(0.1, rtx::color::white),
//            point_t(0.6, rtx::color::white, {3, 2, 7}),
////            direct_t(0.3, rtx::color::white, {1, 3, 4})
//    };

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

    assert(not rtx::objects::li_vec.empty());
    assert(not rtx::objects::cam_vec.empty());

//    rtx::data::reloading.store(false, std::memory_order_seq_cst);
}
