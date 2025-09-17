#include "../../include/minirt.h"

// src/intersect/ft_cylinder.c
static int intersect_caps(t_ray *ray, t_cylinder *cy, double *t_cap, t_hit_record *rec)
{
    double  t_top, t_bottom;
    t_vec3  p_top, p_bottom;

    double denom = vec3_dot(ray->direction, cy->axis);
    if (fabs(denom) < EPSILON)
        t_top = -1.0;
    else
        t_top = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis, cy->height / 2.0)), ray->origin), cy->axis) / denom;
    p_top = ray_at(*ray, t_top);
    if (t_top > EPSILON && vec3_length_squared(vec3_sub(p_top, vec3_add(cy->position, vec3_mul(cy->axis, cy->height / 2.0)))) <= cy->radius * cy->radius)
        *t_cap = t_top;
    else
        *t_cap = -1.0;
    if (fabs(denom) < EPSILON)
        t_bottom = -1.0;
    else
        t_bottom = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis, -cy->height / 2.0)), ray->origin), cy->axis) / denom;
    p_bottom = ray_at(*ray, t_bottom);
    if (t_bottom > EPSILON && vec3_length_squared(vec3_sub(p_bottom, vec3_add(cy->position, vec3_mul(cy->axis, -cy->height / 2.0)))) <= cy->radius * cy->radius)
    {
        if (*t_cap < 0 || t_bottom < *t_cap)
            *t_cap = t_bottom;
    }
    if (*t_cap > EPSILON)
    {
        rec->point = ray_at(*ray, *t_cap);
        if (denom > 0)
            rec->normal = vec3_mul(cy->axis, -1.0);
        else
            rec->normal = cy->axis;
        return (1);
    }
    return (0);
}

// src/intersect/ft_cylinder.c
static int intersect_sides(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
    double  a, b, c;
    double  discriminant;
    double  t_side;
    t_vec3  oc = vec3_sub(ray->origin, cy->position);
    
    a = vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction, cy->axis))), vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction, cy->axis))));
    b = 2.0 * vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction, cy->axis))), vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))));
    c = vec3_dot(vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))), vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis)))) - (cy->radius * cy->radius);
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (0);
    t_side = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t_side > EPSILON)
    {
        double m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc, cy->axis);
        if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
        {
            rec->t = t_side;
            rec->point = ray_at(*ray, rec->t);
            rec->normal = vec3_normalize(vec3_sub(rec->point, vec3_add(cy->position, vec3_mul(cy->axis, m))));
            return (1);
        }
    }
    t_side = (-b + sqrt(discriminant)) / (2.0 * a);
    if (t_side > EPSILON)
    {
        double m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc, cy->axis);
        if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
        {
            rec->t = t_side;
            rec->point = ray_at(*ray, rec->t);
            rec->normal = vec3_normalize(vec3_sub(rec->point, vec3_add(cy->position, vec3_mul(cy->axis, m))));
            return (1);
        }
    }
    return (0);
}

// src/intersect/ft_cylinder.c
int intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
    t_hit_record side_hit;
    t_hit_record cap_hit;
    int side_found = intersect_sides(ray, cy, &side_hit);
    int cap_found = intersect_caps(ray, cy, &cap_hit.t, &cap_hit);
    
    if (side_found && cap_found)
    {
        if (side_hit.t < cap_hit.t)
            *rec = side_hit;
        else
            *rec = cap_hit;
        return (1);
    }
    else if (side_found)
    {
        *rec = side_hit;
        return (1);
    }
    else if (cap_found)
    {
        *rec = cap_hit;
        return (1);
    }
    return (0);
}
