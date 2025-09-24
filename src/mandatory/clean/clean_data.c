/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Frees a single object and its associated data.
 *
 * @details This is a helper function that safely deallocates all memory
 * associated with a single `t_object` structure. It first frees the material
 * and any textures it contains using MiniLibX functions, then frees the
 * object's specific data (e.g., sphere or plane data), and finally frees the
 * object structure itself. Pointers are set to NULL after being freed.
 *
 * @param obj The pointer to the object to be freed. Can be NULL.
 *
 * @return void.
 */
static void	free_one_object(t_object *obj)
{
	if (!obj)
		return ;
	if (obj->material)
	{
		if (obj->material->color_img)
		{
			mlx_delete_texture(obj->material->color_img);
			obj->material->color_img = NULL;
		}
		if (obj->material->texture_img)
		{
			mlx_delete_texture(obj->material->texture_img);
			obj->material->texture_img = NULL;
		}
		free(obj->material);
		obj->material = NULL;
	}
	if (obj->data)
	{
		free(obj->data);
		obj->data = NULL;
	}
	free(obj);
}

/**
 * @brief Frees all objects within the scene.
 *
 * @details This function iterates through the array of scene objects,
 * calling `free_one_object` on each one to ensure all memory for each object
 * is correctly deallocated. After all objects have been freed, it frees the
 * array of object pointers itself.
 *
 * @param scene A pointer to the scene structure containing the objects array.
 *
 * @return void.
 */
void	free_scene_objects(t_scene *scene)
{
	size_t	i;

	if (!scene->objects)
		return ;
	i = 0;
	while (scene->objects[i])
	{
		free_one_object(scene->objects[i]);
		i++;
	}
	free(scene->objects);
	scene->objects = NULL;
}

/**
 * @brief Frees a single light source.
 *
 * @details This is a helper function that deallocates the memory for a
 * `t_light` structure. It performs a null check to prevent errors if the
 * pointer is invalid.
 *
 * @param light The pointer to the light source to be freed.
 *
 * @return void.
 */
static void	free_one_light(t_light *light)
{
	if (!light)
		return ;
	free(light);
}

/**
 * @brief Frees all light sources in the scene.
 *
 * @details This function iterates through the array of light sources,
 * calling `free_one_light` on each one. After freeing each light, it frees
 * the array of light pointers itself.
 *
 * @param scene A pointer to the scene structure containing the lights array.
 *
 * @return void.
 */
void	free_scene_lights(t_scene *scene)
{
	size_t	i;

	if (!scene->lights)
		return ;
	i = 0;
	while (scene->lights[i])
	{
		free_one_light(scene->lights[i]);
		i++;
	}
	free(scene->lights);
	scene->lights = NULL;
}

/**
 * @brief Performs a full cleanup of all program resources.
 *
 * @details This is the main cleanup function for the program's exit. It
 * calls `free_scene_objects` and `free_scene_lights` to deallocate all
 * scene-related memory. It then frees the MiniLibX image and terminates
 * the MiniLibX instance, ensuring a clean shutdown and preventing memory leaks.
 *
 * @param data A pointer to the main program data structure.
 *
 * @return void.
 */
void	cleanup_program(t_data *data)
{
	free_scene_objects(&data->scene);
	free_scene_lights(&data->scene);
	if (data->mlx.img.img_ptr)
		mlx_delete_image(data->mlx.mlx_ptr, data->mlx.img.img_ptr);
	if (data->mlx.mlx_ptr)
		mlx_terminate(data->mlx.mlx_ptr);
}
