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

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_sphere(t_hit_record *rec)
{
	t_vec3	local_point;
	double	u;
	double	v;

	local_point = vec3_sub(rec->point, ((t_sphere *)rec->object->data)->center);
	u = 0.5 + atan2(local_point.z, local_point.x) / (2.0 * M_PI);
	v = 0.5 - asin(local_point.y / ((t_sphere *)rec->object->data)->radius)
		/ M_PI;
	return (vec2_init(u, v));
}
