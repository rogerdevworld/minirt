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

// Añade un objeto de forma dinámica a la escena
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

// Añade una luz de forma dinámica a la escena
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
