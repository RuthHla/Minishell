/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:28:25 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 14:43:48 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_variable_value(t_shell *shell, char *var_name)
{
	char	*buffer;
	char	*value;

	if (ft_strncmp(var_name, "?", 1) == 0)
	{
		buffer = ft_itoa(shell->last_exit);
		return (buffer);
	}
	value = find_variable_in_env(shell->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

int	calculate_expanded_length(char *original, t_var_pos *vars, char **values,
		int var_count)
{
	int	new_len;
	int	i;

	new_len = ft_strlen(original);
	i = 0;
	while (i < var_count)
	{
		new_len -= (vars[i].end - vars[i].start);
		new_len += ft_strlen(values[i]);
		i++;
	}
	return (new_len);
}

void	copy_value(char *expanded, int *exp_idx, char *value)
{
	int	i;
	int	len;

	len = ft_strlen(value);
	i = 0;
	while (i < len)
	{
		expanded[(*exp_idx)++] = value[i];
		i++;
	}
}

char	*build_expanded_string(char *original, t_var_pos *vars, char **values,
		int var_count)
{
	char	*expanded;
	int		orig_idx;
	int		exp_idx;
	int		var_idx;

	expanded = malloc(sizeof(char) * (calculate_expanded_length(original, vars,
					values, var_count) + 1));
	if (!expanded)
		return (NULL);
	orig_idx = 0;
	exp_idx = 0;
	var_idx = 0;
	while (original[orig_idx])
	{
		if (var_idx < var_count && orig_idx == vars[var_idx].start)
		{
			copy_value(expanded, &exp_idx, values[var_idx]);
			orig_idx = vars[var_idx].end;
			var_idx++;
		}
		else
			expanded[exp_idx++] = original[orig_idx++];
	}
	expanded[exp_idx] = '\0';
	return (expanded);
}

char	**get_all_values(t_shell *shell, t_var_pos *vars, int var_count)
{
	char	**values;
	int		i;

	values = malloc(sizeof(char *) * (var_count + 1));
	if (!values)
		return (NULL);
	i = 0;
	while (i < var_count)
	{
		values[i] = get_variable_value(shell, vars[i].name);
		if (!values[i])
		{
			while (--i >= 0)
				free(values[i]);
			free(values);
			return (NULL);
		}
		i++;
	}
	return (values);
}
