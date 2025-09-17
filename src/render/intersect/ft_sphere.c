#include "../../include/minirt.h"

// src/intersect/ft_sphere.c
int intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec)
{
    t_vec3  oc;
    double  a;
    double  b;
    double  c;
    double  discriminant;
    double  t;

    oc = vec3_sub(ray->origin, sp->center);
    a = vec3_dot(ray->direction, ray->direction);
    b = 2.0 * vec3_dot(oc, ray->direction);
    c = vec3_dot(oc, oc) - sp->radius * sp->radius;
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (0);
    t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t < EPSILON)
    {
        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (t < EPSILON)
            return (0);
    }
    rec->t = t;
    rec->point = ray_at(*ray, rec->t);
    rec->normal = vec3_normalize(vec3_sub(rec->point, sp->center));
    if (vec3_dot(ray->direction, rec->normal) > 0)
        rec->normal = vec3_mul(rec->normal, -1.0);
    return (1);
}