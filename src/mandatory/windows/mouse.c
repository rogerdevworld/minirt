/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Adjusts the camera's field of view (FOV) for zooming.
 *
 * @details This function modifies the camera's FOV to simulate 
 zooming in or out.
 * It clamps the FOV value between a minimum of 10 and a 
 maximum of 170 to prevent
 * visual distortions or invalid camera states.
 *
 * @param data A pointer to the main data structure containing 
 the scene and camera information.
 * @param zoom_factor The value by which to change the FOV. A 
 positive value zooms in,
 * a negative value zooms out.
 *
 * @return void.
 */
void	ft_zoom(t_data *data, double zoom_factor)
{
	data->scene.camera.fov -= zoom_factor;
	if (data->scene.camera.fov < 10)
		data->scene.camera.fov = 10;
	if (data->scene.camera.fov > 170)
		data->scene.camera.fov = 170;
}

/**
 * @brief Initializes the mouse state data structure.
 *
 * @details This function sets the initial state of the mouse to a default,
 * non-pressed state with coordinates at (0, 0). This is typically called
 * during program setup to ensure a clean starting point for mouse 
 event handling.
 *
 * @param data A pointer to the main data structure, which 
 contains the mouse state.
 *
 * @return void.
 */
void	init_mouse_state(t_data *data)
{
	data->mouse_state.is_pressed = false;
	data->mouse_state.last_x = 0.0;
	data->mouse_state.last_y = 0.0;
}
