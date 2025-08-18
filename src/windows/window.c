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

int mlx_setup(t_data *data)
{
    // mlx_init() en MLX42 crea tanto la instancia de mlx como la ventana.
    // La anchura y altura se toman de la estructura de la escena.
    data->mlx.mlx_ptr = mlx_init(data->scene.width, data->scene.height, "miniRT", true);
    if (!data->mlx.mlx_ptr)
        return (1); // Fallo al inicializar la ventana y la mlx

    // mlx_new_image() crea un objeto de imagen.
    // Accedemos a img_ptr a través de las estructuras anidadas.
    data->mlx.img.img_ptr = mlx_new_image(data->mlx.mlx_ptr, data->scene.width, data->scene.height);
    if (!data->mlx.img.img_ptr)
    {
        mlx_terminate(data->mlx.mlx_ptr);
        return (1); // Fallo al crear la imagen
    }

    // mlx_image_to_window() asocia la imagen con la ventana.
    // Esto hace que la imagen se muestre.
    if (mlx_image_to_window(data->mlx.mlx_ptr, data->mlx.img.img_ptr, 0, 0) < 0)
    {
        mlx_terminate(data->mlx.mlx_ptr);
        return (1); // Fallo al mostrar la imagen
    }
    
    // Con MLX42 no se necesita mlx_get_data_addr, mlx_new_window, etc.
    // Simplemente trabajas directamente con el puntero de la imagen.
    
    return (0); // Éxito
}

// La función `cleanup_program` accede a `data->mlx.mlx_ptr`
void    cleanup_program(t_data *data)
{
    free_scene_data(&data->scene);
    if (data->mlx.mlx_ptr)
        mlx_terminate(data->mlx.mlx_ptr);
    pthread_mutex_destroy(&data->progress_mutex);
    ft_printf("MiniRT: Cleanup complete.\n");
}

// Función para cerrar la ventana y liberar recursos
// Asumimos que esta es tu función para el hook MLX_ON_DESTROY (evento 17)
int close_window(t_data *data)
{
    cleanup_program(data);
    exit(0);
    return (0);
}

// La función `key_hook` ahora accede a `data->mlx.mlx_ptr`
void key_hook(mlx_key_data_t keydata, void* param)
{
    t_data *data;

    data = (t_data *)param;
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        mlx_close_window(data->mlx.mlx_ptr);
    }
}