/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

/**
 * @brief Calculates the color of a single subpixel.
 *
 * @details This function is the core of the rendering loop for 
 antialiasing. It performs the fundamental ray tracing steps for one 
 tiny point within a pixel.
 *
 * 1. **Ray Generation**: It first generates a ray that passes through 
 the exact center of a given subpixel using `generate_antialiased_ray`.
 * 2. **Intersection**: It then calls `find_closest_hit` to determine 
 if this ray intersects with any object in the scene.
 * 3. **Shading**: If an intersection is found, it calls 
 `calculate_light` to compute the final color of the hit point, taking 
 into account lighting, shadows, and reflections. If no object is hit, 
 the function returns the scene's background color.
 * 4. **Progress Tracking**: A mutex is used to safely increment 
 a shared `ray_count` variable, which helps track rendering progress.
 *
 * @param data A pointer to the main program data structure.
 * @param sp A `t_subpixel_data` struct containing the coordinates 
 of the subpixel.
 *
 * @return The `t_color` of the single subpixel.
 */
t_color	calculate_subpixel_color(t_data *data, t_subpixel_data sp)
{
	t_ray			ray;
	t_hit_record	rec;
	t_color			subpixel_color;

	ray = generate_antialiased_ray(sp, &data->scene);
	pthread_mutex_lock(&data->progress_mutex);
	data->ray_count++;
	pthread_mutex_unlock(&data->progress_mutex);
	rec = find_closest_hit(&ray, &data->scene);
	if (rec.object != NULL)
		subpixel_color = calculate_light(&rec, &data->scene, &ray, 0);
	else
		subpixel_color = data->scene.background_color;
	return (subpixel_color);
}

/**
 * @brief Renders a single pixel by averaging the color of its 
 subpixels.
 *
 * @details This function implements a form of **supersampling 
 antialiasing**. It casts multiple rays for a single pixel on the 
 screen and then averages the colors returned by those rays to produce 
 the final pixel color. This process smooths out jagged edges and 
 improves image quality.
 *
 * 1. **Looping**: It uses nested loops to iterate through each 
 subpixel (`sp.sub_x` and `sp.sub_y`) within a given pixel (`x`, `y`). 
 The number of subpixels is defined by `SUBPIXEL_SAMPLES`.
 * 2. **Accumulation**: In each iteration, it calls 
 `calculate_subpixel_color` to get the color of the current subpixel 
 and adds it to a `final_color` accumulator.
 * 3. **Averaging**: The function returns the accumulated color, 
 which will be divided by the total number of subpixels 
 (`SUBPIXEL_SAMPLES * SUBPIXEL_SAMPLES`) in `render_row` to get 
 the average color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param data A pointer to the main program data structure.
 *
 * @return The sum of all subpixel colors for the current pixel.
 */
t_color	render_pixel(int x, int y, t_data *data)
{
	t_color			final_color;
	t_subpixel_data	sp;

	final_color = vec3_init(0.0, 0.0, 0.0);
	sp.x = x;
	sp.y = y;
	sp.sub_y = 0;
	while (sp.sub_y < SUBPIXEL_SAMPLES)
	{
		sp.sub_x = 0;
		while (sp.sub_x < SUBPIXEL_SAMPLES)
		{
			final_color = vec3_add(final_color, calculate_subpixel_color(data,
						sp));
			sp.sub_x++;
		}
		sp.sub_y++;
	}
	return (final_color);
}

/**
 * @brief Renders all pixels in a single row of the image.
 *
 * @details This function iterates through all the pixels in a single 
 horizontal row (`y`) and calculates their final colors.
 *
 * 1. **Pixel-by-Pixel**: It loops through each pixel in the row 
 using the `x` variable.
 * 2. **Color Calculation**: For each pixel, it calls `render_pixel` 
 to get the summed color from all subpixel samples.
 * 3. **Averaging**: The summed color is then divided by the total 
 number of subpixels to get the final averaged color for the pixel.
 * 4. **Drawing**: The calculated color is then converted to an integer 
 and drawn onto the image using `mlx_put_pixel`.
 *
 * @param data A pointer to the main program data structure.
 * @param y The y-coordinate of the row to be rendered.
 *
 * @return void. The image is updated directly.
 */
