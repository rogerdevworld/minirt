/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_color.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Retrieves the final color of a hit point, considering 
 textures and patterns.
 *
 * @details This function acts as a dispatcher for determining 
 an object's color at a specific
 * intersection point. It checks for a material on the object
  and, in a prioritized order,
 * decides which color source to use:
 *
 * 1. **Textures**: If the material has a texture 
 (`has_texture` is true), the function
 * calls `get_texture_color` to perform a texture lookup, 
 which provides the most
 * detailed color information.
 *
 * 2. **Checkerboard Pattern**: If there's no texture but a 
 checkerboard pattern is enabled
 * (`has_checkerboard` is true), the function calculates a 3D 
 checkerboard pattern. It does
 * this by taking the floor of the hit point's coordinates (x, y, z), 
 scaled by a `check_scale`
 * factor. The sum of these floored values is checked for parity 
 (even or odd). An even
 * sum returns `check_color1`, and an odd sum returns `check_color2`,
 creating the alternating
 * pattern.
 *
 * 3. **Default Color**: If neither a texture nor a checkerboard 
 pattern is specified, the
 * function returns the object's base color (`rec->object->color`), 
 which is the simplest
 * shading option.
 *
 * @param rec A pointer to the hit record containing the object 
 and hit point.
 *
 * @return A `t_color` vector representing the final color of the 
 object at the hit point.
 */
t_color	get_object_color(t_hit_record *rec)
{
	t_vec3	local_point;

	if (!rec->object->material)
		return (rec->object->color);
	if (rec->object->material->has_texture)
		return (get_texture_color(rec));
	else if (rec->object->material->has_checkerboard)
	{
		local_point = rec->point;
		if (((int)floor(local_point.x / rec->object->material->check_scale)
				+ (int)floor(local_point.y / rec->object->material->check_scale)
				+ (int)floor(local_point.z
					/ rec->object->material->check_scale)) % 2 == 0)
			return (rec->object->material->check_color1);
		else
			return (rec->object->material->check_color2);
	}
	else
		return (rec->object->color);
}
