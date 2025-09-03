#include "../../include/minirt.h"

t_ray	generate_antialiased_ray(int x, int y, int sub_x, int sub_y,
		t_scene *scene)
{
	t_ray	ray;
	t_vec3	camera_right;
	t_vec3	camera_up;
	t_vec3	viewport_center;
	double	aspect_ratio;
	double	fov_rad;
	double	x_coord_normalized;
	double	y_coord_normalized;
	double	jitter_x;
	double	jitter_y;

	ray.origin = scene->camera.position;
	fov_rad = scene->camera.fov * M_PI / 180.0;
	aspect_ratio = (double)scene->width / (double)scene->height;
	camera_up = vec3_init(0.0, 1.0, 0.0);
	camera_right = vec3_normalize(vec3_cross(scene->camera.orientation,
				camera_up));
	camera_up = vec3_normalize(vec3_cross(camera_right,
				scene->camera.orientation));
	jitter_x = ((double)sub_x + 0.5) / SUBPIXEL_SAMPLES;
	jitter_y = ((double)sub_y + 0.5) / SUBPIXEL_SAMPLES;
	x_coord_normalized = ((double)x + jitter_x) / scene->width - 0.5;
	y_coord_normalized = 0.5 - ((double)y + jitter_y) / scene->height;
	viewport_center = vec3_add(scene->camera.position,
			vec3_mul(scene->camera.orientation, 1.0));
	ray.direction = vec3_add(vec3_add(viewport_center, vec3_mul(camera_right,
					2.0 * tan(fov_rad / 2.0) * aspect_ratio
					* x_coord_normalized)), vec3_mul(camera_up, 2.0
				* tan(fov_rad / 2.0) * y_coord_normalized));
	ray.direction = vec3_sub(ray.direction, ray.origin);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}

t_ray	generate_ray(int x, int y, t_scene *scene)
{
	t_ray	ray;
	t_vec3	camera_right;
	t_vec3	camera_up;
	t_vec3	viewport_center;
	double	aspect_ratio;
	double	fov_rad;
	double	x_norm;
	double	y_norm;

	ray.origin = scene->camera.position;
	fov_rad = scene->camera.fov * M_PI / 180.0;
	aspect_ratio = (double)scene->width / (double)scene->height;
	camera_up = vec3_init(0.0, 1.0, 0.0);
	camera_right = vec3_normalize(vec3_cross(scene->camera.orientation,
				camera_up));
	camera_up = vec3_normalize(vec3_cross(camera_right,
				scene->camera.orientation));
	x_norm = ((double)x / scene->width) - 0.5;
	y_norm = 0.5 - ((double)y / scene->height);
	viewport_center = vec3_add(scene->camera.position,
			vec3_mul(scene->camera.orientation, 1.0));
	ray.direction = vec3_add(vec3_add(viewport_center, vec3_mul(camera_right,
					2.0 * tan(fov_rad / 2.0) * aspect_ratio * x_norm)),
			vec3_mul(camera_up, 2.0 * tan(fov_rad / 2.0) * y_norm));
	ray.direction = vec3_sub(ray.direction, ray.origin);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}

t_color	get_object_color(t_hit_record *rec)
{
	t_vec3	local_point;
	int		pattern_x;
	int		pattern_y;
	int		pattern_z;

	if (!rec->object->material)
		return (rec->object->color);
	if (rec->object->material->has_texture)
	{
		return (get_texture_color(rec));
	}
	else if (rec->object->material->has_checkerboard)
	{
		local_point = rec->point;
		pattern_x = (int)floor(local_point.x
				/ rec->object->material->check_scale);
		pattern_y = (int)floor(local_point.y
				/ rec->object->material->check_scale);
		pattern_z = (int)floor(local_point.z
				/ rec->object->material->check_scale);
		if ((pattern_x + pattern_y + pattern_z) % 2 == 0)
			return (rec->object->material->check_color1);
		else
			return (rec->object->material->check_color2);
	}
	else
	{
		return (rec->object->color);
	}
}

