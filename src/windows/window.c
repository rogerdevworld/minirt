/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:34:31 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/19 19:07:39 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"
#include <stdlib.h> // Para exit, malloc/free

// Función para poner un píxel en la imagen MLX
// Asume que img->width y img->height se inicializan con el ancho y alto de la escena
void put_pixel_to_img(t_img *img, int x, int y, int color)
{
    char    *dst;

    // Asegúrate de que los límites estén bien definidos para tu imagen
    // Es CRUCIAL que img->width e img->height estén inicializados en mlx_setup
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;

    dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int*)dst = color;
}

// Inicialización de MiniLibX y configuración de la ventana/imagen
void mlx_setup(t_data *data)
{
    // Asegúrate de inicializar los punteros a NULL antes de mlx_init,
    // para que Valgrind no se queje si mlx_init falla y se intenta liberar.
    data->mlx.mlx_ptr = NULL;
    data->mlx.win_ptr = NULL;
    data->mlx.img.img_ptr = NULL;

    data->mlx.mlx_ptr = mlx_init();
    if (!data->mlx.mlx_ptr)
    {
        printf("Error\nNo se pudo inicializar MiniLibX.\n");
        exit(1); // O usa ft_error_exit
    }

    data->mlx.win_ptr = mlx_new_window(data->mlx.mlx_ptr,
                                       data->scene.width,
                                       data->scene.height,
                                       "miniRT");
    if (!data->mlx.win_ptr)
    {
        // Si la ventana falla, limpia mlx_ptr antes de salir
        mlx_destroy_display(data->mlx.mlx_ptr); // Usa destroy_display
        printf("Error\nNo se pudo crear la ventana.\n");
        exit(1); // O usa ft_error_exit
    }

    data->mlx.img.img_ptr = mlx_new_image(data->mlx.mlx_ptr,
                                           data->scene.width,
                                           data->scene.height);
    if (!data->mlx.img.img_ptr)
    {
        // Si la imagen falla, limpia ventana y mlx_ptr
        mlx_destroy_window(data->mlx.mlx_ptr, data->mlx.win_ptr);
        mlx_destroy_display(data->mlx.mlx_ptr);
        printf("Error\nNo se pudo crear la imagen MLX.\n");
        exit(1); // O usa ft_error_exit
    }

    // Obtener la dirección de memoria de la imagen para escribir píxeles
    data->mlx.img.addr = mlx_get_data_addr(data->mlx.img.img_ptr,
                                           &data->mlx.img.bpp,
                                           &data->mlx.img.line_len,
                                           &data->mlx.img.endian);

    // Es crucial inicializar width y height de la imagen para put_pixel_to_img
    data->mlx.img.width = data->scene.width;
    data->mlx.img.height = data->scene.height;
}

// Función para cerrar la ventana y liberar recursos
// Asumimos que esta es tu función para el hook MLX_ON_DESTROY (evento 17)
int close_window(t_data *data)
{
    // Destruye la ventana si existe
    if (data->mlx.win_ptr)
        mlx_destroy_window(data->mlx.mlx_ptr, data->mlx.win_ptr);
    // Destruye la imagen si existe
    if (data->mlx.img.img_ptr)
        mlx_destroy_image(data->mlx.mlx_ptr, data->mlx.img.img_ptr);
    
    // Destruye la conexión con el display de MLX. Esto libera mlx_ptr.
    // ESTO ES CRUCIAL. No uses free(data->mlx.mlx_ptr) directamente.
    if (data->mlx.mlx_ptr) // Asegúrate de que mlx_ptr es válido antes de destruirlo
        mlx_destroy_display(data->mlx.mlx_ptr);
    
    // Aquí puedes añadir la limpieza de tus listas de objetos y luces
    // free_scene_data(&data->scene); // Si tienes una función para esto

    // Destruye el mutex de progreso si aún no se ha destruido.
    // Nota: Si render_threaded ya lo destruye, podrías quitarlo de aquí,
    // pero si sales por la X de la ventana antes de que render_threaded termine
    // (lo cual no debería pasar si el loop está después), podría ser necesario.
    // En este caso, como render_threaded lo destruye, NO lo pongas aquí.

    exit(0); // Termina el programa
    return (0); // No se debería alcanzar
}

// Ejemplo de key_hook para cerrar con ESC
int key_hook(int keycode, t_data *data)
{
    if (keycode == 65307) // ESC keycode
        close_window(data);
    return (0);
}
