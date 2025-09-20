/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uv_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_sphere(t_hit_record *rec)
{
	t_vec3	local_point;
	double	u;
	double	v;

	local_point = vec3_sub(rec->point, ((t_sphere *)rec->object->data)->center);
	u = 0.5 + atan2(local_point.z, local_point.x) / (2.0 * M_PI);
	v = 0.5 - asin(local_point.y / ((t_sphere *)rec->object->data)->radius)
		/ M_PI;
	return (vec2_init(u, v));
}

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_plane(t_hit_record *rec)
{
	t_plane	*pl;
	t_vec3	local_x_axis;
	t_vec3	local_y_axis;
	t_vec3	local_point;
	double	u;
	double	v;

	pl = (t_plane *)rec->object->data;
	local_x_axis = vec3_normalize(vec3_cross(pl->normal, vec3_init(0, 1, 0)));
	if (vec3_length(local_x_axis) < EPSILON)
		local_x_axis = vec3_normalize(vec3_cross(pl->normal, vec3_init(1, 0,
						0)));
	local_y_axis = vec3_normalize(vec3_cross(local_x_axis, pl->normal));
	local_point = vec3_sub(rec->point, pl->position);
	u = vec3_dot(local_point, local_x_axis) / 10.0;
	v = vec3_dot(local_point, local_y_axis) / 10.0;
	u = u - floor(u);
	v = v - floor(v);
	return (vec2_init(u, v));
}

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_cylinder(t_hit_record *rec)
{
	t_cylinder	*cy;
	t_vec3		local_point;
	double		m;
	double		v;
	t_vec3		temp_axis;
	t_vec3		p;
	double		u;

	cy = (t_cylinder *)rec->object->data;
	local_point = vec3_sub(rec->point, cy->position);
	m = vec3_dot(local_point, cy->axis);
	v = (m + cy->height / 2.0) / cy->height;
	temp_axis = vec3_normalize(vec3_cross(cy->axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(cy->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(cy->axis, m));
	u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), cy->axis) < 0)
		u = 1.0 - u;
	return (vec2_init(u, v));
}

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_cone(t_hit_record *rec)
{
	t_cone	*cn;
	t_vec3	local_point;
	double	m;
	double	v;
	t_vec3	temp_axis;
	t_vec3	p;
	double	u;

	cn = (t_cone *)rec->object->data;
	local_point = vec3_sub(rec->point, cn->position);
	m = vec3_dot(local_point, cn->axis);
	v = m / cn->height;
	temp_axis = vec3_normalize(vec3_cross(cn->axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(cn->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(cn->axis, m));
	u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), cn->axis) < 0)
		u = 1.0 - u;
	return (vec2_init(u, v));
}

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_hyperboloid(t_hit_record *rec)
{
	t_hyperboloid	*hp;
	t_vec3			local_point;
	t_vec3			temp_axis;
	t_vec3			p;
	double			u;
	double			v;

	hp = (t_hyperboloid *)rec->object->data;
	local_point = vec3_sub(rec->point, hp->position);
	temp_axis = vec3_normalize(vec3_cross(hp->axis, vec3_init(0, 0, 1)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(hp->axis, vec3_init(0, 1, 0)));
	p = vec3_sub(local_point, vec3_mul(hp->axis, vec3_dot(local_point,
					hp->axis)));
	u = atan2(vec3_dot(p, vec3_cross(hp->axis, temp_axis)), vec3_dot(p,
				temp_axis)) / (2.0 * M_PI) + 0.5;
	v = (vec3_dot(local_point, hp->axis) + hp->height / 2.0) / hp->height;
	return (vec2_init(u, v));
}

// src/assets/ft_uv_mapping.c
t_vec2	get_uv_paraboloid(t_hit_record *rec)
{
	t_parab *pb = (t_parab *)rec->object->data;
	t_vec3 local_point = vec3_sub(rec->point, pb->position);
	double v = vec3_dot(local_point, pb->axis) / pb->height;
	t_vec3 temp_axis = vec3_normalize(vec3_cross(pb->axis, vec3_init(1, 0, 0)));
	if (vec3_length(temp_axis) < EPSILON)
		temp_axis = vec3_normalize(vec3_cross(pb->axis, vec3_init(0, 1, 0)));
	t_vec3 p = vec3_sub(local_point, vec3_mul(pb->axis, vec3_dot(local_point,
					pb->axis)));
	double u = acos(vec3_dot(p, temp_axis) / vec3_length(p)) / (2.0 * M_PI);
	if (vec3_dot(vec3_cross(temp_axis, p), pb->axis) < 0)
		u = 1.0 - u;
	return (vec2_init(u, v));
}