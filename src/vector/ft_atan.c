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

double	ft_atan(double x)
{
	double result = 0.0;
	int i = 0;
	int sign;

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