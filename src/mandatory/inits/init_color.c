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
#include "../../../include/minirt.h"

/**
 * @brief Converts a floating-point color structure to a single integer.
 *
 * @details This function transforms a `t_color` struct, where color components
 * are represented as floating-point values between 0.0 and 1.0, into a 32-bit
 * integer. It maps each floating-point component (red, green, blue) to an 8-bit
 * integer value (0-255), clamping the values to ensure they stay within this
 * range. The resulting integer is formatted in a way suitable for display,
 * typically as ARGB or RGBA.
 *
 * @param color The `t_color` structure with floating-point RGB components.
 *
 * @return A single integer representing the packed color value.
 */
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

/**
 * @brief Converts an integer color value to a floating-point color structure.
 *
 * @details This function performs the reverse operation of `color_to_int`. It
 * takes a 32-bit integer color value, extracts the 8-bit red, green, and blue
 * components using bitwise shifts and masks, and then converts each component
 * into a floating-point value between 0.0 and 1.0. This is useful for
 * converting a stored color value back into a format suitable for calculations.
 *
 * @param color The 32-bit integer color value to be converted.
 *
 * @return A `t_color` structure with floating-point RGB components.
 */
t_color	int_to_color(int color)
{
	t_color	result;

	result.x = ((color >> 24) & 0xFF) / 255.0;
	result.y = ((color >> 16) & 0xFF) / 255.0;
	result.z = ((color >> 8) & 0xFF) / 255.0;
	return (result);
}
