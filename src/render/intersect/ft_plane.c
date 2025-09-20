/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// src/intersect/ft_plane.c
int	intersect_plane(t_ray *ray, t_plane *pl, t_hit_record *rec)
{
	double	denominator;
	double	t;

	denominator = vec3_dot(pl->normal, ray->direction);
	if (fabs(denominator) < EPSILON)
		return (0);
	t = vec3_dot(vec3_sub(pl->position, ray->origin), pl->normal) / denominator;
	if (t < EPSILON)
		return (0);
	rec->t = t;
	rec->point = ray_at(*ray, rec->t);
	if (vec3_dot(pl->normal, ray->direction) > 0)
		rec->normal = vec3_mul(pl->normal, -1.0);
	else
		rec->normal = pl->normal;
	return (1);
}
