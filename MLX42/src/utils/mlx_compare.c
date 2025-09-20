/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_compare.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "MLX42/MLX42_Int.h"

//= Private =//

bool mlx_equal_image(void* lstcontent, void* value)
{
	const mlx_image_t* lcontent = lstcontent;
	const mlx_image_t* lvalue = value;

	return (lcontent == lvalue);
}

bool mlx_equal_inst(void* lstcontent, void* value)
{
	const draw_queue_t* lcontent = lstcontent;
	const mlx_image_t* lvalue = value;

	return (lcontent->image == lvalue);
}