#include "../include/minirt.h"

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
		return (ft_printf("MiniRT: Error: Bad Init Mutex\n"), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (ft_printf("Usage: ./minirt <scene.rt>\n"), 1);
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
