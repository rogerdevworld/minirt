#include "../../include/minirt.h"

void	ft_str_to_lower(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		s[i] = ft_tolower(s[i]);
		i++;
	}
}

int valid_extension_rt(const char *filename)
{
    int len;

    len = ft_strlen(filename);
    if (len < 4)
        return (0);
    return (ft_strncmp(filename + len - 3, ".rt", 3) == 0);
}

void    parse_specular(char *token, t_specular *spec)
{
    char    **parts;

    parts = ft_split(token + 4, ',');
    if (ft_strarr_len(parts) != 2)
        ft_error_exit("Error: Invalid specular format");
    spec->intensity = ft_atod(parts[0]);
    spec->shininess = (int)ft_atod(parts[1]);
    ft_free_str_array(parts);
}

double  parse_mirror(char *token)
{
    return (ft_atod(token + 4));
}

void    parse_checkboard(char *token, t_material *material)
{
    char    **parts;

    parts = ft_split(token + 4, ';');
    if (ft_strarr_len(parts) != 3)
        ft_error_exit("MiniRT: Error: Invalid checkerboard format");
    material->albedo.type = TEX_CHECKERBOARD;
    material->albedo.color1 = parse_vec3_color(parts[0]);
    material->albedo.color2 = parse_vec3_color(parts[1]);
    material->albedo.scale = ft_atod(parts[2]);
    ft_free_str_array(parts);
}

// Corregida la validación de extensiones
int validate_texture_extension(const char *file_name)
{
    int len;
    char *lower_case_name;
    int result;

    if (!file_name)
        return (0);
    len = ft_strlen(file_name);
    if (len < 4)
        return (0);
    lower_case_name = ft_strdup(file_name);
    if (!lower_case_name)
        ft_error_exit("MiniRT: Error: Memory allocation failed");
    ft_str_to_lower(lower_case_name);
    result = (ft_strncmp(lower_case_name + len - 4, ".png", 4) == 0)
        || (ft_strncmp(lower_case_name + len - 4, ".xpm", 4) == 0);
    free(lower_case_name);
    return (result);
}

char    *copy_trimmed_token(char *token, int len)
{
    char    *trimmed;

    trimmed = ft_substr(token, 0, len);
    if (!trimmed)
        ft_error_exit("MiniRT: Error: allocating memory for bmp");
    return (trimmed);
}

// Corregida la lógica de parseo
char    *parse_bump_map(char *token)
{
    char    *path;
    int     len;
    int     fd;

    if (!token)
        ft_error_exit("MiniRT: Error: bump map token is missing");
    token += 4; // Skip "bmp:"
    while (*token && isspace(*token))
        token++;
    if (!*token)
        ft_error_exit("MiniRT: Error: bump map path is empty");
    len = 0;
    while (token[len] && !isspace(token[len]))
        len++;
    path = copy_trimmed_token(token, len);
    if (!validate_texture_extension(path))
    {
        free(path);
        ft_error_exit("MiniRT: Error: texture file must have .png or .xpm extension");
    }
    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        free(path);
        ft_error_exit("MiniRT: Error: Failed to open texture file");
    }
    close(fd);
    return (path);
}

t_material *create_material(t_vec3 albedo_color)
{
    t_material *material;

    material = malloc(sizeof(t_material));
    if (!material)
        ft_error_exit("Error: Failed to allocate material.");
    
    // Initialize the albedo texture as a solid color.
    material->albedo.type = TEX_SOLID;
    material->albedo.color1 = albedo_color;
    material->albedo.color2 = vec3_init(0, 0, 0);
    material->albedo.img_path = NULL;
    material->albedo.img = NULL;
    
    // Initialize the normal map as a solid color (no texture).
    material->normal_map.type = TEX_SOLID;
    material->normal_map.color1 = vec3_init(0, 0, 0);
    material->normal_map.img_path = NULL;
    material->normal_map.img = NULL;

    material->specular.intensity = 0.0;
    material->specular.shininess = 0;
    material->reflectivity = 0.0;
    material->refraction_idx = 0.0;
    return (material);
}

// Modifica tu función create_object
t_object    *create_object(t_object_type type, void *data, t_material *material)
{
    t_object    *obj;

    obj = malloc(sizeof(t_object));
    if (!obj)
    {
        free(data);
        ft_error_exit("Error: Memory allocation for object failed.");
    }
    obj->type = type;
    obj->data = data;
    obj->material = material;
    return (obj);
}

void    apply_object_modifiers(t_object *obj, char **tokens, int start_idx)
{
    int i;

    i = start_idx;
    while (tokens[i])
    {
        if (ft_strncmp(tokens[i], "spc:", 4) == 0)
            parse_specular(tokens[i], &obj->material->specular);
        else if (ft_strncmp(tokens[i], "mir:", 4) == 0)
            obj->material->reflectivity = parse_mirror(tokens[i]);
        else if (ft_strncmp(tokens[i], "chk:", 4) == 0)
            parse_checkboard(tokens[i], obj->material);
        else if (ft_strncmp(tokens[i], "bmp:", 4) == 0)
        {
            // Libre el puntero anterior antes de asignar uno nuevo para evitar un memory leak
            if (obj->material->normal_map.img_path)
                free(obj->material->normal_map.img_path);
            obj->material->normal_map.img_path = parse_bump_map(tokens[i]);
            obj->material->normal_map.type = TEX_IMAGE;
        }
        
        i++;
    }
}