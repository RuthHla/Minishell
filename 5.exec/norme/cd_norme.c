/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_norme.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:16:48 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 12:19:46 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*make_env_kv_cd(const char *name, const char *value)
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

static int	try_replace_cd(char **env, const char *name, const char *value)
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
			nv = make_env_kv_cd(name, value);
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

static int	append_env_cd(char ***penv, const char *name, const char *value)
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
	nv = make_env_kv_cd(name, value);
	if (!nv)
		return (0);
	tmp[n] = nv;
	tmp[n + 1] = NULL;
	return (1);
}

int	setenv_in_vec_cd(char ***penv, const char *name, const char *value)
{
	if (try_replace_cd(*penv, name, value))
		return (1);
	return (append_env_cd(penv, name, value));
}
