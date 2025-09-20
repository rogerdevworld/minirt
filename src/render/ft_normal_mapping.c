/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_normal_mapping.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// src/render/ft_normal_mapping.c
t_mat3	create_tbn_matrix(t_vec3 normal)
{
	t_vec3	tangent;
	t_vec3	bitangent;
	t_vec3	up;
	t_mat3	tbn;

	up = vec3_init(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(normal, up)) > 0.999)
		up = vec3_init(1.0, 0.0, 0.0);
	tangent = vec3_normalize(vec3_cross(up, normal));
	bitangent = vec3_normalize(vec3_cross(normal, tangent));
	tbn.c1 = tangent;
	tbn.c2 = bitangent;
	tbn.c3 = normal;
	return (tbn);
}

// Obtiene la normal de la textura y la convierte a espacio del mundo
t_vec3	get_normal_from_map(t_hit_record *rec, t_vec3 geom_normal)
{
	t_vec2			uv;
	mlx_texture_t	*normal_map;
	int				x_tex;
	int				y_tex;
	int				index;
	t_vec3			new_normal;
	t_mat3			tbn;

	normal_map = rec->object->material->texture_img;
	if (!normal_map || normal_map->width <= 0 || normal_map->height <= 0)
		return (geom_normal);
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
		return (geom_normal);
	x_tex = (int)(uv.x * (normal_map->width - 1));
	y_tex = (int)(uv.y * (normal_map->height - 1));
	index = (y_tex * normal_map->width + x_tex) * 4;
	new_normal.x = (double)normal_map->pixels[index] / 127.5 - 1.0;
	new_normal.y = (double)normal_map->pixels[index + 1] / 127.5 - 1.0;
	new_normal.z = (double)normal_map->pixels[index + 2] / 127.5 - 1.0;
	new_normal = vec3_normalize(new_normal);
	tbn = create_tbn_matrix(geom_normal);
	new_normal = vec3_add(vec3_add(vec3_mul(tbn.c1, new_normal.x),
				vec3_mul(tbn.c2, new_normal.y)), vec3_mul(tbn.c3,
				new_normal.z));
	return (vec3_normalize(new_normal));
}
