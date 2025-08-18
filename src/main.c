#include "../include/minirt.h"

// --- Liberación de memoria ---

static void	free_texture(t_texture *texture, t_mlx *mlx)
{
	if (texture->type == TEX_IMAGE && texture->img)
	{
		mlx_destroy_image(mlx->mlx_ptr, texture->img);
		texture->img = NULL;
	}
	if (texture->img_path)
	{
		free(texture->img_path);
		texture->img_path = NULL;
	}
}

// Function to free an object's content (for ft_lstclear)
static void	free_object_content(void *content)
{
	t_object	*obj;

	if (!content)
		return ;
	obj = (t_object *)content;
	// We can't use mlx_destroy_image here because we don't have the mlx_ptr
	// So we'll have to handle texture cleanup elsewhere or change the function signature
	// For now, let's just free the object's data
	free(obj->data);
	free(obj->material);
	free(obj);
}

void	free_scene_data(t_scene *scene, t_mlx *mlx)
{
	t_list		*current;
	t_object	*obj;

	// The previous loop was incorrect. Use ft_lstclear for a linked list.
	// The correct way to free a t_list is with ft_lstclear, but this doesn't
	// allow access to the mlx_ptr for textures.
	// A better approach is a custom loop to free textures first
	current = scene->objects;
	while (current)
	{
		obj = (t_object *)current->content;
		if (obj)
		{
			// Assuming t_object has a t_material
			if (obj->material)
			{
				free_texture(&obj->material->albedo, mlx);
				free_texture(&obj->material->normal_map, mlx);
			}
		}
		current = current->next;
	}
	// Now it's safe to clear the lists
	ft_lstclear(&scene->objects, free_object_content);
	ft_lstclear(&scene->lights, free);
}

void	cleanup_program(t_data *data)
{
	// The previous loop in free_scene_data was incorrect, so we've
	// refactored it to be a bit more robust.
	free_scene_data(&data->scene, &data->mlx);
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
	// Initialize all fields to zero.
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

void	mlx_setup(t_data *data)
{
	data->mlx.mlx_ptr = mlx_init();
	if (!data->mlx.mlx_ptr)
	{
		ft_printf("MiniRT: Error: No se pudo inicializar MiniLibX.\n");
		exit(1);
	}
	data->mlx.win_ptr = mlx_new_window(data->mlx.mlx_ptr, data->scene.width,
			data->scene.height, "miniRT");
	if (!data->mlx.win_ptr)
	{
		mlx_destroy_display(data->mlx.mlx_ptr);
		ft_printf("MiniRT: Error: No se pudo crear la ventana.\n");
		exit(1);
	}
	data->mlx.img.img_ptr = mlx_new_image(data->mlx.mlx_ptr, data->scene.width,
			data->scene.height);
	if (!data->mlx.img.img_ptr)
	{
		mlx_destroy_window(data->mlx.mlx_ptr, data->mlx.win_ptr);
		mlx_destroy_display(data->mlx.mlx_ptr);
		ft_printf("MiniRT: Error: No se pudo crear la imagen MLX.\n");
		exit(1);
	}
	data->mlx.img.addr = mlx_get_data_addr(data->mlx.img.img_ptr,
			&data->mlx.img.bpp, &data->mlx.img.line_len, &data->mlx.img.endian);
	data->mlx.img.width = data->scene.width;
	data->mlx.img.height = data->scene.height;
}
void load_textures(t_data *data)
{
    t_list      *current_obj_list;
    t_object    *obj;
    t_material  *material;

    if (!data->scene.objects)
        return;

    current_obj_list = data->scene.objects;
    while (current_obj_list)
    {
        obj = (t_object *)current_obj_list->content;
        if (!obj)
        {
            current_obj_list = current_obj_list->next;
            continue;
        }

        // Obtén el puntero al material según el tipo de objeto
        if (obj->type == SPHERE)
            material = &(((t_sphere *)obj->data)->material);
        else if (obj->type == PLANE)
            material = &(((t_plane *)obj->data)->material);
        else if (obj->type == CYLINDER)
            material = &(((t_cylinder *)obj->data)->material);
        else if (obj->type == CONE)
            material = &(((t_cone *)obj->data)->material);
        else if (obj->type == PARABOLOID)
            material = &(((t_parab *)obj->data)->material);
        else if (obj->type == HYPERBOLOID)
            material = &(((t_hyperboloid *)obj->data)->material);
        else
        {
            current_obj_list = current_obj_list->next;
            continue;
        }

        // A partir de aquí, usa 'material' en lugar de 'obj->material'
        if (material->normal_map.type == TEX_IMAGE && material->normal_map.img_path)
        {
            // Verifica y asigna la memoria para la imagen
            if (!material->normal_map.img)
            {
                material->normal_map.img = malloc(sizeof(t_img));
                if (!material->normal_map.img)
                    ft_error_exit("Error: Memory allocation for normal map image failed.");
            }

            // Carga la imagen
            material->normal_map.img->img_ptr = mlx_xpm_file_to_image(
                data->mlx.mlx_ptr,
                material->normal_map.img_path,
                &material->normal_map.img->width,
                &material->normal_map.img->height);

            if (!material->normal_map.img->img_ptr)
            {
                printf("Error: Failed to load normal map from path: %s\n", material->normal_map.img_path);
                free(material->normal_map.img);
                material->normal_map.img = NULL;
                ft_error_exit("Error: Could not load normal map image.");
            }

            // Obtén la dirección de los datos de la imagen
            material->normal_map.img->addr = mlx_get_data_addr(
                material->normal_map.img->img_ptr,
                &material->normal_map.img->bpp,
                &material->normal_map.img->line_len,
                &material->normal_map.img->endian);
        }
        current_obj_list = current_obj_list->next;
    }
}

int	main(int argc, char **argv)
{
	t_data data;

	if (argc != 2)
		return (ft_printf("Usage: ./minirt <scene.rt>\n"), 1);

	if (init_data(&data) != 0)
		return (1);
	parse_rt_file(&data.scene, argv[1]);

	mlx_setup(&data);

	load_textures(&data);

	render_threaded(&data);
	mlx_put_image_to_window(data.mlx.mlx_ptr, data.mlx.win_ptr,
		data.mlx.img.img_ptr, 0, 0);
	mlx_hook(data.mlx.win_ptr, 17, 0, close_window, &data);
	mlx_key_hook(data.mlx.win_ptr, key_hook, &data);
	mlx_loop(data.mlx.mlx_ptr);

	cleanup_program(&data);

	return (0);
}