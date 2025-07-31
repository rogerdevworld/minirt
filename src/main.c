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
#include "../include/minirt.h"

// -----------------------------------------------

// --- Función para obtener el número de procesadores (opcional pero recomendado) ---
int get_num_processors(void) {
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN); // Obtener procesadores online
    if (num_cores < 1)
        return 1; // Volver a 1 si la detección falla
    return (int)num_cores;
}

int	main(int argc, char **argv)
{
	t_data data; // Estructura principal que contiene todo

	// 1. Validar argumentos de línea de comandos
	if (argc != 2)
	{
		ft_error_exit("Error\nUso: ./minirt <archivo_escena.rt>\n");
	}

	// 2. Inicializar la estructura de la escena con valores seguros (NULL/0)
    // Es CRUCIAL inicializar punteros a NULL para evitar Valgrind errors
    // y permitir comprobaciones seguras de `free` más adelante.
    data.scene.lights = NULL;
    data.scene.objects = NULL;
    data.scene.width = 0;
    data.scene.height = 0;
    data.scene.has_ambient = 0;
    data.scene.has_camera = 0;

	// 3. Parsear el archivo de escena .rt
    // Aquí es donde llamas a tu función para leer y llenar `data.scene`
    if (!parse_scene(argv[1], &data.scene))
    {
        // parse_scene debe retornar 0 o false si hay un error en el archivo
        ft_error_exit("Error\nFallo al parsear el archivo de escena o archivo invalido.\n");
    }

    // --- Validación de escena mínima (ejemplo) ---
    // Asegúrate de que tienes al menos una cámara y una luz ambiental,
    // o maneja esos casos en tu renderizador.
    if (!data.scene.has_camera || !data.scene.has_ambient)
    {
        ft_error_exit("Error\nLa escena debe contener al menos una camara (C) y una luz ambiental (A).\n");
    }

	// 4. Configurar el número de hilos (basado en el número de núcleos de la CPU)
	data.num_threads = get_num_processors();
	// Opcional: limitar el número máximo de hilos si lo deseas, por ejemplo a 8
	// if (data.num_threads > 8) data.num_threads = 8;


	// 5. Inicializar el mutex para la barra de progreso
	data.rendered_rows = 0; // Inicializa el contador de filas renderizadas
	if (pthread_mutex_init(&data.progress_mutex, NULL) != 0) {
		ft_error_exit("Error\nNo se pudo inicializar el mutex de progreso.\n");
	}

	// 6. Inicializar Minilibx y crear ventana/imagen
	// (Asegúrate de que mlx_setup ahora usa data.scene.width y data.scene.height)
	mlx_setup(&data);

	// 7. Renderizar la escena usando hilos
	render_threaded(&data); // Llama a la función de renderizado multihilo

	// 8. Configurar los hooks de eventos de la ventana
	mlx_hook(data.mlx.win_ptr, 17, 0, close_window, &data); // Cerrar en botón 'X'
	mlx_key_hook(data.mlx.win_ptr, key_hook, &data); // Cerrar en tecla ESC (o la que definas)

	// 9. Iniciar el bucle principal de Minilibx para mantener la ventana abierta
	mlx_loop(data.mlx.mlx_ptr);

	// Esto no se alcanzará si close_window llama a exit(0)
	return (0);
}