/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Determines if a ray intersects a plane and records the hit.
 *
 * @details This function calculates the intersection of a given 
 ray with an infinite plane.
 * It first computes the denominator of the ray-plane intersection 
 formula, which is the
 * dot product of the plane's normal and the ray's direction. If 
 this denominator is close to
 * zero, it means the ray is parallel to the plane, so there is no 
 intersection. Otherwise, it
 * calculates the intersection distance (`t`). It then checks if `t` 
 is a positive value,
 * which indicates a valid intersection in front of the ray's origin.
 If a hit is found,
 * it populates the `t_hit_record` structure with the intersection 
 distance, the
 * exact hit point, and the correctly oriented surface normal at that 
 point.
 *
 * @param ray A pointer to the ray being cast.
 * @param pl A pointer to the plane object.
 * @param rec A pointer to the `t_hit_record` structure to store the 
 intersection details.
 *
 * @return 1 if a valid intersection is found, 0 otherwise.
 */
int	intersect_plane(t_ray *ray, t_plane *pl, t_hit_record *rec)
{
	double	denominator;
	double	t;

	denominator = vec3_dot(pl->normal, ray->direction);
	if (fabs(denominator) < EPSILON)
		return (0);
	t = vec3_dot(vec3_sub(pl->position, ray->origin), pl->normal) / denominator;
	if (t < EPSILON)
		return (0);
	rec->t = t;
	rec->point = ray_at(*ray, rec->t);
	if (vec3_dot(pl->normal, ray->direction) > 0)
		rec->normal = vec3_mul(pl->normal, -1.0);
	else
		rec->normal = pl->normal;
	return (1);
}
