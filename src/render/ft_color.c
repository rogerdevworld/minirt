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
#include "../../include/minirt.h"

// src/render/ft_color.c
t_color	get_object_color(t_hit_record *rec)
{
	t_vec3	local_point;
	int		pattern_x;
	int		pattern_y;
	int		pattern_z;

	if (!rec->object->material)
		return (rec->object->color);
	if (rec->object->material->has_texture)
		return (get_texture_color(rec));
	else if (rec->object->material->has_checkerboard)
	{
		local_point = rec->point;
		pattern_x = (int)floor(local_point.x
				/ rec->object->material->check_scale);
		pattern_y = (int)floor(local_point.y
				/ rec->object->material->check_scale);
		pattern_z = (int)floor(local_point.z
				/ rec->object->material->check_scale);
		if ((pattern_x + pattern_y + pattern_z) % 2 == 0)
			return (rec->object->material->check_color1);
		else
			return (rec->object->material->check_color2);
	}
	else
		return (rec->object->color);
}
