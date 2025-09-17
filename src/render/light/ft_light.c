#include "../../include/minirt.h"

// src/render/ft_light.c
t_color calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray, int depth)
{
    t_color final_color;
    t_color ambient_color;
    t_light **lights = (t_light **)scene->lights;
    int     i = 0;
    t_vec3 effective_normal;

    if (rec->object == NULL)
        return (scene->background_color);

    // Obtener la normal efectiva, si existe un mapa de normales
    if (rec->object->material && rec->object->material->has_normal_map)
        effective_normal = get_normal_from_map(rec, rec->normal);
    else
        effective_normal = rec->normal;

    t_color object_color = get_object_color(rec);
    ambient_color = vec3_mult_vec(scene->ambient.color, object_color);
    ambient_color = vec3_mul(ambient_color, scene->ambient.ratio);
    final_color = ambient_color;
    while (lights[i] != NULL)
    {
        t_vec3 to_light = vec3_normalize(vec3_sub(lights[i]->position, rec->point));
        t_ray shadow_ray;
        // Usar la normal efectiva para el rayo de sombra
        shadow_ray.origin = vec3_add(rec->point, vec3_mul(effective_normal, EPSILON));
        shadow_ray.direction = to_light;
        if (is_in_shadow(&shadow_ray, scene, lights[i]))
        {
            i++;
            continue;
        }
        double dot_prod = vec3_dot(effective_normal, to_light);
        if (dot_prod > 0)
        {
            t_color diffuse_contribution = vec3_mult_vec(lights[i]->color, object_color);
            diffuse_contribution = vec3_mul(diffuse_contribution, dot_prod * lights[i]->brightness);
            final_color = vec3_add(final_color, diffuse_contribution);
        }
        if (rec->object->material && rec->object->material->specular.intensity > 0.0)
        {
            t_color specular_color = calculate_specular_light(rec, lights[i], ray);
            final_color = vec3_add(final_color, specular_color);
        }
        i++;
    }
    if (rec->object->material && rec->object->material->mirror_ratio > 0.0 && depth < MAX_RECURSION_DEPTH)
    {
        t_ray reflected_ray;
        // Usar la normal efectiva para la reflexión
        reflected_ray.origin = vec3_add(rec->point, vec3_mul(effective_normal, EPSILON));
        reflected_ray.direction = vec3_reflect(ray->direction, effective_normal);
        t_hit_record reflected_rec = find_closest_hit(&reflected_ray, scene);
        t_color reflected_color;
        if (reflected_rec.object != NULL)
            reflected_color = calculate_light(&reflected_rec, scene, &reflected_ray, depth + 1);
        else
            reflected_color = scene->background_color;
        final_color = vec3_add(vec3_mul(final_color, 1.0 - rec->object->material->mirror_ratio), vec3_mul(reflected_color, rec->object->material->mirror_ratio));
    }
    final_color.x = fmin(1.0, fmax(0.0, final_color.x));
    final_color.y = fmin(1.0, fmax(0.0, final_color.y));
    final_color.z = fmin(1.0, fmax(0.0, final_color.z));
    return (final_color);
}

// src/render/ft_light.c
int is_in_shadow(t_ray *shadow_ray, t_scene *scene, t_light *light)
{
    t_hit_record    current_hit;
    t_object        **objects = (t_object **)scene->objects;
    double          light_dist;
    int             i;
    
    i = 0;
    light_dist = vec3_length(vec3_sub(light->position, shadow_ray->origin));
    while (objects[i] != NULL)
    {
        current_hit.t = -1.0;
        if (objects[i]->type == SPHERE)
            intersect_sphere(shadow_ray, (t_sphere *)objects[i]->data, &current_hit);
        else if (objects[i]->type == PLANE)
            intersect_plane(shadow_ray, (t_plane *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CYLINDER)
            intersect_cylinder(shadow_ray, (t_cylinder *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CONE) // <--- Add this back
            intersect_cone(shadow_ray, (t_cone *)objects[i]->data, &current_hit);
        else if (objects[i]->type == HYPERBOLOID) // <--- Add this back
            intersect_hyperboloid(shadow_ray, (t_hyperboloid *)objects[i]->data, &current_hit);
        else if (objects[i]->type == PARABOLOID) // <--- Add this back
            intersect_paraboloid(shadow_ray, (t_parab *)objects[i]->data, &current_hit);
        if (current_hit.t > EPSILON && current_hit.t < light_dist)
            return (1);
        i++;
    }
    return (0);
}
