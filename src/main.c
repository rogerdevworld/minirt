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
#include "../include/minirt.h" // Contains all struct definitions
#include "../include/utils.h"   // Contains prototypes for vec3_init, camera_init, etc.
#include "../include/windows.h" // Contains prototypes for mlx_setup, render_threaded, etc.

// ... rest of your main.c code ...


int	main(int argc, char **argv)
{
	t_data data; // Main structure holding everything

	// Las siguientes dos líneas no hacen nada útil y pueden eliminarse
	// argc =argc;
	// argv = argv;

	// 1. Parse the scene file (simulated here)
	// You will parse your .rt file here and populate data.scene
	// For now, initializing with dummy values for testing
	data.scene.width = 800;
	data.scene.height = 600;
	data.scene.camera = camera_init(vec3_init(0, 0, 0), vec3_init(0, 0, 1), 90);
	data.scene.ambient = ambient_light_init(0.1, vec3_init(1, 1, 1));
	data.scene.lights = NULL; // Initialize to NULL or an empty list
	data.scene.objects = NULL; // Initialize to NULL or an empty list

	// --- New: Set number of threads ---
	data.num_threads = 8; // Example: use 8 threads. Adjust based on CPU cores.
							// You might get this from an environment variable or command line argument.

	// --- AÑADE ESTO AQUÍ para inicializar el mutex y el contador ---
	data.rendered_rows = 0; // Inicializa el contador de filas renderizadas a 0
	if (pthread_mutex_init(&data.progress_mutex, NULL) != 0) {
		// Manejo de error si la inicialización del mutex falla
		// Usar ft_error_exit si está disponible, o simplemente imprimir y salir
		printf("Error\nNo se pudo inicializar el mutex de progreso.\n");
		return (1); // O llama a ft_error_exit("...")
	}
	// ------------------------------------------------------------------

	// 2. Initialize Minilibx and create window/image
	mlx_setup(&data);

	// 3. Render the scene using threads
	render_threaded(&data); // Call the threaded render function

	// 4. Set up event hooks
	// Close window on 'X' button
	mlx_hook(data.mlx.win_ptr, 17, 0, close_window, &data);
	// Close window on ESC key (optional)
	mlx_key_hook(data.mlx.win_ptr, key_hook, &data);

	// 5. Start the Minilibx loop to keep the window open
	mlx_loop(data.mlx.mlx_ptr);

	// No necesitas destruir el mutex aquí, ya lo haces en render_threaded
	// después de que todos los hilos se unan.
	// pthread_mutex_destroy(&data.progress_mutex); // NO AQUÍ, ya se hace en render_threaded

	// Puedes agregar aquí la limpieza de data.scene.lights y data.scene.objects
	// free_scene_data(&data.scene); // Si tienes una función así

	return (0);
}