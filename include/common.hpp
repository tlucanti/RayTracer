
#ifndef COMMON_HPP
# define COMMON_HPP

# define RTX_NAMESPACE rtx
# define RTX_NAMESPACE_BEGIN namespace RTX_NAMESPACE {
# define RTX_NAMESPACE_END }

# include <cllib>
# include <cmath>
# ifdef __APPLE__
#  include <math.h>
# endif /* __APPLE__ */

typedef double FLOAT;
typedef cl_double4 FLOAT4;
typedef cl_double3 FLOAT3;
typedef cl_double2 COMPLEX;
typedef const FLOAT3 &FL3_CREF;

namespace RTX_NAMESPACE
{
    namespace config
    {
        inline unsigned int width {1000};
        inline unsigned int height {1000};
        inline FLOAT fovx {90};
        inline FLOAT fovy {90};
        inline FLOAT forward_move_step {0.1};
        inline FLOAT side_move_speed {0.05};
        inline FLOAT vertical_move_speed {0.05};
        inline FLOAT vertical_look_speed {0.005};
        inline FLOAT horizontal_look_speed {0.005};

        inline constexpr const char *kernel_fname = "../cl/kernel.cl";
//        inline constexpr const char *scene_fname = "../scenes/scene.json";
//        inline constexpr const char *scene_fname = "../scenes/spheres.json";
        inline constexpr const char *scene_fname = "../scenes/planes.json";
        inline constexpr const char *cl_dir = "../cl";
        inline unsigned int current_camera;
    } /* config */

    namespace color
    {
        inline constexpr FLOAT3 white = {{255, 255, 255}};
        inline constexpr FLOAT3 black = {{0, 0, 0}};
        inline constexpr FLOAT3 red = {{255, 0, 0}};
        inline constexpr FLOAT3 green = {{0, 255, 0}};
        inline constexpr FLOAT3 blue = {{0, 0, 255}};
        inline constexpr FLOAT3 cyan = {{0, 255, 255}};
        inline constexpr FLOAT3 magenta = {{255, 0, 255}};
        inline constexpr FLOAT3 yellow = {{255, 255, 0}};
        inline constexpr FLOAT3 purple = magenta;
        inline constexpr FLOAT3 grey = {{100, 100, 100}};
    } /* color */

} /* rtx */


# ifdef __APPLE__
#  define sincos __sincos
# endif /* __APPLE__ */

# define EPS 1e-4
# define PI M_PI

# ifndef ATTRIBUTE
#  define ATTRIBUTE(__attr) __attribute__((__attr))
# endif /* ATTRIBUTE */
# ifndef PACKED
#  define PACKED ATTRIBUTE(__packed__)
# endif /* PACKED */
# ifndef ALIGNED8
#  define ALIGNED8 ATTRIBUTE(__aligned__(8))
# endif /* ALIGNED8 */
# ifndef ALIGNED16
#  define ALIGNED16 ATTRIBUTE(__aligned__(16))
# endif /* ALIGNED16 */
# ifndef ALIGNED32
#  define ALIGNED32 ATTRIBUTE(__aligned__(32))
# endif /* ALIGNED32 */
# ifndef ALIGNED64
#  define ALIGNED64 ATTRIBUTE(__aligned__(64))
# endif /* ALIGNED64 */

#endif /* COMMON_HPP */