void	render_row(t_data *data, int y)
{
	t_color	final_color;
	int		x;

	x = 0;
	while (x < data->scene.width)
	{
		final_color = render_pixel(x, y, data);
		final_color = vec3_mul(final_color, 1.0 / (double)(SUBPIXEL_SAMPLES
					* SUBPIXEL_SAMPLES));
		mlx_put_pixel(data->mlx.img.img_ptr, x, y, color_to_int(final_color));
		x++;
	}
}

/**
 * @brief The main function for each rendering thread.
 *
 * @details This function serves as the entry point for each thread 
 created for rendering. It is responsible for a specific portion of 
 the image. This approach, known as **parallel rendering**, significantly 
 reduces the time required to generate the final image.
 *
 * 1. **Task Assignment**: Each thread receives a `t_thread_data` 
 struct as an argument, which specifies the `start_row` and `end_row` 
 it is responsible for.
 * 2. **Row Rendering**: The thread then enters a loop, calling 
 `render_row` for each row within its assigned range.
 * 3. **Progress Tracking**: A mutex is used to safely update a 
 shared `rendered_rows` counter. This allows the main program to track 
 the overall progress of the rendering job.
 * 4. **Thread Exit**: Once a thread has rendered all its assigned 
 rows, the function returns, and the thread terminates.
 *
 * @param arg A void pointer to a `t_thread_data` struct containing 
 thread-specific data.
 *
 * @return NULL on completion.
 */
void	*render_thread_func(void *arg)
{
	t_thread_data	*thread_data;
	t_data			*data;
	int				y;

	thread_data = (t_thread_data *)arg;
	data = thread_data->global_data;
	y = thread_data->start_row;
	while (y < thread_data->end_row)
	{
		render_row(data, y);
		pthread_mutex_lock(&data->progress_mutex);
		data->rendered_rows++;
		pthread_mutex_unlock(&data->progress_mutex);
		y++;
	}
	return (NULL);
}

// void	*render_thread_func(void *arg)
// {
// 	t_thread_data	*thread_data;
// 	t_data			*data;
// 	t_ray			ray;
// 	t_hit_record	rec;
// 	t_color			final_color;
// 	t_color			subpixel_color;
// 	double			inv_sub_sq;

// 	int y, x;
// 	int sub_x, sub_y;
// 	inv_sub_sq = 1.0 / (double)(SUBPIXEL_SAMPLES * SUBPIXEL_SAMPLES);
// 	thread_data = (t_thread_data *)arg;
// 	data = thread_data->global_data;
// 	y = thread_data->start_row;
// 	while (y < thread_data->end_row)
// 	{
// 		x = 0;
// 		while (x < data->scene.width)
// 		{
// 			final_color = vec3_init(0.0, 0.0, 0.0);
// 			sub_y = 0;
// 			while (sub_y < SUBPIXEL_SAMPLES)
// 			{
// 				sub_x = 0;
// 				while (sub_x < SUBPIXEL_SAMPLES)
// 				{
// 					ray = generate_antialiased_ray(x, y, sub_x, sub_y,
// 							&data->scene);
// 					pthread_mutex_lock(&data->progress_mutex);
// 					data->ray_count++; // Incrementar el contador
// 					pthread_mutex_unlock(&data->progress_mutex);
// 					rec = find_closest_hit(&ray, &data->scene);
// 					if (rec.object != NULL)
// 						subpixel_color = calculate_light(&rec, &data->scene,
// 								&ray, 0);
// 					else
// 						subpixel_color = data->scene.background_color;
// 					final_color = vec3_add(final_color, subpixel_color);
// 					sub_x++;
// 				}
// 				sub_y++;
// 			}
// 			final_color = vec3_mul(final_color, inv_sub_sq);
// 			mlx_put_pixel(data->mlx.img.img_ptr, x, y,
// 				color_to_int(final_color));
// 			x++;
// 		}
// 		pthread_mutex_lock(&data->progress_mutex);
// 		data->rendered_rows++;
// 		pthread_mutex_unlock(&data->progress_mutex);
// 		y++;
// 	}
// 	return (NULL);
// }
