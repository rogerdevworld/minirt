/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parser_utils_materials.c                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/06 20:56:44 by jaacosta          #+#    #+#             */
/* Updated: 2025/08/06 20:56:45 by jaacosta         ###   ########.fr       */
/* */
/* ************************************************************************** */
#include "../../include/minirt.h"

int	valid_extension_rt(const char *filename)
{
	int	len;

	len = 0;
	while (filename[len] != '\0')
		len++;
	return (len > 3 && filename[len - 3] == '.'
			&& filename[len - 2] == 'r'
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
	if (len >= 4 && file_name[len - 4] == '.'
		&& ((file_name[len - 3] == 'p' || file_name[len - 3] == 'P')
		&& (file_name[len - 2] == 'n' || file_name[len - 2] == 'N')
		&& (file_name[len - 1] == 'g' || file_name[len - 1] == 'G')))
		return (1);
	if (len >= 4 && file_name[len - 4] == '.'
		&& ((file_name[len - 3] == 'x' || file_name[len - 3] == 'X')
		&& (file_name[len - 2] == 'p' || file_name[len - 2] == 'P')
		&& (file_name[len - 1] == 'm' || file_name[len - 1] == 'M')))
		return (1);
	return (0);
}

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
		ft_error_exit("MiniRT: Error: texture file must have .png or .xpm extension");
	}
	fd = open_filename(path);
	validate_file(fd, path);
	close(fd);
	return (path);
}

t_material	*create_material(void)
{
	t_material	*material;

	material = malloc(sizeof(t_material));
	if (!material)
		ft_error_exit("Error: Memory allocation for material failed");
	material->specular.intensity = 0.0f;
	material->specular.shininess = 0;
	material->mirror_ratio = 0.0;
	material->has_checkerboard = 0;
	material->check_color1 = (t_vec3){0, 0, 0};
	material->check_color2 = (t_vec3){0, 0, 0};
	material->check_scale = 1.0;
	material->bump_map_path = NULL;
	material->has_bump_map = 0;
	return (material);
}

t_object	*create_object(t_object_type type, void *data, t_vec3 color)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		ft_error_exit("Error: Memory allocation for object failed");
	obj->type = type;
	obj->data = data;
	obj->color = color;
	obj->material = create_material();
	return (obj);
}

void	apply_object_modifiers(t_object *obj, char **tokens, int start_idx)
{
	int	i;

	i = start_idx;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "spc:", 4) == 0)
			parse_specular(tokens[i], &obj->material->specular);
		else if (ft_strncmp(tokens[i], "mir:", 4) == 0)
			obj->material->mirror_ratio = parse_mirror(tokens[i]);
		else if (ft_strncmp(tokens[i], "chk:", 4) == 0)
			parse_checkboard(tokens[i], obj->material);
		else if (ft_strncmp(tokens[i], "bmp:", 4) == 0)
		{
			obj->material->bump_map_path = parse_bump_map(tokens[i]);
			obj->material->has_bump_map = 1;
		}
		i++;
	}
}