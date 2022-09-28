
#include <fstream>
#include <json>
#include <iostream>
#include <rtx.hpp>

void rtx::parse_scene(const char *fname)
{
    std::ifstream stream(fname);
    if (not stream.is_open())
        throw std::runtime_error(std::string("cannot open `") + fname + "` file");
    nlohmann::json data = nlohmann::json::parse(stream);
    for (const auto &i : data)
        std::cout << "data: " << i << std::endl;



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
            torus_t({0, 0, 0}, {0, 0, 1}, 0.2, 2, rtx::color::cyan, 0, 0)
    };

    rtx::objects::cam_vec = {
            camera_t({7, 0, 0}, {-1, 0, 0})
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
