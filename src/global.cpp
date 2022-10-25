
#include <rtx.hpp>

RTX_NAMESPACE_BEGIN

namespace data
{
    mlxlib::MLXwindow   *win = nullptr;
    mlxlib::MLXimage    *img = nullptr;
    cllib::CLkernel     *kernel = nullptr;
    cllib::CLcontext    *context = nullptr;
    cllib::CLqueue      *queue = nullptr;

//    cllib::CLkernel     *blur_kernel = nullptr;
} /* data */

namespace scene
{
    cllib::CLarray<camera_t, cllib::read_only_array> cameras;
    cllib::CLarray<unsigned char, cllib::read_only_array> figures;
    scene_t local_scene {};

    cllib::CLarray<light_t, cllib::read_only_array> lights;

    cllib::CLarray<uint32_t, cllib::write_only_array> canvas;
//    cllib::CLarray<uint32_t, cllib::read_write_array> canvas2;
//    cllib::CLarray<FLOAT, cllib::read_write_array> distances;
} /* scene */

namespace objects
{
    std::vector<camera_t> cam_vec;
    std::vector<sphere_t> sp_vec;
    std::vector<plane_t> pl_vec;
    std::vector<triangle_t> tr_vec;
    std::vector<cone_t> cn_vec;
    std::vector<cylinder_t> cy_vec;
    std::vector<torus_t> to_vec;
    std::vector<box_t> box_vec;

    std::vector<light_t> li_vec;
} /* objects */

RTX_NAMESPACE_END
