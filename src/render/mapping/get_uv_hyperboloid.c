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
t_vec2	get_uv_hyperboloid(t_hit_record *rec)
{
	t_hyperboloid	*hp;
	t_vec3			local_point;
	t_vec3			temp_axis;
	t_vec3			p;
	double			u;
	double			v;

	hp = (t_hyperboloid *)rec->object->data;
	local_point = vec3_sub(rec->point, hp->position);
	temp_axis = vec3_normalize(vec3_cross(hp->axis, vec3_init(0, 0, 1)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(hp->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(hp->axis, vec3_dot(local_point,
					hp->axis)));
	u = atan2(vec3_dot(p, vec3_cross(hp->axis, temp_axis)), vec3_dot(p,
				temp_axis)) / (2.0 * M_PI) + 0.5;
	v = (vec3_dot(local_point, hp->axis) + hp->height / 2.0) / hp->height;
	return (vec2_init(u, v));
}
