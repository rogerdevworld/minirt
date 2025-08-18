/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:34:31 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/19 19:07:39 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// Inicializa una estructura de vector 2D
t_vec2 vec2_init(double x, double y)
{
    t_vec2 vec;
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
// Inicializa un rayo con un origen y una dirección.
// Asegura que la dirección esté normalizada.
t_ray	ray_init(t_vec3 origin, t_vec3 direction)
{
	t_ray	r;

	r.origin = origin;
	r.direction = vec3_normalize(direction);
		// Crucial: la dirección del rayo debe estar normalizada
	return (r);
}
// Inicializa una cámara.
t_camera	camera_init(t_vec3 position, t_vec3 orientation, double fov)
{
	t_camera	c;

	c.position = position;
	c.orientation = vec3_normalize(orientation);
		// La orientación de la cámara debe estar normalizada
	c.fov = fov;
	return (c);
}

// Inicializa la luz ambiental.
t_ambient_light	ambient_light_init(double ratio, t_vec3 color)
{
	t_ambient_light	al;

	al.ratio = ratio;
	al.color = color;
	return (al);
}

// Inicializa una luz puntual.
t_light	light_init(t_vec3 position, double brightness, t_vec3 color)
{
	t_light	l;

	l.position = position;
	l.brightness = brightness;
	l.color = color;
	return (l);
}

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
		// Crucial: la normal del plano debe estar normalizada
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
		// Crucial: el eje del cilindro debe estar normalizado
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
