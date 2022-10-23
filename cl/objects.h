
#ifndef OBJECTS_HPP
# define OBJECTS_HPP

# include "kernel.h"

# ifndef __CPP
#  define DECLARE_CONSTRUCTOR(...)
#  define DECLARE_METHOD(...)
# else
#  include <common.hpp>
#  include "kernel.h"
#  define DECLARE_CONSTRUCTOR(__name, ...) __name(__VA_ARGS__)
#  define DECLARE_METHOD(__retval, __name, ...) __retval __name(__VA_ARGS__)
# endif /* __CPP */

# define PADDING4   UNUSED uint32_t __padding_0;
# define PADDING8   UNUSED uint64_t __padding_1;
# define PADDING16  UNUSED uint64_t __padding_2; UNUSED uint64_t __padding_3;

// -----------------------------------------------------------------------------
typedef enum
{
    AMBIENT = 0,
    DIRECT  = 1,
    POINT   = 2
} light_type_t;

// -----------------------------------------------------------------------------
typedef struct sphere_s
{
    FLOAT3              color;          // 0  -- 32
    uint32_t            specular;       // 32 --
    PADDING4                            //    -- 40
    FLOAT               reflective;     // 40 -- 48
    FLOAT               refractive;     // 48 -- 56
    FLOAT               transparency;   // 56 -- 64
    FLOAT               emission;       //
    FLOAT               radius;         //
    FLOAT3              position;       //

    DECLARE_CONSTRUCTOR(sphere_s, SPHERE_PARAMS);
} PACKED ALIGNED64 sphere_t;

// -----------------------------------------------------------------------------
typedef struct plane_s
{
    FLOAT3              color;          // 0  -- 32
    uint32_t            specular;       // 32 --
    PADDING4                            //    -- 40
    FLOAT               reflective;      // 40 -- 48
    FLOAT               refractive;     // 48 -- 56
    FLOAT               transparency;   // 56 -- 64
    FLOAT3              point;          //
    FLOAT3              normal;         //

    DECLARE_CONSTRUCTOR(plane_s, PLANE_PARAMS);
} PACKED ALIGNED64 plane_t;

// -----------------------------------------------------------------------------
typedef struct triangle_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           refractive;         // 48 -- 56
    FLOAT           transparency;       // 56 -- 64
    FLOAT           emission;           //
    PADDING16                           //
    FLOAT3          normal;             //
    FLOAT3          a;                  //
    FLOAT3          b;                  //
    FLOAT3          c;                  //

    DECLARE_CONSTRUCTOR(triangle_s, TRIANGLE_PARAMS);
} PACKED ALIGNED64 triangle_t;

// -----------------------------------------------------------------------------
typedef struct cone_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           refractive;
    FLOAT           transparency;
    FLOAT           width;              //
    FLOAT           gamma;              //
    FLOAT3          position;           //
    FLOAT3          direction;          //
    FLOAT3          matr[3];            //

    DECLARE_CONSTRUCTOR(cone_s, CONE_PARAMS);
} PACKED ALIGNED64 cone_t;

// -----------------------------------------------------------------------------
typedef struct cylinder_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           refractive;
    FLOAT           transparency;
    FLOAT           radius;             // 48 -- 56
    FLOAT           height;             // 56 -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          direction;          // 96 -- 128

    DECLARE_CONSTRUCTOR(cylinder_s, CYLINDER_PARAMS);
} PACKED ALIGNED64 cylinder_t;

// -----------------------------------------------------------------------------
typedef struct torus_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           refractive;         //
    FLOAT           transparency;       //
    FLOAT           r;                  //
    FLOAT           R;                  //
    FLOAT3          position;           //
    FLOAT3          normal;             //
    FLOAT3          matr[3];            //

    DECLARE_CONSTRUCTOR(torus_s, TORUS_PARAMS);
} PACKED ALIGNED64 torus_t;

// -----------------------------------------------------------------------------
typedef struct box_s
{
    FLOAT3 color;
    uint32_t specular;
    PADDING4
    FLOAT reflective;
    FLOAT refractive;
    FLOAT transparency;

    FLOAT3 position;
    FLOAT a;
    FLOAT b;
    FLOAT c;

    DECLARE_CONSTRUCTOR(box_s, BOX_PARAMS);
} PACKED ALIGNED64 box_t;

// -----------------------------------------------------------------------------
typedef struct light_s
{
    FLOAT3          color;              // 0  -- 32
    union {                             // 32 -- 64
        FLOAT3      direction;
        FLOAT3      position;
    };
    light_type_t    type;               // 64 -- 68

    DECLARE_CONSTRUCTOR(light_s, LIGHT_PARAMS);
} PACKED ALIGNED64 light_t;

// -----------------------------------------------------------------------------
typedef struct camera_s
{
    FLOAT3   position;                  // 0  -- 32
    FLOAT3   direction;                 // 32 -- 64
    FLOAT3   rotate_matrix[3];          // 64 -- 160
    FLOAT    alpha;                     // 160-- 168
    FLOAT    theta;                     // 168-- 176

    DECLARE_CONSTRUCTOR(camera_s, CAMERA_PARAMS);
    DECLARE_METHOD(void, recompute_matrix, void);
} PACKED ALIGNED64 camera_t;

// -----------------------------------------------------------------------------
typedef __constant const sphere_t   *__restrict sphere_ptr;
typedef __constant const plane_t    *__restrict plane_ptr;
typedef __constant const triangle_t *__restrict triangle_ptr;
typedef __constant const cone_t     *__restrict cone_ptr;
typedef __constant const cylinder_t *__restrict cylinder_ptr;
typedef __constant const torus_t    *__restrict torus_ptr;

typedef __constant const light_t    *__restrict light_ptr;
typedef __constant const camera_t   *__restrict camera_ptr;
typedef __constant const void       *__restrict void_ptr;
typedef __constant const unsigned char *__restrict byte_ptr;

typedef struct
{
    sphere_ptr      spheres;
    plane_ptr       planes;
    triangle_ptr    triangles;
    cone_ptr        cones;
    cylinder_ptr    cylinders;
    torus_ptr       torus;

    light_ptr       lights;
    camera_ptr      cameras;

    const uint32_t spheres_num;
    const uint32_t planes_num;
    const uint32_t triangles_num;
    const uint32_t cones_num;
    const uint32_t cylinders_num;
    const uint32_t torus_num;

    const uint32_t lights_num;
    const uint32_t cameras_num;
} scene_t;

typedef const scene_t *__restrict scene_ptr;

// -----------------------------------------------------------------------------
typedef enum obj_type_e
{
    SPHERE,
    PLANE,
    TRIANGLE,
    CONE,
    CYLINDER,
    TOR
} obj_type_t;

#endif /* OBJECTS_HPP */
