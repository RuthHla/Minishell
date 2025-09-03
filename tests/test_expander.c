/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:08:27 by alandel           #+#    #+#             */
/*   Updated: 2025/09/03 16:20:59 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_type_name(t_type type)
{
	if (type == LITERAL)
		return ("LITERAL");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_IN)
		return ("<");
	if (type == HEREDOC)
		return ("<<");
	if (type == REDIR_OUT)
		return (">");
	if (type == APPEND)
		return (">>");
	if (type == DOLLAR)
		return ("$");
	if (type == SPECIAL_VARIABLE)
		return ("SPECIAL_VARIABLE");
	return ("UNKNOWN");
}

const char	*get_cmd_name(t_type_cmd cmd)
{
	if (cmd == T_ECHO)
		return ("echo");
	if (cmd == T_CD)
		return ("cd");
	if (cmd == T_PWD)
		return ("pwd");
	if (cmd == T_EXPORT)
		return ("export");
	if (cmd == T_UNSET)
		return ("unset");
	if (cmd == T_ENV)
		return ("env");
	if (cmd == T_EXIT)
		return ("exit");
	return ("not_builtin");
}

void	print_tokens(t_token *token_list)
{
	for (; token_list; token_list = token_list->next)
		printf("Token: '%s' [Type: %s]\n", token_list->str,
			get_type_name(token_list->type));
}

void	print_command_structure(t_command *cmd_list)
{
	t_command	*cmd;
	t_element	*elem;
	t_redir		*redir;
	const char	*name;

	cmd = cmd_list;
	while (cmd)
	{
		name = get_cmd_name(cmd->cmd);
		if (name)
			printf("(cmd)[%s]\n", name);
		else
			printf("(cmd)[UNKNOWN]\n");
		elem = cmd->element;
		while (elem)
		{
			if (elem->kind == ARG)
			{
				if (elem->u_.arg)
				{
					name = get_type_name(elem->u_.arg->type);
					if (name)
						printf("    (arg)[%s] -> ", name);
					else
						printf("    (arg)[UNKNOWN] -> ");
					if (elem->u_.arg->str)
						printf("%s\n", elem->u_.arg->str);
					else
						printf("(null)\n");
				}
				else
					printf("    (arg)[NULL POINTER]\n");
			}
			else if (elem->kind == REDIR)
			{
				redir = elem->u_.redirs;
				if (!redir)
					printf("    (redir)[NULL POINTER]\n");
				else
				{
					while (redir)
					{
						// Afficher le type de redirection
						name = get_type_name(redir->type);
						if (name)
							printf("    (redir)[%s]\n", name);
						else
							printf("    (redir)[UNKNOWN]\n");
						// Afficher la cible si elle existe
						if (redir->target)
						{
							name = get_type_name(redir->target_type);
							if (name)
								printf("        (target)[%s] -> %s\n", name,
									redir->target);
							else
								printf("        (target)[UNKNOWN] -> %s\n",
									redir->target);
						}
						else
							printf("        (target)[NULL]\n");
						redir = redir->next;
					}
				}
			}
			else
				printf("    (element)[UNKNOWN_KIND=%d]\n", elem->kind);
			elem = elem->next;
		}
		if (cmd->has_pipe_out && cmd->next)
			printf("    [PIPE] -> |\n");
		printf("\n");
		cmd = cmd->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell		shell;
	char		*line;
	t_character	*char_list;
	t_token		*token_list;
	t_command	*cmd_list;

	(void)av;
	if (ac != 1)
		return (1);
	// check no_child_process ?
	if (!envp || !*envp)
		shell.env = init_local_env();
	else
		shell.env = copy_env(envp);
	if (!shell.env)
		return (0);

	shell.last_exit = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		char_list = build_char_list(line);
		if (!char_list)
		{
			free(line);
			continue ;
		}
		token_list = build_token_list(char_list);
		if (!token_list)
		{
			free_character_list(char_list);
			free(line);
			continue ;
		}
		cmd_list = init_struct_globale(token_list, &line);
		if (!cmd_list)
			cleanup(cmd_list);

		expander(&cmd_list, &shell);
		print_command_structure(cmd_list);
	}
	cleanup(cmd_list);
	free_token_list(token_list);
	free_character_list(char_list);
	free(line);
	return (0);
}
