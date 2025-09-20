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
#include "../../include/minirt.h"

// src/render/ft_texture.c
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
	if (rec->object->type == SPHERE)
		uv = get_uv_sphere(rec);
	else if (rec->object->type == PLANE)
		uv = get_uv_plane(rec);
	else if (rec->object->type == CYLINDER)
		uv = get_uv_cylinder(rec);
	else if (rec->object->type == CONE)
		uv = get_uv_cone(rec);
	else if (rec->object->type == HYPERBOLOID)
		uv = get_uv_hyperboloid(rec);
	else if (rec->object->type == PARABOLOID)
		uv = get_uv_paraboloid(rec);
	else
		return (rec->object->color);
	uv.x = fmax(0.0, fmin(1.0, uv.x));
	uv.y = fmax(0.0, fmin(1.0, uv.y));
	index = ((int)(uv.y * (texture->height - 1) * texture->width + (int)(uv.x
					* (texture->width - 1)))) * 4;
	tex_color.x = (double)texture->pixels[index] / 255.0;
	tex_color.y = (double)texture->pixels[index + 1] / 255.0;
	tex_color.z = (double)texture->pixels[index + 2] / 255.0;
	return (tex_color);
}
