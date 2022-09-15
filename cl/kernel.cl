
#ifndef __OPENCL
# define __kernel
# define __global
# define __constant
# define __local
# define INFINITY 1e38f

typedef float float3;
typedef double double3;

int get_global_id(int);
float dot(float3, float3);
float sqrt(float);
float fmin(float, float);
float fmax(float, float);
float3 normalize(float3);
float pow(float, float);

#endif

#define PACKED  __attribute__((packed))
#ifndef NULL
# define NULL    0
#endif /* NULL */
#define EPS 1e-4

typedef double FLOAT;
typedef double3 FLOAT3;

#define BLACK   (FLOAT3)(0.f, 0.f, 0.f)
#define WHITE   (FLOAT3)(254.9f, 254.9f, 254.9f)

typedef struct sphere_s
{
    FLOAT3  center;
    FLOAT   radius;
    FLOAT3     color;
    int   specular;
    FLOAT reflective;
} PACKED sphere_t;

typedef struct ambient_s
{
    FLOAT   intensity;
    FLOAT3    color;
} PACKED ambient_t;

typedef struct point_s
{
    FLOAT3  position;
    FLOAT   intensity;
    FLOAT3    color;
} PACKED point_t;

typedef struct direct_s
{
    FLOAT3  direction;
    FLOAT   intensity;
    FLOAT3    color;
} PACKED direct_t;

typedef struct camera_s
{
    FLOAT3  position;
    FLOAT3  direction;

    FLOAT   alpha;
    FLOAT   theta;
    FLOAT3  rotate_matrix[3];
} PACKED camera_t;

typedef struct scene_s
{
    __global const sphere_t *__restrict spheres;
    const int spheres_num;

    __global const ambient_t *__restrict ambients;
    const int ambients_num;

    __global const point_t *__restrict points;
    const int points_num;

    __global const direct_t *__restrict directs;
    const int directs_num;

    __global const camera_t *__restrict cameras;
    const int cameras_num;
} scene_t;

FLOAT3 rotate_vector(FLOAT3 vec, __global FLOAT3 *matrix)
{
    return (FLOAT3)(
        dot(matrix[0], vec),
        dot(matrix[1], vec),
        dot(matrix[2], vec)
    );
}

FLOAT intersect_sphere(FLOAT3 camera, FLOAT3 direction, __global const sphere_t *__restrict sp)
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

const __global sphere_t *__restrict closest_intersection(
        const scene_t *__restrict scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end,
        FLOAT *closest_t_ptr
    )
{
    FLOAT closest_t = INFINITY;
    __global const sphere_t *closest_sphere = NULL;

    for (int i=0; i < scene->spheres_num; ++i)
    {
        FLOAT t = intersect_sphere(camera, direction, scene->spheres + i);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_sphere = scene->spheres + i;
        }
    }

    *closest_t_ptr = closest_t;
    return closest_sphere;
}

bool shadow_intersection(
        const scene_t *__restrict scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end
    )
{
    for (int i=0; i < scene->spheres_num; ++i)
    {
        FLOAT t = intersect_sphere(camera, direction, scene->spheres + i);

        if (t > start && t < end) // maybe >= <= here
            return true;
    }
    return false;
}

FLOAT3 reflect_ray(FLOAT3 ray, FLOAT3 normal)
{
    return normal * (2 * dot(ray, normal)) - ray;
}

FLOAT compute_lightning_single(
        FLOAT3 light_vector,
        FLOAT3 normal_vector,
        FLOAT3 direction,
        FLOAT light_intensity,
        int specular
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

FLOAT compute_lightning(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        int specular)
{
    FLOAT intensity = 0.f;
    FLOAT3 light_vector;

    for (int i=0; i < scene->ambients_num; ++i)
        intensity += scene->ambients[i].intensity;

    for (int i=0; i < scene->points_num; ++i)
    {
        light_vector = normalize(scene->points[i].position - point);
        if (shadow_intersection(scene, point, light_vector, EPS, 1.f - EPS))
            continue ;
        intensity += compute_lightning_single(light_vector, normal, direction, scene->points[i].intensity, specular);
    }

    for (int i=0; i < scene->directs_num; ++i)
    {
        light_vector = scene->directs[i].direction;
        if (shadow_intersection(scene, point, light_vector, EPS, INFINITY))
            continue ;
        intensity += compute_lightning_single(light_vector, normal, direction, scene->directs[i].intensity, specular);
    }

    return fmin(1 - EPS, intensity);
}

FLOAT3    trace_ray(
        const scene_t *scene,
        FLOAT3 point,
        FLOAT3 direction,
        int recursion_depth
    )
{
    const __global sphere_t *__restrict closest_sphere;
    FLOAT closest_t;
    FLOAT3 color = BLACK;
    FLOAT reflective_prod = 1.f;

    while (recursion_depth > 0)
    {
        --recursion_depth;
        closest_sphere = closest_intersection(scene, point, direction, EPS, INFINITY, &closest_t);
        if (closest_sphere == NULL)
        {
            break ;
        }

        point += closest_t * direction;
        FLOAT3 normal = normalize(point - closest_sphere->center);
        FLOAT factor = compute_lightning(scene, point, normal, direction, closest_sphere->specular);
        FLOAT3 local_color = closest_sphere->color * factor;

        if (recursion_depth == 0)
            color += local_color * reflective_prod /* * closest_sphere->reflective*/;
        else
            color += local_color * (1.0f - closest_sphere->reflective) * reflective_prod;
        reflective_prod *= closest_sphere->reflective;

        direction = reflect_ray(-direction, normal);
    }

    return color;
}

__kernel void ray_tracer(
        __global unsigned int *canvas,

        __global const sphere_t *spheres,
        __global const ambient_t *ambients,
        __global const point_t *points,
        __global const direct_t *directs,
        __global const camera_t *cameras,

        int spheres_num,
        int ambients_num,
        int points_num,
        int directs_num,
        int cameras_num,

        int width,
        int height
    )
{
    const FLOAT rheight = 1.0f / height;
    const FLOAT rwidth = 1.0f / width;
    const int z = get_global_id(0);
    const int y = get_global_id(1);

    const scene_t scene = (scene_t){
        spheres,
        spheres_num,

        ambients,
        ambients_num,

        points,
        points_num,

        directs,
        directs_num,

        cameras,
        cameras_num
    };

    FLOAT3 vec = (FLOAT3)(
        (z - width / 2) * rwidth,
        (y - height / 2) * rheight,
        1
    );
    vec = normalize(vec);
    vec = rotate_vector(vec, cameras[0].rotate_matrix);
    const FLOAT3 color = trace_ray(
        &scene,
        cameras[0].position,
        vec,
        4
    );
    const unsigned int int_color =
            (unsigned int)(color.x) << 16
            | (unsigned int)(color.y) << 8
            | (unsigned int)(color.z);
    canvas[(height - y - 1) * width + z] = int_color;
}