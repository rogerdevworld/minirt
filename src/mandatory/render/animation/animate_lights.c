/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animate_lights.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Animates a light in a circular path.
 *
 * @details This function updates a light's position to simulate 
 circular movement
 * around a fixed point. It uses sine and cosine functions to calculate the new
 * X and Z coordinates based on a given time value.
 *
 * @param light A pointer to the light structure to animate.
 * @param time The current time value used for the animation calculation.
 *
 * @return void.
 */
static void	animate_circular(t_light *light, double time)
{
	double	radius;
	double	speed;

	radius = 10.0;
	speed = 1.0;
	light->position.x = radius * cos(time * speed);
	light->position.z = radius * sin(time * speed);
}

/**
 * @brief Animates a light in a linear back-and-forth path.
 *
 * @details This function moves a light along a straight line, bouncing it
 * back and forth between two points. It uses a sine function to create a
 * smooth, oscillating movement.
 *
 * @param light A pointer to the light structure to animate.
 * @param time The current time value used for the animation calculation.
 *
 * @return void.
 */
static void	animate_linear(t_light *light, double time)
{
	double	start_pos;
	double	end_pos;
	double	travel_range;
	double	speed;

	start_pos = -10.0;
	end_pos = 10.0;
	travel_range = end_pos - start_pos;
	speed = 0.5;
	light->position.x = start_pos + travel_range * 0.5 * (1 + sin(time
				* speed));
}

/**
 * @brief Animates a light in a triangular path.
 *
 * @details This function moves a light along the sides of an 
 equilateral triangle.
 * It uses a normalized time value to determine which side of the triangle the
 * light is on and linearly interpolates its position between the two vertices
 * of that side.
 *
 * @param light A pointer to the light structure to animate.
 * @param time The current time value used for the animation calculation.
 *
 * @return void.
 */
static void	animate_triangular(t_light *light, double time)
{
	double	side_length;
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	p3;
	double	normalized_time;

	side_length = 15.0;
	p1 = vec3_init(0, 0, 0);
	p2 = vec3_init(side_length, 0, 0);
	p3 = vec3_init(side_length / 2, 0, side_length * sin(M_PI / 3));
	normalized_time = fmod(time, 3.0);
	if (normalized_time < 1.0)
		light->position = vec3_add(p1, vec3_mul(vec3_sub(p2, p1),
					normalized_time));
	else if (normalized_time < 2.0)
		light->position = vec3_add(p2, vec3_mul(vec3_sub(p3, p2),
					normalized_time - 1.0));
	else
		light->position = vec3_add(p3, vec3_mul(vec3_sub(p1, p3),
					normalized_time - 2.0));
}

/**
 * @brief Main animation loop for moving lights.
 *
 * @details This function is a callback that updates the position of all lights
 * in the scene that have a movement type assigned. It iterates through the list
 * of lights, calls the appropriate animation function based on the light's
 * `movement_type`, and then triggers a re-render of the scene.
 *
 * @param param A void pointer to the main data structure (`t_data`).
 *
 * @return void.
 */
void	animate_lights(void *param)
{
	t_data	*data;
	t_light	**lights;
	int		i;

	data = (t_data *)param;
	if (!data->scene.lights || data->scene.num_lights == 0)
		return ;
	lights = (t_light **)data->scene.lights;
	data->light_move_time += 0.05;
	i = 0;
	while (i < data->scene.num_lights)
	{
		if (lights[i]->movement_type == MOVEMENT_CIRCLE)
			animate_circular(lights[i], data->light_move_time);
		else if (lights[i]->movement_type == MOVEMENT_TRIANGLE)
			animate_triangular(lights[i], data->light_move_time);
		else if (lights[i]->movement_type == MOVEMENT_LINE)
			animate_linear(lights[i], data->light_move_time);
		i++;
	}
	render_threaded(data);
}
