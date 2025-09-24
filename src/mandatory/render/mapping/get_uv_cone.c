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
 cone's lateral surface.
 *
 * @details This function maps a 3D point on a cone's surface 
 to a 2D texture coordinate
 * system (U, V). The U-coordinate represents the horizontal 
 position around the cone,
 * and the V-coordinate represents the vertical position along its height.
 *
 * The process works as follows:
 *
 * 1. **Normalize the hit point**: The hit point is converted to a 
 local coordinate system
 * relative to the cone's base.
 *
 * 2. **Calculate the V-coordinate**: The V-coordinate is determined 
 by the hit point's
 * vertical position along the cone's axis, normalized by the cone's 
 total height. A
 * value of 0 corresponds to the base, and 1 corresponds to the apex.
 *
 * 3. **Calculate the U-coordinate**: The U-coordinate is based on 
 the angle around the
 * cone's central axis. To find this angle, a temporary reference 
 axis is created. This
 * axis is a vector perpendicular to the cone's main axis, allowing 
 for the calculation
 * of a horizontal angle using `acos`. This angle is then normalized 
 to the range [0, 1].
 * A correction is applied to ensure the texture wraps correctly around the cone
 * by checking the direction of the cross product between the 
 temporary axis and the
 * local point.
 *
 * @param rec A pointer to the hit record containing the object 
 and hit point.
 *
 * @return A `t_vec2` structure containing the calculated UV 
 coordinates (U, V).
 */
t_vec2	get_uv_cone(t_hit_record *rec)
{
	t_cone	*cn;
	t_vec3	local_point;
	t_vec3	temp_axis;
	t_vec3	p;
	double	u;

	cn = (t_cone *)rec->object->data;
	local_point = vec3_sub(rec->point, cn->position);
	temp_axis = vec3_normalize(vec3_cross(cn->axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(cn->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(cn->axis, vec3_dot(local_point,
					cn->axis)));
	u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), cn->axis) < 0)
		u = 1.0 - u;
	return (vec2_init(u, vec3_dot(local_point, cn->axis) / cn->height));
}
