// src/parser/parser_elements.c

#include "../../include/minirt.h" // Ruta relativa desde src/parser/

void parse_light(char **tokens, t_data *data)
{
    // L -5.0,5.0,0.0 0.7 255,255,255
    if (count_tokens(tokens) != 4)
        ft_error_exit(ERR_INVALID_PARAMS);

    t_light *new_light = (t_light *)malloc(sizeof(t_light));
    if (!new_light)
        ft_error_exit(ERR_MEMORY);

    new_light->position = parse_vector(tokens[1], ERR_INVALID_PARAMS);
    new_light->brightness = parse_ratio(tokens[2], ERR_INVALID_PARAMS);
    new_light->color = parse_color(tokens[3], ERR_INVALID_PARAMS);
    new_light->next = NULL;

    t_light **head = (t_light **)&data->scene.lights;
    while (*head)
        head = &(*head)->next;
    *head = new_light;
}

void parse_sphere(char **tokens, t_data *data)
{
    // sp 0.0,0.0,0.0 1.0 255,0,0
    if (count_tokens(tokens) != 4)
        ft_error_exit(ERR_INVALID_PARAMS);

    // --- FIX START ---
    t_sphere *sp = (t_sphere *)malloc(sizeof(t_sphere)); // Declare sp
    if (!sp)
        ft_error_exit(ERR_MEMORY);

    sp->center = parse_vector(tokens[1], ERR_INVALID_PARAMS);
    sp->radius = parse_double(tokens[2], ERR_INVALID_PARAMS) / 2.0; // Diameter -> Radius

    t_color obj_color = parse_color(tokens[3], ERR_INVALID_PARAMS); // Declare obj_color
    // --- FIX END ---

    t_object *new_obj = create_object(OBJ_SPHERE, sp, obj_color);

    t_object **head = (t_object **)&data->scene.objects;
    while (*head)
        head = &(*head)->next;
    *head = new_obj;
}

void parse_plane(char **tokens, t_data *data)
{
    // pl 0.0,-1.0,0.0 0.0,1.0,0.0 0,255,0
    if (count_tokens(tokens) != 4)
        ft_error_exit(ERR_INVALID_PARAMS);

    // --- FIX START ---
    t_plane *pl = (t_plane *)malloc(sizeof(t_plane)); // Declare pl
    if (!pl)
        ft_error_exit(ERR_MEMORY);

    pl->point = parse_vector(tokens[1], ERR_INVALID_PARAMS);
    pl->normal = parse_vector(tokens[2], ERR_INVALID_PARAMS);
    if (!is_normalized_vector(pl->normal))
        ft_error_exit(ERR_INVALID_PARAMS); // Or normalize: pl->normal = vec3_normalize(pl->normal);

    t_color obj_color = parse_color(tokens[3], ERR_INVALID_PARAMS); // Declare obj_color
    // --- FIX END ---

    t_object *new_obj = create_object(OBJ_PLANE, pl, obj_color);

    t_object **head = (t_object **)&data->scene.objects;
    while (*head)
        head = &(*head)->next;
    *head = new_obj;
}

void parse_cylinder(char **tokens, t_data *data)
{
    // cy 1.0,1.0,0.0 0.0,1.0,0.0 0.5 1.5 0,0,255
    if (count_tokens(tokens) != 6)
        ft_error_exit(ERR_INVALID_PARAMS);

    // --- FIX START ---
    t_cylinder *cy = (t_cylinder *)malloc(sizeof(t_cylinder)); // Declare cy
    if (!cy)
        ft_error_exit(ERR_MEMORY);

    cy->position = parse_vector(tokens[1], ERR_INVALID_PARAMS);
    cy->axis = parse_vector(tokens[2], ERR_INVALID_PARAMS);
    if (!is_normalized_vector(cy->axis))
        ft_error_exit(ERR_INVALID_PARAMS); // Or normalize
    cy->radius = parse_double(tokens[3], ERR_INVALID_PARAMS) / 2.0; // Diameter -> Radius
    cy->height = parse_double(tokens[4], ERR_INVALID_PARAMS);

    t_color obj_color = parse_color(tokens[5], ERR_INVALID_PARAMS); // Declare obj_color
    // --- FIX END ---

    t_object *new_obj = create_object(OBJ_CYLINDER, cy, obj_color);

    t_object **head = (t_object **)&data->scene.objects;
    while (*head)
        head = &(*head)->next;
    *head = new_obj;
}

void ft_free_str_array(char **arr)
{
    int i;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void parse_ambient_light(char **tokens, t_data *data)
{
    // Check for duplicate ambient light definition
    if (data->scene.has_ambient)
        ft_error_exit(ERR_DUPLICATE_ELEMENT); // Assuming ERR_DUPLICATE_ELEMENT is defined

    // Validate the number of parameters
    // Expects "A", ratio, and color (3 tokens total)
    if (count_tokens(tokens) != 3)
        ft_error_exit(ERR_INVALID_PARAMS);

    // Parse brightness ratio (0.0 to 1.0)
    data->scene.ambient.ratio = parse_ratio(tokens[1], "ERR_INVALID_RATIO_RANGE"); // Assuming parse_ratio and ERR_INVALID_RATIO_RANGE

    // Parse color (0-255 for each R,G,B component)
    data->scene.ambient.color = parse_color(tokens[2], "ERR_INVALID_COLOR_RANGE"); // Assuming parse_color and ERR_INVALID_COLOR_RANGE

    // Mark that ambient light has been successfully parsed
    data->scene.has_ambient = 1;
}

// Function to parse the camera element (C)
// Format: C <X,Y,Z> <X,Y,Z> <FOV>
void parse_camera(char **tokens, t_data *data)
{
    // Check for duplicate camera definition
    if (data->scene.has_camera)
        ft_error_exit(ERR_DUPLICATE_ELEMENT);

    // Validate the number of parameters
    // Expects "C", position, orientation, FOV (4 tokens total)
    if (count_tokens(tokens) != 4)
        ft_error_exit(ERR_INVALID_PARAMS);

    // Parse camera position
    data->scene.camera.position = parse_vector(tokens[1], ERR_INVALID_PARAMS);

    // Parse camera orientation vector (must be normalized)
    data->scene.camera.orientation = parse_vector(tokens[2], "ERR_INVALID_NORM_VECTOR");
    // You might want to normalize it here if parse_vector doesn't guarantee it
    // data->scene.camera.orientation = vec3_normalize(data->scene.camera.orientation);
    if (!is_normalized_vector(data->scene.camera.orientation)) // Assuming you have this check
        ft_error_exit("ERR_INVALID_NORM_VECTOR");

    // Parse camera FOV (Field of View, 0 to 180 degrees)
    data->scene.camera.fov = parse_double(tokens[3], "ERR_INVALID_RATIO_RANGE"); // parse_double or parse_angle
    if (data->scene.camera.fov < 0 || data->scene.camera.fov > 180)
        ft_error_exit("ERR_INVALID_RATIO_RANGE"); // Using ratio range for FOV too, or make a specific one

    // Mark that camera has been successfully parsed
    data->scene.has_camera = 1;
}