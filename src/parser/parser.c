#include "../../include/minirt.h"

static void	init_scene(t_scene *scene)
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

void	parse_rt_file(t_scene *scene, const char *file_path)
{
	int fd;
	char *line;
	char **tokens;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		ft_error_exit("MiniRT: Error: Cannot open file");

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

		ft_free_str_array(tokens);
		free(line);
	}
	close(fd);
	if (!scene->has_camera || !scene->has_ambient)
		ft_error_exit("MiniRT: Error: A camera and ambient light are required.");
}