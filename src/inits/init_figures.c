/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_figures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:55:35 by jaacosta          #+#    #+#             */
/*   Updated: 2025/09/04 16:55:37 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// Inicializa una esfera.
t_sphere	sphere_init(t_vec3 center, double radius)
{
	t_sphere	s;

	s.center = center;
	s.radius = radius;
	return (s);
}

// Inicializa un plano.
// Asegura que la normal esté normalizada.
t_plane	plane_init(t_vec3 position, t_vec3 normal)
{
	t_plane	p;

	p.position = position;
	p.normal = vec3_normalize(normal);
	return (p);
}

// Inicializa un cilindro.
// Asegura que el eje esté normalizado.
t_cylinder	cylinder_init(t_vec3 position, t_vec3 axis, double radius,
		double height)
{
	t_cylinder	cy;

	cy.position = position;
	cy.axis = vec3_normalize(axis);
	cy.radius = radius;
	cy.height = height;
	return (cy);
}

// Inicializa un cono.
// Asegura que el eje esté normalizado.
t_cone	cone_init(t_vec3 position, t_vec3 axis, double radius, double height)
{
	t_cone	cn;

	cn.position = position;
	cn.axis = vec3_normalize(axis);
	cn.radius = radius;
	cn.height = height;
	return (cn);
}

// Inicializa un hiperboloide.
// Asegura que el eje esté normalizado.
t_hyperboloid	hyperboloid_init(t_vec3 position, t_vec3 axis, double radius_a,
		double radius_b, double height)
{
	t_hyperboloid	hp;

	hp.position = position;
	hp.axis = vec3_normalize(axis);
	hp.radius_a = radius_a;
	hp.radius_b = radius_b;
	hp.height = height;
	return (hp);
}
