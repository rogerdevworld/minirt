/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero  <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:22:49 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:23:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, char c)
{
	const unsigned char		*local_str;
	size_t					len;

	len = ft_strlen(str);
	local_str = (const unsigned char *)str;
	if ((unsigned char)c == '\0')
		return ((char *)&local_str[len]);
	while (len > 0)
	{
		len--;
		if (local_str[len] == (unsigned char)c)
			return ((char *)&local_str[len]);
	}
	return (NULL);
}
/*
int main()
{
    char str[] = "www.holamundo.com";
    char c = '.';

    printf("%s", ft_strrchr(str,c));

    return 0;
}*/
