#include "../../include/minirt.h"

char    *copy_trimmed_token(char *token, int len)
{
    char    *trimmed;
    int     i;

    trimmed = (char *)malloc(len + 1);
    if (!trimmed)
        ft_error_exit("MiniRT: Error: allocating memory for bmp");
    i = 0;
    while (i < len)
    {
        trimmed[i] = token[i];
        i++;
    }
    trimmed[i] = '\0';
    return (trimmed);
}

char    *parse_bump_map(char *token)
{
    char    *path;
    int     len;
    int     fd;

    if (!token)
        ft_error_exit("MiniRT: Error: bump map token is missing");
    token += 4;
    len = 0;
    while (token[len] && token[len] != '\n' && token[len] != '\r'
        && token[len] != ' ' && token[len] != '\t')
        len++;
    if (len <= 4)
        ft_error_exit("MiniRT: Error: bump map path is empty");
    path = copy_trimmed_token(token, len);
    if (!validate_texture_extension(path))
    {
        free(path);
        ft_error_exit("MiniRT: Error:  texture file \
            must have .png or .xpm extension");
    }
    fd = open_filename(path);
    validate_file(fd, path);
    close(fd);
    return (path);
}

static void parse_basic_modifiers(t_object *obj, char **tokens, int *i)
{
    if (ft_strncmp(tokens[*i], "spc:", 4) == 0)
        parse_specular(tokens[*i], &obj->material->specular);
    else if (ft_strncmp(tokens[*i], "mir:", 4) == 0)
        obj->material->mirror_ratio = parse_mirror(tokens[*i]);
    else if (ft_strncmp(tokens[*i], "chk:", 4) == 0)
        parse_checkboard(tokens[*i], obj->material);
}

static void parse_texture_modifier(t_object *obj, char *token)
{
    char    *path;

    if (ft_strncmp(token, "bmp:", 4) == 0)
    {
        path = ft_strtrim(token + 4, " \t\n\r");
        obj->material->texture_img = mlx_load_png(path);
        if (!obj->material->texture_img)
        {
            free(path);
            ft_error_exit("Error: Failed to load PNG bump map.");
        }
        obj->material->has_normal_map = true;
        free(path);
    }
    if (ft_strncmp(token, "tex:", 4) == 0)
    {
        path = ft_strtrim(token + 4, " \t\n\r");
        obj->material->color_img = mlx_load_png(path);
        if (!obj->material->color_img)
        {
            free(path);
            ft_error_exit("Error: Failed to load PNG texture.");
        }
        obj->material->has_texture = true;
        free(path);
    }
}

void    apply_object_modifiers(t_object *obj, char **tokens, int start_idx)
{
    int i;

    i = start_idx;
    while (tokens[i])
    {
        parse_basic_modifiers(obj, tokens, &i);
        parse_texture_modifier(obj, tokens[i]);
        i++;
    }
}