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
 * @brief Calculates the color contribution from a reflected ray.
 *
 * @details This function handles recursive ray tracing for mirrored surfaces.
 * It first checks if the object is reflective and if the recursion 
 depth limit has
 * been reached. If so, it returns black, terminating the recursion. 
 Otherwise, it
 * calculates a **reflected ray** using the surface normal and the 
 incident ray's direction.
 * It then casts this new ray into the scene to find the closest object it hits.
 * The function then recursively calls `calculate_light` on the new 
 hit point to get
 * the reflected color. If the reflected ray hits nothing, the 
 background color is used.
 * The final reflected color is scaled by the object's mirror ratio to simulate
 * different levels of reflectivity.
 *
 * @param rec A pointer to the hit record of the initial intersection.
 * @param scene A pointer to the scene data.
 * @param ray A pointer to the incident ray.
 * @param depth The current recursion depth.
 *
 * @return A `t_color` representing the reflected light's contribution.
 */
t_color	calculate_reflection(t_hit_record *rec, t_scene *scene, t_ray *ray,
		int depth)
{
	t_ray			reflected_ray;
	t_hit_record	reflected_rec;
	t_vec3			effective_normal;
	t_color			reflected_color;

	if (!rec->object->material || rec->object->material->mirror_ratio <= 0.0
		|| depth >= MAX_RECURSION_DEPTH)
		return (vec3_init(0, 0, 0));
	effective_normal = get_effective_normal(rec);
	reflected_ray.origin = vec3_add(rec->point, vec3_mul(effective_normal,
				EPSILON));
	reflected_ray.direction = vec3_reflect(ray->direction, effective_normal);
	reflected_rec = find_closest_hit(&reflected_ray, scene);
	if (reflected_rec.object != NULL)
		reflected_color = calculate_light(&reflected_rec, scene, &reflected_ray,
				depth + 1);
	else
		reflected_color = scene->background_color;
	return (vec3_mul(reflected_color, rec->object->material->mirror_ratio));
}

/**
 * @brief Calculates the diffuse light component at a hit point.
 *
 * @details This function determines the diffuse light contribution 
 from a single light
 * source. It is based on **Lambert's Law**, which states that the 
 diffuse brightness
 * of a surface is proportional to the cosine of the angle between the 
 surface normal
 * and the direction to the light source. It calculates the dot product of the
 * effective normal and the vector to the light. If the dot product is 
 positive (meaning
 * the surface is facing the light), it multiplies the light's color 
 and brightness with
 * the object's color to compute the final diffuse color.
 *
 * @param rec A pointer to the hit record.
 * @param light A pointer to the light source.
 * @param effective_normal The surface normal (potentially from a 
 normal map).
 * @param to_light The normalized vector pointing from the hit point 
 to the light.
 *
 * @return A `t_color` representing the diffuse light contribution, 
 or black if the
 * surface is not facing the light.
 */
t_color	calculate_diffuse_light(t_hit_record *rec, t_light *light,
		t_vec3 effective_normal, t_vec3 to_light)
{
	double	dot_prod;
	t_color	diffuse_color;
	t_color	object_color;

	dot_prod = vec3_dot(effective_normal, to_light);
	if (dot_prod > 0)
	{
		object_color = get_object_color(rec);
		diffuse_color = vec3_mult_vec(light->color, object_color);
		return (vec3_mul(diffuse_color, dot_prod * light->brightness));
	}
	return (vec3_init(0, 0, 0));
}

/**
 * @brief Checks if a hit point is in shadow from a specific light source.
 *
 * @details This function determines if an object is occluded from a 
 light source.
 * It does this by casting a **shadow ray** from the hit point towards 
 the light.
 * It performs a scene intersection test along this ray. If the ray hits any
 * object before reaching the light source, it means the hit point is in shadow,
 * and the function returns `1`. A small epsilon offset is used to prevent
 * self-intersection artifacts.
 *
 * @param ray A pointer to the ray that generated the hit point.
 * @param scene A pointer to the scene data.
 * @param light A pointer to the light source to test against.
 *
 * @return `1` if the hit point is in shadow, `0` otherwise.
 */
