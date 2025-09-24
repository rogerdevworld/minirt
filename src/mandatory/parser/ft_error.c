/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minirt.h"

/**
 * @brief Prints an error message and exits the program with a failure status.
 *

	* @details This function is a robust error-handling routine. 
	It writes the provided
 * string message to the standard error stream (file descriptor 2) followed by a
 * newline. It then immediately terminates the program, returning a non-zero
 * status code to the operating system to indicate an unsuccessful execution.
 *
 * @param msg The constant character string (error message) to be displayed.
 *
 * @return void. The function does not return, as it terminates the program.
 */
void	ft_error_exit(const char *msg)
{
	ft_putendl_fd((char *)msg, 2);
	exit(EXIT_FAILURE);
}
