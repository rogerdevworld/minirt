#include "../../include/minirt.h"

t_ray generate_ray(int x, int y, t_scene *scene)
{
    t_ray   ray;
    t_vec3  camera_right;
    t_vec3  camera_up;
    t_vec3  viewport_center;
    double  aspect_ratio;
    double  fov_rad;
    double  x_norm;
    double  y_norm;

    ray.origin = scene->camera.position;

    fov_rad = scene->camera.fov * M_PI / 180.0;
    aspect_ratio = (double)scene->width / (double)scene->height;

    // Calcular los vectores "right" y "up" de la cámara
    camera_up = vec3_init(0.0, 1.0, 0.0);
    camera_right = vec3_normalize(vec3_cross(scene->camera.orientation, camera_up));
    camera_up = vec3_normalize(vec3_cross(camera_right, scene->camera.orientation));

    // Mapear las coordenadas del píxel a un plano de la vista
    x_norm = ((double)x / scene->width) - 0.5;
    y_norm = 0.5 - ((double)y / scene->height);
    
    // Calcular el punto en el plano de la vista
    viewport_center = vec3_add(scene->camera.position, scene->camera.orientation);
    ray.direction = vec3_add(vec3_add(viewport_center, vec3_mul(camera_right, 2.0 * tan(fov_rad / 2.0) * aspect_ratio * x_norm)),
                             vec3_mul(camera_up, 2.0 * tan(fov_rad / 2.0) * y_norm));
    ray.direction = vec3_sub(ray.direction, ray.origin);
    ray.direction = vec3_normalize(ray.direction);

    return (ray);
}

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
        // printf("Type: %i", objects[i]->type);
        if (objects[i]->type == SPHERE)
            intersect_sphere(ray, (t_sphere *)objects[i]->data, &current_hit);
        else if (objects[i]->type == PLANE)
            intersect_plane(ray, (t_plane *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CYLINDER)
            intersect_cylinder(ray, (t_cylinder *)objects[i]->data, &current_hit);

        if (current_hit.t > EPSILON && (min_t < 0 || current_hit.t < min_t))
        {
            min_t = current_hit.t;
            closest_hit = current_hit;
            // La información del objeto se asigna aquí.
            closest_hit.object = objects[i];
        }
        i++;
    }

    return (closest_hit);
}

t_color calculate_light(t_hit_record *rec, t_scene *scene)
{
    t_color final_color;
    t_color ambient_color;
    t_light **lights = (t_light **)scene->lights;
    int i = 0;

    // 1. Luz ambiente
    ambient_color = vec3_mult_vec(scene->ambient.color, rec->object->color);
    ambient_color = vec3_mul(ambient_color, scene->ambient.ratio);
    final_color = ambient_color;

    // 2. Iterar sobre las luces
    while (lights[i] != NULL)
    {
        // Vector desde el punto de impacto hasta la luz
        t_vec3 to_light = vec3_normalize(vec3_sub(lights[i]->position, rec->point));

        // --- Lanzar rayo de sombra y comprobar intersecciones ---
        t_ray shadow_ray;
        // Mover un poco el origen para evitar auto-intersección
        shadow_ray.origin = vec3_add(rec->point, vec3_mul(rec->normal, EPSILON)); 
        shadow_ray.direction = to_light;
        
        // Comprobar si el punto está en sombra
        if (is_in_shadow(&shadow_ray, scene, lights[i]))
        {
            i++;
            continue; // El punto está en sombra, saltar a la siguiente luz
        }

        // 3. Luz difusa (usando producto escalar)
        double dot_prod = vec3_dot(rec->normal, to_light);
        if (dot_prod > 0)
        {
            t_color light_contribution = vec3_mult_vec(lights[i]->color, rec->object->color);
            light_contribution = vec3_mul(light_contribution, lights[i]->brightness * dot_prod);
            final_color = vec3_add(final_color, light_contribution);
        }
        i++;
    }

    return (final_color);
}

int intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec)
{
    t_vec3  oc;
    double  a;
    double  b;
    double  c;
    double  discriminant;
    double  t;

    oc = vec3_sub(ray->origin, sp->center);
    a = vec3_dot(ray->direction, ray->direction);
    b = 2.0 * vec3_dot(oc, ray->direction);
    c = vec3_dot(oc, oc) - sp->radius * sp->radius;
    
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (0);

    t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t > EPSILON)
    {
        rec->t = t;
        rec->point = ray_at(*ray, rec->t);
        rec->normal = vec3_normalize(vec3_sub(rec->point, sp->center));
        return (1);
    }

    t = (-b + sqrt(discriminant)) / (2.0 * a);
    if (t > EPSILON)
    {
        rec->t = t;
        rec->point = ray_at(*ray, rec->t);
        rec->normal = vec3_normalize(vec3_sub(rec->point, sp->center));
        return (1);
    }
    return (0);
}

