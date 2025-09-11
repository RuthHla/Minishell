/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:02:17 by alandel           #+#    #+#             */
/*   Updated: 2025/09/11 09:58:33 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static char	*create_env_string(const char *var, const char *value)
{
	char	*temp;
	char	*new_var;

	temp = ft_strjoin(var, "=");
	if (!temp)
		return (NULL);
	new_var = ft_strjoin(temp, value);
	free(temp);
	return (new_var);
}

static int	find_and_replace_var(char ***env, const char *var, char *new_var)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	if (!*env)
		return (0);
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], var, var_len) == 0
			&& (*env)[i][var_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	calculate_shlvl(char **envp)
{
	char	*shlvl_str;
	int		current_shlvl;

	if (!envp || !*envp)
		return (1);
	shlvl_str = get_env_value_from_envp(envp, "SHLVL");
	if (shlvl_str)
	{
		current_shlvl = ft_atoi(shlvl_str);
		current_shlvl++;
		return (current_shlvl);
	}
	return (1);
}

void	set_env_var(char ***env, const char *var, const char *value)
{
	char	*new_var;

	if (!env || !var || !value)
		return ;
	new_var = create_env_string(var, value);
	if (!new_var)
		return ;
	if (find_and_replace_var(env, var, new_var))
		return ;
	add_env_var(env, new_var);
}

void	init_shell_shlvl(t_shell *shell, char **envp)
{
	int		shlvl_value;
	char	*new_shlvl_str;

	shlvl_value = calculate_shlvl(envp);
	shell->shlvl = shlvl_value;
	new_shlvl_str = ft_itoa(shlvl_value);
	if (new_shlvl_str)
	{
		set_env_var(&shell->env, "SHLVL", new_shlvl_str);
		free(new_shlvl_str);
	}
}
