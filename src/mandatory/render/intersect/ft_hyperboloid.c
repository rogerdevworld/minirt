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

/**
 * @brief Finds the closest valid intersection distance (t) for a hyperboloid.
 *
 * @details This function takes two potential intersection distances (t1, t2)
 * and determines which, if any, is the closest valid hit. A hit is considered
 * valid if its distance `t` is positive (greater than `EPSILON`) and if the
 * corresponding hit point lies within the finite height (`height/2`) of the
 * hyperboloid along its axis.
 *
 * @param ray A pointer to the ray.
 * @param hp A pointer to the hyperboloid object.
 * @param t1 The first potential intersection distance.
 * @param t2 The second potential intersection distance.
 *
 * @return The closest positive `t` value that is within the hyperboloid's
 * height, or -1.0 if no valid hit is found.
 */
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

/**
 * @brief Calculates the quadratic coefficients for ray-hyperboloid 
 intersection.
 *
 * @details This function determines the A, B, and C coefficients of the
 * quadratic equation for a ray's intersection with a hyperboloid. It first
 * establishes a local orthonormal basis (u_v, w_v, hp->axis) for the
 * hyperboloid. The coefficients are then calculated based on the ray's
 * properties and the hyperboloid's parameters relative to this local basis.
 * The resulting equation is used to solve for the intersection distance.
 *
 * @param ray A pointer to the ray.
 * @param hp A pointer to the hyperboloid object.
 * @param oc The vector from the hyperboloid's center to the ray's origin.
 *
 * @return A `t_vec3` where the components correspond to the A, B, and C
 * coefficients of the quadratic equation.
 */
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

/**
 * @brief Calculates and sets the surface normal at a hit point 
 on a hyperboloid.
 *
 * @details This function determines the normal vector at a given intersection
 * point. The normal is calculated using the gradient of the hyperboloid's
 * implicit function. It first establishes a local orthonormal basis, then
 * uses the dot products of the relative hit point with these basis vectors
 * to compute the normal. The final normal is normalized and oriented to face
 * away from the ray's origin.
 *
 * @param ray A pointer to the ray that hit the hyperboloid.
 * @param hp A pointer to the hyperboloid object.
 * @param rec A pointer to the hit record to be updated.
 *
 * @return void.
 */
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

/**
 * @brief Determines if a ray intersects a hyperboloid and records the hit.
 *
 * @details This is the main function for ray-hyperboloid intersection. It
 * first calculates the quadratic coefficients based on the ray and the
 * hyperboloid's properties. It then solves the quadratic equation to find
 * potential intersection distances. These distances are validated to ensure
 * they are positive and within the hyperboloid's height limits. If a valid
 * hit is found, the function updates the hit record with the intersection
 * point and the surface normal.
 *
 * @param ray A pointer to the ray.
 * @param hp A pointer to the hyperboloid object.
 * @param rec A pointer to the `t_hit_record` to store intersection details.
 *
 * @return 1 if a valid intersection is found, 0 otherwise.
 */
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
