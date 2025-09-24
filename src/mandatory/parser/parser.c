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
#include "../../../include/minirt.h"

/**
 * @brief Opens a file for reading and handles errors.
 *
 * @details This function attempts to open a file specified by `filename` in
 * read-only mode (`O_RDONLY`). If the `open()` system call fails, it prints
 * a descriptive error message using `perror` and terminates the program with
 * a failure status.
 *
 * @param filename The path to the file to be opened.
 *
 * @return The file descriptor of the opened file on success.
 */
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

/**
 * @brief Validates that a file is not a directory and is not empty.
 *
 * @details This function performs a basic validation check on an opened file
 * descriptor (`fd`). It attempts to read one byte to confirm the file is
 * readable and not empty. It handles specific errors, such as a file
 * descriptor pointing to a directory (`EISDIR`), and provides descriptive
 * error messages before exiting the program.
 *
 * @param fd The file descriptor of the opened file.
 * @param file_name The path to the file, used for error messages.
 *
 * @return void.
 */
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

/**
 * @brief Parses a single line from the scene file.
 *
 * @details This function tokenizes a line from the scene file, splitting it by
 * spaces. It then uses the first token as an identifier to dispatch to the
 * appropriate parsing function for a specific element (e.g., ambient light,
 * camera, sphere, etc.). After parsing, it frees the allocated tokens array.
 *
 * @param scene A pointer to the main scene data structure.
 * @param line The string representing a single line from the scene file.
 *
 * @return void.
 */
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

/**
 * @brief Performs initial validation on a file and returns its descriptor.
 *
 * @details This function is a wrapper that first validates the file's extension
 * to ensure it is of type "*.rt". It then opens the file, validates that it
 * is a valid, non-empty file, closes it, and reopens it to reset the file
 * position for subsequent reading. This ensures that the file is ready for
 * line-by-line parsing.
 *
 * @param file_path The path to the file to be opened and validated.
 *
 * @return The file descriptor of the newly opened file.
 */
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

/**
 * @brief Parses a scene from a ".rt" file.
 *
 * @details This is the main parsing function. It first calls
 * `open_and_validate_file` to ensure the file is valid and ready. It then
 * reads the file line by line using `get_next_line`, and for each line, it
 * calls `parse_line` to process the data. After the entire file has been
 * parsed, it closes the file descriptor and performs a final validation check
 * to ensure that both a camera and an ambient light have been defined, which
 * are essential components of the scene.
 *
 * @param scene A pointer to the main scene data structure to be populated.
 * @param file_path The path to the scene file to be parsed.
 *
 * @return void.
 */
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
