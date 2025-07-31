// src/render.c

#include "../include/minirt.h"

int color_to_int(t_color color)
{
    int r = (int)(color.x * 255.99); // Usamos .x, .y, .z si t_color es t_vec3
    int g = (int)(color.y * 255.99);
    int b = (int)(color.z * 255.99);

    // Asegurarse de que los valores están en el rango 0-255
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;

    return (0xFF000000 | (r << 16) | (g << 8) | b); // 0xFF para canal alfa si no lo usas
}

// Función que ejecuta cada hilo de renderizado
void    *render_thread_func(void *arg)
{
    t_thread_data *thread_data = (t_thread_data *)arg;
    t_data *data = thread_data->global_data; // Asegúrate de que es 'global_data'
    int y; // y es la fila actual
    int x; // x es la columna actual
    t_color pixel_color;

    y = thread_data->start_row;
    while (y < thread_data->end_row)
    {
        x = 0;
        while (x < data->scene.width)
        {
            // --- AQUÍ VA TU LÓGICA DE RAY TRACING ---
            // Por ahora, un simple gradiente para probar el renderizado
            double r_comp = (double)x / data->scene.width;
            double g_comp = (double)y / data->scene.height;
            double b_comp = 0.5 * ((double)x / data->scene.width + (double)y / data->scene.height);
            // Asegúrate de que los componentes no excedan 1.0 si los usas para cálculos
            if (b_comp > 1.0) b_comp = 1.0;

            pixel_color = (t_color){r_comp, g_comp, b_comp}; // t_vec3 alias para t_color
            
            // Convertir el color a formato int para MLX
            int mlx_color = color_to_int(pixel_color);

            // Poner el píxel en la imagen MLX
            // Asegúrate de que put_pixel_to_img está disponible (prototipo en window.h o minirt.h)
            put_pixel_to_img(&data->mlx.img, x, y, mlx_color);
            x++;
        }
        
        // --- Actualizar el progreso (protegido por mutex) ---
        pthread_mutex_lock(&data->progress_mutex);
        data->rendered_rows++;
        pthread_mutex_unlock(&data->progress_mutex);
        // ----------------------------------------------------
        y++;
    }
    return (NULL);
}

// Función principal que lanza los hilos de renderizado
void    render_threaded(t_data *data)
{
    pthread_t       threads[data->num_threads];
    t_thread_data   thread_data[data->num_threads];
    int             i;
    int             rows_per_thread;
    int             start_row;

    // Calcular cuántas filas procesará cada hilo
    rows_per_thread = data->scene.height / data->num_threads;
    
    i = 0;
    start_row = 0;
    while (i < data->num_threads)
    {
        thread_data[i].id = i;
        thread_data[i].start_row = start_row;
        // El último hilo toma las filas restantes para cubrir el total
        thread_data[i].end_row = (i == data->num_threads - 1) ? data->scene.height : start_row + rows_per_thread;
        thread_data[i].global_data = data; // Pasa el puntero a los datos globales

        // --- Crear el hilo (pasando NULL para atributos por defecto) ---
        if (pthread_create(&threads[i], NULL, render_thread_func, &thread_data[i]) != 0)
        {
            // Manejo de error: Fallo al crear un hilo
            printf("Error\nFallo al crear el hilo %d.\n", i);
            // Implementar limpieza y salida apropiada
            // Por ejemplo, un bucle para unirse a los hilos ya creados y luego exit
            while (--i >= 0)
                pthread_join(threads[i], NULL);
            pthread_mutex_destroy(&data->progress_mutex); // Destruir el mutex si aún no lo está
            exit(1);
        }
        start_row = thread_data[i].end_row;
        i++;
    }

    // --- Bucle para imprimir el progreso en la consola ---
    // Esto se ejecuta en el hilo principal mientras los hilos de renderizado trabajan
    while (data->rendered_rows < data->scene.height)
    {
        printf("\rRenderizando: %.2f%% completado...",
            ((double)data->rendered_rows / data->scene.height) * 100.0);
        fflush(stdout); // Forzar la impresión inmediata
        usleep(50000); // Esperar 50 milisegundos para no sobrecargar y ver la actualización
    }
    printf("\rRenderizado: 100.00%% completado!\n"); // Mensaje final

    // --- Esperar a que todos los hilos terminen ---
    i = 0;
    while (i < data->num_threads)
    {
        pthread_join(threads[i], NULL); // Esperar a que cada hilo termine
        i++;
    }

    // --- Destruir el mutex de progreso una vez que todos los hilos han terminado ---
    pthread_mutex_destroy(&data->progress_mutex);

    // Mover la imagen renderizada a la ventana MLX
    mlx_put_image_to_window(data->mlx.mlx_ptr, data->mlx.win_ptr, data->mlx.img.img_ptr, 0, 0);
}