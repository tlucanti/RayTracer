
#ifndef RTX_HPP
# define RTX_HPP

# include <mlxlib>
# include <cllib>
# include <common.hpp>
# include <objects.h>

RTX_NAMESPACE_BEGIN

void parse_scene();
void init_gpu();
void init_mlx();
NORET void collapse(int status);

namespace data
{
    extern mlxlib::MLXwindow   *win;
    extern mlxlib::MLXimage    *img;
    extern cllib::CLkernel     *kernel;
    extern cllib::CLcontext    *context;
    extern cllib::CLqueue      *queue;
} /* data */

namespace scene
{
    extern cllib::CLarray<camera_t, cllib::read_only_array> cameras;
    extern cllib::CLarray<sphere_t, cllib::read_only_array> spheres;
    extern cllib::CLarray<plane_t, cllib::read_only_array> planes;
    extern cllib::CLarray<triangle_t, cllib::read_only_array> triangles;
    extern cllib::CLarray<cone_t, cllib::read_only_array> cones;
    extern cllib::CLarray<cylinder_t, cllib::read_only_array> cylinders;
    extern cllib::CLarray<torus_t, cllib::read_only_array> torus;

    extern cllib::CLarray<light_t, cllib::read_only_array> lights;

    extern cllib::CLarray<unsigned int, cllib::read_write_array> canvas;
    extern cllib::CLarray<FLOAT, cllib::read_write_array> distances;
} /* scene */

namespace objects
{
    extern std::vector<camera_t> cam_vec;
    extern std::vector<sphere_t> sp_vec;
    extern std::vector<plane_t> pl_vec;
    extern std::vector<triangle_t> tr_vec;
    extern std::vector<cone_t> cn_vec;
    extern std::vector<cylinder_t> cy_vec;
    extern std::vector<torus_t> to_vec;

    extern std::vector<light_t> li_vec;
} /* objects */

RTX_NAMESPACE_END

#endif /* RTX_HPP */
