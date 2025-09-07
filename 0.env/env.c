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

void add_env_var(char ***env, char *new_var)
{
    int count = count_env_vars(*env);
    char **new_env;
    int i;
    
    new_env = malloc(sizeof(char*) * (count + 2));
    if (!new_env)
        return;
    
    i = 0;
    while (i < count)
    {
        new_env[i] = (*env)[i];
        i++;
    }
    
    new_env[count] = new_var;
    new_env[count + 1] = NULL;
    
    free(*env);
    *env = new_env;
}

void set_env_var(char ***env, const char *var, const char *value)
{
    char *new_var;
    char *temp;
    int i = 0;
    int var_len = ft_strlen(var);
    
    if (!env || !var || !value)
        return;
    
    temp = ft_strjoin(var, "=");
    if (!temp)
        return;
    new_var = ft_strjoin(temp, value);
    free(temp);
    if (!new_var)
        return;
    
    if (*env)
    {
        while ((*env)[i])
        {
            if (ft_strncmp((*env)[i], var, var_len) == 0 && (*env)[i][var_len] == '=')
            {
                free((*env)[i]);
                (*env)[i] = new_var;
                return;
            }
            i++;
        }
    }
    add_env_var(env, new_var);
}

void init_shell_shlvl(t_shell *shell, char **envp)
{
    char *shlvl_str;
    int current_shlvl;
    char *new_shlvl_str;

    if (!envp || !*envp)
    {
        shell->shlvl = 1;
        new_shlvl_str = ft_strdup("1");
    }
    else
    {
        shlvl_str = get_env_value_from_envp(envp, "SHLVL");
        if (shlvl_str)
        {
            current_shlvl = ft_atoi(shlvl_str);
            current_shlvl++;
        }
        else
            current_shlvl = 1;
        
        shell->shlvl = current_shlvl;
        new_shlvl_str = ft_itoa(current_shlvl);
    }
    
    if (new_shlvl_str)
    {
        set_env_var(&shell->env, "SHLVL", new_shlvl_str);
        free(new_shlvl_str);
    }
}

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
