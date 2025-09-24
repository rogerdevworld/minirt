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
#include "../../../include/minirt.h"

/**
 * @brief Parses and adds a hyperboloid to the scene.
 *
 * @details This function processes an array of string tokens to extract the
 * necessary data for a hyperboloid object. It validates the number of tokens
 * and performs memory allocation for the hyperboloid's data and the object
 * wrapper. The function then parses the position, axis, radii, height,
	and color
 * from the tokens, and applies any optional modifiers before adding the object
 * to the scene's list of objects.
 *

	* @param scene A pointer to the scene structure where the new 
	object will be added.
 * @param tokens A null-terminated array of strings containing the hyperboloid's
 * properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses and initializes the scene's camera.
 *
 * @details This function processes the tokens for a camera definition. It
 * checks for the correct number of arguments and ensures that only one camera
 * is defined in the scene file to prevent a fatal error. It parses the camera's
 * position, orientation, and field of view (FOV) and stores them in the
 * scene's camera structure.
 *
 * @param scene A pointer to the scene structure where the camera will be set.
 * @param tokens A null-terminated array of strings containing the camera's
 * properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses and sets the movement type for a light source.
 *
 * @details This is a helper function that checks for optional light
 * movement parameters in the tokens. It validates the keyword "moving" and
 * then assigns the corresponding movement type (circle, triangle, or line)
 * to the light structure. If the movement type is not recognized or the
 * format is incorrect, the program will exit with an error.
 *

	* @param light A pointer to the light structure whose movement 
	type will be set.
 * @param tokens A null-terminated array of strings containing the light's
 * properties.
 * @param num_tokens The total number of tokens for the light definition.
 *
 * @return void.
 */
void	parse_light_movement(t_light *light, char **tokens, int num_tokens)
{
	if (ft_strcmp(tokens[4], "") == 0)
		return ;
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

/**
 * @brief Parses and adds a light source to the scene.
 *
 * @details This function processes the tokens for a light source. It
 * validates the number of tokens, allocates memory for the new light, and
 * parses its position, brightness, and color. It also validates that the
 * brightness is within the acceptable range of 0.0 to 1.0. If optional
 * movement parameters are present, it calls `parse_light_movement` to handle
 * them before adding the light to the scene's list of lights.
 *

	* @param scene A pointer to the scene structure where the new 
	light will be added.

	* @param tokens A null-terminated array of strings containing 
	the light's properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses and initializes the scene's ambient light.
 *

	* @details This function processes the tokens for the ambient light 
	definition. It
 * verifies the token count and checks if an ambient light has already been
 * defined, which would result in a fatal error. It parses the ambient light's
 * ratio and color, validating that the ratio is between 0.0 and 1.0 before
 * updating the scene's ambient light structure.
 *

	* @param scene A pointer to the scene structure where the ambient 
	light will be set.

	* @param tokens A null-terminated array of strings containing the 
	ambient light's
 * properties.
 *
 * @return void.
 */
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
