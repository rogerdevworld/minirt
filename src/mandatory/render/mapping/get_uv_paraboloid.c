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
 * @brief Creates a temporary orthogonal axis for a paraboloid's texture mapping.
 *
 * @details This helper function generates a vector that is perpendicular to the
 * paraboloid's main axis. This temporary axis serves as a reference point for
 * calculating the angular `U` coordinate when wrapping a 2D texture around the
 * 3D surface. The function first attempts to create the perpendicular axis using
 * a cross product with the vector `(1, 0, 0)`. If the paraboloid's axis is
 * parallel to `(1, 0, 0)`, the cross product would be a zero vector. In that
 * case, it falls back to using `(0, 1, 0)` to ensure a valid orthogonal vector
 * is always returned. The resulting vector is normalized to a unit length.
 *
 * @param axis The main axis vector of the paraboloid.
 *
 * @return A normalized `t_vec3` that is perpendicular to the paraboloid's axis.
 */
t_vec3	get_paraboloid_temp_axis(t_vec3 axis)
{
	t_vec3	temp_axis;

	temp_axis = vec3_normalize(vec3_cross(axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(axis, vec3_init(0, 1, 0)));
	return (temp_axis);
}

/**
 * @brief Calculates the U-coordinate for a paraboloid's texture mapping.
 *
 * @details This function determines the horizontal `U` coordinate 
 of a texture at a
 * given hit point on a paraboloid's surface. The `U` coordinate represents the
 * position around the paraboloid's circumference. The process involves:
 *
 * 1. **Orthogonal Projection**: The hit point, in local coordinates, 
 is projected
 * onto a plane perpendicular to the paraboloid's main axis. This 
 creates a 2D
 * representation of the point's position relative to the axis.
 *
 * 2. **Angle Calculation**: An angle is calculated between the 
 projected point and
 * a temporary orthogonal axis (obtained from `get_paraboloid_temp_axis`) using
 * the `acos` function.
 *
 * 3. **Normalization and Correction**: The calculated angle is 
 normalized to a
 * `[0, 1]` range by dividing by `2 * PI`. A final correction is 
 applied by checking
 * the orientation of the cross product of the temporary axis and 
 the projected point.
 * This ensures the texture wraps correctly and avoids seams.
 *
 * @param local_point The hit point relative to the paraboloid's position.
 * @param pb A pointer to the paraboloid object.
 *
 * @return A double representing the calculated U-coordinate.
 */
double	calculate_u_coord(t_vec3 local_point, t_parab *pb)
{
	t_vec3	temp_axis;
	t_vec3	p;
	double	u;

	temp_axis = get_paraboloid_temp_axis(pb->axis);
	p = vec3_sub(local_point, vec3_mul(pb->axis, vec3_dot(local_point,
					pb->axis)));
	u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), pb->axis) < 0)
		u = 1.0 - u;
	return (u);
}

/**
 * @brief Calculates the full UV coordinates for a hit point on a paraboloid.
 *
 * @details This is the main function for paraboloid texture mapping. 
 It takes a hit
 * record and calculates both the **U** and **V** texture coordinates 
 for the hit point.
 *
 * - The **V-coordinate** represents the vertical position along 
 the paraboloid's
 * height. It's determined by the dot product of the local hit point with the
 * paraboloid's axis, normalized by its total height. A value of `0` corresponds
 * to the apex, and `1` to the top edge.
 *
 * - The **U-coordinate** represents the horizontal position 
 around the paraboloid's
 * circumference. It is calculated by calling the helper function
 * `calculate_u_coord`, which handles the angular mapping logic.
 *
 * The final UV coordinates are returned as a single `t_vec2` structure, ready
 * to be used for texture lookup.
 *
 * @param rec A pointer to the hit record containing the object and hit point.
 *
 * @return A `t_vec2` structure containing the calculated UV coordinates (U, V).
 */
t_vec2	get_uv_paraboloid(t_hit_record *rec)
{
	t_parab	*pb;
	t_vec3	local_point;
	double	u;
	double	v;

	pb = (t_parab *)rec->object->data;
	local_point = vec3_sub(rec->point, pb->position);
	v = vec3_dot(local_point, pb->axis) / pb->height;
	u = calculate_u_coord(local_point, pb);
	return (vec2_init(u, v));
}

// t_vec2	get_uv_paraboloid(t_hit_record *rec)
// {
// 	t_parab	*pb;
// 	t_vec3	local_point;
// 	double	v;
// 	t_vec3	temp_axis;
// 	t_vec3	p;
// 	double	u;

// 	pb = (t_parab *)rec->object->data;
// 	local_point = vec3_sub(rec->point, pb->position);
// 	v = vec3_dot(local_point, pb->axis) / pb->height;
// 	temp_axis = vec3_normalize(vec3_cross(pb->axis, vec3_init(1, 0, 0)));
// 	if (vec3_length(temp_axis) < EPSILON)
// 		temp_axis = vec3_normalize(vec3_cross(pb->axis, vec3_init(0, 1, 0)));
// 	p = vec3_sub(local_point, vec3_mul(pb->axis, vec3_dot(local_point,
// 					pb->axis)));
// 	u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
// 	if (vec3_dot(vec3_cross(temp_axis, p), pb->axis) < 0)
// 		u = 1.0 - u;
// 	return (vec2_init(u, v));
// }