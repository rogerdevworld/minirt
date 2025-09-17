#include "../../include/minirt.h"

// src/intersect/ft_paraboloid.c
int intersect_paraboloid(t_ray *ray, t_parab *pb, t_hit_record *rec)
{
    t_vec3  oc;
    double  a, b, c;
    double  discriminant;
    double  t1, t2, t_final;

    oc = vec3_sub(ray->origin, pb->position);
    t_vec3 axis_norm = vec3_normalize(pb->axis);
    double dot_d_axis = vec3_dot(ray->direction, axis_norm);
    double dot_oc_axis = vec3_dot(oc, axis_norm);
    t_vec3 d_proj = vec3_mul(axis_norm, dot_d_axis);
    t_vec3 oc_proj = vec3_mul(axis_norm, dot_oc_axis);
    t_vec3 d_perp = vec3_sub(ray->direction, d_proj);
    t_vec3 oc_perp = vec3_sub(oc, oc_proj);
    a = vec3_dot(d_perp, d_perp);
    b = 2.0 * vec3_dot(d_perp, oc_perp) - 4.0 * pb->focal_lenght * dot_d_axis;
    c = vec3_dot(oc_perp, oc_perp) - 4.0 * pb->focal_lenght * dot_oc_axis;
    discriminant = b * b - 4 * a * c;
    if (discriminant < EPSILON)
        return (0);
    t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    t_final = -1.0;
    if (t1 > EPSILON)
    {
        double height = dot_d_axis * t1 + dot_oc_axis;
        if (height >= 0.0 && height <= pb->height)
            t_final = t1;
    }
    if (t2 > EPSILON)
    {
        double height = dot_d_axis * t2 + dot_oc_axis;
        if (height >= 0.0 && height <= pb->height)
        {
            if (t_final < 0 || t2 < t_final)
                t_final = t2;
        }
    }
    if (t_final < 0)
        return (0);
    rec->t = t_final;
    rec->point = ray_at(*ray, rec->t);
    t_vec3 hit_point_local = vec3_sub(rec->point, pb->position);
    double m = vec3_dot(hit_point_local, axis_norm);
    t_vec3 normal_unnorm = vec3_sub(vec3_sub(hit_point_local, vec3_mul(axis_norm, m)), vec3_mul(axis_norm, 2.0 * pb->focal_lenght));
    rec->normal = vec3_normalize(normal_unnorm);
    if (vec3_dot(rec->normal, ray->direction) > 0)
        rec->normal = vec3_mul(rec->normal, -1.0);
    return (1);
}