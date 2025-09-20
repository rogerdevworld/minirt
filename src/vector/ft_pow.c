/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

double	ft_pow(double base, double exp)
{
	double	result;
	int		i;
	int		neg_exp;

	if (exp == 0)
		return (1.0);
	neg_exp = exp < 0;
	if (neg_exp)
		exp *= -1;
	result = 1.0;
	i = 0;
	while (i < exp)
	{
		result *= base;
		i++;
	}
	if (neg_exp)
		return (1.0 / result);
	return (result);
}
