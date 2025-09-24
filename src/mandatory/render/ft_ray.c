/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Pre-calculates ray generation data for a specific subpixel.
 *
 * @details This function computes all the necessary parameters for 
 casting a ray
 * through a single subpixel on the screen. It determines the camera's
 * orientation and the viewport's dimensions to accurately project a 2D
 * screen coordinate into a 3D world space. The calculations include:
 *
 * - **Field of View (FOV)**: Converts the camera's FOV from degrees to 
 radians.
 * - **Aspect Ratio**: Calculates the ratio of the screen's width to height.
 * - **Camera Basis Vectors**: Creates a local orthonormal basis (right and up
 * vectors) for the camera, which is essential for correctly orienting the ray
 * in the scene.
 * - **Normalized Coordinates**: Computes the subpixel's coordinates 
 in a
 * normalized space ranging from -0.5 to 0.5. The `SUBPIXEL_SAMPLES` 
 macro
 * ensures that the ray is cast from the center of the subpixel, not 
 the top-left
 * corner of the pixel.
 * - **Viewport Center**: Determines the 3D position of the center 
 of the virtual
 * viewport in front of the camera.
 *
 * @param sp A `t_subpixel_data` struct containing the pixel and 
 subpixel coordinates.
 * @param scene A pointer to the scene structure with camera and 
 screen data.
 *
 * @return A `t_ray_data` struct containing all pre-calculated 
 values.
 */
t_ray_data	calculate_ray_data(t_subpixel_data sp, t_scene *scene)
{
	t_ray_data	data;

	data.fov_rad = scene->camera.fov * M_PI / 180.0;
	data.aspect_ratio = (double)scene->width / (double)scene->height;
	data.camera_right = vec3_normalize(vec3_cross(scene->camera.orientation,
				vec3_init(0.0, 1.0, 0.0)));
	data.camera_up = vec3_normalize(vec3_cross(data.camera_right,
				scene->camera.orientation));
	data.x_coord_normalized = ((double)sp.x + ((double)sp.sub_x + 0.5)
			/ SUBPIXEL_SAMPLES) / scene->width - 0.5;
	data.y_coord_normalized = 0.5 - ((double)sp.y + ((double)sp.sub_y + 0.5)
			/ SUBPIXEL_SAMPLES) / scene->height;
	data.viewport_center = vec3_add(scene->camera.position,
			vec3_mul(scene->camera.orientation, 1.0));
	return (data);
}

/**
 * @brief Generates a single ray for antialiasing.
 *
 * @details This is the main function for casting a ray from the camera 
 through a specific
 * subpixel. It uses the pre-calculated data from `calculate_ray_data` 
 to construct the
 * ray's origin and direction.
 *
 * The ray's direction is calculated by projecting the normalized 
 subpixel coordinates
 * onto the camera's viewport plane. The formula uses the `tan` of 
 half the FOV to
 * correctly scale the normalized coordinates based on the camera's 
 perspective and the
 * viewport's distance. The direction vector is then calculated by 
 subtracting the
 * camera's position from the resulting point on the viewport, and 
 this vector is
 * normalized to a unit length.
 *
 * By calling this function for each subpixel within a main pixel 
 and averaging the
 * resulting colors, the final image achieves a smooth, antialiased look. 
 *
 * @param sp A `t_subpixel_data` struct with the subpixel coordinates.
 * @param scene A pointer to the scene data.
 *
 * @return The constructed `t_ray` for the specified subpixel.
 */
t_ray	generate_antialiased_ray(t_subpixel_data sp, t_scene *scene)
{
	t_ray		ray;
	t_ray_data	data;
	t_vec3		direction;

	data = calculate_ray_data(sp, scene);
	ray.origin = scene->camera.position;
	direction = vec3_add(vec3_add(data.viewport_center,
				vec3_mul(data.camera_right, 2.0 * tan(data.fov_rad / 2.0)
					* data.aspect_ratio * data.x_coord_normalized)),
			vec3_mul(data.camera_up, 2.0 * tan(data.fov_rad / 2.0)
				* data.y_coord_normalized));
	ray.direction = vec3_sub(direction, ray.origin);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}
// t_ray	generate_antialiased_ray(int x, int y, int sub_x, int sub_y,
// 		t_scene *scene)
// {
// 	t_ray	ray;
// 	t_vec3	camera_right;
// 	t_vec3	camera_up;
// 	t_vec3	viewport_center;
// 	double	aspect_ratio;
// 	double	fov_rad;
// 	double	x_coord_normalized;
// 	double	y_coord_normalized;

