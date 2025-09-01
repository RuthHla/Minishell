#include "../minishell.h"

/* Pour affichage debug */
static const char	*get_type_name(t_type type)
{
	if (type == LITERAL) return "LITERAL";
	if (type == PIPE) return "PIPE";
	if (type == AMPERSAND) return "AMPERSAND";
	if (type == REDIR_IN) return "<";
	if (type == HEREDOC) return "<<";
	if (type == REDIR_OUT) return ">";
	if (type == APPEND) return ">>";
	if (type == DOLLAR) return "$";
	if (type == SPECIAL_VARIABLE) return "SPECIAL_VARIABLE";
	return "UNKNOWN";
}

const char *get_cmd_name(t_type_cmd cmd)
{
	if (cmd == T_ECHO) return "echo";
	if (cmd == T_CD) return "cd";
	if (cmd == T_PWD) return "pwd";
	if (cmd == T_EXPORT) return "export";
	if (cmd == T_UNSET) return "unset";
	if (cmd == T_ENV) return "env";
	if (cmd == T_EXIT) return "exit";
	return "not_builtin";
}

void print_tokens(t_token *token_list)
{
	for (; token_list; token_list = token_list->next)
		printf("Token: '%s' [Type: %s]\n",
			token_list->str,
			get_type_name(token_list->type));
}

// refaire avec nouvelle structure pour que ce oit dans lordre 
void print_command_structure(t_command *cmd_list)
{
	while (cmd_list)
	{
		printf("(cmd)[%s]\n", get_cmd_name(cmd_list->cmd));
		for (size_t i = 0; i < cmd_list->nb_args; i++)
			printf("    (args)[%s] -> %s\n",
				get_type_name(cmd_list->args[i].type),
				cmd_list->args[i].str);
		for (t_redir *r = cmd_list->redirs; r; r = r->next)
		{
			printf("    (redir)[%s]\n", get_type_name(r->type));
			if (r->target)
				printf("        (target)[%s] -> %s\n", get_type_name(r->target_type), r->target);
			else
				printf("        (target)[UNKNOWN]\n");
		}
		if (cmd_list->has_pipe_out)
			printf("    [PIPE] -> |\n");
		printf("\n");
		cmd_list = cmd_list->next;
	}
}

// void	print_command_structure(t_command *cmd_list)
// {
// 	int			cidx = 0;

// 	printf("=== COMMAND STRUCTURE ===\n");
// 	for (; cmd_list; cmd_list = cmd_list->next, ++cidx)
// 	{
// 		/* 1) commande (type builtin ou not_builtin) */
// 		printf("cmd[%d]: %s", cidx, get_cmd_name(cmd_list->cmd));
// 		/* petit plus: si NOT_BUILTIN et au moins un arg, on montre le binaire */
// 		if (cmd_list->cmd == NOT_BUILTIN && cmd_list->nb_args > 0
// 			&& cmd_list->args && cmd_list->args[0].str)
// 			printf("  program:\"%s\"", cmd_list->args[0].str);
// 		printf("\n");

// 		/* 2) arguments (ordre exact du tableau args[0..nb_args-1]) */
// 		if (!cmd_list->args || cmd_list->nb_args == 0)
// 			printf("  args: [none]\n");
// 		else
// 		{
// 			for (size_t i = 0; i < cmd_list->nb_args; ++i)
// 			{
// 				const char *s = cmd_list->args[i].str ? cmd_list->args[i].str : "";
// 				printf("  arg[%zu]: \"%s\" (type:%s)\n",
// 					i, s, get_type_name(cmd_list->args[i].type));
// 			}
// 		}

// 		/* 3) redirections (ordre exact de la liste chaînée redirs->next) */
// 		if (!cmd_list->redirs)
// 			printf("  redirs: [NULL -> pb]\n");
// 		else
// 		{
// 			int ridx = 0;
// 			for (t_redir *r = cmd_list->redirs; r; r = r->next, ++ridx)
// 			{
// 				printf("  redir[%d]: %s", ridx, get_type_name(r->type));
// 				printf("\n");
// 				if (r->target)
// 				{
// 					/* on affiche le type original de la cible si dispo */
// 					printf("    target: \"%s\" (type:%s)\n",
// 						r->target,
// 						get_type_name(r->target_type));
// 				}
// 				else
// 					printf("    target: [NULL]\n");
// 			}
// 		}

// 		/* 4) pipe sortant si présent */
// 		if (cmd_list->has_pipe_out)
// 			printf("  pipe: |\n");

// 		printf("\n");
// 	}
// 	printf("=========================\n");
// }

int main(void)
{
	char		*line;
	t_character	*char_list;
	t_token     *token_list;
	t_command   *cmd_list;

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

		char_list = build_char_list(line); // <== vérifie le nom exact !
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
		if (cmd_list)
		{
			print_command_structure(cmd_list);
			cleanup(cmd_list);
		}
		free_token_list(token_list);
		free_character_list(char_list);
		free(line);
	}
	return 0;
}
