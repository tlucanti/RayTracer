
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
void init_scene();
void init_kernel();

namespace data
{
    extern mlxlib::MLXwindow   *win;
    extern mlxlib::MLXimage    *img;
    extern cllib::CLkernel     *kernel;
    extern cllib::CLcontext    *context;
    extern cllib::CLqueue      *queue;

//    extern cllib::CLkernel      *blur_kernel;
} /* data */

namespace scene
{
    extern cllib::CLarray<unsigned char, cllib::read_only_array> figures;
    extern scene_t local_scene;

    extern cllib::CLarray<camera_t, cllib::read_only_array> cameras;
    extern cllib::CLarray<light_t, cllib::read_only_array> lights;

    extern cllib::CLarray<unsigned int, cllib::write_only_array> canvas;
//    extern cllib::CLarray<unsigned int, cllib::read_write_array> canvas2;
//    extern cllib::CLarray<FLOAT, cllib::read_write_array> distances;
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
    extern std::vector<box_t> box_vec;

    extern size_t sphere_offset;
    extern size_t plane_offset;
    extern size_t triangle_offset;
    extern size_t cone_offset;
    extern size_t cylinder_offset;
    extern size_t torus_offset;
    extern size_t box_offset;

    extern std::vector<light_t> li_vec;
} /* objects */

RTX_NAMESPACE_END

#endif /* RTX_HPP */
