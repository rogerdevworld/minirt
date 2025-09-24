/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_rotate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Animates the camera to automatically rotate around a central point.
 *
 * @details This function serves as a continuous animation loop for the camera.
 * It checks if the `auto_rotate_active` flag is set. If so, it incrementally
 * updates a rotation angle and uses this angle to calculate a new position for
 * the camera in a circular path. It also recalculates the camera's `forward`,
 * `right`, and `up` vectors to ensure it always faces the center of the
 * rotation. Finally, it triggers a re-render of the scene.
 *
 * @param param A void pointer to the main data structure (`t_data`).
 *
 * @return void.
 */
void	auto_rotate(void *param)
{
	t_data	*data;
	double	radius;
	t_vec3	center;

	data = (t_data *)param;
	if (!data->auto_rotate_active)
		return ;
	data->rotation_angle += 0.01;
	if (data->rotation_angle > 2 * M_PI)
		data->rotation_angle -= 2 * M_PI;
	radius = 10.0;
	center = vec3_init(0, 0, 0);
	data->scene.camera.position.x = center.x + radius
		* cos(data->rotation_angle);
	data->scene.camera.position.z = center.z + radius
		* sin(data->rotation_angle);
	data->scene.camera.forward = vec3_normalize(vec3_sub(center,
				data->scene.camera.position));
	data->scene.camera.right = vec3_normalize(vec3_cross(
				data->scene.camera.forward, (t_vec3){0, 1, 0}));
	data->scene.camera.up = vec3_normalize(vec3_cross(data->scene.camera.right,
				data->scene.camera.forward));
	render_threaded(data);
}
