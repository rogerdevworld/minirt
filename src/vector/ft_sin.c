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

// ft_atan: Approximates the arctangent of x using a Taylor series.
double	ft_atan(double x)
{
	double	result;
	int		i;
	int		sign;

	result = 0.0;
	i = 0;
	if (x > 1.0 || x < -1.0)
	{
		return (0.0);
	}
	while (i < 100)
	{
		if (i % 2 == 0)
			sign = 1;
		else
			sign = -1;
		result += sign * ft_pow(x, 2 * i + 1) / (2 * i + 1);
		i++;
	}
	return (result);
}

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
