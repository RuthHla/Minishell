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

	env = malloc(sizeof(char *) * 5);
	if (!env)
		return (NULL);
	env[0] = ft_strdup("USER=alandel");
	env[1] = ft_strdup("HOME=/home/alandel");
	env[2] = ft_strdup("PATH=/usr/bin:/bin");
	env[3] = ft_strdup("PWD=/home/alandel");
	env[4] = NULL;
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
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
