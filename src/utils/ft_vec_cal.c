/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec_cal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// Suma de dos vectores.
t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return (vec3_init(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

// Resta de dos vectores.
t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return (vec3_init(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

// Multiplicación de un vector por un escalar.
t_vec3	vec3_mul(t_vec3 v, double scalar)
{
	return (vec3_init(v.x * scalar, v.y * scalar, v.z * scalar));
}

// División de un vector por un escalar.
// Manejar división por cero si es necesario
// aunque en ray tracing rara vez ocurre.
t_vec3	vec3_div(t_vec3 v, double scalar)
{
	if (scalar == 0.0)
		return (vec3_init(0, 0, 0));
	return (vec3_init(v.x / scalar, v.y / scalar, v.z / scalar));
}

// Multiplicación componente a componente de dos vectores (útil para colores).
t_vec3	vec3_mult_vec(t_vec3 v1, t_vec3 v2)
{
	return (vec3_init(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z));
}

// src/utils/ft_vec3.c
double  vec3_length_squared(t_vec3 v)
{
    return (vec3_dot(v, v));
}