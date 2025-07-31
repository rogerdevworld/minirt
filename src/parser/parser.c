// src/parser/parser.c

#include "../../include/minirt.h" // Ruta relativa desde src/parser/

static void init_scene(t_scene *scene)
{
    // Valores predeterminados para la escena si no se especifican en el archivo .rt
    scene->width = 800;  // Ancho por defecto
    scene->height = 600; // Alto por defecto
    scene->has_camera = 0;
    scene->has_ambient = 0;
    scene->lights = NULL;
    scene->objects = NULL;
    scene->ambient.ratio = 0.0;
    scene->ambient.color = vec3_init(0, 0, 0); // Color por defecto (negro)
    scene->camera.fov = 0.0;
    scene->camera.orientation = vec3_init(0,0,0);
    scene->camera.position = vec3_init(0,0,0);
}

// Main parsing function
void    parse_rt_file(const char *file_path, t_data *data)
{
    int     fd;
    char    *line;
    char    **tokens;

    validate_args(2, (char *[]){"minirt", (char *)file_path}); // Asume que argv[0] es "minirt"
    init_scene(&data->scene); // Initialize scene fields

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        ft_error_exit(ERR_INVALID_FILE);

    while (1)
    {
        line = get_next_line(fd);
        if (!line)
            break; // End of file

        // Eliminar el carácter de nueva línea si está presente
        if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
            line[ft_strlen(line) - 1] = '\0';

        if (ft_strlen(line) == 0 || line[0] == '#') // Saltar líneas vacías y comentarios
        {
            free(line);
            continue;
        }

        tokens = ft_split(line, ' ');
        free(line); // Liberar la línea leída por GNL

        if (!tokens || !tokens[0])
        {
            free_tokens(tokens);
            ft_error_exit(ERR_INVALID_LINE);
        }

        // Parsear elementos de la escena
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
        // Añade más elementos aquí si los implementas (e.g., "co" para cono)
        else
        {
            free_tokens(tokens); // Asegúrate de liberar los tokens antes de salir por error
            ft_error_exit(ERR_INVALID_LINE); // Identificador desconocido
        }
        free_tokens(tokens); // Liberar los tokens después de procesar la línea
    }
    close(fd);

    // Validación final de la escena (asegurarse de que la cámara y la luz ambiental existen)
    if (!data->scene.has_camera || !data->scene.has_ambient)
    {
        ft_error_exit(ERR_MISSING_ELEMENT);
    }
}

// In src/parser/parser.c (or wherever your main parsing function is)
int parse_scene(const char *file_path, t_data *data) // Or parse_rt_file
{
    int     fd;
    char    *line;
    char    **tokens;
    char    *comment_start; // To find the '#'
    size_t  len;

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        ft_error_exit("ERR_FILE_OPEN");

    while ((line = get_next_line(fd)) != NULL)
    {
        // 1. Remove trailing newline character
        len = strlen(line); // Or ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        // --- FIX START: Remove inline comments ---
        // Find the first '#' character
        comment_start = strchr(line, '#'); // Or ft_strchr(line, '#');
        if (comment_start)
        {
            *comment_start = '\0'; // Null-terminate the string at the '#'
        }
        // --- FIX END ---

        // 2. Skip empty lines or lines that are now empty after comment removal
        // Also check for leading/trailing spaces after split if ft_split is not robust
        if (line[0] == '\0') // Check if line is empty after trimming and comment removal
        {
            free(line);
            continue;
        }

        // Tokenize the line (split by spaces)
        tokens = ft_split(line, ' ');
        if (!tokens || !tokens[0]) // Check if split resulted in no valid tokens
        {
            free(line);
            ft_free_str_array(tokens); // Always free tokens if split was successful but content is empty
            continue; // Skip line if it only contained spaces/tabs or was made empty by stripping
        }

        // 3. Identify element type and call appropriate parser
        if (ft_strncmp(tokens[0], "A", 2) == 0)
            parse_ambient_light(tokens, data);
        else if (ft_strncmp(tokens[0], "C", 2) == 0)
            parse_camera(tokens, data);
        else if (ft_strncmp(tokens[0], "L", 2) == 0)
            parse_light(tokens, data);
        else if (ft_strncmp(tokens[0], "sp", 3) == 0)
            parse_sphere(tokens, data);
        else if (ft_strncmp(tokens[0], "pl", 3) == 0)
            parse_plane(tokens, data);
        else if (ft_strncmp(tokens[0], "cy", 3) == 0)
            parse_cylinder(tokens, data);
        else
        {
            free(line);
            ft_free_str_array(tokens);
            ft_error_exit("ERR_UNKNOWN_IDENTIFIER"); // Should not happen with current logic if file is valid
        }

        free(line);
        ft_free_str_array(tokens);
    }
    close(fd);

    if (!data->scene.has_ambient || !data->scene.has_camera)
        ft_error_exit(ERR_MISSING_ELEMENT);

    return (0);
}