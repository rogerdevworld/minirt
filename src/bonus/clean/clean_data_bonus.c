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

static void	free_one_light(t_light *light)
{
	if (!light)
		return ;
	free(light);
}

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

void	cleanup_program(t_data *data)
{
	free_scene_objects(&data->scene);
	free_scene_lights(&data->scene);
	if (data->mlx.img.img_ptr)
		mlx_delete_image(data->mlx.mlx_ptr, data->mlx.img.img_ptr);
	if (data->mlx.mlx_ptr)
		mlx_terminate(data->mlx.mlx_ptr);
}
