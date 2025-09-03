/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:02:17 by alandel           #+#    #+#             */
/*   Updated: 2025/09/03 12:04:28 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

char	**init_local_env(void)
{
	char	**env;

	env = malloc(sizeof(char *) * 6);
	if (!env)
		return (NULL);
	env[0] = strdup("USER=alandel");
	env[1] = strdup("HOME=/home/alandel");
	env[2] = strdup("SHLVL=1");
	env[3] = strdup("PATH=/usr/bin:/bin");
	env[4] = strdup("PWD=/home/alandel");
	env[5] = NULL;
	return (env);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp && envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp && envp[i])
	{
		new_env[i] = strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
