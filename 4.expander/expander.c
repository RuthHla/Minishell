/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:09:02 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 11:48:13 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

typedef struct s_var_pos
{
	char			*name;
	int				start;
	int				end;
}					t_var_pos;

static t_var_pos	*find_variables_with_positions(char *str, int *count)
{
	t_var_pos	*vars;
	int			i;
	int			j;
	int			index;
	int			var_len;
	int			k;

	i = 0;
	index = 0;
	*count = how_many_variable(str);
	if (*count == 0)
		return (NULL);
	vars = malloc(sizeof(t_var_pos) * (*count));
	if (!vars)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && (valid_variable_char(str[i
					+ 1]) || str[i + 1] == '?'))
		{
			vars[index].start = i;
			j = i + 1;
			if (str[j] == '?')
			{
				vars[index].name = malloc(sizeof(char) * 2);
				if (!vars[index].name)
				{
					while (--index >= 0)
						free(vars[index].name);
					free(vars);
					return (NULL);
				}
				vars[index].name[0] = '?';
				vars[index].name[1] = '\0';
				vars[index].end = j + 1;
				index++;
				i = j;
			}
			else
			{
				var_len = strlen_variable(str, j);
				vars[index].name = malloc(sizeof(char) * (var_len + 1));
				if (!vars[index].name)
				{
					while (--index >= 0)
						free(vars[index].name);
					free(vars);
					return (NULL);
				}
				k = 0;
				while (valid_variable_char(str[j]) || (k > 0
						&& valid_variable_char_number(str[j])))
					vars[index].name[k++] = str[j++];
				vars[index].name[k] = '\0';
				vars[index].end = j;
				index++;
				i = j - 1;
			}
		}
		i++;
	}
	return (vars);
}

static char	*get_variable_value(t_shell *shell, char *var_name)
{
	char	buffer[12];
	char	*value;

	if (ft_strncmp(var_name, "?", 1) == 0)
	{
		itoa(shell->last_exit, buffer, 10);
		return (ft_strdup(buffer));
	}
	value = find_variable_in_env(shell->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static int	calculate_expanded_length(char *original, t_var_pos *vars,
		char **values, int var_count)
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

static char	*build_expanded_string(char *original, t_var_pos *vars,
		char **values, int var_count)
{
	char	*expanded;
	int		new_len;
	int		orig_idx;
	int		exp_idx;
	int		var_idx;
	int		val_len;
	int		i;

	orig_idx = 0;
	exp_idx = 0;
	var_idx = 0;
	new_len = calculate_expanded_length(original, vars, values, var_count);
	expanded = malloc(sizeof(char) * (new_len + 1));
	if (!expanded)
		return (NULL);
	while (original[orig_idx])
	{
		if (var_idx < var_count && orig_idx == vars[var_idx].start)
		{
			val_len = ft_strlen(values[var_idx]);
			i = 0;
			while (i < val_len)
				expanded[exp_idx++] = values[var_idx][i++];
			orig_idx = vars[var_idx].end;
			var_idx++;
		}
		else
			expanded[exp_idx++] = original[orig_idx++];
	}
	expanded[exp_idx] = '\0';
	return (expanded);
}

static char	*expand_string(t_shell *shell, char *str)
{
	t_var_pos	*vars;
	char		**values;
	char		*expanded;
	int			var_count;
	int			i;
	int			j;

	vars = find_variables_with_positions(str, &var_count);
	if (!vars || var_count == 0)
		return (ft_strdup(str));
	values = malloc(sizeof(char *) * var_count);
	if (!values)
	{
		i = 0;
		while (i < var_count)
			free(vars[i++].name);
		free(vars);
		return (NULL);
	}
	i = 0;
	while (i < var_count)
	{
		values[i] = get_variable_value(shell, vars[i].name);
		if (!values[i])
		{
			j = 0;
			while (j < i)
				free(values[j++]);
			j = 0;
			while (j < var_count)
				free(vars[j++].name);
			free(vars);
			free(values);
			return (NULL);
		}
		i++;
	}
	expanded = build_expanded_string(str, vars, values, var_count);
	i = 0;
	while (i < var_count)
	{
		free(vars[i].name);
		free(values[i]);
		i++;
	}
	free(vars);
	free(values);
	return (expanded);
}

void	expander(t_command **cmd_list, t_shell *shell)
{
	t_command	*cmd;
	t_element	*element;
	char		*old_str;
	char		*new_str;

	cmd = *cmd_list;
	while (cmd)
	{
		element = cmd->element;
		while (element)
		{
			if (element->kind == ARG)
			{
				if (ft_strchr(element->u_.arg->str, '$')
					&& (element->u_.arg->type == DOLLAR
						|| element->u_.arg->type == SPECIAL_VARIABLE))
				{
					old_str = element->u_.arg->str;
					new_str = expand_string(shell, old_str);
					if (new_str)
					{
						element->u_.arg->str = new_str;
						free(old_str);
					}
				}
			}
			else if (element->kind == REDIR)
			{
				if (ft_strchr(element->u_.redirs->target, '$')
					&& (element->u_.redirs->target_type == DOLLAR
						|| element->u_.redirs->target_type == SPECIAL_VARIABLE))
				{
					old_str = element->u_.redirs->target;
					new_str = expand_string(shell, old_str);
					if (new_str)
					{
						element->u_.redirs->target = new_str;
						free(old_str);
					}
				}
			}
			element = element->next;
		}
		cmd = cmd->next;
	}
}
