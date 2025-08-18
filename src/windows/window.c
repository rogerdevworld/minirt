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

// Función para poner un píxel en la imagen MLX
// Asume que img->width y img->height se inicializan con el ancho y alto de la escena
void put_pixel_to_img(t_img *img, int x, int y, int color)
{
    char    *dst;

    // printf("width: %i, heigght: %i", img->width, img->height);
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;

    dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int*)dst = color;
}

// Función para cerrar la ventana y liberar recursos
// Asumimos que esta es tu función para el hook MLX_ON_DESTROY (evento 17)
int close_window(t_data *data)
{
    cleanup_program(data);
    exit(0);
    return (0);
}

// Ejemplo de key_hook para cerrar con ESC
int key_hook(int keycode, t_data *data)
{
    if (keycode == 65307)
        close_window(data);
    return (0);
}