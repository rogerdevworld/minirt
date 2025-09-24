/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

/**
 * @brief Initializes the main data structure for the program.
 *
 * @details This function sets up the default values for the scene,
 * including dimensions and thread-related properties. It also
 * initializes a mutex for thread synchronization.
 *
 * @param data A pointer to the main data structure `t_data`.
 *
 * @return 0 on success, or 1 if the mutex initialization fails.
 */
int	init_data(t_data *data)
{
	init_scene(&data->scene);
	data->scene.lights = NULL;
	data->scene.num_lights = 0;
	data->scene.width = 1920;
	data->scene.height = 1080;
	data->num_threads = get_num_processors();
	data->rendered_rows = 0;
	data->auto_rotate_active = false;
	data->rotation_angle = 0.0;
	data->rotation_center = vec3_init(0, 0, 0);
	if (pthread_mutex_init(&data->progress_mutex, NULL) != 0)
		return (printf("MiniRT: Error: Bad Init Mutex\n"), 1);
	return (0);
}

/**
 * @brief Main function for the MiniRT program.
 *
 * @details This function handles the program's lifecycle, including
 * argument validation, data initialization, scene parsing, rendering,
 * and event loop management. It ensures proper cleanup upon exit.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * The second argument is expected to be the scene file path.
 *
 * @return 0 on successful execution, 1 on error (e.g., incorrect arguments,
 * parsing issues, or MLX setup failure).
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (printf("Usage: ./minirt <scene.rt>\n"), 1);
	if (init_data(&data) != 0)
		return (1);
	parse_rt_file(&data.scene, argv[1]);
	if (mlx_setup(&data) != 0)
		return (cleanup_program(&data), 1);
	render_threaded(&data);
	mlx_key_hook(data.mlx.mlx_ptr, key_hook, &data);
	mlx_loop(data.mlx.mlx_ptr);
	cleanup_program(&data);
	return (0);
}
