/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Frees a dynamically allocated array of strings.
 *
 * @details This function safely deallocates a null-terminated array of strings.
 * It first iterates through the array, freeing each individual string pointed to
 * by `arr[i]`, and then frees the array itself. The function handles a NULL
 * input for `arr` without causing any errors.
 *
 * @param arr The null-terminated array of strings to be freed.
 *
 * @return void.
 */
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
