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