// 	ray.origin = scene->camera.position;
// 	fov_rad = scene->camera.fov * M_PI / 180.0;
// 	aspect_ratio = (double)scene->width / (double)scene->height;
// 	camera_up = vec3_init(0.0, 1.0, 0.0);
// 	camera_right = vec3_normalize(vec3_cross(scene->camera.orientation,
// 				camera_up));
// 	camera_up = vec3_normalize(vec3_cross(camera_right,
// 				scene->camera.orientation));
// 	x_coord_normalized = ((double)x + ((double)sub_x + 0.5) / SUBPIXEL_SAMPLES)
// 		/ scene->width - 0.5;
// 	y_coord_normalized = 0.5 - ((double)y + ((double)sub_y + 0.5)
// 			/ SUBPIXEL_SAMPLES) / scene->height;
// 	viewport_center = vec3_add(scene->camera.position,
// 			vec3_mul(scene->camera.orientation, 1.0));
// 	ray.direction = vec3_add(vec3_add(viewport_center, vec3_mul(camera_right,
// 					2.0 * tan(fov_rad / 2.0) * aspect_ratio
// 					* x_coord_normalized)), vec3_mul(camera_up, 2.0
// 				* tan(fov_rad / 2.0) * y_coord_normalized));
// 	ray.direction = vec3_sub(ray.direction, ray.origin);
// 	ray.direction = vec3_normalize(ray.direction);
// 	return (ray);
// }

t_hit_record	get_intersection(t_ray *ray, t_object *object)
{
	t_hit_record	current_hit;

	current_hit.t = -1.0;
	if (object->type == SPHERE)
		intersect_sphere(ray, (t_sphere *)object->data, &current_hit);
	else if (object->type == PLANE)
		intersect_plane(ray, (t_plane *)object->data, &current_hit);
	else if (object->type == CYLINDER)
		intersect_cylinder(ray, (t_cylinder *)object->data, &current_hit);
	else if (object->type == CONE)
		intersect_cone(ray, (t_cone *)object->data, &current_hit);
	else if (object->type == HYPERBOLOID)
		intersect_hyperboloid(ray, (t_hyperboloid *)object->data, &current_hit);
	else if (object->type == PARABOLOID)
		intersect_paraboloid(ray, (t_parab *)object->data, &current_hit);
	return (current_hit);
}

// src/ft_ray.c
t_hit_record	find_closest_hit(t_ray *ray, t_scene *scene)
{
	t_hit_record	closest_hit;
	t_object		**objects;
	double			min_t;
	int				i;
	t_hit_record	current_hit;

	objects = (t_object **)scene->objects;
	min_t = -1.0;
	i = 0;
	closest_hit.object = NULL;
	if (!objects)
		return (closest_hit);
	while (objects[i] != NULL)
	{
		current_hit = get_intersection(ray, objects[i]);
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

// t_hit_record find_closest_hit(t_ray *ray, t_scene *scene)
// {
//     t_hit_record    closest_hit;
//     t_hit_record    current_hit;
//     t_object        **objects = (t_object **)scene->objects;
//     double          min_t = -1.0;
//     int             i = 0;

//     closest_hit.object = NULL;
//     if (!objects)
//         return (closest_hit);
//     while (objects[i] != NULL)
//     {
//         current_hit.t = -1.0;
//         if (objects[i]->type == SPHERE)
//             intersect_sphere(ray, (t_sphere *)objects[i]->data,
// &current_hit);
//         else if (objects[i]->type == PLANE)
//             intersect_plane(ray, (t_plane *)objects[i]->data, &current_hit);
//         else if (objects[i]->type == CYLINDER)
//             intersect_cylinder(ray, (t_cylinder *)objects[i]->data,
// &current_hit);
//         else if (objects[i]->type == CONE)
//             intersect_cone(ray, (t_cone *)objects[i]->data, &current_hit);
//         else if (objects[i]->type == HYPERBOLOID)
//             intersect_hyperboloid(ray, (t_hyperboloid *)objects[i]->data,
// &current_hit);
//         else if (objects[i]->type == PARABOLOID)
//             intersect_paraboloid(ray, (t_parab *)objects[i]->data,
// &current_hit);
//         if (current_hit.t > EPSILON && (min_t < 0 || current_hit.t < min_t))
//         {
//             min_t = current_hit.t;
//             closest_hit = current_hit;
//             closest_hit.object = objects[i];
//             closest_hit.ray = *ray;
//         }
//         i++;
//     }
//     return (closest_hit);
// }