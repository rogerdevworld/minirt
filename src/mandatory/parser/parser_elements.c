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
#include "../../../include/minirt.h"

/**
 * @brief Parses a sphere definition and adds it to the scene.
 *
 * @details This function validates the number of tokens for a sphere, allocates
 * memory for its `t_sphere` data, and populates the `center` and `radius`
 * from the parsed tokens. It then creates a generic object wrapper and applies
 * any additional modifiers before adding it to the scene's list of objects.
 *
 * @param scene A pointer to the main scene structure.
 * @param tokens A null-terminated array of strings containing 
 the sphere's properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses a plane definition and adds it to the scene.
 *
 * @details This function checks the token count for a plane, 
 allocates memory for
 * its `t_plane` data, and parses the `position` and `normal` vector. It
 * normalizes the plane's normal vector to ensure accurate lighting and
 * intersection calculations. The function then creates a generic object and adds
 * it to the scene.
 *
 * @param scene A pointer to the main scene structure.
 * @param tokens A null-terminated array of strings containing the 
 plane's properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses a cylinder definition and adds it to the scene.
 *
 * @details This function validates the token count, allocates memory for the
 * `t_cylinder` data, and parses its `position`, `axis`, `radius`, 
 and `height`.
 * It normalizes the axis vector and handles memory allocation 
 failures gracefully
 * before wrapping the cylinder data in a generic object and 
 adding it to the scene.
 *
 * @param scene A pointer to the main scene structure.
 * @param tokens A null-terminated array of strings containing the 
 cylinder's properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses a cone definition and adds it to the scene.
 *
 * @details This function verifies the token count for a cone, allocates memory
 * for its `t_cone` data, and parses its `position`, `axis`, `radius`, and
 * `height`. It normalizes the axis vector, then creates a generic object
 * containing the cone data and adds it to the scene.
 *
 * @param scene A pointer to the main scene structure.
 * @param tokens A null-terminated array of strings containing the 
 cone's properties.
 *
 * @return void.
 */
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

/**
 * @brief Parses a paraboloid definition and adds it to the scene.
 *
 * @details This function validates the token count for a paraboloid, allocates
 * memory for its `t_parab` data, and parses its `position`, `axis`, 
 `focal_length`,
 * and `height`. It normalizes the axis vector and wraps the data in a generic
 * object before adding it to the scene.
 *
 * @param scene A pointer to the main scene structure.
 * @param tokens A null-terminated array of strings containing the 
 paraboloid's properties.
 *
 * @return void.
 */
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
