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

void	init_thread_data(t_thread_init_data *init_data, t_data *data)
{
	init_data->td[init_data->i].id = init_data->i;
	init_data->td[init_data->i].start_row = *init_data->start_row;
	if (init_data->i == data->num_threads - 1)
		init_data->td[init_data->i].end_row = data->scene.height;
	else
		init_data->td[init_data->i].end_row = *init_data->start_row
			+ init_data->rows_per_thread;
	init_data->td[init_data->i].global_data = data;
	*init_data->start_row = init_data->td[init_data->i].end_row;
}

void	create_and_run_threads(t_data *data, pthread_t *threads,
		t_thread_data *thread_data)
{
	t_thread_init_data	init_d;
	int					rows_per_thread;
	int					start_row;

	init_d.td = thread_data;
	init_d.rows_per_thread = data->scene.height / data->num_threads;
	init_d.i = 0;
	start_row = 0;
	while (init_d.i < data->num_threads)
	{
		init_d.start_row = &start_row;
		init_thread_data(&init_d, data);
		if (pthread_create(&threads[init_d.i], NULL, render_thread_func,
				&thread_data[init_d.i]) != 0)
		{
			printf("Error\nFallo al crear el hilo %d.\n", init_d.i);
			while (--init_d.i >= 0)
				pthread_join(threads[init_d.i], NULL);
			pthread_mutex_destroy(&data->progress_mutex);
			exit(1);
		}
		init_d.i++;
	}
}

static void	manage_thread_lifecycle(t_data *data, pthread_t *threads,
		t_thread_data *thread_data)
{
	int	i;

	create_and_run_threads(data, threads, thread_data);
	i = 0;
	while (i < data->num_threads)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

static void	display_progress(t_data *data)
{
	while (data->rendered_rows < data->scene.height)
	{
		printf("\rRenderizando: %.2f%% completado...",
			((double)data->rendered_rows / data->scene.height) * 100.0);
		fflush(stdout);
		usleep(50000);
	}
	printf("\rRenderizado: 100.00%% completado!\n");
}

void	render_threaded(t_data *data)
{
	pthread_t		*threads;
	t_thread_data	*thread_data;

	threads = (pthread_t *)malloc(sizeof(pthread_t) * data->num_threads);
	if (!threads)
		ft_error_exit("Error\nMemory allocation for threads failed.\n");
	thread_data = (t_thread_data *)malloc(sizeof(t_thread_data)
			* data->num_threads);
	if (!thread_data)
	{
		free(threads);
		ft_error_exit("Error\nMemory allocation for thread_data failed.\n");
	}
	data->rendered_rows = 0;
	data->ray_count = 0;
	manage_thread_lifecycle(data, threads, thread_data);
	display_progress(data);
	free(threads);
	free(thread_data);
}

// void	render_threaded(t_data *data)
// {
// 	pthread_t		threads[data->num_threads];
// 	t_thread_data	thread_data[data->num_threads];
// 	int				i;
// 	int				rows_per_thread;
// 	int				start_row;

// 	data->rendered_rows = 0;
// 	data->ray_count = 0;
// 	rows_per_thread = data->scene.height / data->num_threads;
// 	i = 0;
// 	start_row = 0;
// 	while (i < data->num_threads)
// 	{
// 		thread_data[i].id = i;
// 		thread_data[i].start_row = start_row;
// 		if (i == data->num_threads - 1)
// 			thread_data[i].end_row = data->scene.height;
// 		else
// 			thread_data[i].end_row = start_row + rows_per_thread;
// 		thread_data[i].global_data = data;
// 		if (pthread_create(&threads[i], NULL, render_thread_func,
// 				&thread_data[i]) != 0)
// 		{
// 			printf("Error\nFallo al crear el hilo %d.\n", i);
// 			while (--i >= 0)
// 				pthread_join(threads[i], NULL);
// 			pthread_mutex_destroy(&data->progress_mutex);
// 			exit(1);
// 		}
// 		start_row = thread_data[i].end_row;
// 		i++;
// 	}
// 	while (data->rendered_rows < data->scene.height)
// 	{
// 		printf("\rRenderizando: %.2f%% completado...",
// 			((double)data->rendered_rows / data->scene.height) * 100.0);
// 		fflush(stdout);
// 		usleep(50000);
// 	}
// 	printf("\rRenderizado: 100.00%% completado!\n");
// 	i = 0;
// 	while (i < data->num_threads)
// 	{
// 		pthread_join(threads[i], NULL);
// 		i++;
// 	}
// }

// printf("Total de rayos generados: %lu\n", data->ray_count);