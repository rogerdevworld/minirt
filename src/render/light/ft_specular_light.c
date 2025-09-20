/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_specular_light.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// src/render/ft_specular_light.c
t_color	calculate_specular_light(t_hit_record *rec, t_light *light, t_ray *ray)
{
	t_vec3	to_light;
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	spec_factor;
	t_color	specular_color;
	t_vec3	effective_normal;
	double	diff;

	if (rec->object->material && rec->object->material->has_normal_map)
		effective_normal = get_normal_from_map(rec, rec->normal);
	else
		effective_normal = rec->normal;
	to_light = vec3_normalize(vec3_sub(light->position, rec->point));
	view_dir = vec3_normalize(vec3_mul(ray->direction, -1.0));
	reflect_dir = vec3_reflect(vec3_mul(to_light, -1.0), effective_normal);
	spec_factor = vec3_dot(view_dir, reflect_dir);
	if (spec_factor > 0 && rec->object->material->specular.shininess > 0)
	{
		spec_factor = pow(spec_factor,
				rec->object->material->specular.shininess);
		diff = vec3_dot(effective_normal, to_light);
		if (diff > 0)
		{
			specular_color = vec3_mul(light->color,
					rec->object->material->specular.intensity * spec_factor
					* diff * light->brightness);
			return (specular_color);
		}
	}
	return (vec3_init(0, 0, 0));
}
