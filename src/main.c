/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minirt.h"

// Función para liberar la memoria de la escena (objetos y luces)
void	free_scene_data(t_scene *scene)
{
	t_object	**objects;
	//int			i;
	t_light		**lights;
	int			i;

	if (scene->objects)
	{
		objects = (t_object **)scene->objects;
		i = 0;
		while (objects[i])
		{
			if (objects[i]->bump_map_path)
				free(objects[i]->bump_map_path);
			free(objects[i]->data);
			free(objects[i]);
			i++;
		}
		free(objects);
	}
	if (scene->lights)
	{
		lights = (t_light **)scene->lights;
		i = 0;
		while (lights[i])
		{
			free(lights[i]);
			i++;
		}
		free(lights);
	}
}

// Función que limpia todos los recursos del programa
void	cleanup_program(t_data *data)
{
	free_scene_data(&data->scene);
	if (data->mlx.mlx_ptr && data->mlx.img.img_ptr)
		mlx_destroy_image(data->mlx.mlx_ptr, data->mlx.img.img_ptr);
	if (data->mlx.mlx_ptr && data->mlx.win_ptr)
		mlx_destroy_window(data->mlx.mlx_ptr, data->mlx.win_ptr);
	if (data->mlx.mlx_ptr)
	{
		mlx_destroy_display(data->mlx.mlx_ptr);
		free(data->mlx.mlx_ptr);
	}
	pthread_mutex_destroy(&data->progress_mutex);
	ft_printf("MiniRT: Cleanup complete.\n");
}

int	init_data(t_data *data)
{
	data->scene.width = 1920;
	data->scene.height = 1080;
	data->scene.camera = camera_init(vec3_init(0, 0, 0), vec3_init(0, 0, 1),
			90);
	data->scene.ambient = ambient_light_init(0.1, vec3_init(1, 1, 1));
	data->scene.lights = NULL;
	data->scene.objects = NULL;
	data->scene.has_ambient = 0;
	data->scene.has_camera = 0;
	data->num_threads = get_num_processors();
	data->rendered_rows = 0;
	if (pthread_mutex_init(&data->progress_mutex, NULL) != 0)
	{
		ft_printf("MiniRT: Error: Bad Init Mutex\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data data;

	// if (argc != 2)
	//     return (ft_printf("Usage: ./minirt <scene.rt>\n"), 1);

	// 1.1 Init data
	if (init_data(&data) != 0)
		return (1);

	// 2. Parsear el archivo .rt y construir la escena
	parse_rt_file(&data.scene, argv[1]);

	// 2. Initialize Minilibx and create window/image
	mlx_setup(&data);

	// 3. Render the scene using threads
	render_threaded(&data);

	// 4. Set up event hooks
	// close_window ahora llama a cleanup_program
	mlx_hook(data.mlx.win_ptr, 17, 0, close_window, &data);
	mlx_key_hook(data.mlx.win_ptr, key_hook, &data);

	// 5. Start the Minilibx loop to keep the window open
	mlx_loop(data.mlx.mlx_ptr);

	return (0);
}