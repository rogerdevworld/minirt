/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hyperboloid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

static double	get_closest_t(t_ray *ray, t_hyperboloid *hp, double t1,
		double t2)
{
	double	m1;
	double	m2;
	double	closest_t;
	double	z_limit;

	closest_t = -1.0;
	z_limit = hp->height / 2.0;
	if (t1 > EPSILON)
	{
		m1 = vec3_dot(vec3_sub(ray_at(*ray, t1), hp->position), hp->axis);
		if (fabs(m1) <= z_limit)
			closest_t = t1;
	}
	if (t2 > EPSILON && (closest_t < 0 || t2 < closest_t))
	{
		m2 = vec3_dot(vec3_sub(ray_at(*ray, t2), hp->position), hp->axis);
		if (fabs(m2) <= z_limit)
			closest_t = t2;
	}
	return (closest_t);
}

static t_vec3	calculate_coeffs(t_ray *ray, t_hyperboloid *hp, t_vec3 oc)
{
	t_vec3	coeffs;
	t_vec3	u_v;
	t_vec3	w_v;

	if (fabs(hp->axis.x) > 0.9)
		u_v = vec3_init(0, 1, 0);
	else
		u_v = vec3_init(1, 0, 0);
	u_v = vec3_normalize(vec3_cross(u_v, hp->axis));
	w_v = vec3_normalize(vec3_cross(hp->axis, u_v));
	coeffs.x = (vec3_dot(ray->direction, u_v) * vec3_dot(ray->direction, u_v))
		/ (hp->radius_a * hp->radius_a) + (vec3_dot(ray->direction, w_v)
			* vec3_dot(ray->direction, w_v)) / (hp->radius_b * hp->radius_b)
		- (vec3_dot(ray->direction, hp->axis) * vec3_dot(ray->direction,
				hp->axis));
	coeffs.y = 2.0 * ((vec3_dot(ray->direction, u_v) * vec3_dot(oc, u_v))
			/ (hp->radius_a * hp->radius_a) + (vec3_dot(ray->direction, w_v)
				* vec3_dot(oc, w_v)) / (hp->radius_b * hp->radius_b)
			- (vec3_dot(ray->direction, hp->axis) * vec3_dot(oc, hp->axis)));
	coeffs.z = (vec3_dot(oc, u_v) * vec3_dot(oc, u_v)) / (hp->radius_a
			* hp->radius_a) + (vec3_dot(oc, w_v) * vec3_dot(oc, w_v))
		/ (hp->radius_b * hp->radius_b) - (vec3_dot(oc, hp->axis) * vec3_dot(oc,
				hp->axis));
	return (coeffs);
}

static void	update_normal(t_ray *ray, t_hyperboloid *hp, t_hit_record *rec)
{
	t_vec3	point_relative;
	t_vec3	u_v;
	t_vec3	w_v;

	point_relative = vec3_sub(rec->point, hp->position);
	if (fabs(hp->axis.x) > 0.9)
		u_v = vec3_init(0, 1, 0);
	else
		u_v = vec3_init(1, 0, 0);
	u_v = vec3_normalize(vec3_cross(u_v, hp->axis));
	w_v = vec3_normalize(vec3_cross(hp->axis, u_v));
	rec->normal = vec3_normalize(vec3_add(vec3_add(vec3_mul(u_v, (2.0
							* vec3_dot(point_relative, u_v)) / (hp->radius_a
							* hp->radius_a)), vec3_mul(w_v, (2.0
							* vec3_dot(point_relative, w_v)) / (hp->radius_b
							* hp->radius_b))), vec3_mul(hp->axis, -2.0
					* vec3_dot(point_relative, hp->axis))));
	if (vec3_dot(rec->normal, ray->direction) > 0)
		rec->normal = vec3_mul(rec->normal, -1);
}

int	intersect_hyperboloid(t_ray *ray, t_hyperboloid *hp, t_hit_record *rec)
{
	t_vec3	oc;
	t_vec3	coeffs;
	double	discriminant;
	double	t;

	oc = vec3_sub(ray->origin, hp->position);
	coeffs = calculate_coeffs(ray, hp, oc);
	discriminant = coeffs.y * coeffs.y - 4.0 * coeffs.x * coeffs.z;
	if (discriminant < 0.0)
		return (0);
	t = get_closest_t(ray, hp, (-coeffs.y - sqrt(discriminant)) / (2.0
				* coeffs.x), (-coeffs.y + sqrt(discriminant)) / (2.0
				* coeffs.x));
	if (t < 0)
		return (0);
	rec->t = t;
	rec->point = ray_at(*ray, rec->t);
	update_normal(ray, hp, rec);
	return (1);
}
