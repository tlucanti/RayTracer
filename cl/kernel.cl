
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
# define NULL    (void *)0
#endif /* NULL */

#define BLACK   0x000000
#define WHITE   0xFFFFFF

typedef struct sphere_s
{
   float3  center;
   float   radius;
    float3     color;
    int   specular;
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

float intersect_sphere(float3 camera, float3 direction, __global sphere_t *sp)
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

float3 reflect_ray(float3 ray, float3 normal)
{
    return 2 * normal * dot(ray, normal) - ray;
}

float compute_lightning_single(float3 light_vector, float3 normal_vector, float3 direction, float light_intensity, int specular)
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

    return      intensity;
}

float compute_lightning(
        __global ambient_t *ambients,
        __global point_t *points,
        __global direct_t *directs,

        int ambients_num,
        int points_num,
        int directs_num,

        float3 point,
        float3 normal,
        float3 direction,

        int specular)
{
    float intensity = 0;
    float3 light_vector;

    for (int i=0; i < ambients_num; ++i)
        intensity += ambients[i].intensity;

    for (int i=0; i < points_num; ++i)
    {
        light_vector = normalize(points[i].position - point);
        intensity += compute_lightning_single(light_vector, normal, direction, points[i].intensity, specular);
    }

    for (int i=0; i < directs_num; ++i)
    {
        light_vector = directs[i].direction;
        intensity += compute_lightning_single(light_vector, normal, direction, directs[i].intensity, specular);
    }

    return fmin(1, intensity);
}

float3    trace_ray(
        __global sphere_t *spheres,
        __global ambient_t *ambients,
        __global point_t *points,
        __global direct_t *directs,

        int spheres_num,
        int ambients_num,
        int points_num,
        int directs_num,

        float3 camera,
        float3 direction
    )
{
    float closest_t = INFINITY;
    __global sphere_t *closest_sphere = NULL;

    for (int i=0; i < spheres_num; ++i)
    {
        float t = intersect_sphere(camera, direction, spheres + i);
        if (t < closest_t)
        {
            closest_t = t;
            closest_sphere = spheres + i;
        }
    }
    if (closest_sphere == NULL)
        return BLACK;

    float3 point = camera + closest_t * direction;
    float3 normal = point - closest_sphere->center;
    float factor = compute_lightning(ambients, points, directs, ambients_num, points_num, directs_num, point, normal, direction, closest_sphere->specular);
    return closest_sphere->color * factor;
}

__kernel void ray_tracer(
        __global unsigned int *canvas,

        __global sphere_t *spheres,
        __global ambient_t *ambients,
        __global point_t *points,
        __global direct_t *directs,
        __global camera_t *cameras,

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

    int z = get_global_id(0);
    int y = get_global_id(1);
    float3 vec = (float3)(
        (z - width / 2) * rwidth,
        (y - height / 2) * rheight,
        1
    );
    vec = normalize(vec);
    float3 color = trace_ray(
        spheres,
        ambients,
        points,
        directs,

        spheres_num,
        ambients_num,
        points_num,
        directs_num,

        cameras[0].position,
        vec
    );
    unsigned int int_color =
            (unsigned int)(color.x) << 16
            | (unsigned int)(color.y) << 8
            | (unsigned int)(color.z);
    canvas[(height - y - 1) * width + z] = int_color;
}