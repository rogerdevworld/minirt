/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_figures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Initializes a sphere structure.
 *
 * @details This function is a simple constructor that creates 
 a `t_sphere` object
 * with a specified center and radius.
 *
 * @param center The center point of the sphere.
 * @param radius The radius of the sphere.
 *
 * @return A new `t_sphere` structure with the initialized values.
 */
t_sphere	sphere_init(t_vec3 center, double radius)
{
	t_sphere	s;

	s.center = center;
	s.radius = radius;
	return (s);
}

/**
 * @brief Initializes a plane structure.
 *
 * @details This function creates a `t_plane` object by setting its position and
 * normalizing its normal vector. The normalized normal vector is crucial for
 * accurate lighting and intersection calculations.
 *
 * @param position A point on the plane.
 * @param normal The normal vector defining the plane's orientation.
 *
 * @return A new `t_plane` structure with the initialized values.
 */
t_plane	plane_init(t_vec3 position, t_vec3 normal)
{
	t_plane	p;

	p.position = position;
	p.normal = vec3_normalize(normal);
	return (p);
}

/**
 * @brief Initializes a cylinder structure.
 *
 * @details This function creates a `t_cylinder` object by setting its position,
 * normalizing its axis vector, and defining its radius and height.
 *
 * @param position The center of the cylinder's base.
 * @param axis The vector representing the cylinder's central axis.
 * @param radius The radius of the cylinder.
 * @param height The height of the cylinder.
 *
 * @return A new `t_cylinder` structure with the initialized values.
 */
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

/**
 * @brief Initializes a cone structure.
 *
 * @details This function creates a `t_cone` object by setting its position,
 * normalizing its axis vector, and defining its radius and height.
 *
 * @param position The position of the cone's apex.
 * @param axis The vector representing the cone's central axis.
 * @param radius The radius of the cone's base.
 * @param height The height of the cone.
 *
 * @return A new `t_cone` structure with the initialized values.
 */
t_cone	cone_init(t_vec3 position, t_vec3 axis, double radius, double height)
{
	t_cone	cn;

	cn.position = position;
	cn.axis = vec3_normalize(axis);
	cn.radius = radius;
	cn.height = height;
	return (cn);
}

/**
 * @brief Initializes a hyperboloid structure.
 *
 * @details This function creates a `t_hyperboloid` object by setting its
 * position and normalizing its axis vector. It also initializes other
 * properties (radii and height) to zero, as they are likely set later
 * during parsing or calculation.
 *
 * @param position The center of the hyperboloid.
 * @param axis The vector representing the hyperboloid's central axis.
 *
 * @return A new `t_hyperboloid` structure with the initialized values.
 */
t_hyperboloid	hyperboloid_init(t_vec3 position, t_vec3 axis)
{
	t_hyperboloid	hp;

	hp.position = position;
	hp.axis = vec3_normalize(axis);
	hp.radius_a = 0.0;
	hp.radius_b = 0.0;
	hp.height = 0.0;
	return (hp);
}
