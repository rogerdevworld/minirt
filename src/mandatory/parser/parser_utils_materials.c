/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_materials.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Validates if a filename has a ".rt" extension.
 *
 * @details This function checks if the last three characters of a given filename
 * are ".rt". It's a simple utility for validating scene file names 
 in the program.
 *
 * @param filename The string representing the file path.
 *
 * @return 1 if the filename ends with ".rt", 0 otherwise.
 */
int	valid_extension_rt(const char *filename)
{
	int	len;

	len = 0;
	while (filename[len] != '\0')
		len++;
	return (len > 3 && filename[len - 3] == '.' && filename[len - 2] == 'r'
		&& filename[len - 1] == 't');
}

/**
 * @brief Parses and sets specular properties for an object.
 *
 * @details This function takes a string token representing specular properties,
 * splits it by a comma, and extracts the **intensity** and **shininess**. It
 * validates the format and exits with a fatal error if the token is invalid.
 * The parsed values are then stored in the `t_specular` structure.
 * @param token The string token containing the specular data 
 (e.g., "spc:0.5,100").
 * @param spec A pointer to the `t_specular` structure to be populated.
 *
 * @return void.
 */
void	parse_specular(char *token, t_specular *spec)
{
	char	**parts;

	parts = ft_split(token + 4, ',');
	if (ft_strarr_len(parts) != 2)
		ft_error_exit("Error: Invalid specular format");
	spec->intensity = ft_atod(parts[0]);
	spec->shininess = (int)ft_atod(parts[1]);
	ft_free_str_array(parts);
}

/**
 * @brief Parses and returns the mirror ratio of an object.
 *
 * @details This function extracts the mirror ratio value from a string token,
 * skipping the initial "mir:" prefix. It assumes the token's format is valid.
 *
 * @param token The string token containing the mirror ratio (e.g., "mir:0.8").
 *
 * @return The parsed mirror ratio as a double.
 */
double	parse_mirror(char *token)
{
	return (ft_atod(token + 4));
}

/**
 * @brief Parses and applies a checkerboard pattern to a material.
 *
 * @details This function processes a checkerboard token, splitting it by a
 * semicolon to extract two **colors** and a **scale** value. It validates the
 * format and then populates the material's checkerboard properties, enabling
 * the checkerboard flag.
 *
 * @param token The string token containing the checkerboard data
 * (e.g., "chk:255,0,0;0,255,0;10").
 * @param material A pointer to the `t_material` structure to be modified.
 *
 * @return void.
 */
void	parse_checkboard(char *token, t_material *material)
{
	char	**parts;

	parts = ft_split(token + 4, ';');
	if (ft_strarr_len(parts) != 3)
		ft_error_exit("MiniRT: Error: Invalid checkerboard format");
	material->check_color1 = parse_vec3_color(parts[0]);
	material->check_color2 = parse_vec3_color(parts[1]);
	material->check_scale = ft_atod(parts[2]);
	material->has_checkerboard = 1;
	ft_free_str_array(parts);
}

/**
 * @brief Validates if a filename has a supported image extension (.png or .xpm).
 *
 * @details This function checks if a given filename ends with a valid extension
 * for textures. It supports both ".png" and ".xpm" formats, performing a
 * case-insensitive comparison.
 *
 * @param file_name The string representing the file path.
 *
 * @return 1 if the extension is valid, 0 otherwise.
 */
int	validate_texture_extension(const char *file_name)
{
	int	len;

	if (!file_name)
		return (0);
	len = 0;
	while (file_name[len])
		len++;
	if ((len >= 4 && file_name[len - 4] == '.') && ((file_name[len - 3] == 'p'
				|| file_name[len - 3] == 'P') && (file_name[len - 2] == 'n'
				|| file_name[len - 2] == 'N') && (file_name[len - 1] == 'g'
				|| file_name[len - 1] == 'G')))
		return (1);
	if ((len >= 4 && file_name[len - 4] == '.') && ((file_name[len - 3] == 'x'
				|| file_name[len - 3] == 'X') && (file_name[len - 2] == 'p'
				|| file_name[len - 2] == 'P') && (file_name[len - 1] == 'm'
				|| file_name[len - 1] == 'M')))
		return (1);
	return (0);
}
