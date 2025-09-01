/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:17 by alandel           #+#    #+#             */
/*   Updated: 2025/09/01 18:32:26 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_redir(t_redir *redir)
{
	t_redir	*current;
	t_redir	*next;

	current = redir;
	while (current)
	{
		next = current->next;
		if (current->target)
			free(current->target);
		free(current);
		current = next;
	}
}

static void	free_element_list(t_element *element)
{
	t_element	*current;
	t_element	*next;

	current = element;
	while (current)
	{
		next = current->next;
		if (current->kind == ARG)
		{
			if (current->u_.arg)
			{
				if (current->u_.arg->str)
					free(current->u_.arg->str);
				free(current->u_.arg);
			}
		}
		else if (current->kind == REDIR)
		{
			if (current->u_.redirs)
				free_redir(current->u_.redirs);
		}
		free(current);
		current = next;
	}
}

void	cleanup(t_command *cmd)
{
	t_command	*current;
	t_command	*next;

	current = cmd;
	while (current)
	{
		next = current->next;
		if (current->element)
			free_element_list(current->element);
		free(current);
		current = next;
	}
}

void	cleanall_exit(t_command *cmd, t_token *token_list, char **line)
{
	cleanup(cmd);
	free_token_list(token_list);
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
}
