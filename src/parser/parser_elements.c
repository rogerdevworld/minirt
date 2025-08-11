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
	sp->radius = ft_atod(tokens[2]);
	obj = create_object(SPHERE, sp, parse_vec3_color(tokens[3]));
	//apply bonus
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
	pl->normal = parse_vec3(tokens[2]);
	obj = create_object(PLANE, pl, parse_vec3_color(tokens[3]));
	//apply bonus
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
	cy->axis = parse_vec3(tokens[2]);
	cy->radius = ft_atod(tokens[3]);
	cy->height = ft_atod(tokens[4]);
	obj = create_object(CYLINDER, cy, parse_vec3_color(tokens[5]));
	//apply bonus
	apply_object_modifiers(obj, tokens, 6);
	add_object_to_scene(scene, obj);
}

// Parser de Cono (co) ->bonus
void	parse_cone(t_scene *scene, char **tokens)
{
	t_cone *cn;
	t_object *obj;

	if (ft_strarr_len(tokens) < 6)
		ft_error_exit("Error: Cone format invalid");

	cn = malloc(sizeof(t_cone));
	if (!cn)
		ft_error_exit("Error: Memory allocation failed");
	cn->position = parse_vec3(tokens[1]);
	cn->axis = parse_vec3(tokens[2]);
	cn->angle = ft_atod(tokens[3]);//en grados
	cn->height = ft_atod(tokens[4]);
	obj = create_object(CONE, cn, parse_vec3_color(tokens[5]));
	apply_object_modifiers(obj, tokens, 6);
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
	scene->camera.orientation = parse_vec3(tokens[2]);
	scene->camera.fov = ft_atod(tokens[3]);
	scene->has_camera = 1;
}

// Parser de Luz (L)
void	parse_light(t_scene *scene, char **tokens)
{
	t_light	*light;

	if (ft_strarr_len(tokens) != 4)
		ft_error_exit("Error: Light format invalid");
	light = (t_light *)malloc(sizeof(t_light));
	if (!light)
		ft_error_exit("Error: Memory allocation failed");
	light->position = parse_vec3(tokens[1]);
	light->brightness = ft_atod(tokens[2]);
	light->color = parse_vec3_color(tokens[3]);
	add_light_to_scene(scene, light);
}

// Parser de Luz Ambiental (A)
void	parse_ambient(t_scene *scene, char **tokens)
{
	if (scene->has_ambient)
		ft_error_exit("Error: Multiple ambient lights not allowed");
	if (ft_strarr_len(tokens) != 3)
		ft_error_exit("Error: Ambient light format invalid");
	scene->ambient.ratio = ft_atod(tokens[1]);
	scene->ambient.color = parse_vec3_color(tokens[2]);
	scene->has_ambient = 1;
}

void	ft_error_exit(const char *msg)
{
	ft_putendl_fd((char *)msg, 2);
	exit(EXIT_FAILURE);
}