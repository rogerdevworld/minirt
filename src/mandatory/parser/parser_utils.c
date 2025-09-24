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
#include "../../../include/minirt.h"

/**
 * @brief Parses a string into a positive floating-point number.
 *
 * @details This function trims leading/trailing whitespace 
 from a string, converts it to a
 * double, and then validates that the value is strictly 
 greater than zero. It exits
 * with an error if the string is empty, the memory allocation 
 fails, or the value
 * is not positive.
 *
 * @param str The string to be parsed.
 *
 * @return The validated positive double value.
 */
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

/**
 * @brief Parses and validates a field of view (FOV) value.
 *
 * @details This function takes a string, trims it, and 
 converts it to a double.
 * It ensures the FOV value is within a valid range of **0 to 180 degrees**,
 * which is a standard constraint for a camera's field of view. The function
 * will exit with an error for any invalid input, including a 
 null string or a
 * value outside the acceptable range.
 *
 * @param str The string containing the FOV value.
 *
 * @return The validated FOV value.
 */
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

/**
 * @brief Parses a string into a 3D vector.
 *
 * @details This function splits a comma-separated string 
 (e.g., "x,y,z") into
 * its three numerical components. It validates the format to 
 ensure there are
 * exactly three components before converting them to doubles and c
 reating a `t_vec3`
 * structure. It is used to parse positions, orientations, 
 and other 3D vectors.
 *
 * @param str The string to be parsed.
 *
 * @return A `t_vec3` structure with the parsed coordinates.
 */
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

/**
 * @brief Parses a string into a 3D vector and validates its 
 normalized range.
 *
 * @details Similar to `parse_vec3`, this function splits a 
 comma-separated string
 * and converts it into a `t_vec3`. However, it includes an 
 additional validation
 * step to ensure that each component of the vector is within 
 the **[-1, 1]**
 * range, which is a common requirement for normalized vectors 
 (like surface normals
 * or camera orientations).
 *
 * @param str The string to be parsed.
 *
 * @return A `t_vec3` structure with the parsed and validated coordinates.
 */
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
	if (vec.x < -1 || vec.x > 1 || vec.y < -1 || vec.y > 1 || vec.z < -1
		|| vec.z > 1)
		ft_error_exit("MiniRT: Error: Vector components\
			normalized must be between -1 and 1");
	return (vec);
}

/**
 * @brief Parses a string into a 3D vector representing a color.
 *
 * @details This function is specifically for parsing RGB c
 olor values from a string
 * like "R,G,B". It splits the string, validates the format, 
 and calls a helper
 * function to validate and convert each color component 
 (0-255) into a floating-point
 * value suitable for calculations. The returned `t_vec3` 
 represents the color
 * components in a float-based format.
 *
 * @param str The string containing the color values.
 *
 * @return A `t_vec3` structure representing the parsed color.
 */
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
