
#ifndef CL_KERNEL
# define CL_KERNEL
# ifndef __CPP
#  define UNUSED  __attribute__((unused))
#  ifndef NULL
#   define NULL    0
#  endif /* NULL */
#  define CPP_UNUSED

typedef double FLOAT;
typedef double3 FLOAT3;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long int uint64_t;
typedef long int int64_t;
# endif /* not __CPP */

# define as_sphere(obj_ptr) ((__constant sphere_t *)(obj_ptr))
# define as_plane(obj_ptr) ((__constant plane_t *)(obj_ptr))
# define as_triangle(obj_ptr) ((__constant triangle_t *)(obj_ptr))

# define get_obj_color(obj_ptr) as_sphere(obj_ptr)->color
# define get_obj_specular(obj_ptr) as_sphere(obj_ptr)->specular
# define get_obj_reflective(obj_ptr) as_sphere(obj_ptr)->reflective

# define BLACK   (FLOAT3){0.f, 0.f, 0.f}
# define EPS 1e-4

# define PACKED    __attribute__((__packed__))
# define ALIGNED16 __attribute__((__aligned__(16)))

typedef struct sphere_s
{
    FLOAT3              color;              // 0  -- 32
    uint32_t            specular;           // 32 --
    UNUSED uint32_t     _padding0;          //    -- 40
    FLOAT               reflective;         // 40 -- 48
    FLOAT               radius;             // 48 -- 56
    UNUSED uint64_t     _padding1;          // 56 -- 64
    FLOAT3              center;             // 64 -- 96

# ifdef __CPP
    sphere_s(FLOAT3 center, FLOAT radius, FLOAT3 color, uint32_t specular, FLOAT reflective)
            : center(center), radius(radius), color(color), specular(specular), reflective(reflective), _padding0(), _padding1()
    {}
# endif /* __CPP */
} PACKED ALIGNED16 sphere_t;

typedef struct plane_s
{
    FLOAT3              color;              // 0  -- 32
    uint32_t            specular;           // 32 --
    UNUSED uint32_t     _padding0;          //    -- 40
    FLOAT               reflective;         // 40 -- 48
    FLOAT3              point;              // 48 -- 80
    FLOAT3              normal;             // 80 -- 112

# ifdef __CPP
    plane_s(FLOAT3 point, FLOAT3 normal, FLOAT3 color, uint32_t specular, FLOAT reflective)
            : point(point), normal(normal), color(color), specular(specular), reflective(reflective)
    {
        normalize_ref(this->normal);
    }
# endif /* __CPP */
} PACKED ALIGNED16 plane_t;

typedef struct triangle_s
{
    FLOAT3          color;      // 0  -- 32
    uint32_t        specular;   // 32 --
    UNUSED uint32_t _padding0;  //    -- 40
    FLOAT           reflective;  // 40 -- 48
    UNUSED uint64_t _padding1;  // 48 -- 56
    UNUSED uint64_t _padding2;  // 56 -- 64
    FLOAT3          normal;     // 64 -- 96
    FLOAT3          a;          // 96 -- 128
    FLOAT3          b;          // 128-- 160
    FLOAT3          c;          // 160-- 192

# ifdef __CPP
    triangle_s(FLOAT3 p1, FLOAT3 p2, FLOAT3 p3, FLOAT3 color, uint32_t specular, FLOAT reflective)
        : a(p1), b(p2), c(p3), color(color), specular(specular), reflective(reflective)
    {
        normal = cross(p2 - p1, p3 - p1);
    }
# endif /* __CPP */
} PACKED ALIGNED16 triangle_t;

typedef struct cone_s
{

} PACKED ALIGNED16 cone_t;

typedef struct ambient_s
{
    FLOAT3      color;      // 0  -- 32
    FLOAT       intensity;  // 32 -- 40

# ifdef __CPP
    ambient_s(FLOAT intensity, FLOAT3 color)
            : intensity(intensity), color(color)
    {}
# endif /* __CPP */
} PACKED ALIGNED16 ambient_t;

typedef struct point_s
{
    FLOAT3      color;      // 0  -- 32
    FLOAT3      position;   // 32 -- 64
    FLOAT       intensity;  // 64 -- 72

# ifdef __CPP
    point_s(FLOAT3 position, FLOAT intencity, FLOAT3 color)
            : position(position), intensity(intencity), color(color)
    {}
# endif /* __CPP */
} PACKED ALIGNED16 point_t;

