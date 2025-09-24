/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_normal_mapping.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Creates a Tangent, Bitangent, and Normal (TBN) matrix.
 *
 * @details This function constructs a Tangent, Bitangent, and 
 Normal (TBN) matrix, which is a
 * crucial component of normal mapping. This matrix serves as a 
 local coordinate system for a
 * surface point and is used to transform normal vectors from a 
 normal map (which are in tangent
 * space) into world space.
 *
 * The process is as follows:
 * 1. A default "up" vector, typically (0, 1, 0), is chosen. An 
 edge case is handled
 * if the surface normal is very close to this vector, in which 
 case a different "up" vector is
 * selected to prevent a near-zero cross product.
 * 2. The **tangent** vector is calculated by taking the cross 
 product of the "up" vector and the surface
 * normal. This vector lies on the surface and is tangent to the geometry.
 * 3. The **bitangent** (or binormal) vector is then calculated as 
 the cross product of the normal and the
 * tangent. This completes the orthonormal basis (a set of three 
 perpendicular, unit vectors).
 * 4. These three vectors (tangent, bitangent, and normal) form 
 the columns of the TBN matrix.
 *
 * @param normal The geometric normal of the surface point.
 *
 * @return A `t_mat3` structure representing the TBN matrix.
 */
t_mat3	create_tbn_matrix(t_vec3 normal)
{
	t_vec3	tangent;
	t_vec3	bitangent;
	t_vec3	up;
	t_mat3	tbn;

	up = vec3_init(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(normal, up)) > 0.999)
		up = vec3_init(1.0, 0.0, 0.0);
	tangent = vec3_normalize(vec3_cross(up, normal));
	bitangent = vec3_normalize(vec3_cross(normal, tangent));
	tbn.c1 = tangent;
	tbn.c2 = bitangent;
	tbn.c3 = normal;
	return (tbn);
}

/**
 * @brief Retrieves and decodes a normal vector from a normal map.
 *
 * @details This function performs a texture lookup on a **normal map** 
 to get a new, perturbed normal
 * vector for a hit point. Normal maps typically store normal vectors 
 as color values, where
 * the R, G, and B components correspond to the X, Y, and Z components 
 of the normal. These
 * color values are encoded in the range [0, 255], while normal 
 vectors are in the range [-1, 1].
 *
 * The process is as follows:
 * 1. The function first gets the texture coordinates (UV) for 
 the hit point using `get_texture_uv`.
 * 2. The UV coordinates are then scaled to the dimensions of
 the normal map to find the correct
 * pixel.
 * 3. The RGB color values are read from the texture's pixel data.
 * 4. The values are then decoded from the [0, 255] range to the 
 [-1, 1] range using the formula
 * `value / 127.5 - 1.0`.
 * 5. The decoded vector is normalized to ensure it has a unit length.
 *
 * @param rec A pointer to the hit record.
 * @param normal_map A pointer to the normal map texture.
 *
 * @return A `t_vec3` representing the decoded and normalized normal 
 vector from the texture.
 */
t_vec3	get_normal_from_texture(t_hit_record *rec, mlx_texture_t *normal_map)
{
	t_vec2	uv;
	int		x_tex;
	int		y_tex;
	int		index;
	t_vec3	new_normal;

	uv = get_texture_uv(rec);
	x_tex = (int)(uv.x * (normal_map->width - 1));
	y_tex = (int)(uv.y * (normal_map->height - 1));
	index = (y_tex * normal_map->width + x_tex) * 4;
	new_normal.x = (double)normal_map->pixels[index] / 127.5 - 1.0;
	new_normal.y = (double)normal_map->pixels[index + 1] / 127.5 - 1.0;
	new_normal.z = (double)normal_map->pixels[index + 2] / 127.5 - 1.0;
	return (vec3_normalize(new_normal));
}

