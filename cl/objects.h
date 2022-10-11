
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
    FLOAT               reflective;      // 40 -- 48
    FLOAT               radius;         // 48 --
    PADDING8                            //    -- 64
    FLOAT3              position;       // 64 -- 96

    DECLARE_CONSTRUCTOR(sphere_s, FL3_CREF, FLOAT, FL3_CREF, uint32_t, FLOAT);
} PACKED ALIGNED64 sphere_t;

// -----------------------------------------------------------------------------
typedef struct plane_s
{
    FLOAT3              color;          // 0  -- 32
    uint32_t            specular;       // 32 --
    PADDING4                            //    -- 40
    FLOAT               reflective;      // 40 -- 48
    FLOAT3              point;          // 48 -- 80
    FLOAT3              normal;         // 80 -- 112

    DECLARE_CONSTRUCTOR(plane_s, FL3_CREF, FL3_CREF, FL3_CREF, uint32_t, FLOAT);
} PACKED ALIGNED64 plane_t;

// -----------------------------------------------------------------------------
typedef struct triangle_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    PADDING16                           //    -- 64
    FLOAT3          normal;             // 64 -- 96
    FLOAT3          a;                  // 96 -- 128
    FLOAT3          b;                  // 128-- 160
    FLOAT3          c;                  // 160-- 192

    DECLARE_CONSTRUCTOR(triangle_s, FL3_CREF, FL3_CREF, FL3_CREF, FL3_CREF,
        uint32_t, FLOAT);
} PACKED ALIGNED64 triangle_t;

// -----------------------------------------------------------------------------
typedef struct cone_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           width;              // 48 -- 56
    FLOAT           gamma;              // 56 -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          direction;          // 96 -- 128
    FLOAT3          matr[3];            // 224

    DECLARE_CONSTRUCTOR(cone_s, FL3_CREF, FL3_CREF, FLOAT, FLOAT, FL3_CREF,
        uint32_t, FLOAT);
} PACKED ALIGNED64 cone_t;

// -----------------------------------------------------------------------------
typedef struct cylinder_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           radius;             // 48 -- 56
    FLOAT           height;             // 56 -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          direction;          // 96 -- 128

    DECLARE_CONSTRUCTOR(cylinder_s, FL3_CREF, FL3_CREF, FLOAT, FLOAT,
        FL3_CREF, uint32_t, FLOAT);
} PACKED ALIGNED64 cylinder_t;

// -----------------------------------------------------------------------------
typedef struct torus_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    PADDING4                            //    -- 40
    FLOAT           reflective;          // 40 -- 48
    FLOAT           r;                  // 48 -- 56
    FLOAT           R;                  // 56 -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          normal;             // 96 -- 128
    FLOAT3          matr[3];               // 128-- 224

    DECLARE_CONSTRUCTOR(torus_s, FL3_CREF, FL3_CREF, FLOAT, FLOAT, FL3_CREF,
        uint32_t, FLOAT);
} PACKED ALIGNED64 torus_t;

// -----------------------------------------------------------------------------
typedef struct light_s
{
    light_type_t    type;               // 0  --
    PADDING4                            //    -- 8
    FLOAT           intensity;          // 8  --
    PADDING16                           //    -- 32   
    FLOAT3          color;              // 32 -- 64
    union {                             // 64 -- 96
        FLOAT3      direction;
        FLOAT3      position;
    };

    DECLARE_CONSTRUCTOR(light_s, light_type_t, FLOAT, FL3_CREF, FL3_CREF);
} PACKED ALIGNED64 light_t;

// -----------------------------------------------------------------------------
typedef struct camera_s
{
    FLOAT3   position;                  // 0  -- 32
    FLOAT3   direction;                 // 32 -- 64
    FLOAT3   rotate_matrix[3];          // 64 -- 160
    FLOAT    alpha;                     // 160-- 168
    FLOAT    theta;                     // 168-- 176

    DECLARE_CONSTRUCTOR(camera_s, FL3_CREF, FL3_CREF);
    DECLARE_METHOD(void, recompute_matrix);
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
