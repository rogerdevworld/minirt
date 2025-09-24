/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cone.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Calculates the coefficients of the quadratic equation 
 for ray-cone intersection.
 *
 * @details This function determines the quadratic coefficients 
 (A, B, C) required to solve for the
 * intersection of a ray with a cone. It uses a parametric equation 
 derived from the cone's
 * geometry. The coefficients are based on the ray's origin and 
 direction, the cone's
 * position and axis, and its radius and height.
 *
 * @param ray A pointer to the ray being cast.
 * @param cn A pointer to the cone object.
 *
 * @return A `t_cone_params` structure containing the calculated 
 coefficients.
 */
t_cone_params	get_cone_quadratic(t_ray *ray, t_cone *cn)
{
	t_cone_params	p;
	double			cone_cos2;

	p.oc = vec3_sub(ray->origin, cn->position);
	cone_cos2 = pow(cn->radius / (cn->height / 2.0), 2.0);
	p.a = vec3_dot(ray->direction, ray->direction) - (1 + cone_cos2)
		* pow(vec3_dot(ray->direction, cn->axis), 2);
	p.b = 2.0 * (vec3_dot(ray->direction, p.oc) - (1 + cone_cos2)
			* vec3_dot(ray->direction, cn->axis) * vec3_dot(p.oc, cn->axis));
	p.c = vec3_dot(p.oc, p.oc) - (1 + cone_cos2) * pow(vec3_dot(p.oc, cn->axis),
			2);
	return (p);
}

/**
 * @brief Solves the quadratic equation for a ray-cone intersection.
 *
 * @details This function calculates the two possible intersection 
 distances (t1 and t2)
 * using the quadratic formula. It then validates these distances 
 against the cone's
 * height to ensure the intersection point lies on the finite part 
 of the cone's surface.
 * It returns the smallest valid positive distance, or -1.0 if no 
 valid intersection is found.
 *
 * @param ray A pointer to the ray.
 * @param cn A pointer to the cone.
 * @param p The quadratic parameters (a, b, c) calculated by 
 `get_cone_quadratic`.
 *
 * @return The smallest positive intersection distance (t) or 
 -1.0 if there is no intersection.
 */
double	get_cone_t(t_ray *ray, t_cone *cn, t_cone_params p)
{
	double	disc;
	double	t1;
	double	t2;
	double	m1;
	double	m2;

	disc = p.b * p.b - 4.0 * p.a * p.c;
	if (disc < 0.0)
		return (-1.0);
	t1 = (-p.b - sqrt(disc)) / (2.0 * p.a);
	t2 = (-p.b + sqrt(disc)) / (2.0 * p.a);
	m1 = vec3_dot(ray->direction, cn->axis) * t1 + vec3_dot(p.oc, cn->axis);
	m2 = vec3_dot(ray->direction, cn->axis) * t2 + vec3_dot(p.oc, cn->axis);
	if (t1 > EPSILON && m1 > 0 && m1 < cn->height)
	{
		if (t2 > EPSILON && m2 > 0 && m2 < cn->height)
			return (fmin(t1, t2));
		return (t1);
	}
	if (t2 > EPSILON && m2 > 0 && m2 < cn->height)
		return (t2);
	return (-1.0);
}

/**
 * @brief Determines if a ray intersects a cone and records the hit.
 *
 * @details This is the primary function for ray-cone intersection. 
 It first calculates
 * the quadratic coefficients using `get_cone_quadratic`, then solves for the
 * intersection distance `t` using `get_cone_t`. If a valid 
 intersection is found,
 * it populates the `t_hit_record` struct with the intersection distance, the
 * exact hit point, and the surface normal at that point. It ensures the normal
 * is correctly oriented relative to the ray's direction for accurate shading.
 *
 * @param ray A pointer to the ray.
 * @param cn A pointer to the cone object.
 * @param rec A pointer to the `t_hit_record` structure to store 
 the intersection details.
 *
 * @return 1 if an intersection is found, 0 otherwise.
 */
int	intersect_cone(t_ray *ray, t_cone *cn, t_hit_record *rec)
{
	t_cone_params	p;
	double			t;
	double			m;

	p = get_cone_quadratic(ray, cn);
	if (p.a == 0 && p.b == 0)
		return (0);
	t = get_cone_t(ray, cn, p);
	if (t > EPSILON)
	{
		m = vec3_dot(ray->direction, cn->axis) * t + vec3_dot(p.oc, cn->axis);
		rec->t = t;
		rec->point = ray_at(*ray, rec->t);
		rec->normal = vec3_normalize(vec3_sub(vec3_sub(rec->point,
						cn->position), vec3_mul(cn->axis, m)));
		if (vec3_dot(rec->normal, ray->direction) > 0)
			rec->normal = vec3_mul(rec->normal, -1.0);
		return (1);
	}
	return (0);
}

// int intersect_cone(t_ray *ray, t_cone *cn, t_hit_record *rec)
// {
//     t_vec3  oc = vec3_sub(ray->origin, cn->position);
//     double  cone_cos2 = pow(cn->radius / (cn->height / 2.0), 2.0);
//     double a = vec3_dot(ray->direction, ray->direction) - (1 + cone_cos2)
// * pow(vec3_dot(ray->direction, cn->axis), 2);
//     double b = 2.0 * (vec3_dot(ray->direction, oc) - (1 + cone_cos2)
// * vec3_dot(ray->direction, cn->axis) * vec3_dot(oc, cn->axis));
//     double c = vec3_dot(oc, oc) - (1 + cone_cos2) * pow(vec3_dot(oc,
// cn->axis), 2);
//     double discriminant = b * b - 4.0 * a * c;
//     if (discriminant < 0.0)
//         return (0);
//     double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
//     double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
//     double m1 = vec3_dot(ray->direction, cn->axis) * t1 + vec3_dot(oc,
// cn->axis);
//     double m2 = vec3_dot(ray->direction, cn->axis) * t2 + vec3_dot(oc,
// cn->axis);
//     rec->t = -1.0;
//     if (t1 > EPSILON && m1 > 0 && m1 < cn->height)
//         rec->t = t1;
//     if (t2 > EPSILON && m2 > 0 && m2 < cn->height)
//     {
//         if (rec->t < 0 || t2 < rec->t)
//             rec->t = t2;
//     }
//     if (rec->t > EPSILON)
//     {
//         double m = vec3_dot(ray->direction, cn->axis) * rec->t + vec3_dot(oc,
// cn->axis);
//         rec->point = ray_at(*ray, rec->t);
//         rec->normal = vec3_normalize(vec3_sub(vec3_sub(rec->point,
// cn->position), vec3_mul(cn->axis, m * (1 + cone_cos2))));
//         if (vec3_dot(rec->normal, ray->direction) > 0)
//             rec->normal = vec3_mul(rec->normal, -1);
//         return (1);
//     }
//     return (0);
// }