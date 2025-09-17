#include "../../include/minirt.h"

// src/intersect/ft_paraboloid_utils.c
static void	calculate_abc(t_ray *ray, t_parab *pb, t_vec3 oc, double *a,
		double *b, double *c)
{
	t_vec3	axis_norm;
	t_vec3	d_perp;
	t_vec3	oc_perp;

	axis_norm = vec3_normalize(pb->axis);
	d_perp = vec3_sub(ray->direction, vec3_mul(axis_norm,
				vec3_dot(ray->direction, axis_norm)));
	oc_perp = vec3_sub(oc, vec3_mul(axis_norm, vec3_dot(oc, axis_norm)));
	*a = vec3_dot(d_perp, d_perp);
	*b = 2.0 * vec3_dot(d_perp, oc_perp) - 4.0 * pb->focal_lenght
		* vec3_dot(ray->direction, axis_norm);
	*c = vec3_dot(oc_perp, oc_perp) - 4.0 * pb->focal_lenght * vec3_dot(oc,
			axis_norm);
}

static double	solve_quadratic(double a, double b, double c, t_ray *ray,
		t_parab *pb)
{
	double	discriminant;
	double	height;

	discriminant = b * b - 4 * a * c;
	if (discriminant < EPSILON)
		return (-1.0);
	if ((-b - sqrt(discriminant)) / (2.0 * a) > EPSILON)
	{
		height = vec3_dot(ray->direction, vec3_normalize(pb->axis)) * (-b
				- sqrt(discriminant)) / (2.0 * a)
			+ vec3_dot(vec3_sub(ray->origin, pb->position),
				vec3_normalize(pb->axis));
		if (height >= 0.0 && height <= pb->height)
			return (-b - sqrt(discriminant)) / (2.0 * a);
	}
	if ((-b - sqrt(discriminant)) / (2.0 * a) > EPSILON)
	{
		height = vec3_dot(ray->direction, vec3_normalize(pb->axis)) * (-b
				- sqrt(discriminant)) / (2.0 * a)
			+ vec3_dot(vec3_sub(ray->origin, pb->position),
				vec3_normalize(pb->axis));
		if (height >= 0.0 && height <= pb->height)
			return (-b - sqrt(discriminant)) / (2.0 * a);
	}
	return (-1.0);
}

static void	update_paraboloid_normal(t_ray *ray, t_parab *pb, t_hit_record *rec)
{
	t_vec3	hit_point_local;
	t_vec3	normal_unnorm;
	t_vec3	axis_norm;

	hit_point_local = vec3_sub(rec->point, pb->position);
	axis_norm = vec3_normalize(pb->axis);
	normal_unnorm = vec3_sub(vec3_sub(hit_point_local, vec3_mul(axis_norm,
					vec3_dot(hit_point_local, axis_norm))), vec3_mul(axis_norm,
				2.0 * pb->focal_lenght));
	rec->normal = vec3_normalize(normal_unnorm);
	if (vec3_dot(rec->normal, ray->direction) > 0)
		rec->normal = vec3_mul(rec->normal, -1.0);
}

int	intersect_paraboloid(t_ray *ray, t_parab *pb, t_hit_record *rec)
{
	t_vec3	oc;
	double	t_final;

	double a, b, c;
	oc = vec3_sub(ray->origin, pb->position);
	calculate_abc(ray, pb, oc, &a, &b, &c);
	t_final = solve_quadratic(a, b, c, ray, pb);
	if (t_final < 0)
		return (0);
	rec->t = t_final;
	rec->point = ray_at(*ray, rec->t);
	update_paraboloid_normal(ray, pb, rec);
	return (1);
}
