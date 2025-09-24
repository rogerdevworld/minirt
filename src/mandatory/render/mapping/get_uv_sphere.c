/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uv_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Calculates the UV coordinates for a hit point on a sphere.
 *
 * @details This function maps a 3D point on the surface of a sphere to a 2D
 * texture coordinate system. This is often referred to as 
 **spherical projection**
 * and is a standard method for wrapping a texture around a sphere.
 *
 * The process works by converting the hit point's Cartesian 
 coordinates (x, y, z)
 * into spherical coordinates (theta, phi) and then normalizing 
 these values to the
 * [0.0, 1.0] range.
 *
 * - **Local Point**: The hit point is first translated from world space to the
 * sphere's local space, with its center at the origin `(0, 0, 0)`.
 *
 * - **U-coordinate (Longitude)**: The horizontal `U` coordinate is calculated
 * from the **azimuthal angle** (longitude). This is done using `atan2` with the
 * `x` and `z` components of the local point, which provides the 
 angle in radians.
 * The result is normalized by dividing by `2 * M_PI` and adding `0.5` to ensure
 * the value is in the `[0, 1]` range.
 *
 * - **V-coordinate (Latitude)**: The vertical `V` coordinate is 
 calculated from
 * the **polar angle** (latitude). This is done using `asin` with 
 the `y` component
 * of the local point, which gives the angle from the equator. The result is
 * normalized by dividing by `M_PI` and subtracting from `0.5` to get a `[0, 1]`
 * range where `V=0` is at the top pole and `V=1` is at the bottom pole.
 *
 * The final UV coordinates are returned as a `t_vec2` structure.
 * 
 * @param rec A pointer to the hit record containing the sphere 
 object and the hit point.
 *
 * @return A `t_vec2` structure containing the calculated UV coordinates (U, V).
 */
t_vec2	get_uv_sphere(t_hit_record *rec)
{
	t_vec3	local_point;
	double	u;
	double	v;

	local_point = vec3_sub(rec->point, ((t_sphere *)rec->object->data)->center);
	u = 0.5 + atan2(local_point.z, local_point.x) / (2.0 * M_PI);
	v = 0.5 - asin(local_point.y / ((t_sphere *)rec->object->data)->radius)
		/ M_PI;
	return (vec2_init(u, v));
}