t_color	calculate_specular_light(t_hit_record *rec, t_light *light, t_ray *ray)
{
	t_vec3	to_light;
	t_vec3	view_dir;
	t_vec3	reflected_dir;
	double	spec_factor;
	t_color	specular_color;

	if (!rec || !rec->object || !rec->object->material)
		return (vec3_init(0, 0, 0));
	to_light = vec3_normalize(vec3_sub(light->position, rec->point));
	view_dir = vec3_normalize(vec3_mul(ray->direction, -1.0));
	reflected_dir = vec3_reflect(to_light, rec->normal);
	spec_factor = vec3_dot(view_dir, reflected_dir);
	if (spec_factor > 0)
	{
		spec_factor = pow(spec_factor,
				rec->object->material->specular.shininess);
		specular_color = vec3_mul(light->color,
				rec->object->material->specular.intensity * spec_factor);
		return (specular_color);
	}
	return (vec3_init(0, 0, 0));
}

t_color	calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray,
		int depth)
{
	t_color			final_color;
	t_color			ambient_color;
	t_light			**lights;
	int				i;
	t_color			object_color;
	t_vec3			to_light;
	t_ray			shadow_ray;
	double			dot_prod;
	t_color			diffuse_contribution;
	t_color			specular_color;
	t_ray			reflected_ray;
	t_hit_record	reflected_rec;
	t_color			reflected_color;

	lights = (t_light **)scene->lights;
	i = 0;
	if (rec->object == NULL)
		return (scene->background_color);
	object_color = get_object_color(rec);
	ambient_color = vec3_mult_vec(scene->ambient.color, object_color);
	ambient_color = vec3_mul(ambient_color, scene->ambient.ratio);
	final_color = ambient_color;
	while (lights[i] != NULL)
	{
		to_light = vec3_normalize(vec3_sub(lights[i]->position, rec->point));
		shadow_ray.origin = vec3_add(rec->point, vec3_mul(rec->normal,
					EPSILON));
		shadow_ray.direction = to_light;
		if (is_in_shadow(&shadow_ray, scene, lights[i]))
		{
			i++;
			continue ;
		}
		dot_prod = vec3_dot(rec->normal, to_light);
		if (dot_prod > 0)
		{
			diffuse_contribution = vec3_mult_vec(lights[i]->color,
					object_color);
			diffuse_contribution = vec3_mul(diffuse_contribution, dot_prod
					* lights[i]->brightness);
			final_color = vec3_add(final_color, diffuse_contribution);
		}
		if (rec->object->material
			&& rec->object->material->specular.intensity > 0.0)
		{
			specular_color = calculate_specular_light(rec, lights[i], ray);
			final_color = vec3_add(final_color, specular_color);
		}
		i++;
	}
	if (rec->object->material && rec->object->material->mirror_ratio > 0.0
		&& depth < MAX_RECURSION_DEPTH)
	{
		reflected_ray.origin = vec3_add(rec->point, vec3_mul(rec->normal,
					EPSILON));
		reflected_ray.direction = vec3_reflect(ray->direction, rec->normal);
		reflected_rec = find_closest_hit(&reflected_ray, scene);
		if (reflected_rec.object != NULL)
			reflected_color = calculate_light(&reflected_rec, scene,
					&reflected_ray, depth + 1);
		else
			reflected_color = scene->background_color;
		final_color = vec3_add(vec3_mul(final_color, 1.0
					- rec->object->material->mirror_ratio),
				vec3_mul(reflected_color, rec->object->material->mirror_ratio));
	}
	final_color.x = fmin(1.0, fmax(0.0, final_color.x));
	final_color.y = fmin(1.0, fmax(0.0, final_color.y));
	final_color.z = fmin(1.0, fmax(0.0, final_color.z));
	return (final_color);
}

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

// Ejemplo para un plano
t_vec2	get_uv_plane(t_hit_record *rec)
{
	double	u;
	double	v;

	u = fmod(rec->point.x, 1.0);
	v = fmod(rec->point.z, 1.0);
	if (u < 0)
		u += 1.0;
	if (v < 0)
		v += 1.0;
	return (vec2_init(u, v));
}

// Ejemplo para un cilindro
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

// src/assets/ft_ray.c (o donde tengas tu lógica de renderizado)
t_color	get_texture_color(t_hit_record *rec)
{
	t_vec2			uv;
	mlx_texture_t	*texture;
	int				x_tex;
	int				y_tex;
	int				index;
	t_color			tex_color;

	if (!rec->object->material || !rec->object->material->texture)
		return (rec->object->color);
	texture = rec->object->material->texture;
	if (texture->width <= 0 || texture->height <= 0)
		return (rec->object->color);
	if (rec->object->type == SPHERE)
		uv = get_uv_sphere(rec);
	else if (rec->object->type == PLANE)
		uv = get_uv_plane(rec);
	else if (rec->object->type == CYLINDER)
		uv = get_uv_cylinder(rec);
	else
		return (rec->object->color);
	uv.x = fmax(0.0, fmin(1.0, uv.x));
	uv.y = fmax(0.0, fmin(1.0, uv.y));
	x_tex = (int)(uv.x * (texture->width - 1));
	y_tex = (int)(uv.y * (texture->height - 1));
	index = (y_tex * texture->width + x_tex) * 4;
	tex_color.x = (double)texture->pixels[index] / 255.0;
	tex_color.y = (double)texture->pixels[index + 1] / 255.0;
	tex_color.z = (double)texture->pixels[index + 2] / 255.0;
	return (tex_color);
}

