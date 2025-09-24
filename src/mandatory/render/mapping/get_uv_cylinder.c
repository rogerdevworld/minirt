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
 * @brief Calculates the UV coordinates for a hit point on a 
 cylinder's lateral surface.
 *
 * @details This function maps a 3D point on a cylinder's 
 side to a 2D texture coordinate
 * system (U, V). This is essential for applying textures 
 to the surface.
 *
 * The UV mapping for a cylinder's body works by "unrolling" 
 the curved surface into a flat rectangle.
 * The process is as follows:
 *
 * 1. **Transform to Local Space**: The hit point is first 
 translated from world coordinates
 * to the cylinder's local space, with the origin at the cylinder's center.
 *
 * 2. **Calculate the V-coordinate**: The V-coordinate represents 
 the vertical position along the
 * cylinder's height. This is determined by the dot product of the 
 local point and the
 * cylinder's axis, which gives the height along the axis. This 
 value is then normalized
 * to the range [0, 1] using the cylinder's height. A value of 0 
 typically corresponds
 * to the bottom cap, and 1 to the top cap.
 *
 * 3. **Calculate the U-coordinate**: The U-coordinate represents 
 the horizontal position
 * around the cylinder's circumference. This is calculated using 
 the angle around the
 * cylinder's central axis. A temporary reference axis is created 
 by taking the cross product
 * of the cylinder's axis with a non-parallel vector (e.g., `(1, 0, 0)`). 
 This provides a
 * starting point for the angle measurement. The `acos` function is 
 then used to find the
 * angle between this reference axis and the projected point on the 
 cylinder's cross-section.
 * This angle is normalized to the range [0, 1] to form the U-coordinate. 
 A check is
 * performed to ensure the texture wraps correctly by determining the 
 direction of the
 * cross product and adjusting the U-coordinate if necessary.
 *
 * @param rec A pointer to the hit record containing the object and hit point.
 *
 * @return A `t_vec2` structure containing the calculated UV coordinates (U, V).
 */
t_vec2	get_uv_cylinder(t_hit_record *rec)
{
	t_cylinder	*cy;
	t_vec3		local_point;
	t_vec3		temp_axis;
	t_vec3		p;
	double		u;

	cy = (t_cylinder *)rec->object->data;
	local_point = vec3_sub(rec->point, cy->position);
	temp_axis = vec3_normalize(vec3_cross(cy->axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(cy->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(cy->axis, vec3_dot(local_point,
					cy->axis)));
	u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), cy->axis) < 0)
		u = 1.0 - u;
	return (vec2_init(u, (vec3_dot(local_point, cy->axis) + cy->height / 2.0)
			/ cy->height));
}
