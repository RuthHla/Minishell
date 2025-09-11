/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:06 by alandel           #+#    #+#             */
/*   Updated: 2025/09/11 15:22:04 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_element	*lst_last_node(t_element *head)
{
	t_element	*cur;

	cur = head;
	if (!cur)
		return (NULL);
	while (cur->next)
		cur = cur->next;
	return (cur);
}

int	is_redir(t_type type)
{
	if (type == HEREDOC || type == APPEND || type == REDIR_IN
		|| type == REDIR_OUT)
		return (1);
	return (0);
}

int	is_operator(t_type type)
{
	if (type == PIPE || type == REDIR_IN || type == HEREDOC || type == REDIR_OUT
		|| type == APPEND || type == DOLLAR || type == SPECIAL_VARIABLE)
		return (1);
	return (0);
}

int	is_command(t_type_cmd type)
{
	if (type == T_ECHO || type == T_CD || type == T_PWD || type == T_EXPORT
		|| type == T_UNSET || type == T_ENV || type == T_EXIT)
		return (1);
	return (0);
}

t_type_cmd	identify_builtin(const char *str)
{
	if (!str)
		return (NOT_BUILTIN);
	if (ft_strncmp(str, "echo", 4) == 0)
		return (T_ECHO);
	if (ft_strncmp(str, "cd", 2) == 0)
		return (T_CD);
	if (ft_strncmp(str, "pwd", 3) == 0)
		return (T_PWD);
	if (ft_strncmp(str, "export", 6) == 0)
		return (T_EXPORT);
	if (ft_strncmp(str, "unset", 5) == 0)
		return (T_UNSET);
	if (ft_strncmp(str, "env", 3) == 0)
		return (T_ENV);
	if (ft_strncmp(str, "exit", 4) == 0)
		return (T_EXIT);
	return (NOT_BUILTIN);
}

t_arg	*create_arg(t_type type, const char *str)
{
	t_arg	*arg;

	if (!str)
		return (NULL);
	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->str = ft_strdup(str);
	if (!arg->str)
	{
		free(arg);
		return (NULL);
	}
	arg->type = type;
	return (arg);
}

t_redir	*create_redir(t_type type, const char *target, t_type target_type)
{
	t_redir	*redir;

	if (!target)
		return (NULL);
	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->target = ft_strdup(target);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->type = type;
	redir->target_type = target_type;
	redir->next = NULL;
	return (redir);
}

t_element	*create_element_arg(t_type type, const char *str)
{
	t_element	*element;
	t_arg		*arg;

	arg = create_arg(type, str);
	if (!arg)
		return (NULL);
	element = malloc(sizeof(t_element));
	if (!element)
	{
		if (arg->str)
			free(arg->str);
		free(arg);
		return (NULL);
	}
	element->kind = ARG;
	element->u_.arg = arg;
	element->next = NULL;
	return (element);
}

t_element	*create_element_redir(t_type type, const char *target,
		t_type target_type)
{
	t_element	*element;
	t_redir		*redir;

	if (type == HEREDOC && target_type == DOLLAR)
		redir = create_redir(type, target, LITERAL);
	else
		redir = create_redir(type, target, target_type);
	if (!redir)
		return (NULL);
	element = malloc(sizeof(t_element));
	if (!element)
	{
		if (redir->target)
			free(redir->target);
		free(redir);
		return (NULL);
	}
	element->kind = REDIR;
	element->u_.redirs = redir;
	element->next = NULL;
	return (element);
}

void	add_element_to_command(t_command *cmd, t_element *element)
{
	t_element	*last;

	if (!cmd->element)
		cmd->element = element;
	else
	{
		last = lst_last_node(cmd->element);
		last->next = element;
	}
}
