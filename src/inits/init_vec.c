/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:34:31 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/19 19:07:39 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// Inicializa una estructura de vector 2D
t_vec2	vec2_init(double x, double y)
{
	t_vec2	vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

// Inicializa un vector 3D con los valores dados.
t_vec3	vec3_init(double x, double y, double z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}
