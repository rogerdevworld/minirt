#include "../include/minirt.h"

// Función para liberar la memoria de la escena (objetos y luces)
void	free_scene_data(t_scene *scene)
{
	t_object	**objects;
	int			i;
	t_light		**lights;

	if (scene->objects)
	{
		objects = (t_object **)scene->objects;
		i = 0;
		while (objects[i])
		{
			// Liberar la estructura de material
			if (objects[i]->material)
			{
				if (objects[i]->material->bump_map_path)
					free(objects[i]->material->bump_map_path);
				free(objects[i]->material);
			}
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

int init_data(t_data *data)
{
    // Inicializar todos los campos a cero, previene valores basura.
    init_scene(&data->scene);
    data->scene.width = 1920;
    data->scene.height = 1080;
    data->num_threads = get_num_processors();
    data->rendered_rows = 0;
    if (pthread_mutex_init(&data->progress_mutex, NULL) != 0)
    {
        ft_printf("MiniRT: Error: Bad Init Mutex\n");
        return (1);
    }
    return (0);
}

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 2)
        return (ft_printf("Usage: ./minirt <scene.rt>\n"), 1);

    if (init_data(&data) != 0)
        return (1);

    parse_rt_file(&data.scene, argv[1]);

    if (mlx_setup(&data) != 0)
        return (cleanup_program(&data), 1);

    render_threaded(&data);

    // Los hooks se registran usando el puntero a la instancia de mlx
    mlx_key_hook(data.mlx.mlx_ptr, key_hook, &data);

    // Iniciar el bucle de Minilibx
    mlx_loop(data.mlx.mlx_ptr);

    cleanup_program(&data);

    return (0);
}