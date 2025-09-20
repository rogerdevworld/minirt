/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_elements_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// Nuevo parser para Hiperboloide (hp)
void	parse_hyperboloid(t_scene *scene, char **tokens)
{
	t_hyperboloid	*hb;
	t_object		*obj;

	if (ft_strarr_len(tokens) < 7)
		ft_error_exit("Error: Hyperboloid format invalid");
	hb = malloc(sizeof(t_hyperboloid));
	if (!hb)
		ft_error_exit("Error: Memory allocation failed");
	hb->position = parse_vec3(tokens[1]);
	hb->axis = parse_vec3_normalized(tokens[2]);
	hb->radius_a = parse_positive_double(tokens[3]);
	hb->radius_b = parse_positive_double(tokens[4]);
	hb->height = parse_positive_double(tokens[5]);
	obj = create_object(HYPERBOLOID, hb, parse_vec3_color(tokens[6]));
	apply_object_modifiers(obj, tokens, 7);
	add_object_to_scene(scene, obj);
}

// Parser de Cámara (C)
void	parse_camera(t_scene *scene, char **tokens)
{
	if (scene->has_camera)
		ft_error_exit("Error: Multiple cameras not allowed");
	if (ft_strarr_len(tokens) != 4)
		ft_error_exit("Error: Camera format invalid");
	scene->camera.position = parse_vec3(tokens[1]);
	scene->camera.orientation = parse_vec3_normalized(tokens[2]);
	scene->camera.fov = parse_fov(tokens[3]);
	scene->has_camera = 1;
}

// Parser de Luz (L)
void	parse_light_movement(t_light *light, char **tokens, int num_tokens)
{
	if (ft_strcmp(tokens[4], "moving") == 0)
	{
		if (num_tokens == 6)
		{
			if (ft_strcmp(tokens[5], "circle") == 0)
				light->movement_type = MOVEMENT_CIRCLE;
			else if (ft_strcmp(tokens[5], "triangle") == 0)
				light->movement_type = MOVEMENT_TRIANGLE;
			else if (ft_strcmp(tokens[5], "line") == 0)
				light->movement_type = MOVEMENT_LINE;
			else
				ft_error_exit("Error: Unrecognized light movement type.");
		}
		else
			ft_error_exit("Error: 'moving' light requires a movement type.");
	}
	else
		ft_error_exit("Error: Unrecognized light option.");
}

void	parse_light(t_scene *scene, char **tokens)
{
	t_light	*light;
	int		num_tokens;

	num_tokens = ft_strarr_len(tokens);
	if (num_tokens < 4 || num_tokens > 6)
		ft_error_exit("Error: Light format invalid.");
	light = (t_light *)malloc(sizeof(t_light));
	if (!light)
		ft_error_exit("Error: Memory allocation failed.");
	light->position = parse_vec3(tokens[1]);
	light->brightness = ft_atod(tokens[2]);
	if (light->brightness < 0.0 || light->brightness > 1.0)
		ft_error_exit("Error: brightness must be between 0.0 and 1.0.");
	light->color = parse_vec3_color(tokens[3]);
	light->movement_type = MOVEMENT_NONE;
	if (num_tokens >= 5)
	{
		parse_light_movement(light, tokens, num_tokens);
	}
	add_light_to_scene(scene, light);
}

// Parser de Luz Ambiental (A)
void	parse_ambient(t_scene *scene, char **tokens)
{
	t_vec3	color;
	double	ratio;

	if (scene->has_ambient)
		ft_error_exit("Error: Multiple ambient lights not allowed");
	if (ft_strarr_len(tokens) != 3)
		ft_error_exit("Error: Ambient light format invalid");
	ratio = ft_atod(tokens[1]);
	if (ratio < 0.0 || ratio > 1.0)
		ft_error_exit("Error: Ambient ratio must be betwen 0.0 and 1.0");
	color = parse_vec3_color(tokens[2]);
	scene->ambient.ratio = ratio;
	scene->ambient.color = color;
	scene->has_ambient = 1;
}
