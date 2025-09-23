/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:13 by alandel           #+#    #+#             */
/*   Updated: 2025/09/16 11:31:50 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
