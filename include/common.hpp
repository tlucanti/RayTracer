
#ifndef COMMON_HPP
# define COMMON_HPP

# define RTX_NAMESPACE rtx
# define RTX_NAMESPACE_BEGIN namespace RTX_NAMESPACE {
# define RTX_NAMESPACE_END }

# include <cllib>
# ifdef __APPLE__
#  include <math.h>
#  define sincos __sincos
# endif /* __APPLE__ */

# include <cmath>

# define EPS 1e-4
# define PI M_PI

typedef double FLOAT;
typedef cl_double3 FLOAT3;
typedef cl_double2 FLOAT2;

namespace RTX_NAMESPACE
{
    namespace config
    {
        inline int width;
        inline int height;
        inline FLOAT fovx;
        inline FLOAT fovy;
        inline FLOAT forward_move_step;
        inline FLOAT side_move_speed;
        inline FLOAT vertical_move_speed;
        inline FLOAT vertical_look_speed = 0.005;
        inline FLOAT horizontal_look_speed = 0.005;

        inline constexpr const char *kernel_fname = "../cl/kernel.cl";
        inline constexpr const char *scene_fname = "../scenes/scene.json";
        inline int current_camera;
    } /* config */

    namespace color
    {
        inline constexpr FLOAT3 white = {255, 255, 255};
        inline constexpr FLOAT3 black = {0, 0, 0};
        inline constexpr FLOAT3 red = {255, 0, 0};
        inline constexpr FLOAT3 green = {0, 255, 0};
        inline constexpr FLOAT3 blue = {0, 0, 255};
        inline constexpr FLOAT3 cyan = {0, 255, 255};
        inline constexpr FLOAT3 magenta = {255, 0, 255};
        inline constexpr FLOAT3 yellow = {255, 255, 0};
        inline constexpr FLOAT3 purple = magenta;

        inline constexpr FLOAT3 grey = {100, 100, 100};
    } /* color */

} /* rtx */

#endif /* COMMON_HPP */
