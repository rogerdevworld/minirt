/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// Nueva versión: esta función devuelve 1 si es un entero válido, 0 si no.
int	validate_is_integer(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}

// Nueva versión: ahora la lógica es lineal y segura
double	validate_color_component(char *str)
{
	char	*trimmed;
	int		v;

	trimmed = ft_strtrim(str, " \r\t\n");
	if (!trimmed)
		ft_error_exit("MiniRT: Error: allocating memory in color parser");
	if (!validate_is_integer(trimmed))
	{
		free(trimmed);
		ft_error_exit("MiniRT: Error: color components must be valid integers");
	}
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		ft_error_exit("MiniRT: Error: color component is empty");
	}
	v = ft_atoi(trimmed);
	if (v < 0 || v > 255)
	{
		free(trimmed);
		ft_error_exit("MiniRT: Error: Color values must be in range 0–255");
	}
	free(trimmed);
	return (v / 255.0);
}

int	ft_strarr_len(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}

static double	parse_integer_part(const char **str, int *sign)
{
	double	result;

	result = 0.0;
	if (**str == '-')
	{
		*sign = -1;
		(*str)++;
	}
	while (**str && **str != '.')
	{
		result = result * 10.0 + (**str - '0');
		(*str)++;
	}
	return (result);
}

double	ft_atod(const char *str)
{
	double	result;
	double	decimal_part;
	int		sign;

	sign = 1;
	result = parse_integer_part(&str, &sign);
	decimal_part = 1.0;
	if (*str == '.')
	{
		str++;
		while (*str)
		{
			decimal_part /= 10.0;
			result = result + (*str - '0') * decimal_part;
			str++;
		}
	}
	return (result * sign);
}
