
#ifndef CL_TRACER
# define CL_TRACER

# ifndef __CPP
#  include "kernel_common.h"
# endif /* __CPP */
# include "kernel.h"
# include "objects.h"
# include "algo.h"
# include "inc.h"

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
bool check_sphere(
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT3 position,
        FLOAT radius
    )
{
    FLOAT3 oc = camera - position;

    FLOAT a = dot(direction, direction);                                        // FIXME: always equals to 1
    FLOAT b = 2 * dot(oc, direction);                                           // FIXME: change to 2b
    FLOAT c = dot(oc, oc) - radius * radius;                                    // FIXME: change to sp->r2

    return (b*b - 4*a*c) > 0;
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT intersect_sphere(
        FLOAT3 camera,
        FLOAT3 direction,
        sphere_ptr sp
    )
{
   FLOAT3  oc = camera - sp->position;

   FLOAT a = dot(direction, direction);                                         // FIXME: always equals to 1
   FLOAT b = 2 * dot(oc, direction);                                            // TODO: maybe get this as func argument
   FLOAT c = dot(oc, oc) - sp->radius * sp->radius;                             // FIXME: change to sp->r2

   FLOAT discriminant = b*b - 4*a*c;
   if (discriminant < EPS)
       return INFINITY;
   discriminant = sqrt(discriminant);
   FLOAT x1 = (-b + discriminant) / 2 / a;                                      // FIXME: firstly divide, then multiply
   FLOAT x2 = (-b - discriminant) / 2 / a;
   FLOAT mn = fmin(x1, x2);                                                     // FIXME: check position of roots using
   if (mn < EPS)                                                                // a, b, c values, and return minimal
       return fmax(x1, x2);                                                     // positive root without fmin and fmax
   return mn;
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT intersect_plane(
        FLOAT3 camera,
        FLOAT3 direction,
        plane_ptr pl
    )
{
    FLOAT3 co = pl->point - camera;                                             // FIXME: maybe not creating new object,
                                                                                // and use camera object here
    return dot(co, pl->normal) / dot(direction, pl->normal);                    // TODO: maybe get denumenator as
                                                                                // function argument
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT intersect_triangle(
        FLOAT3 camera,
        FLOAT3 direction,
        triangle_ptr tr
    )
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

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT intersect_cone(
        FLOAT3 camera,
        FLOAT3 direction,
        cone_ptr cn,
        FLOAT3 *param
    )
{
    FLOAT r_width = 1. / sqrt(cn->width);                                       // FIXME: use sqrt of width in structure
    FLOAT r_width2 = r_width * r_width;

    direction = rotate_vector(direction, cn->matr);
    camera = rotate_vector(camera - cn->position, cn->matr);
    direction.x *= r_width;
    direction.y *= r_width;
    camera.x *= r_width;
    camera.y *= r_width;

    FLOAT a = direction.x * direction.x + direction.y * direction.y             // FIXME: make vector dir2 {x, y, -z}
        - direction.z * direction.z;                                            // and use dot(dir, dir2)
    FLOAT b = camera.x * direction.x + camera.y * direction.y
        - camera.z * direction.z;                                               // TODO: maybe here
                                                                                // dot(camera, direction)
                                                                                //      - 2 * camera.z * direction.z
    b *= 2;
    FLOAT c = camera.x * camera.x + camera.y * camera.y
        - camera.z * camera.z + cn->gamma;

    FLOAT discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return INFINITY;
    discriminant = sqrt(discriminant);
    FLOAT x1 = (-b + discriminant) / 2 / a;                                     // FIXME: firstly divide, then multiply
    FLOAT x2 = (-b - discriminant) / 2 / a;
    FLOAT ret = fmin(x1, x2);
    if (ret < EPS)
        ret = fmax(x1, x2);

    if (param == NULLPTR)
        return ret;
    FLOAT3 point = camera + direction * ret;
    *param = ASSIGN_FLOAT3(
        point.x * r_width2,
        point.y * r_width2 * r_width2,
        -point.z);
    return ret;
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT intersect_cylinder(
        FLOAT3 camera,
        FLOAT3 direction,
        cylinder_ptr cy
    )
{
    FLOAT3 oc = camera - cy->position;
    FLOAT3 A = cross(oc, cy->direction);
    FLOAT3 B = cross(direction, cy->direction);

    FLOAT a = dot(B, B);
    FLOAT b = 2. * dot(A, B);
    FLOAT c = dot(A, A) - cy->radius * cy->radius;

    FLOAT discriminant = b * b - 4. * a * c;
    if (discriminant < EPS)
        return INFINITY;

    discriminant = sqrt(discriminant);
    FLOAT x1 = (-b + discriminant) / (2. * a);
    FLOAT x2 = (-b - discriminant) / (2. * a);

    if (x1 > x2 && x2 > EPS)
        SWAP(x1, x2);
    if (isinf(cy->height))
        return x1;

    FLOAT3 point = camera + direction * x1;
    FLOAT height = dot(point - cy->position, cy->direction);
    if (fabs(height) < cy->height)
        return x1;
    point = camera + direction * x2;
    height = dot(point - cy->position, cy->direction);
    if (fabs(height) < cy->height)
        return x2;
    return INFINITY;
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT intersect_torus(
        FLOAT3 camera,
        FLOAT3 direction,
        torus_ptr to,
        FLOAT3 *param
    )
{
    if (!check_sphere(camera, direction, to->position, to->R + to->r))
        return INFINITY;
    direction = rotate_vector(direction, to->matr);
    camera = rotate_vector(camera - to->position, to->matr);
    FLOAT3 camera_origin = camera;
    FLOAT3 direction_origin = direction;

    FLOAT ksi = to->R * to->R - to->r * to->r;                                  // FIXME: move this to torus structure
    FLOAT M4R2 = -4. * to->R * to->R;

    FLOAT a = dot(direction, direction);                                        // FIXME: this is always equals 1
    FLOAT b = 2. * dot(direction, camera);
    FLOAT c = dot(camera, camera) + ksi;

    camera.z = 0.;
    direction.z = 0.;
    FLOAT d = dot(direction, direction) * M4R2;
    FLOAT e = 2. * dot(direction, camera) * M4R2;
    FLOAT f = dot(camera, camera) * M4R2;

    FLOAT A = a*a;
    FLOAT B = 2.*a*b;
    FLOAT C = b*b + 2.*a*c + d;
    FLOAT D = 2.*b*c + e;
    FLOAT E = c*c + f;

    B /= A;                                                                     // FIXME: this is redunant
    C /= A;
    D /= A;
    E /= A;

    FLOAT res = quartic_complex_solve(B, C, D, E);
    if (param == NULLPTR || res == INFINITY)
        return res;
    FLOAT3 point = camera_origin + direction_origin * res;
    FLOAT dt = dot(point, point);
    FLOAT R2 = to->R * to->R;
    FLOAT r2 = to->r * to->r;
    *param = ASSIGN_FLOAT3(
        point.x * (-R2 - r2 + dt),
        point.y * (-R2 - r2 + dt),
        point.z * (R2 - r2 + dt)
    );
//    if (point.x * point.x + point.y * point.y <= R2)
//        *param *= 0;//-*param;
    return res;
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
void_ptr closest_intersection(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT start,
        FLOAT end,
        FLOAT *__restrict closest_t_ptr,
        obj_type_t *__restrict closest_type_ptr,
        FLOAT3 *__restrict param
    )
{
    FLOAT closest_t = INFINITY;
    void_ptr closest_obj = NULLPTR;
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

    for (uint32_t i=0; i < scene->cones_num; ++i)
    {
        FLOAT t = intersect_cone(camera, direction, scene->cones + i, param);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_obj = scene->cones + i;
            closest_type = CONE;
        }
    }

    for (uint32_t i=0; i < scene->cylinders_num; ++i)
    {
        FLOAT t = intersect_cylinder(camera, direction, scene->cylinders + i);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_obj = scene->cylinders + i;
            closest_type = CYLINDER;
        }
    }

    for (uint32_t i=0; i < scene->torus_num; ++i)
    {
        FLOAT3 param2;
        FLOAT t = intersect_torus(camera, direction, scene->torus + i, &param2);

        if (t < start || t > end) // maybe >= <= here
            continue ;
        if (t < closest_t)
        {
            closest_t = t;
            closest_obj = scene->torus + i;
            closest_type = TOR;
            *param = param2;
        }
    }

    *closest_t_ptr = closest_t;
    *closest_type_ptr = closest_type;
    return closest_obj;
}

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
bool shadow_intersection(
        scene_ptr scene,
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

    for (uint32_t i=0; i < scene->cones_num; ++i)
    {
        FLOAT t = intersect_cone(camera, direction, scene->cones + i, NULLPTR);
        if (t > start && t < end)
            return true;
    }

    for (uint32_t i=0; i < scene->cylinders_num; ++i)
    {
        FLOAT t = intersect_cylinder(camera, direction, scene->cylinders + i);
        if (t > start && t < end)
            return true;
    }

    for (uint32_t i=0; i < scene->torus_num; ++i)
    {
        FLOAT t = intersect_torus(camera, direction, scene->torus + i, NULLPTR);
        if (t > start && t < end)
            return true;
    }

    return false;
}

#ifdef RTX_EMISSION
CPP_UNUSED CPP_INLINE
FLOAT3 compute_emission(scene_ptr scene, FLOAT3 point, FLOAT3 normal)
{
    FLOAT3 factor = ASSIGN_FLOAT3(0., 0., 0.);

    for (uint32_t i=0; i < scene->spheres_num; ++i)
    {
        FLOAT3 direction = normalize(scene->spheres[i].position - point);
        sphere_ptr sphere = scene->spheres + i;

//        if (shadow_intersection(scene, point, direction, EPS, len))
//            continue ;

        FLOAT normal_angle = dot(normal, direction);
        if (normal_angle > EPS)
            factor += sphere->color * (1. / 255.) * sphere->emission * normal_angle;
    }

    return factor;
}
#endif /* RTX_EMISSION */

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT3 compute_lightning(
        scene_ptr scene,
        FLOAT3 point,
        FLOAT3 normal,
        FLOAT3 direction,
        uint32_t specular,
        FLOAT3 *specular_computed
    )
{
    FLOAT3 color = BLACK;
    FLOAT end;
    FLOAT3 light_vector;

    for (uint32_t i=0; i < scene->lights_num; ++i)
    {
        switch (scene->lights[i].type)
        {
            case AMBIENT:
                color += scene->lights[i].color;
                continue ;
            case DIRECT:
                light_vector = -scene->lights[i].direction;
                end = INFINITY;
                break ;
            case POINT:
                light_vector = scene->lights[i].position - point;
                end = length(light_vector);
                light_vector *= 1. / end;
                break ;
        }

        if (shadow_intersection(scene, point, light_vector, EPS, end))
            continue ;

        // diffuse lightning
        FLOAT normal_angle = dot(normal, light_vector);
        if (normal_angle > EPS)
            color += scene->lights[i].color * normal_angle;

        // specular light
        if (specular > 0)
        {
            FLOAT3 reflected = reflect_ray(light_vector, normal);
            FLOAT reflected_angle = dot(reflected, direction);
            if (reflected_angle < EPS)
                *specular_computed += scene->lights[i].color * pow(reflected_angle, specular);
        }
    }
#ifdef RTX_EMISSION
    color += compute_emission(scene, point, normal);
#endif /* RTX_EMISSION */
    return color;
}
// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
FLOAT3 compute_direct_lightning(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction,
        FLOAT t
    )
{
    FLOAT3 blinding = BLACK;
    FLOAT3 point = camera + direction * t;

    for (uint32_t i=0; i < scene->lights_num; ++i) {
        if (scene->lights[i].type != POINT)
            continue ;
        FLOAT3 co = scene->lights[i].position - camera;
        if (dot(co, direction) < EPS)
            continue ;

        FLOAT3 cp = scene->lights[i].position - point;
        (void)cp;
//        shadow_intersection(scene, point, normalize(cp), EPS,  length(cp));
        if (shadow_intersection(scene, point, normalize(cp), EPS,  length(cp)))
            continue ;

        FLOAT distance = length(cross(co, direction));
        FLOAT intensity = scene->lights[i].color.x + scene->lights[i].color.y + scene->lights[i].color.z;
        FLOAT factor = 1. * intensity / (distance * distance);

        blinding += scene->lights[i].color * factor;
    }
    return blinding;
}

// -----------------------------------------------------------------------------
#ifdef RTX_TRANSPARENCY
# define RECURSION_DEPTH 16
CPP_UNUSED CPP_INLINE
FLOAT3 trace_ray(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction
    )
{
    void_ptr    closest_obj;
    FLOAT3      _color = BLACK;
    FLOAT       closest_t;
    obj_type_t  closest_type;
    FLOAT3      param;

    FLOAT3 ray_queue_dir[RECURSION_DEPTH];
    FLOAT3 ray_queue_pos[RECURSION_DEPTH];
    FLOAT3 ray_queue_col[RECURSION_DEPTH];
    uint32_t ray_enqueued[RECURSION_DEPTH] = {};
    FLOAT reflective_prod[RECURSION_DEPTH];

    reflective_prod[1] = 1.;
    ray_queue_dir[1] = direction;
    ray_queue_pos[1] = camera;
    ray_enqueued[1] = 1;

    for (uint32_t current_ray = 1; current_ray < RECURSION_DEPTH; ++current_ray)
    {
        ray_queue_col[current_ray] = BLACK;
        if (ray_enqueued[current_ray] == 0) {
            continue;
        }
        closest_obj = closest_intersection(
            scene,
            ray_queue_pos[current_ray],
            ray_queue_dir[current_ray],
            EPS,
            INFINITY,
            &closest_t,
            &closest_type,
            &param
        );
#ifdef RTX_DIRECT
        ray_queue_col[current_ray] += compute_direct_lightning(scene, ray_queue_pos[current_ray], ray_queue_dir[current_ray], closest_t);
#endif /* RTX_DIRECT */
        //        *distance = closest_t;
        if (closest_obj == NULLPTR) {
            continue ;
        }

        ray_queue_pos[current_ray] += ray_queue_dir[current_ray] * closest_t;
        FLOAT3 normal;
        switch (closest_type)
        {
            case SPHERE:
                normal = normalize(ray_queue_pos[current_ray] - as_sphere(closest_obj)->position);                   // FIXME: just divide to sphere radius
                break ;
            case PLANE: normal = as_plane(closest_obj)->normal; break ;
            case TRIANGLE: normal = as_triangle(closest_obj)->normal; break ;
            case CONE: normal = normalize(param); break ;
            case CYLINDER: {
                const FLOAT3 op = ray_queue_pos[current_ray] - as_cylinder(closest_obj)->position;
                const FLOAT dt = dot(as_cylinder(closest_obj)->direction, op);
                normal = op - as_cylinder(closest_obj)->direction * dt;
                normal *= 1. / as_cylinder(closest_obj)->radius;
                break ;
            }
            case TOR: normal = normalize(param); break ;
        }

        FLOAT3 specular_val = BLACK;
        FLOAT3 factor = compute_lightning(
            scene,
            ray_queue_pos[current_ray],
            normal,
            ray_queue_dir[current_ray],
            get_obj_specular(closest_obj),
            &specular_val
        );
        FLOAT3 local_color = get_obj_color(closest_obj) * factor;
        local_color += specular_val * 255.;
        local_color.x = fmin(255. - EPS, local_color.x);
        local_color.y = fmin(255. - EPS, local_color.y);
        local_color.z = fmin(255. - EPS, local_color.z);
#ifdef RTX_EMISSION
        if (closest_type == SPHERE && as_sphere(closest_obj)->emission > 0.)
            local_color = get_obj_color(closest_obj);
#endif /* RTX_EMISSION */

        FLOAT refl = get_obj_reflective(closest_obj);
        FLOAT trans = get_obj_transparency(closest_obj);
        FLOAT refr = get_obj_refractive(closest_obj);
        if (current_ray >= RECURSION_DEPTH / 2 || (refl < EPS && trans < EPS)) {
            ray_queue_col[current_ray] += local_color * reflective_prod[current_ray];
        } else {
            reflective_prod[current_ray * 2] = reflective_prod[current_ray] * refl * (ONE - trans);
            reflective_prod[current_ray * 2 + 1] = reflective_prod[current_ray] * (ONE - refl) * trans;
            ray_queue_col[current_ray] += local_color * reflective_prod[current_ray] * (ONE - refl) * (ONE - trans);

            ray_queue_dir[current_ray * 2] = reflect_ray(-ray_queue_dir[current_ray], normal);
# ifdef RTX_REFRACTIVE
            ray_queue_dir[current_ray * 2 + 1] = refract_ray_slow(-ray_queue_dir[current_ray], normal, refr);
# else /* no RTX_REFRACTIVE */
            ray_queue_dir[current_ray * 2 + 1] = ray_queue_dir[current_ray];
# endif
            ray_queue_pos[current_ray * 2] = ray_queue_pos[current_ray];
            ray_queue_pos[current_ray * 2 + 1] = ray_queue_pos[current_ray];

        }
        if (trans > EPS)
            ray_enqueued[current_ray * 2 + 1] = 1;
        if (refl > EPS)
            ray_enqueued[current_ray * 2] = 1;
    }

    _color += ray_queue_col[1];
    for (uint32_t i=2; i < RECURSION_DEPTH; ++i) {
        _color += ray_queue_col[i];
    }

    _color.x = fmin(255. - EPS, _color.x);
    _color.y = fmin(255. - EPS, _color.y);
    _color.z = fmin(255. - EPS, _color.z);
    return _color;
}
#else /* no RTX_TRANSPARENCY */
# define RECURSION_DEPTH 4
CPP_UNUSED CPP_INLINE
FLOAT3 trace_ray(
        scene_ptr scene,
        FLOAT3 camera,
        FLOAT3 direction
)
{
    void_ptr    closest_obj;
    FLOAT3      color = BLACK;
    FLOAT       reflective_prod = 1;
    FLOAT       closest_t;
    obj_type_t  closest_type;
    FLOAT3      param;

    uint32_t recursion_depth = RECURSION_DEPTH;
    while (recursion_depth > 0)
    {
        --recursion_depth;
        closest_obj = closest_intersection(
                scene,
                camera,
                direction,
                EPS,
                INFINITY,
                &closest_t,
                &closest_type,
                &param
        );
# ifdef RTX_DIRECT
        color += compute_direct_lightning(scene, camera, direction, closest_t);
# endif /* RTX_DIRECT */
//        *distance = closest_t;
        if (closest_obj == NULLPTR)
            break ;


        camera += direction * closest_t;
        FLOAT3 normal;
        switch (closest_type)
        {
            case SPHERE:
                normal = normalize(camera - as_sphere(closest_obj)->position);
                break ;
            case PLANE: normal = as_plane(closest_obj)->normal; break ;
            case TRIANGLE: normal = as_triangle(closest_obj)->normal; break ;
            case CONE: normal = normalize(param); break ;
            case CYLINDER: {
                const FLOAT3 op = camera - as_cylinder(closest_obj)->position;
                const FLOAT dt = dot(as_cylinder(closest_obj)->direction, op);
                normal = op - as_cylinder(closest_obj)->direction * dt;
                normal *= 1. / as_cylinder(closest_obj)->radius;
                break ;
            }
            case TOR: normal = normalize(param); break ;
        }

        FLOAT3 specular_val = BLACK;
        FLOAT3 factor = compute_lightning(
                scene,
                camera,
                normal,
                direction,
                get_obj_specular(closest_obj),
                &specular_val
        );
        FLOAT3 local_color = get_obj_color(closest_obj) * factor;
        local_color += specular_val;
        local_color.x = fmin(255. - EPS, local_color.x);
        local_color.y = fmin(255. - EPS, local_color.y);
        local_color.z = fmin(255. - EPS, local_color.z);
# ifdef RTX_EMISSION
        if (closest_type == SPHERE && as_sphere(closest_obj)->emission > 0.)
            local_color = get_obj_color(closest_obj);
# endif /* RTX_EMISSION */

        if (recursion_depth == 0)
            color += local_color * reflective_prod; // * closest_sphere->reflective;
        else
            color += local_color
                     * (1.0 - get_obj_reflective(closest_obj)) * reflective_prod;
        reflective_prod *= get_obj_reflective(closest_obj);

        direction = reflect_ray(-direction, normal);
    }

    color.x = fmin(255. - EPS, color.x);
    color.y = fmin(255. - EPS, color.y);
    color.z = fmin(255. - EPS, color.z);
    return color;
}
#endif /* RTX_TRANSPARENCY */

// -----------------------------------------------------------------------------
CPP_UNUSED CPP_INLINE
__kernel void ray_tracer(
        __global uint32_t *canvas,

        byte_ptr figures,

        light_ptr lights,
        camera_ptr cameras,

        const uint32_t spheres_num,
        const uint32_t planes_num,
        const uint32_t triangles_num,
        const uint32_t cones_num,
        const uint32_t cylinders_num,
        const uint32_t torus_num,

        const uint32_t lights_num,
        const uint32_t cameras_num,

        const uint32_t width,
        const uint32_t height
    )
{

    const FLOAT rheight = 1. / height;
    const FLOAT rwidth = 1. / width;
    const uint32_t z = get_global_id(0);
    const uint32_t y = get_global_id(1);

    scene_t scene = {
        .lights = lights,
        .cameras = cameras,
        .spheres_num = spheres_num,
        .planes_num = planes_num,
        .triangles_num = triangles_num,
        .cones_num = cones_num,
        .cylinders_num = cylinders_num,
        .torus_num = torus_num,
        .lights_num = lights_num,
        .cameras_num = cameras_num
    };
    size_t shift = 0;

    scene.spheres = creinterpret_cast(sphere_ptr, figures + shift);
    shift += spheres_num * sizeof(sphere_t);

    scene.planes = creinterpret_cast(plane_ptr, figures + shift);
    shift += planes_num * sizeof(plane_t);

    scene.triangles = creinterpret_cast(triangle_ptr, figures + shift);
    shift += triangles_num * sizeof(triangle_t);

    scene.cones = creinterpret_cast(cone_ptr, figures + shift);
    shift += cones_num * sizeof(cone_t);

    scene.cylinders = creinterpret_cast(cylinder_ptr, figures + shift);
    shift += cylinders_num * sizeof(cylinder_t);

    scene.torus = creinterpret_cast(torus_ptr, figures + shift);
    shift += torus_num * sizeof(torus_ptr);

    FLOAT3 vec = ASSIGN_FLOAT3(
        (z - width / 2.) * rwidth,                                              // FIXME: open brakets (simplify)
        (y - height / 2.) * rheight,
        1
    );
    vec = normalize(vec);
    vec = rotate_vector(vec, cameras[0].rotate_matrix);
//    FLOAT distance;
    const FLOAT3 color = trace_ray(
        &scene,
        cameras[0].position,
        vec
    );

    uint32_t pix_pos;

    pix_pos = (height - y - 1u) * (width) + z;
    const uint32_t int_color =
          cstatic_cast(uint32_t, color.x) << 16u
        | cstatic_cast(uint32_t, color.y) << 8u
        | cstatic_cast(uint32_t, color.z);
    canvas[pix_pos] = int_color;
}

CPP_UNUSED CPP_INLINE
__kernel void blur_convolution(
    __global uint32_t *input,
    __global uint32_t *output,
    __global FLOAT *distances,

    const int32_t width,
    const int32_t height
)
{
    const int32_t z = cstatic_cast(int, get_global_id(0));
    const int32_t y = cstatic_cast(int, get_global_id(1));
    const int pix_pos = (height - y - 1) * (width) + z;

    FLOAT3 convolution_val = ASSIGN_FLOAT3(0., 0., 0.);
    int conv_size = cstatic_cast(int, fmin(distances[pix_pos], 20.));
    int conv_step = conv_size / 10;
    int cnt = 0;

    for (int i=-conv_size; i <= conv_size; i+=conv_step) {
        for (int j=-conv_size; j <= conv_size; j+=conv_step) {
            int column = z + j;
            int row = height - y - 1 + i;

            row = max(0, row);
            row = min(height - 1, row);
            column = max(0, column);
            column = min(width - 1, column);

            const int pos = row * width + column;
            convolution_val.z += input[pos] & 0xFF;
            convolution_val.y += (input[pos] >> 8u) & 0xFF;
            convolution_val.x += (input[pos] >> 16u) & 0xFF;
            ++cnt;
        }
    }
    convolution_val *= 1. / cnt;
    const uint32_t blur_val =
            cstatic_cast(uint32_t, convolution_val.x) << 16u
            | cstatic_cast(uint32_t, convolution_val.y) << 8u
            | cstatic_cast(uint32_t, convolution_val.z);
    output[pix_pos] = blur_val;
}


# ifdef __CPP
#  undef dot
#  undef cross
# endif /* __CPP */

# define __VERSION 8
#endif /* CL_TRACER */
