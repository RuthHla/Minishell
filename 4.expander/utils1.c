/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:49:21 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 14:47:41 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_variable_char_number(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

char	*find_variable_in_env(char **env, char *variable)
{
	int	i;
	int	len;

	len = ft_strlen(variable);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], variable, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	how_many_variable(char *str)
{
	int	i;
	int	j;
	int	n_var;

	i = 0;
	n_var = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			j = i + 1;
			if (valid_variable_char(str[j]) || str[j] == '?')
			{
				n_var++;
				while (valid_variable_char(str[j]) || str[j] == '?')
					j++;
				i = j - 1;
			}
		}
		i++;
	}
	return (n_var);
}

int	strlen_variable(char *str, int j)
{
	int	count;

	count = 0;
	if (str[j] == '?')
		return (1);
	while (str[j] && valid_variable_char(str[j]))
	{
		count++;
		j++;
	}
	return (count);
}

char	**find_variable_in_str(char *str)
{
	char	**variable;
	int		i;
	int		index;
	int		result;

	if (how_many_variable(str) == 0)
		return (NULL);
	variable = malloc(sizeof(char *) * (how_many_variable(str) + 1));
	if (!variable)
		return (NULL);
	i = -1;
	index = 0;
	while (str[++i])
	{
		if (str[i] == '$')
		{
			result = process_dollar(str, i, variable, &index);
			if (result == -1)
				return (cleanup_variables(variable, index));
			i = result;
		}
	}
	variable[index] = NULL;
	return (variable);
}
