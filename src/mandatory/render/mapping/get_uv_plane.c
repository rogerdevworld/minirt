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
 * @brief Generates two orthogonal basis vectors for a plane.
 *
 * @details This function creates a local 2D coordinate system on a 3D plane.
 * It takes the plane's normal vector and generates two new, normalized vectors
 * that are perpendicular to each other and to the normal vector.
 *
 * The process is as follows:
 * 1. **Allocate Memory**: It allocates memory for two `t_vec3` 
 vectors to store the axes.
 * 2. **Find the First Axis (X-axis)**: It tries to find a vector 
 perpendicular to the
 * plane's normal by taking the cross product with a standard up 
 vector `(0, 1, 0)`.
 * This is typically the fastest and most common case.
 * 3. **Handle Edge Case**: If the plane's normal is parallel to 
 the `(0, 1, 0)` vector,
 * the cross product would be a zero vector. To handle this, it 
 performs a fallback
 * by using the cross product with a different standard vector, 
 `(1, 0, 0)`, to ensure
 * a valid perpendicular vector is always found.
 * 4. **Find the Second Axis (Y-axis)**: The second axis is found by 
 taking the cross
 * product of the first axis and the plane's normal. This guarantees 
 that all three
 * vectors (the two new axes and the normal) are mutually orthogonal.
 *
 * @param normal The normalized normal vector of the plane.
 *
 * @return An array of two `t_vec3` vectors representing the 
 local X and Y axes, or NULL
 * if memory allocation fails.
 */
t_vec3	*get_plane_axes(t_vec3 normal)
{
	t_vec3	*axes;
	t_vec3	local_x_axis;

	axes = malloc(sizeof(t_vec3) * 2);
	if (!axes)
		return (NULL);
	local_x_axis = vec3_normalize(vec3_cross(normal, vec3_init(0, 1, 0)));
	if (vec3_length(local_x_axis) < EPSILON)
		local_x_axis = vec3_normalize(vec3_cross(normal, vec3_init(1, 0, 0)));
	axes[0] = local_x_axis;
	axes[1] = vec3_normalize(vec3_cross(local_x_axis, normal));
	return (axes);
}

/**
 * @brief Calculates the UV coordinates for a hit point on a plane.
 *
 * @details This function maps a 3D hit point on a plane to a 2D 
 texture coordinate
 * system. The UV coordinates are used to look up the correct 
 color from a texture
 * image.
 *
 * The process is as follows:
 * 1. **Get Local Axes**: It first calls `get_plane_axes` to 
 obtain two orthogonal
 * vectors that lie on the plane. These vectors serve as the local X (`axes[0]`)
 * and Y (`axes[1]`) axes for the texture mapping.
 * 2. **Transform to Local Space**: The hit point is translated 
 into a local coordinate
 * system with its origin at the plane's position.
 * 3. **Calculate UV Coordinates**: The UV coordinates are 
 calculated by taking the
 * dot product of the local point with each of the local axes. This projects the
 * 3D point onto the 2D plane defined by the axes. The result is then scaled
 * (in this case, by dividing by `10.0`) to control the size 
 of the texture tiling.
 * 4. **Tile the Texture**: The fractional part of the UV 
 coordinates is extracted
 * using the `floor` function. This causes the texture to repeat, or "tile,"
 * across the entire plane's surface.
 * 5. **Clean Up**: The memory allocated for the axes is freed.
 *
 * @param rec A pointer to the hit record containing the plane 
 object and the hit point.
 *
 * @return A `t_vec2` structure containing the final UV coordinates (U, V).
 */
t_vec2	get_uv_plane(t_hit_record *rec)
{
	t_plane	*pl;
	t_vec3	local_point;
	t_vec3	*axes;
	t_vec2	uv;

	pl = (t_plane *)rec->object->data;
	axes = get_plane_axes(pl->normal);
	if (!axes)
		return ((t_vec2){0.0, 0.0});
	local_point = vec3_sub(rec->point, pl->position);
	uv.x = vec3_dot(local_point, axes[0]) / 10.0;
	uv.y = vec3_dot(local_point, axes[1]) / 10.0;
	free(axes);
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	return (uv);
}

// t_vec2	get_uv_plane(t_hit_record *rec)
// {
// 	t_plane	*pl;
// 	t_vec3	local_x_axis;
// 	t_vec3	local_y_axis;
// 	t_vec3	local_point;
// 	double	u;
// 	double	v;

// 	pl = (t_plane *)rec->object->data;
// 	local_x_axis = vec3_normalize(vec3_cross(pl->normal, vec3_init(0, 1, 0)));
// 	if (vec3_length(local_x_axis) < EPSILON)
// 		local_x_axis = vec3_normalize(vec3_cross(pl->normal, vec3_init(1, 0,
// 						0)));
// 	local_y_axis = vec3_normalize(vec3_cross(local_x_axis, pl->normal));
// 	local_point = vec3_sub(rec->point, pl->position);
// 	u = vec3_dot(local_point, local_x_axis) / 10.0;
// 	v = vec3_dot(local_point, local_y_axis) / 10.0;
// 	u = u - floor(u);
// 	v = v - floor(v);
// 	return (vec2_init(u, v));
// }