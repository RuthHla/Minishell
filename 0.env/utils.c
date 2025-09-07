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

int get_shlvl_value(char **envp)
{
	int i = 0;
	
	if (!envp)
		return (0);
	
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			return (ft_atoi(envp[i] + 6));
		i++;
	}
	
	return (0);
}

char *get_env_value_from_envp(char **envp, const char *var)
{
    int i = 0;
    int var_len = ft_strlen(var);
    
    if (!envp || !var)
        return NULL;
        
    while (envp[i])
    {
        if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
            return &envp[i][var_len + 1];
        i++;
    }
    return NULL;
}

char *get_env_value(char **env, const char *var)
{
    int i = 0;
    int var_len = ft_strlen(var);
    
    if (!env || !var)
        return NULL;
        
    while (env[i])
    {
        if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
            return &env[i][var_len + 1];
        i++;
    }
    return NULL;
}

int count_env_vars(char **env)
{
    int count = 0;
    if (!env)
        return 0;
    while (env[count])
        count++;
    return count;
}