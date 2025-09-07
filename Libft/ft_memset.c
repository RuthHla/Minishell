/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:36:30 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:36:34 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t num)
{
	unsigned char	*p;
	size_t			c;

	c = 0;
	p = (unsigned char *)ptr;
	while (c < num)
	{
		p[c] = (unsigned char)value;
		c++;
	}
	return (ptr);
}
