/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cylinder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

static int	intersect_sides(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
	t_vec3			oc;
	t_vec3			coeffs;
	t_hit_params	params;
	double			discriminant;
	double			t_side;

	oc = vec3_sub(ray->origin, cy->position);
	coeffs = calculate_quadratic_coeffs(ray, oc, cy);
	discriminant = coeffs.y * coeffs.y - 4 * coeffs.x * coeffs.z;
	if (discriminant < 0)
		return (0);
	params.ray = ray;
	params.cy = cy;
	params.oc = oc;
	t_side = (-coeffs.y - sqrt(discriminant)) / (2.0 * coeffs.x);
	if (check_hit_and_update_rec(&params, rec, t_side))
		return (1);
	t_side = (-coeffs.y + sqrt(discriminant)) / (2.0 * coeffs.x);
	if (check_hit_and_update_rec(&params, rec, t_side))
		return (1);
	return (0);
}

static double	calculate_cap_intersection(t_ray *ray, t_cylinder *cy,
		t_vec3 cap_center)
{
	double	denom;
	double	t;
	t_vec3	p;

	denom = vec3_dot(ray->direction, cy->axis);
	if (fabs(denom) < EPSILON)
		return (-1.0);
	t = vec3_dot(vec3_sub(cap_center, ray->origin), cy->axis) / denom;
	if (t < EPSILON)
		return (-1.0);
	p = ray_at(*ray, t);
	if (vec3_length_squared(vec3_sub(p, cap_center)) > cy->radius * cy->radius)
		return (-1.0);
	return (t);
}

static int	intersect_caps(t_ray *ray, t_cylinder *cy, double *t_cap,
		t_hit_record *rec)
{
	t_vec3	top_center;
	t_vec3	bottom_center;
	double	t_top;
	double	t_bottom;

	top_center = vec3_add(cy->position, vec3_mul(cy->axis, cy->height / 2.0));
	t_top = calculate_cap_intersection(ray, cy, top_center);
	bottom_center = vec3_add(cy->position, vec3_mul(cy->axis, -cy->height
				/ 2.0));
	t_bottom = calculate_cap_intersection(ray, cy, bottom_center);
	*t_cap = -1.0;
	if (t_top > EPSILON && (t_bottom < EPSILON || t_top < t_bottom))
		*t_cap = t_top;
	else if (t_bottom > EPSILON)
		*t_cap = t_bottom;
	if (*t_cap > EPSILON)
	{
		rec->point = ray_at(*ray, *t_cap);
		if (vec3_dot(ray->direction, cy->axis) > 0)
			rec->normal = vec3_mul(cy->axis, -1.0);
		else
			rec->normal = cy->axis;
		return (1);
	}
	return (0);
}

static void	select_closest_hit(t_hit_record *rec, t_closest_hit_data hit_data)
{
	if (hit_data.side_found && hit_data.cap_found)
	{
		if (hit_data.side_hit.t < hit_data.cap_hit.t)
			*rec = hit_data.side_hit;
		else
			*rec = hit_data.cap_hit;
	}
	else if (hit_data.side_found)
		*rec = hit_data.side_hit;
	else if (hit_data.cap_found)
		*rec = hit_data.cap_hit;
}

int	intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
	t_closest_hit_data	hit_data;

	hit_data.side_found = intersect_sides(ray, cy, &hit_data.side_hit);
	hit_data.cap_found = intersect_caps(ray, cy, &hit_data.cap_hit.t,
			&hit_data.cap_hit);
	if (!hit_data.side_found && !hit_data.cap_found)
		return (0);
	select_closest_hit(rec, hit_data);
	return (1);
}

// // src/intersect/ft_cylinder.c
// static int	intersect_caps(t_ray *ray, t_cylinder *cy, double *t_cap,
// 		t_hit_record *rec)
// {
// 	double	denom;

