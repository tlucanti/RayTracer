
#include <rtx.hpp>

RTX_NAMESPACE_BEGIN

namespace data
{
    mlxlib::MLXwindow   *win = nullptr;
    mlxlib::MLXimage    *img = nullptr;
    cllib::CLkernel     *kernel = nullptr;
    cllib::CLcontext    *context = nullptr;
    cllib::CLqueue      *queue = nullptr;
} /* data */

namespace scene
{
    cllib::CLarray<camera_t, cllib::read_only_array> cameras;
    cllib::CLarray<sphere_t, cllib::read_only_array> spheres;
    cllib::CLarray<plane_t, cllib::read_only_array> planes;
    cllib::CLarray<triangle_t, cllib::read_only_array> triangles;
    cllib::CLarray<cone_t, cllib::read_only_array> cones;
    cllib::CLarray<cylinder_t, cllib::read_only_array> cylinders;
    cllib::CLarray<torus_t, cllib::read_only_array> torus;

    cllib::CLarray<light_t, cllib::read_only_array> lights;

    cllib::CLarray<unsigned int, cllib::write_only_array> canvas;
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

    std::vector<light_t> li_vec;
} /* objects */

RTX_NAMESPACE_END
