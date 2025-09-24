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

/**
 * @brief Calculates the coefficients of the quadratic equation 
 for ray-cylinder intersection.
 *
 * @details This function determines the A, B, and C coefficients 
 for the ray-cylinder
 * intersection quadratic equation. It is based on a mathematical 
 formulation that
 * accounts for the cylinder's axis and radius. The calculations 
 are performed
 * to simplify the intersection formula, making it ready to be 
 solved.
 *
 * @param ray A pointer to the ray being cast.
 * @param oc The vector from the cylinder's center to the ray's 
 origin.
 * @param cy A pointer to the cylinder object.
 *
 * @return A `t_vec3` where the x, y, and z components correspond 
 to the A, B, and C
 * coefficients, respectively.
 */
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

/**
 * @brief Checks if a ray hit is valid and updates the hit record.
 *
 * @details This function validates a potential intersection distance 
 (`t_side`)
 * by checking if it's positive and if the corresponding hit point 
 lies within
 * the cylinder's finite height. If the hit is valid, it populates 
 the `t_hit_record`
 * with the intersection distance, the precise hit point in 3D space, 
 and the
 * surface normal at that point.
 *
 * @param params A pointer to a structure containing necessary hit 
 parameters
 * (ray, object-center vector, cylinder, etc.).
 * @param rec A pointer to the `t_hit_record` structure to be updated.
 * @param t_side The calculated intersection distance to be validated.
 *
 * @return 1 if a valid hit is found, 0 otherwise.
 */
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
