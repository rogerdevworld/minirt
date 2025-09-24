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
#include "../../../../include/minirt.h"

/**
 * @brief Calculates the specular factor for a hit point.
 *
 * @details This function computes the dot product of the **view vector** (the
 * direction from the hit point to the camera) and the **reflected 
 light vector**.
 * This dot product, known as the specular factor, determines the 
 intensity of the
 * specular highlight. A value of `1.0` indicates a perfect reflection directly
 * into the camera's view, resulting in the brightest highlight. 
 The function also
 * uses the effective normal, which can be a perturbed normal from a normal map
 * to create more detailed reflections.
 *
 * @param rec A pointer to the hit record.
 * @param light A pointer to the light source.
 * @param ray A pointer to the incoming ray.
 *
 * @return A double value representing the specular factor.
 */
double	get_specular_factor(t_hit_record *rec, t_light *light, t_ray *ray)
{
	t_vec3	effective_normal;
	t_vec3	to_light;
	t_vec3	view_dir;
	t_vec3	reflect_dir;

	if (rec->object->material && rec->object->material->has_normal_map)
		effective_normal = get_normal_from_map(rec, rec->normal);
	else
		effective_normal = rec->normal;
	to_light = vec3_normalize(vec3_sub(light->position, rec->point));
	view_dir = vec3_normalize(vec3_mul(ray->direction, -1.0));
	reflect_dir = vec3_reflect(vec3_mul(to_light, -1.0), effective_normal);
	return (vec3_dot(view_dir, reflect_dir));
}

/**
 * @brief Calculates the specular light color contribution at a hit point.
 *
 * @details This function calculates the specular highlight, which simulates the
 * shiny appearance of a surface. It first obtains the specular factor using
 * `get_specular_factor`. If this factor is positive, it is raised to the power
 * of the material's **shininess** property. A higher shininess value 
 results in a
 * smaller, sharper highlight, characteristic of very smooth surfaces. The
 * function then scales the light's color by this factor, the material's
 * **specular intensity**, and the light's brightness. This result is the color
 * of the specular highlight.
 *
 * @param rec A pointer to the hit record.
 * @param light A pointer to the light source.
 * @param ray A pointer to the incoming ray.
 *
 * @return A `t_color` representing the specular light contribution, 
 or black if there
 * is no highlight.
 */
t_color	calculate_specular_light(t_hit_record *rec, t_light *light, t_ray *ray)
{
	double	spec_factor;
	double	diff;
	t_vec3	effective_normal;
	t_vec3	to_light;

	spec_factor = get_specular_factor(rec, light, ray);
	if (spec_factor > 0 && rec->object->material->specular.shininess > 0)
	{
		spec_factor = pow(spec_factor,
				rec->object->material->specular.shininess);
		if (rec->object->material && rec->object->material->has_normal_map)
			effective_normal = get_normal_from_map(rec, rec->normal);
		else
			effective_normal = rec->normal;
		to_light = vec3_normalize(vec3_sub(light->position, rec->point));
		diff = vec3_dot(effective_normal, to_light);
		if (diff > 0)
		{
			return (vec3_mul(light->color,
					rec->object->material->specular.intensity * spec_factor
					* diff * light->brightness));
		}
	}
	return (vec3_init(0, 0, 0));
}

// src/render/ft_specular_light.c
// t_color	calculate_specular_light(t_hit_record *rec, t_light *light,
// t_ray *ray)
// {
// 	t_vec3	to_light;
// 	t_vec3	view_dir;
// 	t_vec3	reflect_dir;
// 	double	spec_factor;
// 	t_color	specular_color;
// 	t_vec3	effective_normal;
// 	double	diff;

// 	if (rec->object->material && rec->object->material->has_normal_map)
// 		effective_normal = get_normal_from_map(rec, rec->normal);
// 	else
// 		effective_normal = rec->normal;
// 	to_light = vec3_normalize(vec3_sub(light->position, rec->point));
// 	view_dir = vec3_normalize(vec3_mul(ray->direction, -1.0));
// 	reflect_dir = vec3_reflect(vec3_mul(to_light, -1.0), effective_normal);
// 	spec_factor = vec3_dot(view_dir, reflect_dir);
// 	if (spec_factor > 0 && rec->object->material->specular.shininess > 0)
// 	{
// 		spec_factor = pow(spec_factor,
// 				rec->object->material->specular.shininess);
// 		diff = vec3_dot(effective_normal, to_light);
// 		if (diff > 0)
// 		{
// 			specular_color = vec3_mul(light->color,
// 					rec->object->material->specular.intensity * spec_factor
// 					* diff * light->brightness);
// 			return (specular_color);
// 		}
// 	}
// 	return (vec3_init(0, 0, 0));
// }
