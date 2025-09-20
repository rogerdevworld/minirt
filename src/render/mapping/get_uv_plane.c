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
#include "../../include/minirt.h"

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_plane(t_hit_record *rec)
{
	t_plane	*pl;
	t_vec3	local_x_axis;
	t_vec3	local_y_axis;
	t_vec3	local_point;
	double	u;
	double	v;

	pl = (t_plane *)rec->object->data;
	local_x_axis = vec3_normalize(vec3_cross(pl->normal, vec3_init(0, 1, 0)));
	if (vec3_length(local_x_axis) < EPSILON)
		local_x_axis = vec3_normalize(vec3_cross(pl->normal, vec3_init(1, 0,
						0)));
	local_y_axis = vec3_normalize(vec3_cross(local_x_axis, pl->normal));
	local_point = vec3_sub(rec->point, pl->position);
	u = vec3_dot(local_point, local_x_axis) / 10.0;
	v = vec3_dot(local_point, local_y_axis) / 10.0;
	u = u - floor(u);
	v = v - floor(v);
	return (vec2_init(u, v));
}
