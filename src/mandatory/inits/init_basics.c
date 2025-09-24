/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_basics.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

void	init_scene(t_scene *scene)
{
	scene->width = 0;
	scene->height = 0;
	scene->has_camera = 0;
	scene->has_ambient = 0;
	scene->lights = NULL;
	scene->objects = NULL;
	scene->ambient.ratio = 0.0;
	scene->ambient.color = vec3_init(0, 0, 0);
	scene->camera.fov = 0.0;
	scene->camera.orientation = vec3_init(0, 0, 0);
	scene->camera.position = vec3_init(0, 0, 0);
}

t_material	*create_material(void)
{
	t_material	*material;

	material = malloc(sizeof(t_material));
	if (!material)
		ft_error_exit("Error: Memory allocation for material failed");
	ft_memset(material, 0, sizeof(t_material));
	material->specular.intensity = 0.0f;
	material->specular.shininess = 0;
	material->mirror_ratio = 0.0;
	material->has_checkerboard = 0;
	material->check_color1 = (t_vec3){0, 0, 0};
	material->check_color2 = (t_vec3){0, 0, 0};
	material->check_scale = 1.0;
	material->has_texture = false;
	material->has_normal_map = false;
	material->color_img = NULL;
	material->texture_img = NULL;
	return (material);
}

t_object	*create_object(t_object_type type, void *data, t_vec3 color)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		ft_error_exit("Error: Memory allocation for object failed");
	ft_memset(obj, 0, sizeof(t_object));
	obj->type = type;
	obj->data = data;
	obj->color = color;
	obj->material = create_material();
	return (obj);
}
