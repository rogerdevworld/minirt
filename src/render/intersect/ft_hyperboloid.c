#include "../../include/minirt.h"

// src/intersect/ft_hyperboloid.c
int intersect_hyperboloid(t_ray *ray, t_hyperboloid *hp, t_hit_record *rec)
{
    t_vec3  oc;
    double  a, b, c;
    double  t1, t2, discriminant;
    t_vec3  u_v, w_v;
    double  dot_d_axis, dot_oc_axis;

    oc = vec3_sub(ray->origin, hp->position);
    dot_d_axis = vec3_dot(ray->direction, hp->axis);
    dot_oc_axis = vec3_dot(oc, hp->axis);
    if (fabs(hp->axis.x) > 0.9)
        u_v = vec3_init(0, 1, 0);
    else
        u_v = vec3_init(1, 0, 0);
    u_v = vec3_normalize(vec3_cross(u_v, hp->axis));
    w_v = vec3_normalize(vec3_cross(hp->axis, u_v));
    double d_u = vec3_dot(ray->direction, u_v);
    double d_w = vec3_dot(ray->direction, w_v);
    double oc_u = vec3_dot(oc, u_v);
    double oc_w = vec3_dot(oc, w_v);
    double z_limit = hp->height / 2.0;
    a = (d_u * d_u) / (hp->radius_a * hp->radius_a) + (d_w * d_w) / (hp->radius_b * hp->radius_b) - (dot_d_axis * dot_d_axis);
    b = 2.0 * ((d_u * oc_u) / (hp->radius_a * hp->radius_a) + (d_w * oc_w) / (hp->radius_b * hp->radius_b) - (dot_d_axis * dot_oc_axis));
    c = (oc_u * oc_u) / (hp->radius_a * hp->radius_a) + (oc_w * oc_w) / (hp->radius_b * hp->radius_b) - (dot_oc_axis * dot_oc_axis);
    discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0)
        return (0);
    t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    rec->t = -1.0;
    if (t1 > EPSILON)
    {
        double m1 = vec3_dot(vec3_sub(ray_at(*ray, t1), hp->position), hp->axis);
        if (fabs(m1) <= z_limit)
            rec->t = t1;
    }
    if (t2 > EPSILON)
    {
        double m2 = vec3_dot(vec3_sub(ray_at(*ray, t2), hp->position), hp->axis);
        if (fabs(m2) <= z_limit && (rec->t < 0 || t2 < rec->t))
            rec->t = t2;
    }
    if (rec->t < 0)
        return (0);
    rec->point = ray_at(*ray, rec->t);
    t_vec3 point_relative = vec3_sub(rec->point, hp->position);
    double x_n = (2.0 * vec3_dot(point_relative, u_v)) / (hp->radius_a * hp->radius_a);
    double y_n = (2.0 * vec3_dot(point_relative, w_v)) / (hp->radius_b * hp->radius_b);
    double z_n = -2.0 * vec3_dot(point_relative, hp->axis);
    rec->normal = vec3_normalize(vec3_add(vec3_add(vec3_mul(u_v, x_n), vec3_mul(w_v, y_n)), vec3_mul(hp->axis, z_n)));
    if (vec3_dot(rec->normal, ray->direction) > 0)
        rec->normal = vec3_mul(rec->normal, -1);
    return (1);
}
