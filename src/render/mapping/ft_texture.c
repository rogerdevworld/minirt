#include "../../include/minirt.h"

// src/render/ft_texture.c
t_color	get_texture_color(t_hit_record *rec)
{
	t_vec2 uv;
	mlx_texture_t *texture;
	int x_tex;
	int y_tex;
	int index;

	if (!rec->object->material || !rec->object->material->has_texture)
		return (rec->object->color);
	texture = rec->object->material->color_img;
	if (texture->width <= 0 || texture->height <= 0)
		return (rec->object->color);

	if (rec->object->type == SPHERE)
		uv = get_uv_sphere(rec);
	else if (rec->object->type == PLANE)
		uv = get_uv_plane(rec);
	else if (rec->object->type == CYLINDER)
		uv = get_uv_cylinder(rec);
	else if (rec->object->type == CONE)
		uv = get_uv_cone(rec);
	else if (rec->object->type == HYPERBOLOID)
		uv = get_uv_hyperboloid(rec);
	else if (rec->object->type == PARABOLOID)
		uv = get_uv_paraboloid(rec);
	else
		return (rec->object->color);
	uv.x = fmax(0.0, fmin(1.0, uv.x));
	uv.y = fmax(0.0, fmin(1.0, uv.y));
	x_tex = (int)(uv.x * (texture->width - 1));
	y_tex = (int)(uv.y * (texture->height - 1));
	index = (y_tex * texture->width + x_tex) * 4;

	t_color tex_color;
	tex_color.x = (double)texture->pixels[index] / 255.0;
	tex_color.y = (double)texture->pixels[index + 1] / 255.0;
	tex_color.z = (double)texture->pixels[index + 2] / 255.0;
	return (tex_color);
}