int	is_in_shadow(t_ray *ray, t_scene *scene, t_light *light)
{
	t_hit_record	current_hit;
	double			light_dist;
	t_vec3			shadow_ray_origin;
	t_ray			shadow_ray;

	shadow_ray_origin = vec3_add(ray->origin, vec3_mul(ray->direction,
				EPSILON));
	light_dist = vec3_length(vec3_sub(light->position, shadow_ray_origin));
	shadow_ray.origin = shadow_ray_origin;
	shadow_ray.direction = vec3_normalize(vec3_sub(light->position,
				shadow_ray_origin));
	current_hit = find_closest_hit(&shadow_ray, scene);
	if (current_hit.object != NULL && current_hit.t > EPSILON
		&& current_hit.t < light_dist)
		return (1);
	return (0);
}

/**
 * @brief Dispatches the intersection test to the correct object type.
 *
 * @details This is a general-purpose function that acts as a central hub for
 * object intersection tests. It inspects the `type` field of an object and
 * calls the corresponding specialized intersection function (e.g.,
 * `intersect_sphere`, `intersect_plane`, etc.). This approach simplifies the
 * main ray-tracing loop by abstracting the different geometric intersection
 * algorithms behind a single function call.
 *
 * @param obj A pointer to the object to be tested.
 * @param ray A pointer to the ray being cast.
 * @param rec A pointer to the hit record to be populated with 
 intersection details.
 *
 * @return void. The `rec` parameter is updated with the result.
 */
void	get_object_intersection(t_object *obj, t_ray *ray, t_hit_record *rec)
{
	rec->t = -1.0;
	if (obj->type == SPHERE)
		intersect_sphere(ray, (t_sphere *)obj->data, rec);
	else if (obj->type == PLANE)
		intersect_plane(ray, (t_plane *)obj->data, rec);
	else if (obj->type == CYLINDER)
		intersect_cylinder(ray, (t_cylinder *)obj->data, rec);
	else if (obj->type == CONE)
		intersect_cone(ray, (t_cone *)obj->data, rec);
	else if (obj->type == HYPERBOLOID)
		intersect_hyperboloid(ray, (t_hyperboloid *)obj->data, rec);
	else if (obj->type == PARABOLOID)
		intersect_paraboloid(ray, (t_parab *)obj->data, rec);
}

/**
 * @brief Main function to calculate the final color of a hit point.
 *
 * @details This function orchestrates the lighting and shading process
  for a single pixel.
 * It first checks for a valid hit. If no object is hit, it returns the
  scene's background color.
 * Otherwise, it calculates the **ambient light**, **direct lighting** 
 (including diffuse and
 * specular components) from all light sources, and recursively 
 calculates the **reflection**
 * for mirrored surfaces. These color contributions are combined to
  produce the final color.
 * The function also handles the recursive logic for reflections by 
 incrementing the depth
 * counter. Finally, it clamps the color values to a valid range to 
 prevent overflow.
 *
 * @param rec A pointer to the hit record.
 * @param scene A pointer to the scene data.
 * @param ray A pointer to the ray that hit the object.
 * @param depth The current recursion depth.
 *
 * @return The final `t_color` of the hit point.
 */
t_color	calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray,
		int depth)
{
	t_color	final_color;
	t_vec3	effective_normal;
	t_color	reflected_color;

	if (rec->object == NULL)
		return (scene->background_color);
	effective_normal = get_effective_normal(rec);
	final_color = calculate_ambient_light(rec, scene);
	final_color = vec3_add(final_color, calculate_direct_lighting(rec, scene,
				ray, effective_normal));
	if (rec->object->material && rec->object->material->mirror_ratio > 0.0
		&& depth < MAX_RECURSION_DEPTH)
	{
		reflected_color = calculate_reflection(rec, scene, ray, depth);
		final_color = vec3_add(vec3_mul(final_color, 1.0
					- rec->object->material->mirror_ratio), reflected_color);
	}
	return (clamp_color(final_color));
}

// // src/render/ft_light.c
// t_color	calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray,
// 		int depth)
// {
// 	t_color			final_color;
// 	t_color			ambient_color;
// 	t_light			**lights;
// 	int				i;
// 	t_vec3			effective_normal;
// 	t_color			object_color;
// 	t_vec3			to_light;
// 	t_ray			shadow_ray;
// 	double			dot_prod;
// 	t_color			diffuse_contribution;
// 	t_color			specular_color;
// 	t_ray			reflected_ray;
// 	t_hit_record	reflected_rec;
// 	t_color			reflected_color;

