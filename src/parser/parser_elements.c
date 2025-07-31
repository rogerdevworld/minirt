#include "../../include/minirt.h"

// src/parser/parser_elements.c
void parse_ambient_light(char **tokens, t_data *data)
{
    if (data->scene.has_ambient)
        ft_error_exit(ERR_DUPLICATE_ELEMENT);
    // CHANGE THIS LINE:
    if (count_tokens(tokens) != 3) // Use count_tokens
        ft_error_exit(ERR_INVALID_PARAMS);

    data->scene.ambient.ratio = parse_ratio(tokens[1], ERR_INVALID_PARAMS);
    data->scene.ambient.color = parse_color(tokens[2], ERR_INVALID_PARAMS);
    data->scene.has_ambient = 1;
}