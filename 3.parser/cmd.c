/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:20 by alandel           #+#    #+#             */
/*   Updated: 2025/06/26 13:10:54 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// Proteger fonction -> utiliser une variuable plutit que la liste elle meme + verif tj check liste non null

// static void	add_redir(t_command *cmd, t_type type, char *target)
// {
// 	t_redir	*redir;

// 	redir = malloc(sizeof(t_redir));
// 	if (!redir)
// 		return 0;
// 	redir->type = type;
// 	if (target != NULL && target[0] != '\0')
// 	{
// 		redir->target = strdup(target);
// 		if (!redir->target)
// 		{
// 			free(redir);
// 			return ;
// 		}
// 		redir->error = 0;
// 	}
// 	else
// 	{
// 		redir->target = NULL;
// 		redir->error = 1;
// 	}
// 	redir->next = cmd->redirs;
// 	cmd->redirs = redir;
// }

static int	add_cmd(t_command *cmd, char *str)
{
	if (cmd->cmd == CMD_NONE)
	{
		cmd->cmd = identify_builtin(str);
		return (1);
	}
	return (0);
}

int	add_argument(t_command *cmd, t_type type, const char *str)
{
	t_arg	*new_args;
	char	*dup;

	dup = strdup(str);
	if (!dup)
		return (0);
	new_args = realloc(cmd->args, sizeof *new_args * (cmd->nb_args + 1));
	if (!new_args)
	{
		free(dup);
		return (0);
	}
	cmd->args = new_args;
	cmd->args[cmd->nb_args].type = type;
	cmd->args[cmd->nb_args].str = dup;
	cmd->nb_args++;
	return (1);
}

int	handle_pipe(t_token **token_list, t_command **current)
{
	t_command	*new_node;

	new_node = NULL;
	if ((*token_list)->type == PIPE)
	{
		new_node = create_new_command();
		(*current)->has_pipe_out = 1;
		(*current)->next = new_node;
		new_node->previous = *current;
		(*current) = new_node;
		*token_list = (*token_list)->next;
		return (1);
	}
	return (0);
}

int	handle_redir(t_token **token_list, t_command **current) // ajouter l'expansion en + du LITERAL ?
{
	t_token *redir;
	t_token *target;
	t_redir *node;

	redir = (*token_list);
	target = redir->next;

	node = malloc(sizeof(t_redir));
	if (!node)
		return 0;
	node->type = redir->type;
	node->target = strdup(target->str);
	if(!node->target)
	{
		free(node);
		return 0;
	}
	node->error = 0;
	node->next =(*current)->redirs;
	(*current)->redirs = node;

	*token_list = target->next;

	return 1;
}

static void	handle_cmd_n_arg(t_token **token_list, t_command **current, char **line)
{
	if ((*current)->cmd == CMD_NONE)
	{
		if (!add_cmd(*current, (*token_list)->str))
			cleanall_exit(*current, (*token_list), line); // pas bon car fait token_list-next juste apres
	}
	else
	{
		if (!add_argument(*current, (*token_list)->type, (*token_list)->str))
			cleanall_exit(*current, *token_list, line); // pas bon car fait token_list-next juste apres
	}
	*token_list = (*token_list)->next;
}

int	save_all(t_command *cmd, t_token *token_list, char **line)
{
	t_command	*current;

	current = cmd;
	while (token_list)
	{
		
		handle_pipe(&token_list, &current);
		if (is_redir(token_list->type))
        {
            if (!handle_redir(&token_list, &current))
                return 0; /* malloc/strdup fail */
            continue;
        }
		else if (token_list->type == LITERAL || token_list->type == DOLLAR || token_list->type == SPECIAL_VARIABLE)
			handle_cmd_n_arg(&token_list, &current, line);
		else
		{
			if (!add_argument(current, token_list->type, token_list->str))
				cleanall_exit(current, token_list, line);
			token_list = token_list->next;
			continue ;
		}
	}
	return 1;
}
