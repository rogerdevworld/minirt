/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Calculates a point on a ray given a parameter 't'.
 *
 * @details This function computes the 3D coordinates of a point 
 that lies along a given ray.
 * The ray is defined by its origin and direction vector. 
 The parameter 't' determines
 * the distance from the ray's origin to the desired point. 
 A positive 't' value
 * corresponds to a point in front of the origin (along the direction vector),
 * while a negative 't' value corresponds to a point behind it.
 *
 * The formula used is 'P(t) = O + t*D', where:
 * - P is the resulting point on the ray.
 * - O is the ray's origin.
 * - D is the ray's direction vector.
 * - t is the scalar parameter.
 *
 * This function is fundamental in ray tracing for finding intersection points
 * between a ray and objects in the scene.
 *
 * @param r A `t_ray` struct containing the ray's origin and direction.
 * @param t The scalar parameter determining the distance along the ray.
 * @return A `t_vec3` struct representing the 3D coordinates of the 
 point on the ray.
 */
t_vec3	ray_at(t_ray r, double t)
{
	return (vec3_add(r.origin, vec3_mul(r.direction, t)));
}
