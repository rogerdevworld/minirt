/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rotate_vector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minirt.h"

/**
 * @brief Rotates a vector around a specified axis by a given angle.
 *
 * @details This function implements a rotation transformation using the
 * Rodrigues' rotation formula. It calculates the new coordinates of a vector
 * `v` after rotating it by `angle` radians around a normalized `axis`. The
 * formula is optimized to avoid explicit matrix multiplication.
 *
 * @param v The vector to be rotated.
 * @param axis The axis of rotation. This vector is normalized internally.
 * @param angle The angle of rotation in radians.
 *
 * @return The new vector after the rotation has been applied.
 */
// t_vec3	rotate_vector(t_vec3 v, t_vec3 axis, double angle)
// {
// 	t_vec3	result;
// 	double	c;
// 	double	s;
// 	double	t;
// 	double	x;
// 	double	y;
// 	double	z;

// 	c = cos(angle);
// 	s = sin(angle);
// 	t = 1.0 - c;
// 	axis = vec3_normalize(axis);
// 	x = axis.x;
// 	y = axis.y;
// 	z = axis.z;
// 	result.x = (t * x * x + c) * v.x + (t * x * y - s * z) * v.y + (t * x * z
// 			+ s * y) * v.z;
// 	result.y = (t * x * y + s * z) * v.x + (t * y * y + c) * v.y + (t * y * z
// 			- s * x) * v.z;
// 	result.z = (t * x * z - s * y) * v.x + (t * y * z + s * x) * v.y + (t * z
// 			* z + c) * v.z;
// 	return (result);
// }
