/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

void	render_threaded(t_data *data)
{
	pthread_t		threads[data->num_threads];
	t_thread_data	thread_data[data->num_threads];
	int				i;
	int				rows_per_thread;
	int				start_row;

	data->rendered_rows = 0;
	data->ray_count = 0;
	rows_per_thread = data->scene.height / data->num_threads;
	i = 0;
	start_row = 0;
	while (i < data->num_threads)
	{
		thread_data[i].id = i;
		thread_data[i].start_row = start_row;
		if (i == data->num_threads - 1)
			thread_data[i].end_row = data->scene.height;
		else
			thread_data[i].end_row = start_row + rows_per_thread;
		thread_data[i].global_data = data;
		if (pthread_create(&threads[i], NULL, render_thread_func,
				&thread_data[i]) != 0)
		{
			ft_printf("Error\nFallo al crear el hilo %d.\n", i);
			while (--i >= 0)
				pthread_join(threads[i], NULL);
			pthread_mutex_destroy(&data->progress_mutex);
			exit(1);
		}
		start_row = thread_data[i].end_row;
		i++;
	}
	while (data->rendered_rows < data->scene.height)
	{
		printf("\rRenderizando: %.2f%% completado...",
			((double)data->rendered_rows / data->scene.height) * 100.0);
		fflush(stdout);
		usleep(50000);
	}
	printf("\rRenderizado: 100.00%% completado!\n");
	i = 0;
	while (i < data->num_threads)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

// printf("Total de rayos generados: %lu\n", data->ray_count);
// No necesitas destruir el mutex aquí, ya lo haces en `cleanup_program`