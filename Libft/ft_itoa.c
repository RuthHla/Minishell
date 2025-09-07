/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:23:36 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:28:23 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	count(int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

void	fill(char *str, int n, size_t c)
{
	if (n == 0)
	{
		str[0] = '0';
		return ;
	}
	if (n < 0)
	{
		str[0] = '-';
		if (n == INT_MIN)
		{
			str[--c] = '8';
			n /= 10;
		}
		n = -n;
	}
	while (n > 0)
	{
		str[--c] = (n % 10) + '0';
		n = n / 10;
	}
}

char	*ft_itoa(int n)
{
	size_t	c;
	char	*str;

	c = count(n);
	str = (char *)malloc((c + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[c] = '\0';
	fill(str, n, c);
	return (str);
}
