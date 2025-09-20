/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;
	int	a;

	r = (int)(color.x * 255.00);
	g = (int)(color.y * 255.00);
	b = (int)(color.z * 255.00);
	a = 255;
	if (r < 0)
		r = 0;
	if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	if (b > 255)
		b = 255;
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

t_color	int_to_color(int color)
{
	t_color	result;

	result.x = ((color >> 24) & 0xFF) / 255.0;
	result.y = ((color >> 16) & 0xFF) / 255.0;
	result.z = ((color >> 8) & 0xFF) / 255.0;
	return (result);
}
