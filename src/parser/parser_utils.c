/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 22:25:08 by jaacosta          #+#    #+#             */
/*   Updated: 2025/08/12 22:25:11 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

// Validacion para el angulo de cono
double	parse_angle(char *str)
{
	double	angle;

	if (!str)
		ft_error_exit("MiniRT: Error: FOV string is NULL");
	angle = ft_atod(str);
	if (angle < 0 || angle > 90)
		ft_error_exit("MiniRT: Error: Angle must be in range (0,90) degrees");
	return (angle);
}

// Validadcion para los valores doubles que deben ser positivos
double	parse_positive_double(char *str)
{
	double	val;
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\r\n");
	if (!trimmed)
		ft_error_exit("MiniRT: Error: Memory allocation failed");
	if (trimmed[0] == '\0')
		ft_error_exit("MiniRT: Error: Empty numeric value");
	val = ft_atod(trimmed);
	free(trimmed);
	if (val <= 0)
		ft_error_exit("MiniRT: Error: Incorrect data values");
	return (val);
}

// Validacion del valor fov de la camara
double	parse_fov(char *str)
{
	double	fov;
	char	*trimmed;

	fov = 0.0;
	if (!str)
		ft_error_exit("MiniRT: Error: FOV string is NULL");
	trimmed = ft_strtrim(str, " \r\t\n");
	if (!trimmed)
		ft_error_exit("MiniRT: Error: allocating memory failed in parse_fov");
	fov = ft_atod(trimmed);
	free(trimmed);
	if (fov < 0 || fov > 180)
		ft_error_exit("MiniRT: Error: FOV must be in range (0, 180)");
	return (fov);
}

// Convierte un string "x,y,z" a un t_vec3
t_vec3  parse_vec3(char *str)
{
    t_vec3  vec;
    char    **coords;

	coords = ft_split(str, ',');
	if (ft_strarr_len(coords) != 3)
		ft_error_exit("MiniRT: Error: Invalid vector format");
	vec.x = ft_atod(coords[0]);
	vec.y = ft_atod(coords[1]);
	vec.z = ft_atod(coords[2]);
	ft_free_str_array(coords);
	return (vec);
}

// valida que el vector "x,y,z" se encuentre normalizado entre [-1,1]
t_vec3	parse_vec3_normalized(char *str)
{
	t_vec3	vec;
	char	**coords;

	if (!str)
		ft_error_exit("MiniRT: Error: color string is NULL");
	coords = ft_split(str, ',');
	if (ft_strarr_len(coords) != 3)
	{
		ft_free_str_array(coords);
		ft_error_exit("MiniRT: Error: Invalid vector format");
	}
	vec.x = ft_atod(coords[0]);
	vec.y = ft_atod(coords[1]);
	vec.z = ft_atod(coords[2]);
	ft_free_str_array(coords);
	if (vec.x < -1 || vec.x > 1
		|| vec.y < -1 || vec.y > 1
		|| vec.z < -1 || vec.z > 1)
		ft_error_exit("MiniRT: Error: Vector components normalized must be between -1 and 1");
	return (vec);
}

void	validate_is_integer(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			ft_error_exit("MiniRT: Error: color components must be integers");
		i++;
	}
}

//valida un string que trae los valores de r,g,b
double	validate_color_component(char *str)
{
	char	*trimmed;
	int		v;

	trimmed = ft_strtrim(str, " \r\t\n");
	if (!trimmed)
		ft_error_exit("MiniRT: Error: allocating memory in color parser");
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		ft_error_exit("MiniRT: Error: color component is empty");
	}
	validate_is_integer(trimmed);
	v = ft_atoi(trimmed);
	free(trimmed);
	if (v < 0 || v > 255)
		ft_error_exit("MiniRT: Error: Color values must be in range 0–255");
	return (v / 255.0);
}

// Convierte un string de color "r,g,b" a un t_vec3
t_vec3  parse_vec3_color(char *str)
{
    t_vec3  color;
    char    **rgb;

	if (!str)
		ft_error_exit("MiniRT: Error: color string is NULL");
	rgb = ft_split(str, ',');
	if (ft_strarr_len(rgb) != 3)
	{
		ft_free_str_array(rgb);
		ft_error_exit("MiniRT: Error: Invalid color format");
	}
	color.x = validate_color_component(rgb[0]);
	color.y = validate_color_component(rgb[1]);
	color.z = validate_color_component(rgb[2]);
	ft_free_str_array(rgb);
	return (color);
}

// Añade un objeto de forma dinámica a la escena
void    add_object_to_scene(t_scene *scene, t_object *obj)
{
    int         count;
    t_object    **new_objects;

	count = 0;
	if (scene->objects)
		while (((t_object **)scene->objects)[count])
			count++;
	new_objects = (t_object **)malloc(sizeof(t_object *) * (count + 2));
	if (!new_objects)
		ft_error_exit("MiniRT: Error: Malloc failed for objects");
	if (scene->objects)
	{
		ft_memcpy(new_objects, scene->objects, sizeof(t_object *) * (count
				+ 1));
		free(scene->objects);
	}
	new_objects[count] = obj;
	new_objects[count + 1] = NULL;
	scene->objects = new_objects;
}

// Añade una luz de forma dinámica a la escena
void    add_light_to_scene(t_scene *scene, t_light *light)
{
    int     count;
    t_light **new_lights;

	count = 0;
	if (scene->lights)
		while (((t_light **)scene->lights)[count])
			count++;
	new_lights = (t_light **)malloc(sizeof(t_light *) * (count + 2));
	if (!new_lights)
		ft_error_exit("MiniRT: Error: Malloc failed for lights");
	if (scene->lights)
	{
		ft_memcpy(new_lights, scene->lights, sizeof(t_light *) * (count + 1));
		free(scene->lights);
	}
	new_lights[count] = light;
	new_lights[count + 1] = NULL;
	scene->lights = new_lights;
}

void    ft_free_str_array(char **arr)
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

int ft_strarr_len(char **arr)
{
    int len;

    len = 0;
    if (!arr)
        return (0);
    while (arr[len])
        len++;
    return (len);
}

double  ft_atod(const char *str)
{
    double  result;
    double  decimal_part;
    int     sign;

	result = 0.0;
	decimal_part = 1.0;
	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str && *str != '.')
	{
		result = result * 10.0 + (*str - '0');
		str++;
	}
	if (*str == '.')
	{
		str++;
		while (*str)
		{
			decimal_part /= 10.0;
			result = result + (*str - '0') * decimal_part;
			str++;
		}
	}
	return (result * sign);
}
