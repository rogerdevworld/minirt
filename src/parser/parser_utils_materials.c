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

#include "../../include/minirt.h"

int	valid_extension_rt(const char *filename)
{
	int	len;

	len = 0;
	while (filename[len] != '\0')
		len++;
	return (len > 3 && filename[len - 3] == '.' && filename[len - 2] == 'r'
		&& filename[len - 1] == 't');
}

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

double	parse_mirror(char *token)
{
	return (ft_atod(token + 4));
}

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

int	validate_texture_extension(const char *file_name)
{
	int	len;

	if (!file_name)
		return (0);
	len = 0;
	while (file_name[len])
		len++;
	if ((len >= 4 && file_name[len - 4] == '.') && (file_name[len - 3] == 'p')
		|| (file_name[len - 3] == 'P') && (file_name[len - 2] == 'n')
		|| (file_name[len - 2] == 'N') && (file_name[len - 1] == 'g')
		|| (file_name[len - 1] == 'G'))
		return (1);
	if (len >= 4 && file_name[len - 4] == '.' && (file_name[len - 3] == 'x')
		|| (file_name[len - 3] == 'X') && (file_name[len - 2] == 'p')
		|| (file_name[len - 2] == 'P') && (file_name[len - 1] == 'm')
		|| (file_name[len - 1] == 'M'))
		return (1);
	return (0);
}