typedef struct direct_s
{
    FLOAT3      color;      // 0  -- 32
    FLOAT3      direction;  // 32 -- 64
    FLOAT       intensity;  // 64 -- 72

# ifdef __CPP
    direct_s(FLOAT3 dir, FLOAT intensity, FLOAT3 color)
            : direction(dir), intensity(intensity), color(color)
    {
        normalize_ref(direction);
    }
# endif /* __CPP */
} PACKED ALIGNED16 direct_t;

typedef struct camera_s
{
    FLOAT3   position;          // 0  -- 32
    FLOAT3   direction;         // 32 -- 64

    FLOAT3   rotate_matrix[3];  // 64 -- 96
    FLOAT    alpha;             // 96 -- 104
    FLOAT    theta;             // 104-- 112

# ifdef __CPP
    camera_s(FLOAT3 pos, FLOAT3 dir)
            : position(pos), direction(dir), rotate_matrix()
    {
        normalize_ref(direction);

        alpha = atan2(direction.x, direction.z);
        if (std::isinf(alpha) or std::isnan(alpha))
            alpha = 0;

        theta = atan2(direction.y, direction.z * direction.z + direction.x * direction.x);
        if (std::isinf(theta) or std::isnan(theta))
            theta = 0;
        recompute_matrix();
    }

    void recompute_matrix()
    {
        FLOAT	sin_alpha;
        FLOAT   cos_alpha;
        FLOAT   sin_theta;
        FLOAT   cos_theta;

        sincos(alpha, &sin_alpha, &cos_alpha);
        sincos(theta, &sin_theta, &cos_theta);

        rotate_matrix[0] = {cos_alpha, sin_alpha * sin_theta, sin_alpha * cos_theta};
        rotate_matrix[1] = {0, cos_theta, -sin_theta};
        rotate_matrix[2] = {-sin_alpha, sin_theta * cos_alpha, cos_alpha * cos_theta};
    }
# endif /* __CPP */
} PACKED ALIGNED16 camera_t;

# ifdef __CPP
#  define __constant
#  define __global
#  define __kernel
#  define CPP_UNUSED __attribute__((unused))
CPP_UNUSED FLOAT3 normalize(FLOAT3) {return{};}
CPP_UNUSED uint32_t get_global_id(uint32_t) {return{};}
CPP_UNUSED FLOAT fmin(FLOAT, FLOAT) {return{};}
CPP_UNUSED FLOAT fmax(FLOAT, FLOAT) {return{};}
# endif /* __CPP */

typedef struct scene_s
{
    __constant const sphere_t   *__restrict spheres;
    __constant const plane_t    *__restrict planes;
    __constant const triangle_t *__restrict triangles;
    __constant const ambient_t  *__restrict ambients;
    __constant const point_t    *__restrict points;
    __constant const direct_t   *__restrict directs;
    __constant const camera_t   *__restrict cameras;

    const uint32_t spheres_num;
    const uint32_t planes_num;
    const uint32_t triangles_num;
    const uint32_t ambients_num;
    const uint32_t points_num;
    const uint32_t directs_num;
    const uint32_t cameras_num;
} scene_t;

typedef enum obj_type_e
{
    SPHERE,
    PLANE,
    TRIANGLE
} obj_type_t;

CPP_UNUSED
FLOAT3 rotate_vector(FLOAT3 vec, __constant const FLOAT3 *matrix)
{
    return (FLOAT3) {
        dot(matrix[0], vec),
        dot(matrix[1], vec),
        dot(matrix[2], vec)
    };
}

CPP_UNUSED
FLOAT intersect_sphere(FLOAT3 camera, FLOAT3 direction, __constant const sphere_t *__restrict sp)
{
   FLOAT3  oc = camera - sp->center;

   FLOAT a = dot(direction, direction);
   FLOAT b = 2 * dot(oc, direction); // change to 2b
   FLOAT c = dot(oc, oc) - sp->radius * sp->radius;

   FLOAT discriminant = b*b - 4*a*c;
   if (discriminant < EPS)
       return INFINITY;
   discriminant = sqrt(discriminant);
   FLOAT x1 = (-b + discriminant) / 2 / a; // firstly divide, then multiply
   FLOAT x2 = (-b - discriminant) / 2 / a;
   FLOAT mn = fmin(x1, x2);
   if (mn < EPS)
       return fmax(x1, x2);
   return mn;
}

