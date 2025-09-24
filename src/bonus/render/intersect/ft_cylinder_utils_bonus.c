/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cylinder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

t_vec3	calculate_quadratic_coeffs(t_ray *ray, t_vec3 oc, t_cylinder *cy)
{
	t_vec3	coeffs;
	t_vec3	v_a;
	t_vec3	v_b;

	v_a = vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction,
					cy->axis)));
	coeffs.x = vec3_dot(v_a, v_a);
	v_b = vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis)));
	coeffs.y = 2.0 * vec3_dot(v_a, v_b);
	coeffs.z = vec3_dot(v_b, v_b) - (cy->radius * cy->radius);
	return (coeffs);
}

int	check_hit_and_update_rec(t_hit_params *params, t_hit_record *rec,
		double t_side)
{
	double	m;

	if (t_side > EPSILON)
	{
		m = vec3_dot(params->ray->direction, params->cy->axis) * t_side
			+ vec3_dot(params->oc, params->cy->axis);
		if (m >= -params->cy->height / 2.0 && m <= params->cy->height / 2.0)
		{
			rec->t = t_side;
			rec->point = ray_at(*params->ray, rec->t);
			rec->normal = vec3_normalize(vec3_sub(rec->point,
						vec3_add(params->cy->position,
							vec3_mul(params->cy->axis, m))));
			return (1);
		}
	}
	return (0);
}
