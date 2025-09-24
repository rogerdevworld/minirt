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
