/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"

// Parser de Esfera (sp)
void	parse_sphere(t_scene *scene, char **tokens)
{
	t_sphere	*sp;
	t_object	*obj;

	if (ft_strarr_len(tokens) < 4)
		ft_error_exit("Error: Sphere format invalid");
	sp = (t_sphere *)malloc(sizeof(t_sphere));
	if (!sp)
		ft_error_exit("Error: Memory allocation failed");
	sp->center = parse_vec3(tokens[1]);
	sp->radius = parse_positive_double(tokens[2]);
	obj = create_object(SPHERE, sp, parse_vec3_color(tokens[3]));
	apply_object_modifiers(obj, tokens, 4);
	add_object_to_scene(scene, obj);
}

// Parser de Plano (pl)
void	parse_plane(t_scene *scene, char **tokens)
{
	t_plane		*pl;
	t_object	*obj;

	if (ft_strarr_len(tokens) < 4)
		ft_error_exit("Error: Plane format invalid");
	pl = (t_plane *)malloc(sizeof(t_plane));
	if (!pl)
		ft_error_exit("Error: Memory allocation failed");
	pl->position = parse_vec3(tokens[1]);
	pl->normal = parse_vec3_normalized(tokens[2]);
	obj = create_object(PLANE, pl, parse_vec3_color(tokens[3]));
	apply_object_modifiers(obj, tokens, 4);
	add_object_to_scene(scene, obj);
}

// Parser de Cilindro (cy)
void	parse_cylinder(t_scene *scene, char **tokens)
{
	t_cylinder	*cy;
	t_object	*obj;

	if (ft_strarr_len(tokens) < 6)
		ft_error_exit("Error: Cylinder format invalid");
	cy = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!cy)
		ft_error_exit("Error: Memory allocation failed");
	cy->position = parse_vec3(tokens[1]);
	cy->axis = parse_vec3_normalized(tokens[2]);
	cy->radius = parse_positive_double(tokens[3]);
	cy->height = parse_positive_double(tokens[4]);
	obj = create_object(CYLINDER, cy, parse_vec3_color(tokens[5]));
	apply_object_modifiers(obj, tokens, 6);
	add_object_to_scene(scene, obj);
}

// Parser de Cono (cn)
void	parse_cone(t_scene *scene, char **tokens)
{
	t_cone		*cn;
	t_object	*obj;

	if (ft_strarr_len(tokens) < 6)
		ft_error_exit("Error: Cone format invalid");
	cn = malloc(sizeof(t_cone));
	if (!cn)
		ft_error_exit("Error: Memory allocation failed");
	cn->position = parse_vec3(tokens[1]);
	cn->axis = parse_vec3_normalized(tokens[2]);
	cn->radius = parse_positive_double(tokens[3]);
	cn->height = parse_positive_double(tokens[4]);
	obj = create_object(CONE, cn, parse_vec3_color(tokens[5]));
	apply_object_modifiers(obj, tokens, 6);
	add_object_to_scene(scene, obj);
}

// Parser de Paraboloide (pb) ->bonus
void	parse_paraboloid(t_scene *scene, char **tokens)
{
	t_parab		*pb;
	t_object	*obj;

	if (ft_strarr_len(tokens) < 6)
		ft_error_exit("Error: Cone format invalid");
	pb = malloc(sizeof(t_parab));
	if (!pb)
		ft_error_exit("Error: Memory allocation failed");
	pb->position = parse_vec3(tokens[1]);
	pb->axis = parse_vec3_normalized(tokens[2]);
	pb->focal_lenght = parse_positive_double(tokens[3]);
	pb->height = parse_positive_double(tokens[4]);
	obj = create_object(PARABOLOID, pb, parse_vec3_color(tokens[5]));
	apply_object_modifiers(obj, tokens, 6);
	add_object_to_scene(scene, obj);
}
