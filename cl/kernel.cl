
#ifndef __OPENCL
# define __kernel
# define __global
# define __constant
# define __local
# define INFINITY 1e38f

typedef float float3;

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

#define BLACK   0x000000
#define WHITE   0xFFFFFF

typedef struct sphere_s
{
   float3  center;
   float   radius;
    float3     color;
    int   specular;
    float reflective;
} PACKED sphere_t;

typedef struct ambient_s
{
    float   intensity;
    float3    color;
} PACKED ambient_t;

typedef struct point_s
{
    float3  position;
    float   intensity;
    float3    color;
} PACKED point_t;

typedef struct direct_s
{
    float3  direction;
    float   intensity;
    float3    color;
} PACKED direct_t;

typedef struct camera_s
{
    float3  position;
    float3  direction;
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

float intersect_sphere(float3 camera, float3 direction, __global const sphere_t *__restrict sp)
{
   float3  oc = camera - sp->center;
   
   float a = dot(direction, direction);
   float b = 2 * dot(oc, direction);
   float c = dot(oc, oc) - sp->radius * sp->radius;

   float discriminant = b*b - 4*a*c;
   if (discriminant < 0)
       return INFINITY;
   discriminant = sqrt(discriminant);
   float x1 = (-b + discriminant) / 2 / a;
   float x2 = (-b - discriminant) / 2 / a;
   float mn = fmin(x1, x2);
   if (mn < 1e-4)
       return fmax(x1, x2);
   return mn;
}

const __global sphere_t *__restrict closest_intersection(
        const scene_t *__restrict scene,
        float3 camera,
        float3 direction,
        float start,
        float end,
        float *closest_t_ptr
    )
{
    float closest_t = INFINITY;
    __global const sphere_t *closest_sphere = NULL;

    for (int i=0; i < scene->spheres_num; ++i)
    {
        float t = intersect_sphere(camera, direction, scene->spheres + i);

        if (t < start || t > end)
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

float3 reflect_ray(float3 ray, float3 normal)
{
    return 2 * normal * dot(ray, normal) - ray;
}

float compute_lightning_single(
        float3 light_vector,
        float3 normal_vector,
        float3 direction,
        float light_intensity,
        int specular
    )
{
    float intensity = 0;

    float normal_angle = dot(normal_vector, light_vector);
    if (normal_angle > 1e-3)
        intensity += light_intensity * normal_angle;

    if (specular > 0)
    {
        float3 reflected = reflect_ray(light_vector, normal_vector);
        float reflected_angle = dot(reflected, direction);
        if (reflected_angle < 1e-3)
            intensity += light_intensity * pow(reflected_angle, specular);
    }

    return intensity;
}

float compute_lightning(
        const scene_t *scene,
        float3 point,
        float3 normal,
        float3 direction,
        int specular)
{
    float intensity = 0;
    float3 light_vector;
    float _;

    for (int i=0; i < scene->ambients_num; ++i)
        intensity += scene->ambients[i].intensity;

    for (int i=0; i < scene->points_num; ++i)
    {
        light_vector = normalize(scene->points[i].position - point);
        if (closest_intersection(scene, point, light_vector, 1e-3, 1, &_) != NULL)
            continue ;
        intensity += compute_lightning_single(light_vector, normal, direction, scene->points[i].intensity, specular);
    }

    for (int i=0; i < scene->directs_num; ++i)
    {
        light_vector = scene->directs[i].direction;
        if (closest_intersection(scene, point, light_vector, 1e-3, INFINITY, &_) != NULL)
            continue ;
        intensity += compute_lightning_single(light_vector, normal, direction, scene->directs[i].intensity, specular);
    }

    return fmin(1, intensity);
}

float3    trace_ray(
        const scene_t *scene,
        float3 camera,
        float3 direction,
        int recursion_depth
    )
{
    const __global sphere_t *__restrict closest_sphere;
    float closest_t;

    closest_sphere = closest_intersection(scene, camera, direction, 0, INFINITY, &closest_t);
    if (closest_sphere == NULL)
        return BLACK;

    float3 point = camera + closest_t * direction;
    float3 normal = normalize(point - closest_sphere->center);
    float factor = compute_lightning(scene, point, normal, direction, closest_sphere->specular);
    float3 local_color = closest_sphere->color * factor;

    if (closest_sphere->reflective < 0 || recursion_depth <= 0)
        return local_color;

    float3 reflected_ray = reflect_ray(-direction, normal);
    float3 reflected_color = trace_ray(scene, point, reflected_ray, recursion_depth - 1);
    float reflective = closest_sphere->reflective;

    return local_color * (1 - reflective) + reflected_color * reflective;
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
    const float rheight = 1.0f / height;
    const float rwidth = 1.0f / width;
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

    const float3 vec = normalize((float3)(
        (z - width / 2) * rwidth,
        (y - height / 2) * rheight,
        1
    ));
    const float3 color = trace_ray(
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