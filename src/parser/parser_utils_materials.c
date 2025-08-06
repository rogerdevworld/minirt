/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_materials.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:56:44 by jaacosta          #+#    #+#             */
/*   Updated: 2025/08/06 20:56:45 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

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

void	parse_checkboard(char *token, t_object *obj)
{
	char	**parts;

    parts = ft_split(token + 4, ';');
	if (ft_strarr_len(parts) != 3)
        ft_error_exit("Error: Invalid checkerboard format");
	obj->check_color1 = parse_vec3_color(parts[0]);
	obj->check_color2 = parse_vec3_color(parts[1]);
	obj->check_scale = ft_atod(parts[2]);
	obj->has_checkerboard = 1;
	ft_free_str_array(parts);
}

char	*parse_bump_map(char *token)
{
	return (ft_strdup(token + 4));
}

void	apply_object_modifiers(t_object *obj, char **tokens, int start_idx)
{
	int i;

	i = start_idx;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "spc:", 4) == 0)
			parse_specular(tokens[i], &obj->specular);
		else if (ft_strncmp(tokens[i], "mir:", 4) == 0)
			obj->mirror_ratio = parse_mirror(tokens[i]);
		else if (ft_strncmp(tokens[i], "chk:", 4) == 0)
			parse_checkboard(tokens[i], obj);
		else if (ft_strncmp(tokens[i], "bmp:", 4) == 0)
			obj->bump_map_path = parse_bump_map(tokens[i]);
		i++;
	}
}