/**
 * @brief Main function for applying normal map-based surface normals.
 *
 * @details This function orchestrates the entire normal mapping 
 process. It replaces a surface's
 * geometric normal with a more detailed one from a normal map, 
 creating the illusion of high
 * surface detail.
 *
 * The process is as follows:
 * 1. It first checks if a normal map is assigned to the object's 
 material. If not, it returns the
 * original geometric normal.
 * 2. It calls `get_normal_from_texture` to get the new normal vector 
 from the normal map. This
 * vector is in **tangent space**, a local coordinate system relative 
 to the surface.
 * 3. It then calls `create_tbn_matrix` to generate the Tangent, 
 Bitangent, and Normal matrix for the
 * hit point. This matrix is essential for converting the new normal 
 from tangent space to
 * **world space**.
 * 4. The new normal vector is transformed into world space by 
 multiplying it by the TBN matrix.
 * This aligns the perturbed normal with the correct orientation 
 of the object in the scene.
 * 5. The final, transformed normal is normalized and returned.
 *
 * @param rec A pointer to the hit record.
 * @param geom_normal The geometric normal of the surface.
 *
 * @return A `t_vec3` representing the final, perturbed surface 
 normal for lighting calculations.
 */
t_vec3	get_normal_from_map(t_hit_record *rec, t_vec3 geom_normal)
{
	mlx_texture_t	*normal_map;
	t_vec3			new_normal;
	t_mat3			tbn;

	normal_map = rec->object->material->texture_img;
	if (!normal_map || normal_map->width <= 0 || normal_map->height <= 0)
		return (geom_normal);
	new_normal = get_normal_from_texture(rec, normal_map);
	tbn = create_tbn_matrix(geom_normal);
	new_normal = vec3_add(vec3_add(vec3_mul(tbn.c1, new_normal.x),
				vec3_mul(tbn.c2, new_normal.y)), vec3_mul(tbn.c3,
				new_normal.z));
	return (vec3_normalize(new_normal));
}

// Obtiene la normal de la textura y la convierte a espacio del mundo
// t_vec3	get_normal_from_map(t_hit_record *rec, t_vec3 geom_normal)
// {
// 	t_vec2			uv;
// 	mlx_texture_t	*normal_map;
// 	int				x_tex;
// 	int				y_tex;
// 	int				index;
// 	t_vec3			new_normal;
// 	t_mat3			tbn;

// 	normal_map = rec->object->material->texture_img;
// 	if (!normal_map || normal_map->width <= 0 || normal_map->height <= 0)
// 		return (geom_normal);
// 	if (rec->object->type == SPHERE)
// 		uv = get_uv_sphere(rec);
// 	else if (rec->object->type == PLANE)
// 		uv = get_uv_plane(rec);
// 	else if (rec->object->type == CYLINDER)
// 		uv = get_uv_cylinder(rec);
// 	else if (rec->object->type == CONE)
// 		uv = get_uv_cone(rec);
// 	else if (rec->object->type == HYPERBOLOID)
// 		uv = get_uv_hyperboloid(rec);
// 	else if (rec->object->type == PARABOLOID)
// 		uv = get_uv_paraboloid(rec);
// 	else
// 		return (geom_normal);
// 	x_tex = (int)(uv.x * (normal_map->width - 1));
// 	y_tex = (int)(uv.y * (normal_map->height - 1));
// 	index = (y_tex * normal_map->width + x_tex) * 4;
// 	new_normal.x = (double)normal_map->pixels[index] / 127.5 - 1.0;
// 	new_normal.y = (double)normal_map->pixels[index + 1] / 127.5 - 1.0;
// 	new_normal.z = (double)normal_map->pixels[index + 2] / 127.5 - 1.0;
// 	new_normal = vec3_normalize(new_normal);
// 	tbn = create_tbn_matrix(geom_normal);
// 	new_normal = vec3_add(vec3_add(vec3_mul(tbn.c1, new_normal.x),
// 				vec3_mul(tbn.c2, new_normal.y)), vec3_mul(tbn.c3,
// 				new_normal.z));
// 	return (vec3_normalize(new_normal));
// }
