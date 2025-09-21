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
#include "../../include/minirt.h"

t_ray generate_antialiased_ray(int x, int y, int sub_x, int sub_y, t_scene *scene)
{
    t_ray   ray;
    t_vec3  camera_right;
    t_vec3  camera_up;
    t_vec3  viewport_center;
    double  aspect_ratio;
    double  fov_rad;
    double  x_coord_normalized;
    double  y_coord_normalized;

    ray.origin = scene->camera.position;
    fov_rad = scene->camera.fov * M_PI / 180.0;
    aspect_ratio = (double)scene->width / (double)scene->height;
    camera_up = vec3_init(0.0, 1.0, 0.0);
    camera_right = vec3_normalize(vec3_cross(scene->camera.orientation, camera_up));
    camera_up = vec3_normalize(vec3_cross(camera_right, scene->camera.orientation));
    x_coord_normalized = ((double)x + ((double)sub_x + 0.5) / SUBPIXEL_SAMPLES) / scene->width - 0.5;
    y_coord_normalized = 0.5 - ((double)y + ((double)sub_y + 0.5) / SUBPIXEL_SAMPLES) / scene->height;
    viewport_center = vec3_add(scene->camera.position, vec3_mul(scene->camera.orientation, 1.0));
    ray.direction = vec3_add(vec3_add(viewport_center, vec3_mul(camera_right, 2.0 * tan(fov_rad / 2.0) * aspect_ratio * x_coord_normalized)), vec3_mul(camera_up, 2.0 * tan(fov_rad / 2.0) * y_coord_normalized));
    ray.direction = vec3_sub(ray.direction, ray.origin);
    ray.direction = vec3_normalize(ray.direction);
    return (ray);
}

// src/ft_ray.c
t_hit_record find_closest_hit(t_ray *ray, t_scene *scene)
{
    t_hit_record    closest_hit;
    t_hit_record    current_hit;
    t_object        **objects = (t_object **)scene->objects;
    double          min_t = -1.0;
    int             i = 0;

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
            intersect_cylinder(ray, (t_cylinder *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CONE)
            intersect_cone(ray, (t_cone *)objects[i]->data, &current_hit);
        else if (objects[i]->type == HYPERBOLOID)
            intersect_hyperboloid(ray, (t_hyperboloid *)objects[i]->data, &current_hit);
        else if (objects[i]->type == PARABOLOID)
            intersect_paraboloid(ray, (t_parab *)objects[i]->data, &current_hit);
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
