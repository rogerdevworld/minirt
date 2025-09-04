/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:34:13 by jaacosta          #+#    #+#             */
/*   Updated: 2025/09/04 20:34:16 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"

int	get_num_processors(void)
{
	long	num_cores;

	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (num_cores < 1)
		return (1);
	return ((int)num_cores);
}
