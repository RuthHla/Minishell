/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:20:54 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 14:37:32 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_special_var(void)
{
	char	*var;

	var = malloc(sizeof(char) * 2);
	if (!var)
		return (NULL);
	var[0] = '?';
	var[1] = '\0';
	return (var);
}

char	*extract_normal_var(char *str, int j, int *new_pos)
{
	char	*var;
	int		var_len;
	int		k;

	var_len = strlen_variable(str, j);
	var = malloc(sizeof(char) * (var_len + 1));
	if (!var)
		return (NULL);
	k = 0;
	while (valid_variable_char(str[j]))
		var[k++] = str[j++];
	var[k] = '\0';
	*new_pos = j - 1;
	return (var);
}

void	*cleanup_variables(char **variable, int index)
{
	while (--index >= 0)
		free(variable[index]);
	free(variable);
	return (NULL);
}

int	process_dollar(char *str, int i, char **variable, int *index)
{
	int		j;
	int		new_pos;
	char	*var;

	j = i + 1;
	if (!valid_variable_char(str[j]) && str[j] != '?')
		return (i);
	if (str[j] == '?')
	{
		var = create_special_var();
		if (!var)
			return (-1);
		variable[(*index)++] = var;
		return (j);
	}
	var = extract_normal_var(str, j, &new_pos);
	if (!var)
		return (-1);
	variable[(*index)++] = var;
	return (new_pos);
}

void	*cleanup_vars_pos(t_var_pos *vars, int index)
{
	while (--index >= 0)
		free(vars[index].name);
	free(vars);
	return (NULL);
}
