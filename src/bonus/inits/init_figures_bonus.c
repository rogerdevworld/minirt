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

t_sphere	sphere_init(t_vec3 center, double radius)
{
	t_sphere	s;

	s.center = center;
	s.radius = radius;
	return (s);
}

t_plane	plane_init(t_vec3 position, t_vec3 normal)
{
	t_plane	p;

	p.position = position;
	p.normal = vec3_normalize(normal);
	return (p);
}

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

t_cone	cone_init(t_vec3 position, t_vec3 axis, double radius, double height)
{
	t_cone	cn;

	cn.position = position;
	cn.axis = vec3_normalize(axis);
	cn.radius = radius;
	cn.height = height;
	return (cn);
}

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
