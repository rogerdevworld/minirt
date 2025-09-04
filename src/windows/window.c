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

int	mlx_setup(t_data *data)
{
	data->mlx.mlx_ptr = mlx_init(data->scene.width, data->scene.height,
			"miniRT", true);
	if (!data->mlx.mlx_ptr)
		return (1);
	data->mlx.img.img_ptr = mlx_new_image(data->mlx.mlx_ptr, 4060, 2160);
	if (!data->mlx.img.img_ptr)
	{
		mlx_terminate(data->mlx.mlx_ptr);
		return (1);
	}
	if (mlx_image_to_window(data->mlx.mlx_ptr, data->mlx.img.img_ptr, 0, 0) < 0)
	{
		mlx_terminate(data->mlx.mlx_ptr);
		return (1);
	}
	return (0);
}

// Función para cerrar la ventana y liberar recursos
// Asumimos que esta es tu función para el hook MLX_ON_DESTROY (evento 17)
int	close_window(t_data *data)
{
	cleanup_program(data);
	exit(0);
	return (0);
}

// La función `key_hook` ahora accede a `data->mlx.mlx_ptr`
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(data->mlx.mlx_ptr);
	}
}
