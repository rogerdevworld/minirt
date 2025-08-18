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
    viewport_center = vec3_add(scene->camera.position, vec3_mul(scene->camera.orientation, 1.0));
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
        if (objects[i]->type == SPHERE)
            intersect_sphere(ray, (t_sphere *)objects[i]->data, &current_hit);
        else if (objects[i]->type == PLANE)
            intersect_plane(ray, (t_plane *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CYLINDER)
            intersect_cylinder(ray, (t_cylinder *)objects[i]->data, &current_hit);
        else if (objects[i]->type == CONE) // Nuevo: Cono
            intersect_cone(ray, (t_cone *)objects[i]->data, &current_hit);
        else if (objects[i]->type == HYPERBOLOID) // Nuevo: Hiperboloide
            intersect_hyperboloid(ray, (t_hyperboloid *)objects[i]->data, &current_hit);

        if (current_hit.t > EPSILON && (min_t < 0 || current_hit.t < min_t))
        {
            min_t = current_hit.t;
            closest_hit = current_hit;
            closest_hit.object = objects[i];
            closest_hit.ray = *ray; // Almacena el rayo para la reflexión
        }
        i++;
    }
    return (closest_hit);
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

// Función corregida para la intersección de un rayo con un cono
int intersect_cone(t_ray *ray, t_cone *cn, t_hit_record *rec)
{
    t_vec3  oc;
    double  a, b, c, discriminant;
    double  t, t1, t2;
    double  m, m1, m2;
    double  cone_cos2;

    oc = vec3_sub(ray->origin, cn->position);
    
    // Calcular el coseno al cuadrado del ángulo del cono
    // tan(theta) = radius / (height/2)
    // cos2(theta) = 1 / (1 + tan2(theta)) = 1 / (1 + (2*radius/height)^2)
    cone_cos2 = (cn->radius / (cn->height / 2.0));
    cone_cos2 = 1.0 / (1.0 + cone_cos2 * cone_cos2);

    // Coeficientes de la ecuación cuadrática
    a = vec3_dot(ray->direction, ray->direction) - cone_cos2 * pow(vec3_dot(ray->direction, cn->axis), 2);
    b = 2.0 * (vec3_dot(ray->direction, oc) - cone_cos2 * vec3_dot(ray->direction, cn->axis) * vec3_dot(oc, cn->axis));
    c = vec3_dot(oc, oc) - cone_cos2 * pow(vec3_dot(oc, cn->axis), 2);

    discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0)
        return (0);

    t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    
    // Calcular las distancias a lo largo del eje del cono
    m1 = vec3_dot(ray->direction, cn->axis) * t1 + vec3_dot(oc, cn->axis);
    m2 = vec3_dot(ray->direction, cn->axis) * t2 + vec3_dot(oc, cn->axis);

    if (t1 > EPSILON && m1 >= -cn->height / 2.0 && m1 <= cn->height / 2.0)
        t = t1;
    else if (t2 > EPSILON && m2 >= -cn->height / 2.0 && m2 <= cn->height / 2.0)
        t = t2;
    else
        return (0);

    m = vec3_dot(ray->direction, cn->axis) * t + vec3_dot(oc, cn->axis);
    
    rec->t = t;
    rec->point = ray_at(*ray, rec->t);
    rec->normal = vec3_normalize(vec3_sub(vec3_sub(rec->point, cn->position), 
                                          vec3_mul(cn->axis, m * (1 + (cn->radius / (cn->height/2.0)) * (cn->radius / (cn->height/2.0))))));
    
    if (vec3_dot(rec->normal, ray->direction) > 0)
        rec->normal = vec3_mul(rec->normal, -1);
    
    return (1);
}