CPP_UNUSED
FLOAT intersect_plane(FLOAT3 camera, FLOAT3 direction, __constant const plane_t *__restrict pl)
{
    FLOAT3 co = pl->point - camera; // maybe not creating new object, and use camera object here
    return dot(co, pl->normal) / dot(direction, pl->normal);
}

CPP_UNUSED
FLOAT intersect_triangle(FLOAT3 camera, FLOAT3 direction, __constant const triangle_t *__restrict tr)
{
    FLOAT3 co = tr->a - camera;
    FLOAT t = dot(co, tr->normal) / dot(direction, tr->normal);
    FLOAT3 p = camera + direction * t;
    FLOAT3 ab = tr->b - tr->a;
    FLOAT3 bc = tr->c - tr->b;
    FLOAT3 ca = tr->a - tr->c;
    bool A = cross(p - tr->a, ab).z > 0;
    bool B = cross(p - tr->b, bc).z > 0;
    bool C = cross(p - tr->c, ca).z > 0;
    if ((A & B & C) | !(A | B | C))
        return t;
    return INFINITY;
}

CPP_UNUSED
const __constant void *__restrict closest_intersection(
        const scene_t *__restrict scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end,
        FLOAT *closest_t_ptr,
        obj_type_t *closest_type_ptr
    )
{
    FLOAT closest_t = INFINITY;
    __constant const void *closest_obj = NULL;
    obj_type_t closest_type;

    for (uint32_t i=0; i < scene->spheres_num; ++i)
    {
        FLOAT t = intersect_sphere(camera, direction, scene->spheres + i);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_obj = scene->spheres + i;
            closest_type = SPHERE;
        }
    }

    for (uint32_t i=0; i < scene->planes_num; ++i)
    {
        FLOAT t = intersect_plane(camera, direction, scene->planes + i);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_obj = scene->planes + i;
            closest_type = PLANE;
        }
    }

    for (uint32_t i=0; i < scene->triangles_num; ++i)
    {
        FLOAT t = intersect_triangle(camera, direction, scene->triangles + i);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_obj = scene->triangles + i;
            closest_type = TRIANGLE;
        }
    }

    *closest_t_ptr = closest_t;
    *closest_type_ptr = closest_type;
    return closest_obj;
}

CPP_UNUSED
bool shadow_intersection(
        const scene_t *__restrict scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end
    )
{
    for (uint32_t i=0; i < scene->spheres_num; ++i)
    {
        FLOAT t = intersect_sphere(camera, direction, scene->spheres + i);
        if (t > start && t < end) // maybe >= <= here
            return true;
    }

    for (uint32_t i=0; i < scene->planes_num; ++i)
    {
        FLOAT t = intersect_plane(camera, direction, scene->planes + i);
        if (t > start && t < end)
            return true;
    }

    for (uint32_t i=0; i < scene->triangles_num; ++i)
    {
        FLOAT t = intersect_triangle(camera, direction, scene->triangles + i);
        if (t > start && t < end)
            return true;
    }
    return false;
}

CPP_UNUSED
FLOAT3 reflect_ray(FLOAT3 ray, FLOAT3 normal)
{
    return normal * (2 * dot(ray, normal)) - ray;
}

CPP_UNUSED
FLOAT compute_lightning_single(
        FLOAT3 light_vector,
        FLOAT3 normal_vector,
        FLOAT3 direction,
        FLOAT light_intensity,
        uint32_t specular
    )
{
    FLOAT intensity = 0.f;

    // diffuse lightning
    FLOAT normal_angle = dot(normal_vector, light_vector);
    if (normal_angle > EPS)
        intensity += light_intensity * normal_angle;

    // specular light
    if (specular > 0)
    {
        FLOAT3 reflected = reflect_ray(light_vector, normal_vector);
        FLOAT reflected_angle = dot(reflected, direction);
        if (reflected_angle < EPS)
            intensity += light_intensity * pow(reflected_angle, specular);
    }

    return intensity;
}

