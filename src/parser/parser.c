// src/parser.c

#include "../../include/minirt.h"

static void init_scene(t_scene *scene)
{
    scene->width = 0; // Se establecerán por defecto o desde la cámara/config
    scene->height = 0;
    scene->has_camera = 0;
    scene->has_ambient = 0;
    scene->lights = NULL;
    scene->objects = NULL;
    // Inicializar otros valores predeterminados si es necesario
    scene->ambient.ratio = 0.0;
    scene->ambient.color = vec3_init(0, 0, 0);
    scene->camera.fov = 0.0;
    scene->camera.orientation = vec3_init(0,0,0);
    scene->camera.position = vec3_init(0,0,0);
}

// Function to handle errors and exit
void    ft_error_exit(char *msg)
{
    ft_putendl_fd(msg, 2); // Escribe en stderr
    exit(EXIT_FAILURE);
}

// Function to free token array
void    free_tokens(char **tokens)
{
    int i = 0;
    if (!tokens)
        return ;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

// Main parsing function
void    parse_rt_file(const char *file_path, t_data *data)
{
    int     fd;
    char    *line;
    char    **tokens;

    validate_args(2, (char *[]){"minirt", (char *)file_path}); // Simple validation
    init_scene(&data->scene); // Initialize scene fields

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        ft_error_exit(ERR_INVALID_FILE);

    while (1)
    {
        line = get_next_line(fd);
        if (!line)
            break; // End of file

        // Remove newline character if present
        if (line[ft_strlen(line) - 1] == '\n')
            line[ft_strlen(line) - 1] = '\0';

        if (ft_strlen(line) == 0 || line[0] == '#') // Skip empty lines and comments
        {
            free(line);
            continue;
        }

        tokens = ft_split(line, ' ');
        free(line); // Free the line read by GNL

        if (!tokens || !tokens[0])
        {
            free_tokens(tokens);
            ft_error_exit(ERR_INVALID_LINE);
        }

        if (ft_strcmp(tokens[0], "A") == 0)
            parse_ambient_light(tokens, data);
        else if (ft_strcmp(tokens[0], "C") == 0)
            parse_camera(tokens, data);
        else if (ft_strcmp(tokens[0], "L") == 0)
            parse_light(tokens, data);
        else if (ft_strcmp(tokens[0], "sp") == 0)
            parse_sphere(tokens, data);
        else if (ft_strcmp(tokens[0], "pl") == 0)
            parse_plane(tokens, data);
        else if (ft_strcmp(tokens[0], "cy") == 0)
            parse_cylinder(tokens, data);
        // Add more elements as you implement them (e.g., co for cone)
        else
            ft_error_exit(ERR_INVALID_LINE);

        free_tokens(tokens);
    }
    close(fd);

    // Final scene validation (e.g., ensure camera and ambient light exist)
    if (!data->scene.has_camera || !data->scene.has_ambient)
        ft_error_exit(ERR_MISSING_ELEMENT);
}