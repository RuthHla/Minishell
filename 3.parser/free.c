/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:17 by alandel           #+#    #+#             */
/*   Updated: 2025/06/26 11:36:18 by alandel          ###   ########.fr       */
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

void	free_args(t_arg *args, size_t nb_args)
{
	size_t	i;

	i = 0;
	if (!args)
		return ;
	while (i < nb_args)
	{
		if (args[i].str)	
			free(args[i++].str);
		i++;
	}
	free(args);
}

void	cleanup(t_command *cmd)
{
	t_command	*current;
	t_command	*next;

	current = cmd;
	while (current)
	{
		next = current->next;
		free_args(current->args, current->nb_args);
		free_redir(current->redirs);
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
