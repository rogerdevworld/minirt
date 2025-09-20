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
t_vec2	get_uv_paraboloid(t_hit_record *rec)
{
	t_parab *pb = (t_parab *)rec->object->data;
	t_vec3 local_point = vec3_sub(rec->point, pb->position);
	double v = vec3_dot(local_point, pb->axis) / pb->height;
	t_vec3 temp_axis = vec3_normalize(vec3_cross(pb->axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(pb->axis, vec3_init(0, 1, 0)));
	t_vec3 p = vec3_sub(local_point, vec3_mul(pb->axis, vec3_dot(local_point,
					pb->axis)));
	double u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), pb->axis) < 0)
		u = 1.0 - u;
	return (vec2_init(u, v));
}