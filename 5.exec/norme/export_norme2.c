/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_norme2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:49:33 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:05:11 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	try_replace(char **env, const char *name, const char *value)
{
	size_t	ln;
	int		i;
	char	*nv;

	if (!env)
		return (0);
	ln = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, ln) == 0 && env[i][ln] == '=')
		{
			nv = make_env_kv(name, value);
			if (!nv)
				return (0);
			free(env[i]);
			env[i] = nv;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	append_env(char ***penv, const char *name, const char *value)
{
	char	**env;
	char	**tmp;
	char	*nv;
	int		n;

	env = *penv;
	n = 0;
	while (env && env[n])
		n++;
	tmp = (char **)realloc(env, (n + 2) * sizeof(*tmp));
	if (!tmp)
		return (0);
	*penv = tmp;
	nv = make_env_kv(name, value);
	if (!nv)
		return (0);
	tmp[n] = nv;
	tmp[n + 1] = NULL;
	return (1);
}

int	setenv_in_vec(char ***penv, const char *name, const char *value)
{
	if (try_replace(*penv, name, value))
		return (1);
	return (append_env(penv, name, value));
}

int	print_all_exports(t_shell *sh)
{
	int	i;

	i = 0;
	while (sh->env && sh->env[i])
	{
		print_export_line(sh->env[i]);
		i++;
	}
	return (0);
}

int	process_export_args(char **argv, int idx, t_shell *sh)
{
	int	status;

	status = 0;
	while (argv[idx])
	{
		if (!handle_export_arg(argv[idx], sh))
			status = 1;
		idx++;
	}
	return (status);
}
