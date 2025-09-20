/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

int	open_filename(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: cannot open the file");
		exit(1);
	}
	return (fd);
}

void	validate_file(int fd, const char *file_name)
{
	char	buffer[1];
	ssize_t	b_read;

	b_read = read(fd, buffer, 1);
	if (b_read < 0)
	{
		if (errno == EISDIR)
			printf("Error: %s is not a file, is a directory\n", file_name);
		else
			perror("Error: cannot read the file");
		close(fd);
		exit(1);
	}
	else if (b_read == 0)
	{
		printf("Error: the file %s is empty\n", file_name);
		close(fd);
		exit(1);
	}
}

static void	parse_line(t_scene *scene, char *line)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens)
		ft_error_exit("MiniRT: Error: ft_split failed");
	if (ft_strcmp(tokens[0], "A") == 0)
		parse_ambient(scene, tokens);
	else if (ft_strcmp(tokens[0], "C") == 0)
		parse_camera(scene, tokens);
	else if (ft_strcmp(tokens[0], "L") == 0)
		parse_light(scene, tokens);
	else if (ft_strcmp(tokens[0], "sp") == 0)
		parse_sphere(scene, tokens);
	else if (ft_strcmp(tokens[0], "pl") == 0)
		parse_plane(scene, tokens);
	else if (ft_strcmp(tokens[0], "cy") == 0)
		parse_cylinder(scene, tokens);
	else if (ft_strcmp(tokens[0], "cn") == 0)
		parse_cone(scene, tokens);
	else if (ft_strcmp(tokens[0], "pb") == 0)
		parse_paraboloid(scene, tokens);
	else if (ft_strcmp(tokens[0], "hp") == 0)
		parse_hyperboloid(scene, tokens);
	ft_free_str_array(tokens);
}

static int	open_and_validate_file(const char *file_path)
{
	int	fd;

	if (!valid_extension_rt(file_path))
		ft_error_exit("MiniRT: Error: the file must be of type '*.rt'");
	fd = open_filename(file_path);
	validate_file(fd, file_path);
	close(fd);
	fd = open_filename(file_path);
	return (fd);
}

void	parse_rt_file(t_scene *scene, const char *file_path)
{
	int		fd;
	char	*line;

	fd = open_and_validate_file(file_path);
	line = get_next_line(fd);
	while (line != NULL)
	{
		parse_line(scene, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!scene->has_camera || !scene->has_ambient)
		ft_error_exit("MiniRT: Error: A camera and \
            ambient light are required.");
}
