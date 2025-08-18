#include "../../include/minirt.h"

// Parser for Sphere (sp)
void    parse_sphere(t_scene *scene, char **tokens)
{
    t_sphere    *sp;
    t_object    *obj;
    t_material  *material;

    if (ft_strarr_len(tokens) < 4)
        ft_error_exit("Error: Sphere format invalid");
    sp = (t_sphere *)malloc(sizeof(t_sphere));
    if (!sp)
        ft_error_exit("Error: Memory allocation failed");
    sp->center = parse_vec3(tokens[1]);
    sp->radius = parse_positive_double(tokens[2]);
    
    // Allocate and initialize the material.
    material = create_material(parse_vec3_color(tokens[3]));
    obj = create_object(SPHERE, sp, material);
    apply_object_modifiers(obj, tokens, 4);
    add_object_to_scene(scene, obj);
}

// Parser for Plane (pl)
void    parse_plane(t_scene *scene, char **tokens)
{
    t_plane     *pl;
    t_object    *obj;
    t_material  *material;

    if (ft_strarr_len(tokens) < 4)
        ft_error_exit("Error: Plane format invalid");
    pl = (t_plane *)malloc(sizeof(t_plane));
    if (!pl)
        ft_error_exit("Error: Memory allocation failed");
    pl->position = parse_vec3(tokens[1]);
    pl->normal = parse_vec3_normalized(tokens[2]);
    
    material = create_material(parse_vec3_color(tokens[3]));
    obj = create_object(PLANE, pl, material);
    apply_object_modifiers(obj, tokens, 4);
    add_object_to_scene(scene, obj);
}

// Parser de Cilindro (cy)
void    parse_cylinder(t_scene *scene, char **tokens)
{
    t_cylinder  *cy;
    t_object    *obj;
    t_material  *material;

    if (ft_strarr_len(tokens) < 6)
        ft_error_exit("Error: Cylinder format invalid");
    cy = (t_cylinder *)malloc(sizeof(t_cylinder));
    if (!cy)
        ft_error_exit("Error: Memory allocation failed");
    cy->position = parse_vec3(tokens[1]);
    cy->axis = parse_vec3_normalized(tokens[2]);
    cy->radius = parse_positive_double(tokens[3]);
    cy->height = parse_positive_double(tokens[4]);
    
    material = create_material(parse_vec3_color(tokens[5]));
    obj = create_object(CYLINDER, cy, material);
    apply_object_modifiers(obj, tokens, 6);
    add_object_to_scene(scene, obj);
}

// Parser de Cono (cn)
void    parse_cone(t_scene *scene, char **tokens)
{
    t_cone      *cn;
    t_object    *obj;
    t_material  *material;

    if (ft_strarr_len(tokens) < 6)
        ft_error_exit("Error: Cone format invalid");
    cn = malloc(sizeof(t_cone));
    if (!cn)
        ft_error_exit("Error: Memory allocation failed");
    cn->position = parse_vec3(tokens[1]);
    cn->axis = parse_vec3_normalized(tokens[2]);
    cn->radius = parse_positive_double(tokens[3]);
    cn->height = parse_positive_double(tokens[4]);
    
    material = create_material(parse_vec3_color(tokens[5]));
    obj = create_object(CONE, cn, material);
    apply_object_modifiers(obj, tokens, 6);
    add_object_to_scene(scene, obj);
}

// Parser de Paraboloide (pb)
void    parse_paraboloid(t_scene *scene, char **tokens)
{
    t_parab     *pb;
    t_object    *obj;
    t_material  *material;

    if (ft_strarr_len(tokens) < 6)
        ft_error_exit("Error: Paraboloid format invalid");
    pb = malloc(sizeof(t_parab));
    if (!pb)
        ft_error_exit("Error: Memory allocation failed");
    pb->position = parse_vec3(tokens[1]);
    pb->axis = parse_vec3_normalized(tokens[2]);
    pb->focal_lenght = parse_positive_double(tokens[3]);
    pb->height = parse_positive_double(tokens[4]);
    
    material = create_material(parse_vec3_color(tokens[5]));
    obj = create_object(PARABOLOID, pb, material);
    apply_object_modifiers(obj, tokens, 6);
    add_object_to_scene(scene, obj);
}

// Parser de Hiperboloide (hp)
void    parse_hyperboloid(t_scene *scene, char **tokens)
{
    t_hyperboloid *hb;
    t_object *obj;
    t_material *material;

    if (ft_strarr_len(tokens) < 7)
        ft_error_exit("Error: Hyperboloid format invalid");
    hb = malloc(sizeof(t_hyperboloid));
    if (!hb)
        ft_error_exit("Error: Memory allocation failed");
    hb->position = parse_vec3(tokens[1]);
    hb->axis = parse_vec3_normalized(tokens[2]);
    hb->radius_a = parse_positive_double(tokens[3]);
    hb->radius_b = parse_positive_double(tokens[4]);
    hb->height = parse_positive_double(tokens[5]);
    
    material = create_material(parse_vec3_color(tokens[6]));
    obj = create_object(HYPERBOLOID, hb, material);
    apply_object_modifiers(obj, tokens, 7);
    add_object_to_scene(scene, obj);
}

// Parser de Cámara (C)
void	parse_camera(t_scene *scene, char **tokens)
{
	if (scene->has_camera)
		ft_error_exit("Error: Multiple cameras not allowed");
	if (ft_strarr_len(tokens) != 4)
		ft_error_exit("Error: Camera format invalid");
	scene->camera.position = parse_vec3(tokens[1]);
	scene->camera.orientation = parse_vec3_normalized(tokens[2]);
	scene->camera.fov = parse_fov(tokens[3]);
	scene->has_camera = 1;
}

// Parser de Luz (L)
void	parse_light(t_scene *scene, char **tokens)
{
	t_light	*light;
	t_vec3	color;
	double	brightness;

	if (ft_strarr_len(tokens) != 4)
		ft_error_exit("Error: Light format invalid");
	light = (t_light *)malloc(sizeof(t_light));
	if (!light)
		ft_error_exit("Error: Memory allocation failed");
	light->position = parse_vec3(tokens[1]);
	brightness = ft_atod(tokens[2]);
	if (brightness < 0.0 || brightness > 1.0)
		ft_error_exit("Error: brightness must be betwen 0.0 and 1.0");
	light->brightness = brightness;
	color = parse_vec3_color(tokens[3]);
	light->color = color;
	add_light_to_scene(scene, light);
}

// Parser de Luz Ambiental (A)
void	parse_ambient(t_scene *scene, char **tokens)
{
	t_vec3	color;
	double	ratio;

	if (scene->has_ambient)
		ft_error_exit("Error: Multiple ambient lights not allowed");
	if (ft_strarr_len(tokens) != 3)
		ft_error_exit("Error: Ambient light format invalid");
	ratio = ft_atod(tokens[1]);
	if (ratio < 0.0 || ratio > 1.0)
		ft_error_exit("Error: Ambient ratio must be betwen 0.0 and 1.0");
	color = parse_vec3_color(tokens[2]);
	scene->ambient.ratio = ratio;
	scene->ambient.color = color;
	scene->has_ambient = 1;
}

void	ft_error_exit(const char *msg)
{
	ft_putendl_fd((char *)msg, 2);
	exit(EXIT_FAILURE);
}