// 	double t_top, t_bottom;
// 	t_vec3 p_top, p_bottom;
// 	denom = vec3_dot(ray->direction, cy->axis);
// 	if (fabs(denom) < EPSILON)
// 		t_top = -1.0;
// 	else
// 		t_top = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis,
// 							cy->height / 2.0)), ray->origin), cy->axis) / denom;
// 	p_top = ray_at(*ray, t_top);
// 	if (t_top > EPSILON && vec3_length_squared(vec3_sub(p_top,
// 				vec3_add(cy->position, vec3_mul(cy->axis, cy->height
// 						/ 2.0)))) <= cy->radius * cy->radius)
// 		*t_cap = t_top;
// 	else
// 		*t_cap = -1.0;
// 	if (fabs(denom) < EPSILON)
// 		t_bottom = -1.0;
// 	else
// 		t_bottom = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis,
// 							-cy->height / 2.0)), ray->origin), cy->axis)
// 			/ denom;
// 	p_bottom = ray_at(*ray, t_bottom);
// 	if (t_bottom > EPSILON && vec3_length_squared(vec3_sub(p_bottom,
// 				vec3_add(cy->position, vec3_mul(cy->axis, -cy->height
// 						/ 2.0)))) <= cy->radius * cy->radius)
// 	{
// 		if (*t_cap < 0 || t_bottom < *t_cap)
// 			*t_cap = t_bottom;
// 	}
// 	if (*t_cap > EPSILON)
// 	{
// 		rec->point = ray_at(*ray, *t_cap);
// 		if (denom > 0)
// 			rec->normal = vec3_mul(cy->axis, -1.0);
// 		else
// 			rec->normal = cy->axis;
// 		return (1);
// 	}
// 	return (0);
// }

// // src/intersect/ft_cylinder.c
// static int	intersect_sides(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
// {
// 	double	discriminant;
// 	double	t_side;
// 	t_vec3	oc;
// 	double	m;
// 	double	m;

// 	double a, b, c;
// 	oc = vec3_sub(ray->origin, cy->position);
// 	a = vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis,
// 					vec3_dot(ray->direction, cy->axis))),
// 			vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction,
// 						cy->axis))));
// 	b = 2.0 * vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis,
// 					vec3_dot(ray->direction, cy->axis))), vec3_sub(oc,
// 				vec3_mul(cy->axis, vec3_dot(oc, cy->axis))));
// 	c = vec3_dot(vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))),
// 			vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))))
// 		- (cy->radius * cy->radius);
// 	discriminant = b * b - 4 * a * c;
// 	if (discriminant < 0)
// 		return (0);
// 	t_side = (-b - sqrt(discriminant)) / (2.0 * a);
// 	if (t_side > EPSILON)
// 	{
// 		m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc,
// 				cy->axis);
// 		if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
// 		{
// 			rec->t = t_side;
// 			rec->point = ray_at(*ray, rec->t);
// 			rec->normal = vec3_normalize(vec3_sub(rec->point,
// 						vec3_add(cy->position, vec3_mul(cy->axis, m))));
// 			return (1);
// 		}
// 	}
// 	t_side = (-b + sqrt(discriminant)) / (2.0 * a);
// 	if (t_side > EPSILON)
// 	{
// 		m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc,
// 				cy->axis);
// 		if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
// 		{
// 			rec->t = t_side;
// 			rec->point = ray_at(*ray, rec->t);
// 			rec->normal = vec3_normalize(vec3_sub(rec->point,
// 						vec3_add(cy->position, vec3_mul(cy->axis, m))));
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// // src/intersect/ft_cylinder.c
// int	intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
// {
// 	t_hit_record	side_hit;
// 	t_hit_record	cap_hit;
// 	int				side_found;
// 	int				cap_found;

// 	side_found = intersect_sides(ray, cy, &side_hit);
// 	cap_found = intersect_caps(ray, cy, &cap_hit.t, &cap_hit);
// 	if (side_found && cap_found)
// 	{
// 		if (side_hit.t < cap_hit.t)
// 			*rec = side_hit;
// 		else
// 			*rec = cap_hit;
// 		return (1);
// 	}
// 	else if (side_found)
// 	{
// 		*rec = side_hit;
// 		return (1);
// 	}
// 	else if (cap_found)
// 	{
// 		*rec = cap_hit;
// 		return (1);
// 	}
// 	return (0);
// }
