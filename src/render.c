#include "../include/minirt.h"

int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(color.x * 255.99);
	g = (int)(color.y * 255.99);
	b = (int)(color.z * 255.99);
	if (r < 0)
		r = 0;
	if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	if (b > 255)
		b = 255;
	return (0xFF000000 | (r << 16) | (g << 8) | b);
}

/* src/render.c */
void *render_thread_func(void *arg)
{
    t_thread_data   *thread_data;
    t_data          *data;
    int             y;
    int             x;
    t_ray           ray;
    t_hit_record    rec;
    t_color         final_color;

    thread_data = (t_thread_data *)arg;
    data = thread_data->global_data;
    y = thread_data->start_row;
    while (y < thread_data->end_row)
    {
        x = 0;
        while (x < data->scene.width)
        {
            // 1. Generar el rayo para el píxel (x, y)
            ray = generate_ray(x, y, &data->scene);
            // 2. Encontrar la colisión más cercana
            rec = find_closest_hit(&ray, &data->scene);
            // 3. Calcular el color final del píxel
            if (rec.object != NULL)
            {
                final_color = calculate_light(&rec, &data->scene, &ray, 0);
            }
            else
            {
                final_color = data->scene.background_color; 
            }
            
            // 4. Convertir y pintar el píxel con la función de la nueva lib
            int mlx_color = color_to_int(final_color);
            // mlx_put_pixel es la forma correcta para MLX42
            mlx_put_pixel(data->mlx.img.img_ptr, x, y, mlx_color);
            x++;
        }
        pthread_mutex_lock(&data->progress_mutex);
        data->rendered_rows++;
        pthread_mutex_unlock(&data->progress_mutex);
        
        y++;
    }
    return (NULL);
}

/* src/render.c */
void    render_threaded(t_data *data)
{
    pthread_t threads[data->num_threads];
    t_thread_data thread_data[data->num_threads];
    int i;
    int rows_per_thread;
    int start_row;

    rows_per_thread = data->scene.height / data->num_threads;

    i = 0;
    start_row = 0;
    while (i < data->num_threads)
    {
        thread_data[i].id = i;
        thread_data[i].start_row = start_row;
        if (i == data->num_threads - 1)
            thread_data[i].end_row = data->scene.height;
        else
            thread_data[i].end_row = start_row + rows_per_thread;
        thread_data[i].global_data = data;
        if (pthread_create(&threads[i], NULL, render_thread_func,
                &thread_data[i]) != 0)
        {
            printf("Error\nFallo al crear el hilo %d.\n", i);
            while (--i >= 0)
                pthread_join(threads[i], NULL);
            pthread_mutex_destroy(&data->progress_mutex);
            exit(1);
        }
        ft_printf("row per threads: %i, start: %i, end: %i\n", rows_per_thread,
            thread_data[i].start_row, thread_data[i].end_row);
        start_row = thread_data[i].end_row;
        i++;
    }

    // --- Bucle para imprimir el progreso en la consola ---
    while (data->rendered_rows < data->scene.height)
    {
        printf("\rRenderizando: %.2f%% completado...",
            ((double)data->rendered_rows / data->scene.height) * 100.0);
        fflush(stdout);
        usleep(50000);
    }
    printf("\rRenderizado: 100.00%% completado!\n");
    i = 0;
    while (i < data->num_threads)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    // No necesitas destruir el mutex aquí, ya lo haces en `cleanup_program`
}