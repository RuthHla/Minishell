/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_norme.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:52:44 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:05:30 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_ident_export(const char *s)
{
	size_t	i;

	i = 0;
	if (!s || !is_var_start(s[i]))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!is_var_continue(s[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*dup_n(const char *s, size_t n)
{
	char	*p;
	size_t	i;

	p = (char *)malloc(n + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < n)
	{
		p[i] = s[i];
		i++;
	}
	p[n] = '\0';
	return (p);
}

char	*make_env_kv(const char *name, const char *value)
{
	size_t	ln;
	size_t	lv;
	char	*kv;

	ln = ft_strlen(name);
	lv = ft_strlen(value);
	kv = (char *)malloc(ln + 1 + lv + 1);
	if (!kv)
		return (NULL);
	ft_memcpy(kv, name, ln);
	kv[ln] = '=';
	ft_memcpy(kv + ln + 1, value, lv);
	kv[ln + 1 + lv] = '\0';
	return (kv);
}

void	fill_argv(t_element *e, char **argv)
{
	size_t	i;

	i = 0;
	while (e)
	{
		if (e->kind == ARG)
		{
			if (e->u_.arg)
				argv[i] = e->u_.arg->str;
			else
				argv[i] = NULL;
			i++;
		}
		e = e->next;
	}
	argv[i] = NULL;
}

char	**build_export_argv(t_element *e)
{
	size_t	argc;
	char	**argv;

	argc = count_args(e);
	argv = (char **)ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	fill_argv(e, argv);
	return (argv);
}
