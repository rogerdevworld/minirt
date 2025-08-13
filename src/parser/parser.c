/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 22:24:37 by jaacosta          #+#    #+#             */
/*   Updated: 2025/08/12 22:24:40 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

void	init_scene(t_scene *scene)
{
	scene->width = 0;
	scene->height = 0;
	scene->has_camera = 0;
	scene->has_ambient = 0;
	scene->lights = NULL;
	scene->objects = NULL;
	scene->ambient.ratio = 0.0;
	scene->ambient.color = vec3_init(0, 0, 0);
	scene->camera.fov = 0.0;
	scene->camera.orientation = vec3_init(0, 0, 0);
	scene->camera.position = vec3_init(0, 0, 0);
}

// Function to free token array
void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

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

void	parse_rt_file(t_scene *scene, const char *file_path)
{
	int		fd;
	char	*line;
	char	**tokens;

	if (!valid_extension_rt(file_path))
		ft_error_exit("MiniRT: Error: the file must be of type '*.rt'");
	fd = open_filename(file_path);
	validate_file(fd, file_path);
	close(fd);
	fd = open_filename(file_path);
	while ((line = get_next_line(fd)) != NULL)
	{
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
		free(line);
	}
	close(fd);
	if (!scene->has_camera || !scene->has_ambient)
		ft_error_exit("MiniRT: Error: A camera and \
			ambient light are required.");
}
