/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_norme.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:20:24 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:05:16 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_var_start(int c)
{
	return (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	is_var_continue(int c)
{
	return (is_var_start(c) || (c >= '0' && c <= '9'));
}

int	is_valid_ident(const char *s)
{
	size_t	i;

	i = 0;
	if (!s || !is_var_start(s[i]))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!is_var_continue(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	remove_env_var(char ***penv, const char *name)
{
	char	**env;
	size_t	ln;
	int		w;
	int		r;

	env = *penv;
	if (!env)
		return ;
	ln = ft_strlen(name);
	w = 0;
	r = 0;
	while (env[r])
	{
		if (!(strncmp(env[r], name, ln) == 0 && env[r][ln] == '='))
			env[w++] = env[r];
		else
			free(env[r]);
		r++;
	}
	env[w] = NULL;
}

size_t	count_args(t_element *e)
{
	size_t	count;

	count = 0;
	while (e)
	{
		if (e->kind == ARG)
			count++;
		e = e->next;
	}
	return (count);
}
