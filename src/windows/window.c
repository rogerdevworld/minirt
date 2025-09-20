/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// moving
// mlx_key_hook(data->mlx.mlx_ptr, key_hook, data);
// mlx_scroll_hook(data->mlx.mlx_ptr, scroll_hook, data);
// mlx_cursor_hook(data->mlx.mlx_ptr, mouse_move_hook, data);
// mlx_mouse_hook(data->mlx.mlx_ptr, mouse_press_hook, data);
// mlx_loop_hook(data->mlx.mlx_ptr, auto_rotate, data);
// mlx_loop_hook(data->mlx.mlx_ptr, animate_lights, data);
int	mlx_setup(t_data *data)
{
	data->mlx.mlx_ptr = mlx_init(data->scene.width, data->scene.height,
			"miniRT", true);
	if (!data->mlx.mlx_ptr)
		return (1);
	data->mlx.img.img_ptr = mlx_new_image(data->mlx.mlx_ptr, data->scene.width,
			data->scene.height);
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

int	close_window(t_data *data)
{
	cleanup_program(data);
	exit(0);
	return (0);
}
