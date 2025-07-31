/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
// main.c
#include "../../include/minirt.h"// src/utils/error.c (o donde manejes tus utilidades)

#include <stdio.h> // Para printf
#include <stdlib.h> // Para exit()

// Necesitarás una función para liberar la memoria de la escena si se ha parseado parcialmente
// void free_scene_data(t_scene *scene); // Prototipo

void ft_error_exit(const char *msg)
{
    // Puedes añadir lógica para liberar recursos aquí si es necesario
    // Por ejemplo, si el error ocurre después de iniciar MLX, puedes cerrarlo
    // Pero si el error es en el parsing, no hay mucho que liberar aún.

    fprintf(stderr, "%s", msg); // Imprime el mensaje de error a stderr
    exit(1); // Sale del programa con código de error
}