// 	lights = (t_light **)scene->lights;
// 	i = 0;
// 	if (rec->object == NULL)
// 		return (scene->background_color);
// 	if (rec->object->material && rec->object->material->has_normal_map)
// 		effective_normal = get_normal_from_map(rec, rec->normal);
// 	else
// 		effective_normal = rec->normal;
// 	object_color = get_object_color(rec);
// 	ambient_color = vec3_mult_vec(scene->ambient.color, object_color);
// 	ambient_color = vec3_mul(ambient_color, scene->ambient.ratio);
// 	final_color = ambient_color;
// 	while (lights[i] != NULL)
// 	{
// 		to_light = vec3_normalize(vec3_sub(lights[i]->position, rec->point));
// 		shadow_ray.origin = vec3_add(rec->point, vec3_mul(effective_normal,
// 					EPSILON));
// 		shadow_ray.direction = to_light;
// 		if (is_in_shadow(&shadow_ray, scene, lights[i]))
// 		{
// 			i++;
// 			continue ;
// 		}
// 		dot_prod = vec3_dot(effective_normal, to_light);
// 		if (dot_prod > 0)
// 		{
// 			diffuse_contribution = vec3_mult_vec(lights[i]->color,
// 					object_color);
// 			diffuse_contribution = vec3_mul(diffuse_contribution, dot_prod
// 					* lights[i]->brightness);
// 			final_color = vec3_add(final_color, diffuse_contribution);
// 		}
// 		if (rec->object->material
// 			&& rec->object->material->specular.intensity > 0.0)
// 		{
// 			specular_color = calculate_specular_light(rec, lights[i], ray);
// 			final_color = vec3_add(final_color, specular_color);
// 		}
// 		i++;
// 	}
// 	if (rec->object->material && rec->object->material->mirror_ratio > 0.0
// 		&& depth < MAX_RECURSION_DEPTH)
// 	{
// 		reflected_ray.origin = vec3_add(rec->point, vec3_mul(effective_normal,
// 					EPSILON));
// 		reflected_ray.direction = vec3_reflect(ray->direction,
// 				effective_normal);
// 		reflected_rec = find_closest_hit(&reflected_ray, scene);
// 		if (reflected_rec.object != NULL)
// 			reflected_color = calculate_light(&reflected_rec, scene,
// 					&reflected_ray, depth + 1);
// 		else
// 			reflected_color = scene->background_color;
// 		final_color = vec3_add(vec3_mul(final_color, 1.0
// 					- rec->object->material->mirror_ratio),
// 				vec3_mul(reflected_color, rec->object->material->mirror_ratio));
// 	}
// 	final_color.x = fmin(1.0, fmax(0.0, final_color.x));
// 	final_color.y = fmin(1.0, fmax(0.0, final_color.y));
// 	final_color.z = fmin(1.0, fmax(0.0, final_color.z));
// 	return (final_color);
// }

// // src/render/ft_light.c
// int	is_in_shadow(t_ray *shadow_ray, t_scene *scene, t_light *light)
// {
// 	t_hit_record	current_hit;
// 	t_object		**objects;
// 	double			light_dist;
// 	int				i;

// 	objects = (t_object **)scene->objects;
// 	i = 0;
// 	light_dist = vec3_length(vec3_sub(light->position, shadow_ray->origin));
// 	while (objects[i] != NULL)
// 	{
// 		current_hit.t = -1.0;
// 		if (objects[i]->type == SPHERE)
// 			intersect_sphere(shadow_ray, (t_sphere *)objects[i]->data,
// 				&current_hit);
// 		else if (objects[i]->type == PLANE)
// 			intersect_plane(shadow_ray, (t_plane *)objects[i]->data,
// 				&current_hit);
// 		else if (objects[i]->type == CYLINDER)
// 			intersect_cylinder(shadow_ray, (t_cylinder *)objects[i]->data,
// 				&current_hit);
// 		else if (objects[i]->type == CONE)
// 			intersect_cone(shadow_ray, (t_cone *)objects[i]->data,
// 				&current_hit);
// 		else if (objects[i]->type == HYPERBOLOID)
// 			intersect_hyperboloid(shadow_ray, (t_hyperboloid *)objects[i]->data,
// 				&current_hit);
// 		else if (objects[i]->type == PARABOLOID)
// 			intersect_paraboloid(shadow_ray, (t_parab *)objects[i]->data,
// 				&current_hit);
// 		if (current_hit.t > EPSILON && current_hit.t < light_dist)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }
