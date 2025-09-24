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

/**
 * @brief Initializes the main scene data structure.
 *
 * @details This function sets all members of the `t_scene` struct to their
 * initial, default values. This is crucial for ensuring a clean state
 * before parsing a scene file or performing any rendering. It explicitly
 * sets all pointers to NULL and all numeric values to 0 or 0.0.
 *
 * @param scene A pointer to the `t_scene` structure to be initialized.
 *
 * @return void.
 */
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

/**
 * @brief Allocates and initializes a new material structure.
 *
 * @details This function allocates memory for a new `t_material` object.
 * It initializes the material's properties, such as specular intensity,
 * mirror ratio, and checkerboard/texture flags, to their default values.
 * It also sets the texture image pointers to NULL. If memory allocation fails,
 * the program will exit with an error.
 *
 * @return A pointer to the newly created and initialized `t_material` structure.
 */
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

/**
 * @brief Allocates and initializes a new object structure.
 *
 * @details This function is a factory for creating a new `t_object`. It
 * allocates memory for the object, sets its type, geometric data, and color.
 * It then calls `create_material` to attach a default material to the object.
 * The function will exit with an error if memory allocation fails.
 *
 * @param type The type of the object (e.g., sphere, plane, etc.).
 * @param data A void pointer to the specific geometric data for the object.
 * @param color The base color of the object.
 *
 * @return A pointer to the newly created and initialized `t_object`.
 */
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