CPP_UNUSED
FLOAT compute_lightning(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        uint32_t specular)
{
    FLOAT intensity = 0.f;
    FLOAT3 light_vector;

    for (uint32_t i=0; i < scene->ambients_num; ++i)
        intensity += scene->ambients[i].intensity;

    for (uint32_t i=0; i < scene->points_num; ++i)
    {
        light_vector = normalize(scene->points[i].position - point);
        if (shadow_intersection(scene, point, light_vector, EPS, 1.f - EPS))
            continue ;
        intensity += compute_lightning_single(light_vector, normal, direction, scene->points[i].intensity, specular);
    }

    for (uint32_t i=0; i < scene->directs_num; ++i)
    {
        light_vector = scene->directs[i].direction;
        if (shadow_intersection(scene, point, light_vector, EPS, INFINITY))
            continue ;
        intensity += compute_lightning_single(light_vector, normal, direction, scene->directs[i].intensity, specular);
    }

    return fmin(1 - EPS, intensity);
}

CPP_UNUSED
FLOAT3    trace_ray(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 direction,
        uint32_t recursion_depth
    )
{
    const __constant void *__restrict closest_obj;
    FLOAT closest_t;
    FLOAT3 color = BLACK;
    FLOAT reflective_prod = 1.f;
    obj_type_t closest_type;

    while (recursion_depth > 0)
    {
        --recursion_depth;
        closest_obj = closest_intersection(scene, point, direction, EPS, INFINITY, &closest_t, &closest_type);
        if (closest_obj == NULL)
            break ;

        point += direction * closest_t;
        FLOAT3 normal;
        switch (closest_type)
        {
            case SPHERE: normal = normalize(point - as_sphere(closest_obj)->center); break ;
            case PLANE: normal = as_plane(closest_obj)->normal; break ;
            case TRIANGLE: normal = as_triangle(closest_obj)->normal; break ;
        }
        FLOAT factor = compute_lightning(scene, point, normal, direction, get_obj_specular(closest_obj));
        FLOAT3 local_color = get_obj_color(closest_obj) * factor;

        if (recursion_depth == 0)
            color += local_color * reflective_prod /* * closest_sphere->reflective*/;
        else
            color += local_color * (1.0f - get_obj_reflective(closest_obj)) * reflective_prod;
        reflective_prod *= get_obj_reflective(closest_obj);

        direction = reflect_ray(-direction, normal);
    }

    return color;
}

CPP_UNUSED
__kernel void ray_tracer(
        __global uint32_t *canvas,

        __constant const sphere_t *__restrict spheres,
        __constant const plane_t *__restrict planes,
        __constant const triangle_t *__restrict triangles,
        __constant const ambient_t *__restrict ambients,
        __constant const point_t *__restrict points,
        __constant const direct_t *__restrict directs,
        __constant const camera_t *__restrict cameras,

        const uint32_t spheres_num,
        const uint32_t planes_num,
        const uint32_t triangles_num,
        const uint32_t ambients_num,
        const uint32_t points_num,
        const uint32_t directs_num,
        const uint32_t cameras_num,

        const uint32_t width,
        const uint32_t height
    )
{

    const FLOAT rheight = 1.0 / height;
    const FLOAT rwidth = 1.0 / width;
    const uint32_t z = get_global_id(0);
    const uint32_t y = get_global_id(1);

    const scene_t scene = (scene_t){
        spheres,
        planes,
        triangles,
        ambients,
        points,
        directs,
        cameras,

        spheres_num,
        planes_num,
        triangles_num,
        ambients_num,
        points_num,
        directs_num,
        cameras_num
    };

    FLOAT3 vec = (FLOAT3) {
        (z - width / 2.) * rwidth,
        (y - height / 2.) * rheight,
        1
    };
    vec = normalize(vec);
    vec = rotate_vector(vec, cameras[0].rotate_matrix);
    const FLOAT3 color = trace_ray(
        &scene,
        cameras[0].position,
        vec,
        4
    );
    const uint32_t int_color =
            (uint32_t)(color.x) << 16
            | (uint32_t)(color.y) << 8
            | (uint32_t)(color.z);
    canvas[(height - y - 1) * width + z] = int_color;
}

#endif /* CL_KERNEL */
