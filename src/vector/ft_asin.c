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

// Approximates the arc sine of x using its series expansion
double	ft_asin(double x)
{
	double	result;
	double	term;
	double	i;

	if (x < -1.0 || x > 1.0)
		return (NAN);
	result = 0.0;
	term = x;
	i = 1.0;
	while (ft_abs(term) > 1e-15)
	{
		result += term;
		term = term * (ft_pow(i, 2) / (i + 1) / (i + 2)) * ft_pow(x, 2);
		i += 2;
	}
	return (result);
}
