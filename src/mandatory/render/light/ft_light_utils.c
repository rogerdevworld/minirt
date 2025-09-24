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

/**
 * @brief Calculates the ambient light color for a given hit point.
 *
 * @details This function determines the contribution of ambient 
 light to an object's color.
 * It multiplies the scene's ambient light color by the object's 
 own color, then scales
 * the result by the ambient light's intensity ratio. This provides 
 a base level of
 * illumination for areas not directly lit by light sources, ensuring 
 they aren't
 * completely black.
 *
 * @param rec A pointer to the hit record containing the object's properties.
 * @param scene A pointer to the scene structure, which holds the 
 ambient light data.
 *
 * @return A `t_color` vector representing the final ambient color contribution.
 */
t_color	calculate_ambient_light(t_hit_record *rec, t_scene *scene)
{
	t_color	object_color;
	t_color	ambient_color;

	object_color = get_object_color(rec);
	ambient_color = vec3_mult_vec(scene->ambient.color, object_color);
	return (vec3_mul(ambient_color, scene->ambient.ratio));
}

/**
 * @brief Clamps color components to the valid range [0.0, 1.0].
 *
 * @details This function ensures that the R, G, and B components 
 of a color vector
 * do not exceed the maximum or fall below the minimum values. 
 This is crucial for
 * preventing color overflow, which can occur after lighting 
 calculations and
 * produce invalid or distorted colors.
 *
 * @param color The `t_color` vector to be clamped.
 *
 * @return A `t_color` vector with its components clamped to 
 the [0.0, 1.0] range.
 */
t_color	clamp_color(t_color color)
{
	color.x = fmin(1.0, fmax(0.0, color.x));
	color.y = fmin(1.0, fmax(0.0, color.y));
	color.z = fmin(1.0, fmax(0.0, color.z));
	return (color);
}

/**
 * @brief Returns the effective normal vector for a hit point.
 *
 * @details This function checks if the hit object has a normal 
 map (also known as a
 * bump map) defined. If so, it retrieves the perturbed normal 
 vector from the map
 * to create a detailed surface appearance. Otherwise, it returns the standard
 * geometric normal of the object. This allows for surfaces to appear
 * textured and bumpy without requiring additional geometric complexity.
 *
 * @param rec A pointer to the hit record.
 *
 * @return A `t_vec3` representing the effective surface normal.
 */
t_vec3	get_effective_normal(t_hit_record *rec)
{
	if (rec->object->material && rec->object->material->has_normal_map)
		return (get_normal_from_map(rec, rec->normal));
	return (rec->normal);
}

/**
 * @brief Calculates the combined diffuse and specular light 
 contribution from a single light source.
 *
 * @details This function determines the total lighting effect of 
 one light source on a
 * specific hit point. It first checks for shadows by casting a 
 ray from the hit
 * point to the light. If the path is not blocked, it calculates 
 the **diffuse**
 * component (how light scatters) and the **specular** component 
 (shiny highlights)
 * and sums them.
 *
 * @param ctx A pointer to the `t_light_context` containing all 
 necessary data.
 * @param light A pointer to the `t_light` source to evaluate.
 *
 * @return A `t_color` vector representing the light's contribution. 
 Returns black if the point is in shadow.
 */
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

/**
 * @brief Calculates the total direct lighting from all light s
 ources in the scene.
 *
 * @details This function iterates through every light source 
 defined in the scene
 * and sums their individual contributions. For each light, it calls
 * `get_light_contribution` to determine the combined diffuse 
 and specular lighting,
 * taking into account potential shadows. The final result is the total
 * illumination from all direct light sources.
 *
 * @param rec A pointer to the hit record.
 * @param scene A pointer to the scene structure.
 * @param ray A pointer to the primary ray that hit the object.
 * @param effective_normal The surface normal (potentially 
 from a normal map) at the hit point.
 *
 * @return A `t_color` vector representing the total direct lighting.
 */
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