int intersect_hyperboloid(t_ray *ray, t_hyperboloid *hp, t_hit_record *rec)
{
    t_vec3  oc;
    double  a, b, c;
    double  t, t1, t2, discriminant;
    t_vec3  u_v, w_v;
    double  dot_d_axis, dot_oc_axis;

    oc = vec3_sub(ray->origin, hp->position);
    dot_d_axis = vec3_dot(ray->direction, hp->axis);
    dot_oc_axis = vec3_dot(oc, hp->axis);

    // Calcular los vectores ortogonales al eje para la transformación
    if (fabs(hp->axis.x) > 0.9)
        u_v = vec3_init(0, 1, 0);
    else
        u_v = vec3_init(1, 0, 0);
    u_v = vec3_normalize(vec3_cross(u_v, hp->axis));
    w_v = vec3_normalize(vec3_cross(hp->axis, u_v));

    double d_u = vec3_dot(ray->direction, u_v);
    double d_w = vec3_dot(ray->direction, w_v);
    double oc_u = vec3_dot(oc, u_v);
    double oc_w = vec3_dot(oc, w_v);

    double z_limit = hp->height / 2.0;

    // Coeficientes de la ecuación cuadrática del hiperboloide
    a = (d_u * d_u) / (hp->radius_a * hp->radius_a) + (d_w * d_w) / (hp->radius_b * hp->radius_b) - (dot_d_axis * dot_d_axis);
    b = 2.0 * ((d_u * oc_u) / (hp->radius_a * hp->radius_a) + (d_w * oc_w) / (hp->radius_b * hp->radius_b) - (dot_d_axis * dot_oc_axis));
    c = (oc_u * oc_u) / (hp->radius_a * hp->radius_a) + (oc_w * oc_w) / (hp->radius_b * hp->radius_b) - (dot_oc_axis * dot_oc_axis);

    discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0)
        return (0);

    t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    
    // Se calcula la distancia a lo largo del eje para verificar si está dentro de la altura del hiperboloide
    double m1 = vec3_dot(ray_at(*ray, t1), hp->axis);
    double m2 = vec3_dot(ray_at(*ray, t2), hp->axis);

    if (t1 > EPSILON && m1 >= hp->position.z - z_limit && m1 <= hp->position.z + z_limit)
        t = t1;
    else if (t2 > EPSILON && m2 >= hp->position.z - z_limit && m2 <= hp->position.z + z_limit)
        t = t2;
    else
        return (0);
    
    rec->t = t;
    rec->point = ray_at(*ray, rec->t);

    // Calcular la normal en el punto de intersección
    t_vec3 point_relative = vec3_sub(rec->point, hp->position);
    double x_n = (2.0 * vec3_dot(point_relative, u_v)) / (hp->radius_a * hp->radius_a);
    double y_n = (2.0 * vec3_dot(point_relative, w_v)) / (hp->radius_b * hp->radius_b);
    double z_n = -2.0 * vec3_dot(point_relative, hp->axis);
    
    rec->normal = vec3_normalize(vec3_add(vec3_add(vec3_mul(u_v, x_n), vec3_mul(w_v, y_n)), vec3_mul(hp->axis, z_n)));
    
    // Asegurarse de que la normal apunte hacia la dirección del rayo incidente
    if (vec3_dot(rec->normal, ray->direction) > 0)
        rec->normal = vec3_mul(rec->normal, -1);

    return (1);
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

t_color get_texture_color(t_hit_record *rec, t_texture *texture)
{
    // Check if the texture pointer itself is valid.
    printf("get_texture_color: texture ptr: %p\n", texture);
    if (!texture)
    {
        printf("Error: texture is NULL!\n");
        return (vec3_init(0, 0, 0)); // Return a default color
    }
    
    // Check the type of the texture.
    printf("get_texture_color: texture type: %d\n", texture->type);

    if (texture->type == TEX_CHECKERBOARD)
    {
        // Check if the colors are valid before using them.
        printf("Checkerboard: color1 x:%.2f, y:%.2f, z:%.2f\n", texture->color1.x, texture->color1.y, texture->color1.z);
        printf("Checkerboard: color2 x:%.2f, y:%.2f, z:%.2f\n", texture->color2.x, texture->color2.y, texture->color2.z);

        t_vec3 local_point = rec->point;
        int pattern_x = (int)floor(local_point.x / texture->scale);
        int pattern_y = (int)floor(local_point.y / texture->scale);
        int pattern_z = (int)floor(local_point.z / texture->scale);

        if ((pattern_x + pattern_y + pattern_z) % 2 == 0)
            return (texture->color1);
        else
            return (texture->color2);
    }
    else if (texture->type == TEX_IMAGE)
    {
        t_vec2      uv;
        int         x;
        int         y;
        char        *dst;

        // Check if the image pointer is valid.
        printf("Image texture: img ptr: %p\n", texture->img);

        if (!texture->img)
        {
            printf("Error: Image pointer is NULL!\n");
            return (texture->color1);
        }
        printf("Image texture: img width: %d, height: %d\n", texture->img->width, texture->img->height);
        printf("Image texture: img line_len: %d, bpp: %d\n", texture->img->line_len, texture->img->bpp);
        
        // This is the line where the corruption is likely occurring.
        if (rec->object->type == SPHERE)
        {
            printf("Image texture: sphere detected.\n");
            t_vec3 local_point = vec3_normalize(vec3_sub(rec->point, ((t_sphere*)rec->object->data)->center));
            uv.x = 0.5 + atan2(local_point.z, local_point.x) / (2.0 * M_PI);
            uv.y = 0.5 - asin(local_point.y) / M_PI;
        }
        else if (rec->object->type == PLANE)
        {
            printf("Image texture: plane detected.\n");
            double scale = texture->scale;
            uv.x = fmod(rec->point.x / scale, 1.0);
            uv.y = fmod(rec->point.z / scale, 1.0);
            if (uv.x < 0) uv.x += 1.0;
            if (uv.y < 0) uv.y += 1.0;
        }
        
        // Check the calculated UV coordinates.
        printf("UV coordinates: x=%.4f, y=%.4f\n", uv.x, uv.y);

        x = (int)(uv.x * texture->img->width);
        y = (int)(uv.y * texture->img->height);

        x = x % texture->img->width;
        y = y % texture->img->height;
        if (x < 0) x = texture->img->width + x;
        if (y < 0) y = texture->img->height + y;

        // Check the final pixel coordinates.
        printf("Pixel coordinates: x=%d, y=%d\n", x, y);

        // This is where the read occurs. If any of the above pointers are bad, this will crash.
        dst = texture->img->addr + (y * texture->img->line_len + x * (texture->img->bpp / 8));
        
        // Check if dst pointer is valid before dereferencing.
        printf("dst pointer: %p\n", dst);

        t_color color;
        unsigned int pixel_color = *(unsigned int *)dst;
        color.x = (double)((pixel_color >> 16) & 0xFF) / 255.0;
        color.y = (double)((pixel_color >> 8) & 0xFF) / 255.0;
        color.z = (double)(pixel_color & 0xFF) / 255.0;

        return (color);
    }
    // Return a solid color if the type is not handled.
    printf("Solid color: x:%.2f, y:%.2f, z:%.2f\n", texture->color1.x, texture->color1.y, texture->color1.z);
    return (texture->color1);
}

