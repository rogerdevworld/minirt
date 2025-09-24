/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sphere.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Calculates the discriminant for a ray-sphere intersection.
 *
 * @details This function computes the discriminant of the quadratic 
 equation that
 * solves for a ray-sphere intersection. The discriminant determines the number
 * of real solutions: a negative value means no intersection, zero means one
 * (tangent) intersection, and a positive value means two intersections.
 * The calculation is derived from the geometric relationship between 
 the ray's
 * origin, the sphere's center, and the sphere's radius.
 *
 * @param ray A pointer to the ray being cast.
 * @param oc The vector from the sphere's center to the ray's origin.
 * @param radius The radius of the sphere.
 *
 * @return The discriminant value, or -1.0 if it is negative.
 */
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

/**
 * @brief Calculates the intersection distance 't' from the discriminant.
 *
 * @details This function solves the quadratic equation for the 
 intersection distance
 * `t` using the discriminant calculated by `get_discriminant`. It finds the
 * smallest positive `t` value, which corresponds to the first 
 point where the ray
 * hits the sphere. It handles cases where the ray starts inside the sphere or
 * only grazes its surface. If no valid `t` value is found, it returns -1.0.
 *
 * @param ray A pointer to the ray.
 * @param oc The vector from the sphere's center to the ray's origin.
 * @param disc The pre-calculated discriminant value.
 *
 * @return The smallest positive intersection distance `t` or -1.0 if 
 none exists.
 */
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

/**
 * @brief Updates the hit record with the intersection details.
 *
 * @details This function populates the `t_hit_record` structure with all the
 * necessary information after a valid intersection has been found. 
 It sets the
 * intersection distance `t`, calculates the precise 3D hit point, and computes
 * the surface normal at that point. The normal is then oriented to ensure it
 * faces away from the ray's origin for accurate shading and lighting 
 calculations.
 *
 * @param ray A pointer to the ray.
 * @param sp A pointer to the sphere object.
 * @param rec A pointer to the `t_hit_record` to be updated.
 * @param t The valid intersection distance.
 *
 * @return void.
 */
static void	update_hit_record(t_ray *ray, t_sphere *sp, t_hit_record *rec,
		double t)
{
	t_vec3	point;
	t_vec3	normal;

	rec->t = t;
	point = ray_at(*ray, rec->t);
	rec->point = point;
	normal = vec3_normalize(vec3_sub(point, sp->center));
	if (vec3_dot(ray->direction, normal) > 0)
		rec->normal = vec3_mul(normal, -1.0);
	else
		rec->normal = normal;
}

/**
 * @brief Determines if a ray intersects a sphere and records the hit.
 *
 * @details This is the main function for ray-sphere intersection. It first
 * computes the discriminant of the ray-sphere equation using `get_discriminant`.
 * If an intersection is possible, it solves for the closest 
 intersection distance
 * `t` using `get_t_value`. If a valid hit is confirmed, it calls
 * `update_hit_record` to store all the relevant intersection data in the `rec`
 * structure.
 *
 * @param ray A pointer to the ray.
 * @param sp A pointer to the sphere object.
 * @param rec A pointer to the `t_hit_record` to store intersection details.
 *
 * @return 1 if a valid intersection is found, 0 otherwise.
 */
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
