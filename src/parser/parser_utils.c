#include "../../include/minirt.h"

// Convierte un string "x,y,z" a un t_vec3
t_vec3	parse_vec3(char *str)
{
	t_vec3	vec;
	char	**coords;

	coords = ft_split(str, ',');
	if (ft_strarr_len(coords) != 3)
		ft_error_exit("Error: Invalid vector format");
	vec.x = ft_atod(coords[0]);
	vec.y = ft_atod(coords[1]);
	vec.z = ft_atod(coords[2]);
	ft_free_str_array(coords);
	return (vec);
}

// Convierte un string de color "r,g,b" a un t_vec3
t_vec3	parse_vec3_color(char *str)
{
	t_vec3	color;
	char	**rgb;

	rgb = ft_split(str, ',');
	if (ft_strarr_len(rgb) != 3)
		ft_error_exit("Error: Invalid color format");
	color.x = ft_atod(rgb[0]) / 255.0;
	color.y = ft_atod(rgb[1]) / 255.0;
	color.z = ft_atod(rgb[2]) / 255.0;
	ft_free_str_array(rgb);
	return (color);
}

// Añade un objeto de forma dinámica a la escena
void	add_object_to_scene(t_scene *scene, t_object *obj)
{
	int			count;
	t_object	**new_objects;

	count = 0;
	if (scene->objects)
		while (((t_object **)scene->objects)[count])
			count++;
	new_objects = (t_object **)malloc(sizeof(t_object *) * (count + 2));
	if (!new_objects)
		ft_error_exit("Error: Malloc failed for objects");
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
void	add_light_to_scene(t_scene *scene, t_light *light)
{
	int		count;
	t_light	**new_lights;

	count = 0;
	if (scene->lights)
		while (((t_light **)scene->lights)[count])
			count++;
	new_lights = (t_light **)malloc(sizeof(t_light *) * (count + 2));
	if (!new_lights)
		ft_error_exit("Error: Malloc failed for lights");
	if (scene->lights)
	{
		ft_memcpy(new_lights, scene->lights, sizeof(t_light *) * (count + 1));
		free(scene->lights);
	}
	new_lights[count] = light;
	new_lights[count + 1] = NULL;
	scene->lights = new_lights;
}

void	ft_free_str_array(char **arr)
{
	int	i;

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

int	ft_strarr_len(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}

double	ft_atod(const char *str)
{
	double	result;
	double	decimal_part;
	int		sign;

	result = 0.0;
	decimal_part = 1.0;
	sign = 1;
	// 1. Manejar el signo
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	// 2. Convertir la parte entera
	while (*str && *str != '.')
	{
		result = result * 10.0 + (*str - '0');
		str++;
	}
	// 3. Convertir la parte decimal
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
t_object	*create_object(t_object_type type, void *data, t_vec3 color)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		ft_error_exit("Error: Memory allocation for object failed");
	obj->type = type;
	obj->data = data;
	obj->color = color;
	return (obj);
}