t_color get_object_color(t_hit_record *rec)
{
    return (get_texture_color(rec, &rec->object->material->albedo));
}

t_color calculate_specular_light_with_normal(t_hit_record *rec, t_light *light, t_ray *ray, t_vec3 normal)
{
    t_vec3      to_light;
    t_vec3      view_dir;
    t_vec3      reflected_dir;
    double      spec_factor;
    t_color     specular_color;

    to_light = vec3_normalize(vec3_sub(light->position, rec->point));
    view_dir = vec3_normalize(vec3_mul(ray->direction, -1.0));
    reflected_dir = vec3_reflect(to_light, normal);

    spec_factor = vec3_dot(view_dir, reflected_dir);
    if (spec_factor > 0)
    {
        spec_factor = pow(spec_factor, rec->object->material->specular.shininess);
        specular_color = vec3_mul(light->color, rec->object->material->specular.intensity * spec_factor);
        return (specular_color);
    }
    return (vec3_init(0, 0, 0));
}

t_color calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
    t_color final_color;
    t_color object_color;
    t_vec3  final_normal;
    t_light **lights = (t_light **)scene->lights;
    int     i = 0;

    if (rec->object->material->normal_map.type == TEX_IMAGE)
    {
        t_color bump_color = get_texture_color(rec, &rec->object->material->normal_map);
        t_vec3 bump_normal = vec3_init(
            (bump_color.x * 2.0) - 1.0,
            (bump_color.y * 2.0) - 1.0,
            (bump_color.z * 2.0) - 1.0
        );
        final_normal = vec3_normalize(vec3_add(rec->normal, bump_normal));
    }
    else
    {
        final_normal = rec->normal;
    }
    if (vec3_dot(final_normal, ray->direction) > 0)
        final_normal = vec3_mul(final_normal, -1);

    object_color = get_object_color(rec);

    t_color ambient_color = vec3_mult_vec(scene->ambient.color, object_color);
    ambient_color = vec3_mul(ambient_color, scene->ambient.ratio);
    final_color = ambient_color;

    while (lights[i] != NULL)
    {
        t_vec3 to_light = vec3_normalize(vec3_sub(lights[i]->position, rec->point));
        t_ray shadow_ray;
        shadow_ray.origin = vec3_add(rec->point, vec3_mul(final_normal, EPSILON));
        shadow_ray.direction = to_light;

        if (is_in_shadow(&shadow_ray, scene, lights[i]))
        {
            i++;
            continue;
        }

        double dot_prod = vec3_dot(final_normal, to_light);
        if (dot_prod > 0)
        {
            t_color light_contribution = vec3_mult_vec(lights[i]->color, object_color);
            light_contribution = vec3_mul(light_contribution, lights[i]->brightness * dot_prod);
            final_color = vec3_add(final_color, light_contribution);
        }

        if (rec->object->material->specular.intensity > 0.0)
        {
            t_color specular_color = calculate_specular_light_with_normal(rec, lights[i], ray, final_normal);
            final_color = vec3_add(final_color, specular_color);
        }
        i++;
    }
    return (final_color);
}