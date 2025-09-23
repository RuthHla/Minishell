/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:34:22 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 14:53:17 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_var_data(t_var_pos *vars, char **values, int count)
{
	int	i;

	if (vars)
	{
		i = 0;
		while (i < count)
		{
			if (vars[i].name)
				free(vars[i].name);
			i++;
		}
		free(vars);
	}
	if (values)
	{
		i = 0;
		while (i < count)
		{
			if (values[i])
				free(values[i]);
			i++;
		}
		free(values);
	}
}

int	extract_special_var_pos(t_var_pos *var, int start, int j)
{
	var->start = start;
	var->name = malloc(sizeof(char) * 2);
	if (!var->name)
		return (-1);
	var->name[0] = '?';
	var->name[1] = '\0';
	var->end = j + 1;
	return (j);
}

int	extract_variable_pos(char *str, t_var_pos *var, int start, int j)
{
	int	var_len;
	int	k;

	var->start = start;
	var_len = strlen_variable(str, j);
	var->name = malloc(sizeof(char) * (var_len + 1));
	if (!var->name)
		return (-1);
	k = 0;
	while (valid_variable_char(str[j]) || (k > 0
			&& valid_variable_char_number(str[j])))
		var->name[k++] = str[j++];
	var->name[k] = '\0';
	var->end = j;
	return (j - 1);
}

int	process_dollar_pos(char *str, int i, t_var_pos *var)
{
	int	j;

	j = i + 1;
	if (!str[j] || (!valid_variable_char(str[j]) && str[j] != '?'))
		return (0);
	if (str[j] == '?')
		return (extract_special_var_pos(var, i, j));
	return (extract_variable_pos(str, var, i, j));
}

t_var_pos	*find_variables_with_positions(char *str, int *count)
{
	t_var_pos	*vars;
	int			i;
	int			index;
	int			result;

	*count = how_many_variable(str);
	if (*count == 0)
		return (NULL);
	vars = malloc(sizeof(t_var_pos) * (*count));
	if (!vars)
		return (NULL);
	i = -1;
	index = 0;
	while (str[++i])
	{
		if (str[i] == '$')
		{
			result = process_dollar_pos(str, i, &vars[index]);
			if (result == -1)
				return (cleanup_vars_pos(vars, index));
			if (result > 0)
				i = result + (++index * 0);
		}
	}
	return (vars);
}
