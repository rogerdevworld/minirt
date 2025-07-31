/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   windows.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:24:12 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:24:32 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WINDOWS_H
# define WINDOWS_H

# include "minirt.h" // <--- IMPORTANT: Include minirt.h FIRST

// --- MLX initialization and management (from mlx_utils.c) ---
void    mlx_setup(t_data *data);
int     close_window(t_data *data);
int     key_hook(int keycode, t_data *data);
void    put_pixel_to_img(t_img *img, int x, int y, int color);

// --- Rendering (main rendering function now manages threads, from render.c) ---
void    render_threaded(t_data *data);
void    *thread_render_rows(void *arg); // Function executed by each thread

#endif // WINDOWS_H