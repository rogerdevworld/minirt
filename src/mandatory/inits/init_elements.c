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
#include "../../../include/minirt.h"

/**
 * @brief Creates and initializes a new ray structure.
 *
 * @details This function serves as a constructor for a ray. It sets the ray's
 * **origin** and normalizes its **direction** vector to ensure it's a unit

	* vector. A normalized direction is essential for accurate 
	calculations in ray tracing,
 * such as distance and intersection checks.
 *
 * @param origin The starting point of the ray.
 * @param direction The direction vector of the ray.
 *
 * @return A new `t_ray` structure with the initialized values.
 */
t_ray	ray_init(t_vec3 origin, t_vec3 direction)
{
	t_ray	r;

	r.origin = origin;
	r.direction = vec3_normalize(direction);
	return (r);
}

/**
 * @brief Initializes a camera structure.
 *
 * @details This function creates a new `t_camera` object. It sets the camera's
 * **position**, normalizes its **orientation** vector to ensure a consistent
 * unit direction, and sets its **field of view** (FOV). A properly initialized
 * camera is the viewpoint from which the scene is rendered.
 *
 * @param position The location of the camera in 3D space.
 * @param orientation The direction the camera is facing.
 * @param fov The camera's field of view in degrees.
 *
 * @return A new `t_camera` structure with the initialized values.
 */
t_camera	camera_init(t_vec3 position, t_vec3 orientation, double fov)
{
	t_camera	c;

	c.position = position;
	c.orientation = vec3_normalize(orientation);
	c.fov = fov;
	return (c);
}

/**
 * @brief Initializes an ambient light structure.
 *
 * @details This function is a simple constructor that sets the **ratio** and
 * **color** of an ambient light source. Ambient light provides a base level of
 * illumination to all objects in the scene,
	regardless of their position relative
 * to other light sources.
 *
 * @param ratio The intensity ratio of the ambient light (0.0 to 1.0).
 * @param color The color of the ambient light.
 *
 * @return A new `t_ambient_light` structure.
 */
t_ambient_light	ambient_light_init(double ratio, t_vec3 color)
{
	t_ambient_light	al;

	al.ratio = ratio;
	al.color = color;
	return (al);
}

/**
 * @brief Initializes a point light source structure.
 *
 * @details This function creates a new `t_light` object. It sets the light's
 * **position**, **brightness**, and **color**. Point lights emit light equally
 * in all directions from a single point in space.
 *
 * @param position The location of the light source in 3D space.
 * @param brightness The intensity of the light (0.0 to 1.0).
 * @param color The color of the light.
 *
 * @return A new `t_light` structure.
 */
t_light	light_init(t_vec3 position, double brightness, t_vec3 color)
{
	t_light	l;

	l.position = position;
	l.brightness = brightness;
	l.color = color;
	return (l);
}
