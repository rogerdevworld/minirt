/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// Calculates the tangent of x using ft_sin and ft_cos.
double	ft_atan2(double y, double x)
{
	if (x > 0)
		return (ft_atan(y / x));
	if (x < 0 && y >= 0)
		return (ft_atan(y / x) + M_PI);
	if (x < 0 && y < 0)
		return (ft_atan(y / x) - M_PI);
	if (x == 0 && y > 0)
		return (M_PI / 2);
	if (x == 0 && y < 0)
		return (-M_PI / 2);
	return (0.0);
}
