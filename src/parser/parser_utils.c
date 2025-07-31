// src/parser_utils.c

#include "../../include/minirt.h"

// Simple argument validation
void    validate_args(int argc, char **argv)
{
    if (argc != 2)
        ft_error_exit(ERR_TOO_MANY_ARGS);
    if (ft_strlen(argv[1]) < 3 || ft_strcmp(argv[1] + ft_strlen(argv[1]) - 3, ".rt") != 0)
        ft_error_exit(ERR_INVALID_EXT);
}

// Checks if a string represents a valid float
int is_valid_float(char *str)
{
    int i = 0;
    int dot_count = 0;
    if (!str || (!ft_isdigit(str[0]) && str[0] != '-' && str[0] != '+'))
        return (0);
    if (str[0] == '-' || str[0] == '+')
        i++;
    while (str[i])
    {
        if (str[i] == '.')
            dot_count++;
        else if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (dot_count <= 1);
}

// Checks if a string represents a valid integer
int is_valid_int(char *str)
{
    int i = 0;
    if (!str || (!ft_isdigit(str[0]) && str[0] != '-' && str[0] != '+'))
        return (0);
    if (str[0] == '-' || str[0] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

// Checks if a string represents a valid color component (0-255)
int is_valid_color_component(char *str)
{
    long val;
    if (!is_valid_int(str))
        return (0);
    val = ft_atol(str); // Assuming ft_atol can handle long
    return (val >= 0 && val <= 255);
}

// Checks if a vector is normalized (components between -1.0 and 1.0)
int is_normalized_vector(t_vec3 vec)
{
    return (vec.x >= -1.0 && vec.x <= 1.0 &&
            vec.y >= -1.0 && vec.y <= 1.0 &&
            vec.z >= -1.0 && vec.z <= 1.0);
}


// Parses a vector string "x,y,z" into t_vec3
t_vec3 parse_vector(char *str, char *err_msg)
{
    char    **coords;
    t_vec3  vec;

    coords = ft_split(str, ',');
    if (!coords || ft_lstsize(coords) != 3 || // Assuming ft_lstsize works for char**
        !is_valid_float(coords[0]) || !is_valid_float(coords[1]) || !is_valid_float(coords[2]))
    {
        free_tokens(coords);
        ft_error_exit(err_msg);
    }
    vec.x = ft_atof(coords[0]); // Assuming ft_atof exists in libft
    vec.y = ft_atof(coords[1]);
    vec.z = ft_atof(coords[2]);
    free_tokens(coords);
    return (vec);
}

// Parses a color string "R,G,B" (0-255) into t_vec3 (0.0-1.0)
t_vec3 parse_color(char *str, char *err_msg)
{
    char    **components;
    t_vec3  color;

    components = ft_split(str, ',');
    if (!components || ft_lstsize(components) != 3 ||
        !is_valid_color_component(components[0]) ||
        !is_valid_color_component(components[1]) ||
        !is_valid_color_component(components[2]))
    {
        free_tokens(components);
        ft_error_exit(err_msg);
    }
    color.x = ft_atol(components[0]) / 255.0;
    color.y = ft_atol(components[1]) / 255.0;
    color.z = ft_atol(components[2]) / 255.0;
    free_tokens(components);
    return (color);
}

// Parses a double from string
double parse_double(char *str, char *err_msg)
{
    if (!is_valid_float(str))
        ft_error_exit(err_msg);
    return (ft_atof(str));
}

// Parses FOV (0-180 degrees)
double parse_fov(char *str, char *err_msg)
{
    long fov_val;
    if (!is_valid_int(str))
        ft_error_exit(err_msg);
    fov_val = ft_atol(str);
    if (fov_val < 0 || fov_val > 180)
        ft_error_exit(err_msg);
    return ((double)fov_val);
}

// Parses ratio (0.0-1.0)
double parse_ratio(char *str, char *err_msg)
{
    double ratio_val;
    if (!is_valid_float(str))
        ft_error_exit(err_msg);
    ratio_val = ft_atof(str);
    if (ratio_val < 0.0 || ratio_val > 1.0)
        ft_error_exit(err_msg);
    return (ratio_val);
}

// Helper to create a generic object node
t_object *create_object(t_object_type type, void *data_ptr, t_vec3 color)
{
    t_object *new_obj;

    new_obj = (t_object *)malloc(sizeof(t_object));
    if (!new_obj)
        ft_error_exit(ERR_MEMORY);
    new_obj->type = type;
    new_obj->color = color;
    new_obj->data = data_ptr;
    return (new_obj);
}

// Function to free the entire scene data (call before exiting)
void free_scene_data(t_scene *scene)
{
    // ft_lstclear(&scene->objects, your_free_object_data_function);
    // ft_lstclear(&scene->lights, your_free_light_data_function);
    // You'll need to implement your_free_object_data_function and your_free_light_data_function
    // These functions should free the 'data' pointer inside t_object and t_light if they were malloc'd
}
// src/parser/parser_utils.c

// In parse_vector:
t_vec3 parse_vector(char *str, char *err_msg)
{
    char    **coords;
    t_vec3  vec;

    coords = ft_split(str, ',');
    // CHANGE THIS LINE:
    if (!coords || count_tokens(coords) != 3 || // Use your new count_tokens function
        !is_valid_float(coords[0]) || !is_valid_float(coords[1]) || !is_valid_float(coords[2]))
    {
        free_tokens(coords);
        ft_error_exit(err_msg);
    }
    vec.x = ft_atof(coords[0]);
    vec.y = ft_atof(coords[1]);
    vec.z = ft_atof(coords[2]);
    free_tokens(coords);
    return (vec);
}

// In parse_color:
t_vec3 parse_color(char *str, char *err_msg)
{
    char    **components;
    t_vec3  color;

    components = ft_split(str, ',');
    // CHANGE THIS LINE:
    if (!components || count_tokens(components) != 3 ||
        !is_valid_color_component(components[0]) ||
        !is_valid_color_component(components[1]) ||
        !is_valid_color_component(components[2]))
    {
        free_tokens(components);
        ft_error_exit(err_msg);
    }
    color.x = ft_atol(components[0]) / 255.0;
    color.y = ft_atol(components[1]) / 255.0;
    color.z = ft_atol(components[2]) / 255.0;
    free_tokens(components);
    return (color);
}
// src/parser/parser_utils.c (add this function)

// ... (existing functions like parse_vector, etc.) ...

// Counts the number of strings in a NULL-terminated char** array
int count_tokens(char **tokens)
{
    int i = 0;
    if (!tokens)
        return (0);
    while (tokens[i])
        i++;
    return (i);
}