t_hit_record	find_closest_hit(t_ray *ray, t_scene *scene)
{
	t_hit_record	closest_hit;
	t_hit_record	current_hit;
	t_object		**objects;
	double			min_t;
	int				i;

	objects = (t_object **)scene->objects;
	min_t = -1.0;
	i = 0;
	closest_hit.object = NULL;
	if (!objects)
		return (closest_hit);
	while (objects[i] != NULL)
	{
		current_hit.t = -1.0;
		if (objects[i]->type == SPHERE)
			intersect_sphere(ray, (t_sphere *)objects[i]->data, &current_hit);
		else if (objects[i]->type == PLANE)
			intersect_plane(ray, (t_plane *)objects[i]->data, &current_hit);
		else if (objects[i]->type == CYLINDER)
			intersect_cylinder(ray, (t_cylinder *)objects[i]->data,
				&current_hit);
		else if (objects[i]->type == CONE)
			intersect_cone(ray, (t_cone *)objects[i]->data, &current_hit);
		else if (objects[i]->type == HYPERBOLOID)
			intersect_hyperboloid(ray, (t_hyperboloid *)objects[i]->data,
				&current_hit);
		if (current_hit.t > EPSILON && (min_t < 0 || current_hit.t < min_t))
		{
			min_t = current_hit.t;
			closest_hit = current_hit;
			closest_hit.object = objects[i];
			closest_hit.ray = *ray;
		}
		i++;
	}
	return (closest_hit);
}

int	intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;

	oc = vec3_sub(ray->origin, sp->center);
	a = vec3_dot(ray->direction, ray->direction);
	b = 2.0 * vec3_dot(oc, ray->direction);
	c = vec3_dot(oc, oc) - sp->radius * sp->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (t < EPSILON)
	{
		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t < EPSILON)
			return (0);
	}
	rec->t = t;
	rec->point = ray_at(*ray, rec->t);
	rec->normal = vec3_normalize(vec3_sub(rec->point, sp->center));
	if (vec3_dot(ray->direction, rec->normal) > 0)
		rec->normal = vec3_mul(rec->normal, -1.0);
	return (1);
}

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
double	vec3_length_squared(t_vec3 v)
{
	return (vec3_dot(v, v));
}

static int	intersect_caps(t_ray *ray, t_cylinder *cy, double *t_cap,
		t_hit_record *rec)
{
	double	denom;

	double t_top, t_bottom;
	t_vec3 p_top, p_bottom;
	denom = vec3_dot(ray->direction, cy->axis);
	if (fabs(denom) < EPSILON)
		t_top = -1.0;
	else
		t_top = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis,
							cy->height / 2.0)), ray->origin), cy->axis) / denom;
	p_top = ray_at(*ray, t_top);
	if (t_top > EPSILON && vec3_length_squared(vec3_sub(p_top,
				vec3_add(cy->position, vec3_mul(cy->axis, cy->height
						/ 2.0)))) <= cy->radius * cy->radius)
		*t_cap = t_top;
	else
		*t_cap = -1.0;
	if (fabs(denom) < EPSILON)
		t_bottom = -1.0;
	else
		t_bottom = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis,
							-cy->height / 2.0)), ray->origin), cy->axis)
			/ denom;
	p_bottom = ray_at(*ray, t_bottom);
	if (t_bottom > EPSILON && vec3_length_squared(vec3_sub(p_bottom,
				vec3_add(cy->position, vec3_mul(cy->axis, -cy->height
						/ 2.0)))) <= cy->radius * cy->radius)
	{
		if (*t_cap < 0 || t_bottom < *t_cap)
			*t_cap = t_bottom;
	}
	if (*t_cap > EPSILON)
	{
		rec->point = ray_at(*ray, *t_cap);
		if (denom > 0)
			rec->normal = vec3_mul(cy->axis, -1.0);
		else
			rec->normal = cy->axis;
		return (1);
	}
	return (0);
}

