/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Adds a new object to the scene's list of objects.
 *
 * @details This function dynamically resizes the array of object pointers to
 * accommodate a new object. It allocates a new array that is one size larger,
 * copies all existing object pointers to the new array, frees the old array,
 * and then adds the new object pointer to the end of the list. The array is
 * always kept null-terminated to facilitate iteration.
 *
 * @param scene A pointer to the scene structure.
 * @param obj The new object pointer to be added.
 *
 * @return void.
 */
void	add_object_to_scene(t_scene *scene, t_object *obj)
{
	int			count;
	t_object	**new_objects;

	count = 0;
	if (scene->objects)
	{
		while (((t_object **)scene->objects)[count])
			count++;
	}
	new_objects = (t_object **)malloc(sizeof(t_object *) * (count + 2));
	if (!new_objects)
		ft_error_exit("MiniRT: Error: Malloc failed for objects");
	if (scene->objects)
	{
		ft_memcpy(new_objects, scene->objects, sizeof(t_object *) * (count
				+ 1));
		free(scene->objects);
	}
	new_objects[count] = obj;
	new_objects[count + 1] = NULL;
	scene->objects = new_objects;
}

/**
 * @brief Adds a new light source to the scene's list of lights.
 *
 * @details This function dynamically resizes the array of light pointers to
 * add a new light source. It first counts the existing lights, allocates a
 * new array that is one size larger, copies the existing light pointers,
 * and then frees the old array. The new light is then added, and the array
 * is null-terminated. This function also increments the total number of
 * lights in the scene.
 *
 * @param scene A pointer to the scene structure.
 * @param light The new light pointer to be added.
 *
 * @return void.
 */
void	add_light_to_scene(t_scene *scene, t_light *light)
{
	int		count;
	t_light	**new_lights;

	count = 0;
	if (scene->lights)
		while (((t_light **)scene->lights)[count])
			count++;
	new_lights = (t_light **)malloc(sizeof(t_light *) * (count + 2));
	if (!new_lights)
		ft_error_exit("MiniRT: Error: Malloc failed for lights");
	if (scene->lights)
	{
		ft_memcpy(new_lights, scene->lights, sizeof(t_light *) * (count + 1));
		free(scene->lights);
	}
	new_lights[count] = light;
	new_lights[count + 1] = NULL;
	scene->lights = new_lights;
	scene->num_lights++;
}
