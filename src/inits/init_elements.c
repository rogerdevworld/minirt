/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_elements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// Inicializa un rayo con un origen y una dirección.
// Asegura que la dirección esté normalizada.
t_ray	ray_init(t_vec3 origin, t_vec3 direction)
{
	t_ray	r;

	r.origin = origin;
	r.direction = vec3_normalize(direction);
	return (r);
}

// Inicializa una cámara.
t_camera	camera_init(t_vec3 position, t_vec3 orientation, double fov)
{
	t_camera	c;

	c.position = position;
	c.orientation = vec3_normalize(orientation);
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