static int	intersect_sides(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
	double	discriminant;
	double	t_side;
	t_vec3	oc;
	double	m;

	double a, b, c;
	oc = vec3_sub(ray->origin, cy->position);
	a = vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis,
					vec3_dot(ray->direction, cy->axis))),
			vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction,
						cy->axis))));
	b = 2.0 * vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis,
					vec3_dot(ray->direction, cy->axis))), vec3_sub(oc,
				vec3_mul(cy->axis, vec3_dot(oc, cy->axis))));
	c = vec3_dot(vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))),
			vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))))
		- (cy->radius * cy->radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	t_side = (-b - sqrt(discriminant)) / (2.0 * a);
	if (t_side > EPSILON)
	{
		m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc,
				cy->axis);
		if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
		{
			rec->t = t_side;
			rec->point = ray_at(*ray, rec->t);
			rec->normal = vec3_normalize(vec3_sub(rec->point,
						vec3_add(cy->position, vec3_mul(cy->axis, m))));
			return (1);
		}
	}
	t_side = (-b + sqrt(discriminant)) / (2.0 * a);
	if (t_side > EPSILON)
	{
		m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc,
				cy->axis);
		if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
		{
			rec->t = t_side;
			rec->point = ray_at(*ray, rec->t);
			rec->normal = vec3_normalize(vec3_sub(rec->point,
						vec3_add(cy->position, vec3_mul(cy->axis, m))));
			return (1);
		}
	}
	return (0);
}

int	intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
	t_hit_record	side_hit;
	t_hit_record	cap_hit;
	int				side_found;
	int				cap_found;

	side_found = intersect_sides(ray, cy, &side_hit);
	cap_found = intersect_caps(ray, cy, &cap_hit.t, &cap_hit);
	if (side_found && cap_found)
	{
		if (side_hit.t < cap_hit.t)
			*rec = side_hit;
		else
			*rec = cap_hit;
		return (1);
	}
	else if (side_found)
	{
		*rec = side_hit;
		return (1);
	}
	else if (cap_found)
	{
		*rec = cap_hit;
		return (1);
	}
	return (0);
}

int	intersect_cone(t_ray *ray, t_cone *cn, t_hit_record *rec)
{
	t_vec3	oc;
	double	cone_cos2;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;
	double	m1;
	double	m2;
	double	m;

	oc = vec3_sub(ray->origin, cn->position);
	cone_cos2 = pow(cn->radius / (cn->height / 2.0), 2.0);
	a = vec3_dot(ray->direction, ray->direction) - (1 + cone_cos2)
		* pow(vec3_dot(ray->direction, cn->axis), 2);
	b = 2.0 * (vec3_dot(ray->direction, oc) - (1 + cone_cos2)
			* vec3_dot(ray->direction, cn->axis) * vec3_dot(oc, cn->axis));
	c = vec3_dot(oc, oc) - (1 + cone_cos2) * pow(vec3_dot(oc, cn->axis), 2);
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0.0)
		return (0);
	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	// Calcular m, la distancia a lo largo del eje del cono
	m1 = vec3_dot(ray->direction, cn->axis) * t1 + vec3_dot(oc, cn->axis);
	m2 = vec3_dot(ray->direction, cn->axis) * t2 + vec3_dot(oc, cn->axis);
	rec->t = -1.0;
	if (t1 > EPSILON && m1 > 0 && m1 < cn->height)
		rec->t = t1;
	if (t2 > EPSILON && m2 > 0 && m2 < cn->height)
	{
		if (rec->t < 0 || t2 < rec->t)
			rec->t = t2;
	}
	if (rec->t > EPSILON)
	{
		m = vec3_dot(ray->direction, cn->axis) * rec->t + vec3_dot(oc,
				cn->axis);
		rec->point = ray_at(*ray, rec->t);
		rec->normal = vec3_normalize(vec3_sub(vec3_sub(rec->point,
						cn->position), vec3_mul(cn->axis, m * (1
							+ cone_cos2))));
		if (vec3_dot(rec->normal, ray->direction) > 0)
			rec->normal = vec3_mul(rec->normal, -1);
		return (1);
	}
	return (0);
}

