/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	ft_atoi_part(const char **str, int *sign)
{
	double	result;

	result = 0.0;
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '-')
	{
		*sign = -1;
		(*str)++;
	}
	else if (**str == '+')
		(*str)++;
	while (**str >= '0' && **str <= '9')
		result = result * 10.0 + (*((*str)++) - '0');
	return (result);
}

double	ft_atof(const char *str)
{
	double	result;
	double	fraction;
	int		sign;

	sign = 1;
	result = ft_atoi_part(&str, &sign);
	fraction = 1.0;
	if (*str == '.')
	{
		str++;
		while (*str >= '0' && *str <= '9')
		{
			fraction *= 0.1;
			result += (*str++ - '0') * fraction;
		}
	}
	return (result * sign);
}
