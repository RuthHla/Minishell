/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:01:38 by alandel           #+#    #+#             */
/*   Updated: 2025/09/26 14:47:17 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_string(t_shell *shell, char *str)
{
	t_var_pos	*vars;
	char		**values;
	char		*expanded;
	int			var_count;

	vars = find_variables_with_positions(str, &var_count);
	if (!vars || var_count == 0)
		return (ft_strdup(str));
	values = get_all_values(shell, vars, var_count);
	if (!values)
	{
		free_var_data(vars, NULL, var_count);
		return (NULL);
	}
	expanded = build_expanded_string(str, vars, values, var_count);
	free_var_data(vars, values, var_count);
	return (expanded);
}

static void	expand_arg(t_element *element, t_shell *shell)
{
	char	*old_str;
	char	*new_str;

	if (ft_strchr(element->u_.arg->str, '$') && (element->u_.arg->type == DOLLAR
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

static void	expand_redir(t_element *element, t_shell *shell)
{
	char	*old_str;
	char	*new_str;

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

void	process_command_elements(t_command *cmd, t_shell *shell)
{
	t_element	*e;
	int			first_arg;

	first_arg = 1;
	e = cmd->element;
	while (e)
	{
		if (e->kind == ARG)
		{
			expand_arg(e, shell);
			if (first_arg && e->u_.arg->str[0] == '\0'
				&& (e->u_.arg->type == DOLLAR
					|| e->u_.arg->type == SPECIAL_VARIABLE))
			{
				e = remove_empty_var_arg(cmd, e);
				continue ;
			}
			first_arg = 0;
		}
		else if (e->kind == REDIR)
			expand_redir(e, shell);
		e = e->next;
	}
}

void	expander(t_command **cmd_list, t_shell *shell)
{
	t_command	*cmd;

	cmd = *cmd_list;
	while (cmd)
	{
		process_command_elements(cmd, shell);
		cmd = cmd->next;
	}
}