int intersect_plane(t_ray *ray, t_plane *pl, t_hit_record *rec)
{
    double  denominator;
    double  t;
    
    denominator = vec3_dot(pl->normal, ray->direction);
    if (fabs(denominator) < EPSILON)
        return (0);

    t = vec3_dot(vec3_sub(pl->position, ray->origin), pl->normal) / denominator;
    if (t < EPSILON)
        return (0);

    rec->t = t;
    rec->point = ray_at(*ray, rec->t);
    rec->normal = pl->normal;
    if (vec3_dot(rec->normal, ray->direction) > 0)
        rec->normal = vec3_mul(rec->normal, -1.0);

    return (1);
}

static int intersect_caps(t_ray *ray, t_cylinder *cy, double *t_cap, t_hit_record *rec)
{
    double  t_top;
    double  t_bottom;
    t_vec3  p_top;
    t_vec3  p_bottom;
    
    // Intersección con la tapa superior
    t_top = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis, cy->height / 2.0)), ray->origin), cy->axis) / vec3_dot(ray->direction, cy->axis);
    p_top = ray_at(*ray, t_top);
    if (t_top > EPSILON && vec3_length(vec3_sub(p_top, vec3_add(cy->position, vec3_mul(cy->axis, cy->height / 2.0)))) <= cy->radius)
        t_bottom = t_top;
    else
        t_bottom = -1.0;
        
    // Intersección con la tapa inferior
    t_bottom = vec3_dot(vec3_sub(vec3_add(cy->position, vec3_mul(cy->axis, -cy->height / 2.0)), ray->origin), cy->axis) / vec3_dot(ray->direction, cy->axis);
    p_bottom = ray_at(*ray, t_bottom);
    if (t_bottom > EPSILON && vec3_length(vec3_sub(p_bottom, vec3_add(cy->position, vec3_mul(cy->axis, -cy->height / 2.0)))) <= cy->radius)
    {
        if (t_bottom < t_top || t_top < 0)
            t_top = t_bottom;
    }
    
    if (t_top > EPSILON)
    {
        *t_cap = t_top;
        rec->point = ray_at(*ray, *t_cap);
        if (t_top == t_top) // Esto es para determinar si es la tapa de arriba o abajo
            rec->normal = cy->axis;
        else
            rec->normal = vec3_mul(cy->axis, -1.0);
        return (1);
    }
    return (0);
}


static int intersect_sides(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
    double  a, b, c;
    double  discriminant;
    double  t_side;
    t_vec3  oc = vec3_sub(ray->origin, cy->position);
    
    a = vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction, cy->axis))),
                 vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction, cy->axis))));
    b = 2.0 * vec3_dot(vec3_sub(ray->direction, vec3_mul(cy->axis, vec3_dot(ray->direction, cy->axis))),
                       vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))));
    c = vec3_dot(vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis))),
                 vec3_sub(oc, vec3_mul(cy->axis, vec3_dot(oc, cy->axis)))) - (cy->radius * cy->radius);

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (0);

    t_side = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t_side > EPSILON)
    {
        double m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc, cy->axis);
        if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
        {
            rec->t = t_side;
            rec->point = ray_at(*ray, rec->t);
            rec->normal = vec3_normalize(vec3_sub(rec->point, vec3_add(cy->position, vec3_mul(cy->axis, m))));
            return (1);
        }
    }
    
    t_side = (-b + sqrt(discriminant)) / (2.0 * a);
    if (t_side > EPSILON)
    {
        double m = vec3_dot(ray->direction, cy->axis) * t_side + vec3_dot(oc, cy->axis);
        if (m >= -cy->height / 2.0 && m <= cy->height / 2.0)
        {
            rec->t = t_side;
            rec->point = ray_at(*ray, rec->t);
            rec->normal = vec3_normalize(vec3_sub(rec->point, vec3_add(cy->position, vec3_mul(cy->axis, m))));
            return (1);
        }
    }
    return (0);
}

int intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec)
{
    t_hit_record side_hit;
    t_hit_record cap_hit;
    int side_found = intersect_sides(ray, cy, &side_hit);
    int cap_found = intersect_caps(ray, cy, &cap_hit.t, &cap_hit);
    
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

// Nuevo archivo para funciones de iluminación o utilidades
int is_in_shadow(t_ray *shadow_ray, t_scene *scene, t_light *light)
{
    t_hit_record    current_hit;
    t_object        **objects = (t_object **)scene->objects;
    double          light_dist;
    int             i;
    
    i = 0;
    // 1. Calcular la distancia desde el punto de impacto hasta la luz
    light_dist = vec3_length(vec3_sub(light->position, shadow_ray->origin));

    // 2. Iterar sobre todos los objetos para buscar una obstrucción
    while (objects[i] != NULL)
    {
        current_hit.t = -1.0;
        
        // Llamar a las funciones de intersección correspondientes
        if (objects[i]->type == SPHERE)
            intersect_sphere(shadow_ray, (t_sphere *)objects[i]->data, &current_hit);
        else if (objects[i]->type == PLANE)
            intersect_plane(shadow_ray, (t_plane *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CYLINDER)
            intersect_cylinder(shadow_ray, (t_cylinder *)objects[i]->data, &current_hit);

        // 3. Si hay una colisión, verificar si está entre el punto y la luz
        if (current_hit.t > EPSILON && current_hit.t < light_dist)
        {
            return (1); // El punto está en sombra
        }
        i++;
    }
    return (0); // No hay ningún objeto obstruyendo la luz
}