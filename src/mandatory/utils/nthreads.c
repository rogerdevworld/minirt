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

#include "../../include/minirt.h"

int	get_num_processors(void)
{
	long	num_cores;

	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (num_cores < 1)
		return (1);
	return ((int)num_cores);
}
