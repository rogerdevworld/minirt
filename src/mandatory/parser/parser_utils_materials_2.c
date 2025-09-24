/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_materials_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Allocates and copies a substring.
 *
 * @details This function creates a new,
	null-terminated string by copying a specified

	* number of characters from a source token. It's used to 
	extract a specific part of
 * a string token, for example,
 
	a file path. It performs dynamic memory allocation and
 * exits with a fatal error if the allocation fails.
 *
 * @param token The source string from which to copy.
 * @param len The number of characters to copy.
 *
 * @return A newly allocated,
	null-terminated string containing the copied characters.
 */
char	*copy_trimmed_token(char *token, int len)
{
	char	*trimmed;
	int		i;

	trimmed = (char *)malloc(len + 1);
	if (!trimmed)
		ft_error_exit("MiniRT: Error: allocating memory for bmp");
	i = 0;
	while (i < len)
	{
		trimmed[i] = token[i];
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}

/**
 * @brief Parses a bump map file path from a token and validates it.
 *

	* @details This function processes a string token to extract a file 
	path for a bump
 * map (normal map). It skips the initial "bmp:" prefix,
	copies the trimmed path, and

	* then validates the file's extension to ensure it is a .png or 
	.xpm file. It also
 * opens and closes the file to confirm its existence and accessibility, exiting
 * with an error if validation fails.
 *
 * @param token The string token containing the "bmp:" prefix and the file path.
 *
 * @return The validated and trimmed file path as a new string.
 */
char	*parse_bump_map(char *token)
{
	char	*path;
	int		len;
	int		fd;

	if (!token)
		ft_error_exit("MiniRT: Error: bump map token is missing");
	token += 4;
	len = 0;
	while (token[len] && token[len] != '\n' && token[len] != '\r'
		&& token[len] != ' ' && token[len] != '\t')
		len++;
	if (len <= 4)
		ft_error_exit("MiniRT: Error: bump map path is empty");
	path = copy_trimmed_token(token, len);
	if (!validate_texture_extension(path))
	{
		free(path);
		ft_error_exit("MiniRT: Error:  texture file \
            must have .png or .xpm extension");
	}
	fd = open_filename(path);
	validate_file(fd, path);
	close(fd);
	return (path);
}

/**
 * @brief Parses and applies basic object modifiers.
 *
 * @details This is a helper function that checks for and processes basic object
 * modifiers based on specific prefixes: "spc:" for specular properties, "mir:"
 * for mirror ratio, and "chk:" for checkerboard textures. It calls dedicated
 * parsing functions for each modifier.
 *
 * @param obj A pointer to the object to be modified.
 * @param tokens The array of string tokens containing the modifiers.
 * @param i A pointer to the current index in the tokens array.
 *
 * @return void.
 */
static void	parse_basic_modifiers(t_object *obj, char **tokens, int *i)
{
	if (ft_strncmp(tokens[*i], "spc:", 4) == 0)
		parse_specular(tokens[*i], &obj->material->specular);
	else if (ft_strncmp(tokens[*i], "mir:", 4) == 0)
		obj->material->mirror_ratio = parse_mirror(tokens[*i]);
	else if (ft_strncmp(tokens[*i], "chk:", 4) == 0)
		parse_checkboard(tokens[*i], obj->material);
}

/**
 * @brief Parses and applies a texture or normal map to an object.
 *
 * @details This function handles the parsing of texture and bump map tokens. It
 * checks for either the "tex:" or "bmp:" prefix, extracts the file path, and
 * loads the image using `mlx_load_png`. Upon successful loading, it sets the
 * corresponding texture pointer and a boolean flag in the object's material,
 * and handles errors if the file cannot be loaded.
 *
 * @param obj A pointer to the object whose material will be modified.
 * @param token The string token containing the texture or bump map information.
 *
 * @return void.
 */
static void	parse_texture_modifier(t_object *obj, char *token)
{
	char	*path;
	void	**img_ptr;
	bool	*flag_ptr;

	if (ft_strncmp(token, "bmp:", 4) == 0)
	{
		img_ptr = (void **)&obj->material->texture_img;
		flag_ptr = &obj->material->has_normal_map;
	}
	else if (ft_strncmp(token, "tex:", 4) == 0)
	{
		img_ptr = (void **)&obj->material->color_img;
		flag_ptr = &obj->material->has_texture;
	}
	else
		return ;
	path = ft_strtrim(token + 4, " \t\n\r");
	*img_ptr = mlx_load_png(path);
	if (!*img_ptr)
	{
		free(path);
		ft_error_exit("Error: Failed to load PNG texture.");
	}
	*flag_ptr = true;
	free(path);
}

/**
 * @brief Applies all modifiers to an object from a list of tokens.
 *
 * @details This is the main function for applying object modifiers. It iterates
 * through the `tokens` array starting from a given index and calls a series of
 * helper functions (`parse_basic_modifiers`, `parse_texture_modifier`) to
 * process and apply each modifier to the object's properties.
 *
 * @param obj A pointer to the object to be modified.
 * @param tokens A null-terminated array of strings containing the modifiers.
 * @param start_idx The starting index of the modifiers in the tokens array.
 *
 * @return void.
 */
void	apply_object_modifiers(t_object *obj, char **tokens, int start_idx)
{
	int	i;

	i = start_idx;
	while (tokens[i])
	{
		parse_basic_modifiers(obj, tokens, &i);
		parse_texture_modifier(obj, tokens[i]);
		i++;
	}
}
