/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:08:22 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 15:13:48 by alandel          ###   ########.fr       */
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

void	cleanall(t_character *char_list, t_token *token_list,
		t_command *cmd)
{
	free_character_list(char_list);
	free_token_list(token_list);
	cleanup(cmd);
}
