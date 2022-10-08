
#ifndef OBJECTS_HPP
# define OBJECTS_HPP

# ifndef __CPP
#  include "kernel.h"
#  define DECLARE_CONSTRUCTOR
# else
#  include <common.hpp>
#  define DECLARE_CONSTRUCTOR(__name, ...) __name(__VA_ARGS__)
# endif /* __CPP */

// ----------------------------------------------------------------------------
typedef enum
{
    AMBIENT = 0,
    DIRECT  = 1,
    POINT   = 2
} light_type_t;

// ----------------------------------------------------------------------------
typedef struct sphere_s
{
    FLOAT3              color;          // 0  -- 32
    uint32_t            specular;       // 32 --
    UNUSED uint32_t     _padding0;      //    -- 40
    FLOAT               reflective;     // 40 -- 48
    FLOAT               radius;         // 48 -- 56
    UNUSED uint64_t     _padding1;      // 56 -- 64
    FLOAT3              position;       // 64 -- 96

    DECLARE_CONSTRUCTOR(sphere_s, FL3_CREF, FLOAT, FL3_CREF, uint32_t, FLOAT);
} PACKED ALIGNED8 sphere_t;

// -----------------------------------------------------------------------------
typedef struct plane_s
{
    FLOAT3              color;          // 0  -- 32
    uint32_t            specular;       // 32 --
    UNUSED uint32_t     _padding0;      //    -- 40
    FLOAT               reflective;     // 40 -- 48
    FLOAT3              point;          // 48 -- 80
    FLOAT3              normal;         // 80 -- 112

    DECLARE_CONSTRUCTOR(plane_s, FL3_CREF, FLOAT3, FL3_CREF, uint32_t, FLOAT);
} PACKED ALIGNED8 plane_t;

// -----------------------------------------------------------------------------
typedef struct triangle_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    UNUSED uint32_t _padding0;          //    -- 40
    FLOAT           reflective;         // 40 -- 48
    UNUSED uint64_t _padding1;          //    --
    UNUSED uint64_t _padding2;          //    -- 64
    FLOAT3          normal;             // 64 -- 96
    FLOAT3          a;                  // 96 -- 128
    FLOAT3          b;                  // 128-- 160
    FLOAT3          c;                  // 160-- 192

    DECLARE_CONSTRUCTOR(triangle_s, FL3_CREF, FL3_CREF, FL3_CREF, FL3_CREF,
        uint32_t, FLOAT);
} PACKED ALIGNED8 triangle_t;

// -----------------------------------------------------------------------------
typedef struct cone_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    UNUSED uint32_t _padding;           //    -- 40
    FLOAT           reflective;         // 40 -- 48
    FLOAT           width;              // 48 -- 56
    FLOAT           gamma;              // 56 -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          direction;          // 96 -- 128
    FLOAT3          matr[3];            // 224

    DECLARE_CONSTRUCTOR(cone_s, FL3_CREF, FL3_CREF, FLOAT, FLOAT, FL3_CREF,
        uint32_t, FLOAT);
} PACKED ALIGNED8 cone_t;

// -----------------------------------------------------------------------------
typedef struct cylinder_s
{
    FLOAT3          color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    UNUSED uint32_t _padding0;          //    -- 40
    FLOAT           reflective;         // 40 -- 48
    FLOAT           radius;             // 48 --
    UNUSED uint64_t _padding1;          //    -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          direction;          // 96 -- 128

    DECLARE_CONSTRUCTOR(cylinder_s, FL3_CREF, FL3_CREF, FLOAT, FL3_CREF, 
        uint32_t, FLOAT);
# endif /* __CPP */
} PACKED ALIGNED8 cylinder_t;

// -----------------------------------------------------------------------------
typedef struct torus_s
{
    FL3_CREF        color;              // 0  -- 32
    uint32_t        specular;           // 32 --
    UNUSED uint32_t _padding0;          //    --
    FLOAT           reflective;         //    -- 40
    FLOAT           r;                  // 40 -- 48
    FLOAT           R;                  // 58 --
    UNUSED uint64_t _padding1           //    -- 64
    FLOAT3          position;           // 64 -- 96
    FLOAT3          normal;             // 96 -- 128

    DECLARE_CONSTRUCTOR(torus_s, FL3_CREF, FL3_CREF, FLOAT, FLOAT, FL3_CREF,
        uint32_t, FLOAT);
} PACKED ALIGNED8 torus_t;

// -----------------------------------------------------------------------------
typedef struct light_s
{
    light_type_t    type;               // 0  -- 32
    FLOAT           intensity;          // 32 --
    FLOAT3          color;
    union {
        FLOAT3      direction;
        FLOAT3      position;
    };

    DECLARE_CONSTRUCTOR(light_s, light_type_t, FLOAT, FL3_CREF, FL3_CREF);
} PACKED ALIGNED8 light_t;

#endif /* OBJECTS_HPP */
