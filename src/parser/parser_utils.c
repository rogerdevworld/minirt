/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// Validadcion para los valores doubles que deben ser positivos
double	parse_positive_double(char *str)
{
	double	val;
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\r\n");
	if (!trimmed)
		ft_error_exit("MiniRT: Error: Memory allocation failed");
	if (trimmed[0] == '\0')
		ft_error_exit("MiniRT: Error: Empty numeric value");
	val = ft_atod(trimmed);
	free(trimmed);
	if (val <= 0)
		ft_error_exit("MiniRT: Error: Incorrect data values");
	return (val);
}

// Validacion del valor fov de la camara
double	parse_fov(char *str)
{
	double	fov;
	char	*trimmed;

	fov = 0.0;
	if (!str)
		ft_error_exit("MiniRT: Error: FOV string is NULL");
	trimmed = ft_strtrim(str, " \r\t\n");
	if (!trimmed)
		ft_error_exit("MiniRT: Error: allocating memory failed in parse_fov");
	fov = ft_atod(trimmed);
	free(trimmed);
	if (fov < 0 || fov > 180)
		ft_error_exit("MiniRT: Error: FOV must be in range (0, 180)");
	return (fov);
}

// Convierte un string "x,y,z" a un t_vec3
t_vec3	parse_vec3(char *str)
{
	t_vec3	vec;
	char	**coords;

	coords = ft_split(str, ',');
	if (ft_strarr_len(coords) != 3)
		ft_error_exit("MiniRT: Error: Invalid vector format");
	vec.x = ft_atod(coords[0]);
	vec.y = ft_atod(coords[1]);
	vec.z = ft_atod(coords[2]);
	ft_free_str_array(coords);
	return (vec);
}

// valida que el vector "x,y,z" se encuentre normalizado entre [-1,1]
t_vec3	parse_vec3_normalized(char *str)
{
	t_vec3	vec;
	char	**coords;

	if (!str)
		ft_error_exit("MiniRT: Error: color string is NULL");
	coords = ft_split(str, ',');
	if (ft_strarr_len(coords) != 3)
	{
		ft_free_str_array(coords);
		ft_error_exit("MiniRT: Error: Invalid vector format");
	}
	vec.x = ft_atod(coords[0]);
	vec.y = ft_atod(coords[1]);
	vec.z = ft_atod(coords[2]);
	ft_free_str_array(coords);
	if (vec.x < -1 || vec.x > 1
		|| vec.y < -1 || vec.y > 1
		|| vec.z < -1 || vec.z > 1)
		ft_error_exit("MiniRT: Error: Vector components\
			normalized must be between -1 and 1");
	return (vec);
}

t_vec3	parse_vec3_color(char *str)
{
	t_vec3	color;
	char	**rgb;

	if (!str)
		ft_error_exit("MiniRT: Error: color string is NULL");
	rgb = ft_split(str, ',');
	if (ft_strarr_len(rgb) != 3)
	{
		ft_free_str_array(rgb);
		ft_error_exit("MiniRT: Error: Invalid color format");
	}
	color.x = validate_color_component(rgb[0]);
	color.y = validate_color_component(rgb[1]);
	color.z = validate_color_component(rgb[2]);
	ft_free_str_array(rgb);
	return (color);
}
