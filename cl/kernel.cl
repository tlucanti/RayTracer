
//typedef float float3;

#define PACKED  __attribute__((packed))
#define NULL    (void *)0

#define BLACK   0x000000
#define WHITE   0xFFFFFF

typedef struct sphere_s
{
   float3  center;
   float   radius;
   int     color;
} PACKED sphere_t;

typedef struct camera_s
{
    float3  center;
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

int    trace_ray(__global sphere_t *spheres, int spheres_num, float3 camera, float3 direction)
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
    return closest_sphere->color;
}

__kernel void ray_tracer(
        __global unsigned int *canvas,
        __global sphere_t *spheres,
        int spheres_num,
        __global camera_t *cameras,
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
    int color = trace_ray(spheres, spheres_num, cameras[0].direction, vec);
    canvas[y * width + z] = color;
}