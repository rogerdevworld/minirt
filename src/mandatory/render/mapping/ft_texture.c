/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_texture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Dispatches the UV coordinate calculation to the correct 
 object type.
 *
 * @details This function determines the appropriate method for calculating
 * **UV coordinates** based on the type of object that was hit. UV 
 mapping is a
 * technique for projecting a 2D image (a texture) onto the 3D surface of a
 * model. The UV coordinates are analogous to a 2D coordinate system where
 * `U` represents the horizontal axis and `V` represents the vertical axis of
 * the texture. This function acts as a dispatcher, calling the specialized
 * `get_uv_*` function for spheres, planes, cylinders, etc., 
 to get the correct
 * texture coordinates for the hit point.
 *
 * @param rec A pointer to the hit record containing the object 
 and hit point.
 *
 * @return A `t_vec2` structure containing the UV coordinates for 
 the hit point.
 */
t_vec2	get_texture_uv(t_hit_record *rec)
{
	if (rec->object->type == SPHERE)
		return (get_uv_sphere(rec));
	else if (rec->object->type == PLANE)
		return (get_uv_plane(rec));
	else if (rec->object->type == CYLINDER)
		return (get_uv_cylinder(rec));
	else if (rec->object->type == CONE)
		return (get_uv_cone(rec));
	else if (rec->object->type == HYPERBOLOID)
		return (get_uv_hyperboloid(rec));
	else if (rec->object->type == PARABOLOID)
		return (get_uv_paraboloid(rec));
	return ((t_vec2){0.0, 0.0});
}

/**
 * @brief Retrieves the color of a texture at a given hit point.
 *
 * @details This function is responsible for performing a **texture lookup**.
 * It first checks if the object has a texture assigned to it. If not, it returns
 * the object's default color. If a texture exists, it calls `get_texture_uv`
 * to find the correct UV coordinates for the hit point. These UV coordinates,
 * which are in the range [0.0, 1.0], are then scaled to the dimensions of the
 * texture image to find the exact pixel index. The function then reads the RGB
 * color from the texture's pixel data at that index and converts it to a
 * floating-point `t_color` format for lighting and shading calculations.
 *
 * @param rec A pointer to the hit record.
 *
 * @return A `t_color` vector representing the color from the texture, or the
 * object's base color if no texture is found.
 */
t_color	get_texture_color(t_hit_record *rec)
{
	t_vec2			uv;
	mlx_texture_t	*texture;
	int				index;
	t_color			tex_color;

	if (!rec->object->material || !rec->object->material->has_texture)
		return (rec->object->color);
	texture = rec->object->material->color_img;
	if (texture->width <= 0 || texture->height <= 0)
		return (rec->object->color);
	uv = get_texture_uv(rec);
	uv.x = fmax(0.0, fmin(1.0, uv.x));
	uv.y = fmax(0.0, fmin(1.0, uv.y));
	index = ((int)(uv.y * texture->height - 1) * texture->width + (int)(uv.x
				* (texture->width - 1))) * 4;
	tex_color.x = (double)texture->pixels[index] / 255.0;
	tex_color.y = (double)texture->pixels[index + 1] / 255.0;
	tex_color.z = (double)texture->pixels[index + 2] / 255.0;
	return (tex_color);
}
