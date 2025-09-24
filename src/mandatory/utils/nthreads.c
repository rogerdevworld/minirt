/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Obtiene el número de procesadores lógicos disponibles en el sistema.
 *
 * @details Esta función utiliza la llamada al sistema `sysconf` con la constante
 * `_SC_NPROCESSORS_ONLN` para obtener el número de núcleos de CPU disponibles
 * para su uso. Es un método estándar y portable en sistemas tipo POSIX para
 * determinar la capacidad de paralelización del hardware.
 *
 * @return Retorna un entero que representa el número de procesadores lógicos.
 * En caso de que la llamada a `sysconf` falle o retorne un valor menor a 1,
 * la función devuelve 1 para asegurar que siempre haya al menos un hilo de
 * ejecución.
 */
int	get_num_processors(void)
{
	long	num_cores;

	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (num_cores < 1)
		return (1);
	return ((int)num_cores);
}
