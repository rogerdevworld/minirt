/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_light.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

t_color	calculate_ambient_light(t_hit_record *rec, t_scene *scene)
{
	t_color	object_color;
	t_color	ambient_color;

	object_color = get_object_color(rec);
	ambient_color = vec3_mult_vec(scene->ambient.color, object_color);
	return (vec3_mul(ambient_color, scene->ambient.ratio));
}

// Clamps a color's components to the range [0.0, 1.0]
t_color	clamp_color(t_color color)
{
	color.x = fmin(1.0, fmax(0.0, color.x));
	color.y = fmin(1.0, fmax(0.0, color.y));
	color.z = fmin(1.0, fmax(0.0, color.z));
	return (color);
}

// Determines the effective normal (either from a normal map or the hit record)
t_vec3	get_effective_normal(t_hit_record *rec)
{
	if (rec->object->material && rec->object->material->has_normal_map)
		return (get_normal_from_map(rec, rec->normal));
	return (rec->normal);
}

// Calculates the combined diffuse and specular contribution from a single light
t_color	get_light_contribution(t_light_context *ctx, t_light *light)
{
	t_ray	shadow_ray;
	t_color	contribution;

	shadow_ray.origin = vec3_add(ctx->rec->point,
			vec3_mul(ctx->effective_normal, EPSILON));
	shadow_ray.direction = vec3_normalize(vec3_sub(light->position,
				ctx->rec->point));
	if (is_in_shadow(&shadow_ray, ctx->scene, light))
		return (vec3_init(0, 0, 0));
	contribution = calculate_diffuse_light(ctx->rec, light,
			ctx->effective_normal, shadow_ray.direction);
	if (ctx->rec->object->material
		&& ctx->rec->object->material->specular.intensity > 0.0)
		contribution = vec3_add(contribution, calculate_specular_light(ctx->rec,
					light, ctx->ray));
	return (contribution);
}

// Calculates the total direct lighting by iterating over all lights
t_color	calculate_direct_lighting(t_hit_record *rec, t_scene *scene, t_ray *ray,
		t_vec3 effective_normal)
{
	t_color			total_light;
	t_light			**lights;
	t_light_context	ctx;
	int				i;

	total_light = vec3_init(0, 0, 0);
	lights = (t_light **)scene->lights;
	i = 0;
	ctx.rec = rec;
	ctx.scene = scene;
	ctx.ray = ray;
	ctx.effective_normal = effective_normal;
	while (lights[i] != NULL)
	{
		total_light = vec3_add(total_light, get_light_contribution(&ctx,
					lights[i]));
		i++;
	}
	return (total_light);
}
