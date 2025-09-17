#include "../../include/minirt.h"

// src/intersect/ft_sphere.c
static double	get_discriminant(t_ray *ray, t_vec3 oc, double radius)
{
	double	b;
	double	c;
	double	discriminant;

	b = 2.0 * vec3_dot(oc, ray->direction);
	c = vec3_dot(oc, oc) - radius * radius;
	discriminant = b * b - 4 * vec3_dot(ray->direction, ray->direction) * c;
	if (discriminant < 0)
		return (-1.0);
	return (discriminant);
}

static double	get_t_value(t_ray *ray, t_vec3 oc, double disc)
{
	double	t;
	double	a;
	double	b;

	a = vec3_dot(ray->direction, ray->direction);
	b = 2.0 * vec3_dot(oc, ray->direction);
	t = (-b - sqrt(disc)) / (2.0 * a);
	if (t < EPSILON)
	{
		t = (-b + sqrt(disc)) / (2.0 * a);
		if (t < EPSILON)
			return (-1.0);
	}
	return (t);
}

static void	update_hit_record(t_ray *ray, t_sphere *sp, t_hit_record *rec,
		double t)
{
	t_vec3	point;
	t_vec3	normal;

	rec->t = t;
	point = ray_at(*ray, rec->t);
	rec->point = point;
	normal = vec3_normalize(vec3_sub(point, sp->center));
	// Ajusta la normal si es necesario
	if (vec3_dot(ray->direction, normal) > 0)
		rec->normal = vec3_mul(normal, -1.0);
	else
		rec->normal = normal;
}

// src/intersect/ft_sphere.c
int	intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec)
{
	t_vec3	oc;
	double	discriminant;
	double	t;

	oc = vec3_sub(ray->origin, sp->center);
	discriminant = get_discriminant(ray, oc, sp->radius);
	if (discriminant < 0)
		return (0);
	t = get_t_value(ray, oc, discriminant);
	if (t < 0)
		return (0);
	update_hit_record(ray, sp, rec, t);
	return (1);
}
