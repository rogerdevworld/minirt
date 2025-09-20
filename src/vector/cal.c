/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cal.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 18:31:54 by rmarrero          #+#    #+#             */
/*   Updated: 2025/09/20 18:31:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// This is a simplified factorial function for the example
double	ft_factorial(int n)
{
	if (n == 0)
		return (1);
	return (n * ft_factorial(n - 1));
}

// Simplified Taylor series implementation for sine
double	ft_sin(double x)
{
	double	result;
	double	term;
	int		i;

	result = 0.0;
	i = 0;
	while (i < 10)
	{
		term = ft_pow(-1, i) * ft_pow(x, 2 * i + 1) / ft_factorial(2 * i + 1);
		result += term;
		i++;
	}
	return (result);
}

// Approximates the cosine of x using a Taylor series.
double	ft_cos(double x)
{
	double	result;
	int		i;
	int		sign;

	result = 0.0;
	i = 0;
	while (i < 10)
	{
		if (i % 2 == 0)
			sign = 1;
		else
			sign = -1;
		result += sign * ft_pow(x, 2 * i) / ft_factorial(2 * i);
		i++;
	}
	return (result);
}

// Calculates the tangent of x using ft_sin and ft_cos.
double	ft_tan(double x)
{
	double	cos_val;

	cos_val = ft_cos(x);
	if (cos_val == 0.0)
		return (INFINITY);
	return (ft_sin(x) / cos_val);
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

// ft_floor: Returns the largest integer value less than or equal to x.
double	ft_floor(double x)
{
	long long	int_part;

	int_part = (long long)x;
	if (x >= 0.0 || (double)int_part == x)
		return ((double)int_part);
	else
		return ((double)int_part - 1.0);
}
