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
 hyperboloid.
 *
 * @details This function maps a 3D hit point on the surface of a 
 hyperboloid to a 2D
 * texture coordinate system, which is crucial for applying textures.
 *
 * The UV mapping works by effectively "unwrapping" the 
 hyperboloid's surface. The
 * process is as follows:
 *
 * 1. **Local Space Conversion**: The hit point is translated 
 to a local coordinate system
 * where the hyperboloid's center is at the origin.
 *
 * 2. **V-coordinate Calculation**: The V-coordinate represents 
 the vertical position along the
 * hyperboloid's central axis. It's determined by the dot product 
 of the local point and the
 * hyperboloid's axis. This value is then normalized to the range 
 `[0, 1]` using the hyperboloid's
 * total height. A value of 0 corresponds to the bottom of the 
 hyperboloid, and 1 corresponds
 * to the top.
 *
 * 3. **U-coordinate Calculation**: The U-coordinate represents 
 the horizontal position around
 * the hyperboloid's circumference. This is calculated using the 
 angle around the central axis. A
 * temporary reference axis (`temp_axis`) is created perpendicular 
 to the hyperboloid's axis.
 * The `atan2` function is then used to find the angle between the 
 projected point on the
 * cross-section and this reference axis. This angle is normalized 
 to the range `[0, 1]` to
 * create the U-coordinate. An offset of `0.5` is added to ensure 
 the result is always positive.
 *
 * This method ensures that the texture wraps correctly and smoothly 
 around the
 * curved surface of the hyperboloid.
 *
 * @param rec A pointer to the hit record containing the object and hit point.
 *
 * @return A `t_vec2` structure containing the calculated UV coordinates (U, V).
 */
t_vec2	get_uv_hyperboloid(t_hit_record *rec)
{
	t_hyperboloid	*hp;
	t_vec3			local_point;
	t_vec3			temp_axis;
	t_vec3			p;

	hp = (t_hyperboloid *)rec->object->data;
	local_point = vec3_sub(rec->point, hp->position);
	temp_axis = vec3_normalize(vec3_cross(hp->axis, vec3_init(0, 0, 1)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(hp->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(hp->axis, vec3_dot(local_point,
					hp->axis)));
	return (vec2_init(atan2(vec3_dot(p, vec3_cross(hp->axis, temp_axis)),
				vec3_dot(p, temp_axis)) / (2.0 * M_PI) + 0.5,
			(vec3_dot(local_point, hp->axis) + hp->height / 2.0) / hp->height));
}
