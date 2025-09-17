#include "../../include/minirt.h"

// src/intersect/ft_cone.c
int intersect_cone(t_ray *ray, t_cone *cn, t_hit_record *rec)
{
    t_vec3  oc = vec3_sub(ray->origin, cn->position);
    double  cone_cos2 = pow(cn->radius / (cn->height / 2.0), 2.0);
    double a = vec3_dot(ray->direction, ray->direction) - (1 + cone_cos2) * pow(vec3_dot(ray->direction, cn->axis), 2);
    double b = 2.0 * (vec3_dot(ray->direction, oc) - (1 + cone_cos2) * vec3_dot(ray->direction, cn->axis) * vec3_dot(oc, cn->axis));
    double c = vec3_dot(oc, oc) - (1 + cone_cos2) * pow(vec3_dot(oc, cn->axis), 2);
    double discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0)
        return (0);
    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    double m1 = vec3_dot(ray->direction, cn->axis) * t1 + vec3_dot(oc, cn->axis);
    double m2 = vec3_dot(ray->direction, cn->axis) * t2 + vec3_dot(oc, cn->axis);
    rec->t = -1.0;
    if (t1 > EPSILON && m1 > 0 && m1 < cn->height)
        rec->t = t1;
    if (t2 > EPSILON && m2 > 0 && m2 < cn->height)
    {
        if (rec->t < 0 || t2 < rec->t)
            rec->t = t2;
    }
    if (rec->t > EPSILON)
    {
        double m = vec3_dot(ray->direction, cn->axis) * rec->t + vec3_dot(oc, cn->axis);
        rec->point = ray_at(*ray, rec->t);
        rec->normal = vec3_normalize(vec3_sub(vec3_sub(rec->point, cn->position), vec3_mul(cn->axis, m * (1 + cone_cos2))));
        if (vec3_dot(rec->normal, ray->direction) > 0)
            rec->normal = vec3_mul(rec->normal, -1);
        return (1);
    }
    return (0);
}