int	intersect_hyperboloid(t_ray *ray, t_hyperboloid *hp, t_hit_record *rec)
{
	t_vec3	oc;
	double	d_u;
	double	d_w;
	double	oc_u;
	double	oc_w;
	double	z_limit;
	double	m1;
	double	m2;
	t_vec3	point_relative;
	double	x_n;
	double	y_n;
	double	z_n;

	double a, b, c;
	double t, t1, t2, discriminant;
	t_vec3 u_v, w_v;
	double dot_d_axis, dot_oc_axis;
	oc = vec3_sub(ray->origin, hp->position);
	dot_d_axis = vec3_dot(ray->direction, hp->axis);
	dot_oc_axis = vec3_dot(oc, hp->axis);
	if (fabs(hp->axis.x) > 0.9)
		u_v = vec3_init(0, 1, 0);
	else
		u_v = vec3_init(1, 0, 0);
	u_v = vec3_normalize(vec3_cross(u_v, hp->axis));
	w_v = vec3_normalize(vec3_cross(hp->axis, u_v));
	d_u = vec3_dot(ray->direction, u_v);
	d_w = vec3_dot(ray->direction, w_v);
	oc_u = vec3_dot(oc, u_v);
	oc_w = vec3_dot(oc, w_v);
	z_limit = hp->height / 2.0;
	a = (d_u * d_u) / (hp->radius_a * hp->radius_a) + (d_w * d_w)
		/ (hp->radius_b * hp->radius_b) - (dot_d_axis * dot_d_axis);
	b = 2.0 * ((d_u * oc_u) / (hp->radius_a * hp->radius_a) + (d_w * oc_w)
			/ (hp->radius_b * hp->radius_b) - (dot_d_axis * dot_oc_axis));
	c = (oc_u * oc_u) / (hp->radius_a * hp->radius_a) + (oc_w * oc_w)
		/ (hp->radius_b * hp->radius_b) - (dot_oc_axis * dot_oc_axis);
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0.0)
		return (0);
	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	m1 = vec3_dot(ray_at(*ray, t1), hp->axis);
	m2 = vec3_dot(ray_at(*ray, t2), hp->axis);
	if (t1 > EPSILON && m1 >= hp->position.z - z_limit && m1 <= hp->position.z
		+ z_limit)
		t = t1;
	else if (t2 > EPSILON && m2 >= hp->position.z - z_limit
		&& m2 <= hp->position.z + z_limit)
		t = t2;
	else
		return (0);
	rec->t = -1.0;
	if (t1 > EPSILON && m1 >= hp->position.z - z_limit && m1 <= hp->position.z
		+ z_limit)
		rec->t = t1;
	if (t2 > EPSILON && m2 >= hp->position.z - z_limit && m2 <= hp->position.z
		+ z_limit)
	{
		if (rec->t < 0 || t2 < rec->t)
			rec->t = t2;
	}
	if (rec->t < 0)
		return (0);
	rec->point = ray_at(*ray, rec->t);
	point_relative = vec3_sub(rec->point, hp->position);
	x_n = (2.0 * vec3_dot(point_relative, u_v)) / (hp->radius_a * hp->radius_a);
	y_n = (2.0 * vec3_dot(point_relative, w_v)) / (hp->radius_b * hp->radius_b);
	z_n = -2.0 * vec3_dot(point_relative, hp->axis);
	rec->normal = vec3_normalize(vec3_add(vec3_add(vec3_mul(u_v, x_n),
					vec3_mul(w_v, y_n)), vec3_mul(hp->axis, z_n)));
	if (vec3_dot(rec->normal, ray->direction) > 0)
		rec->normal = vec3_mul(rec->normal, -1);
	return (1);
}

int	is_in_shadow(t_ray *shadow_ray, t_scene *scene, t_light *light)
{
	t_hit_record	current_hit;
	t_object		**objects;
	double			light_dist;
	int				i;

	objects = (t_object **)scene->objects;
	i = 0;
	light_dist = vec3_length(vec3_sub(light->position, shadow_ray->origin));
	while (objects[i] != NULL)
	{
		current_hit.t = -1.0;
		if (objects[i]->type == SPHERE)
			intersect_sphere(shadow_ray, (t_sphere *)objects[i]->data,
				&current_hit);
		else if (objects[i]->type == PLANE)
			intersect_plane(shadow_ray, (t_plane *)objects[i]->data,
				&current_hit);
		else if (objects[i]->type == CYLINDER)
			intersect_cylinder(shadow_ray, (t_cylinder *)objects[i]->data,
				&current_hit);
		if (current_hit.t > EPSILON && current_hit.t < light_dist)
		{
			return (1);
		}
		i++;
	}
	return (0);
}
