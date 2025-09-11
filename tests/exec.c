/* ************************************************************************** */
/*                                                                            */
/*                                test_expander.c                              */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static const char *type_name(t_type t)
// {
// 	if (t == LITERAL) return "LITERAL";
// 	if (t == PIPE) return "PIPE";
// 	if (t == REDIR_IN) return "<";
// 	if (t == HEREDOC) return "<<";
// 	if (t == REDIR_OUT) return ">";
// 	if (t == APPEND) return ">>";
// 	if (t == DOLLAR) return "$";
// 	if (t == SPECIAL_VARIABLE) return "SPECIAL_VARIABLE";
// 	return "UNKNOWN";
// }

// static const char *cmd_name(t_type_cmd c)
// {
// 	if (c == T_ECHO) return "echo";
// 	if (c == T_CD) return "cd";
// 	if (c == T_PWD) return "pwd";
// 	if (c == T_EXPORT) return "export";
// 	if (c == T_UNSET) return "unset";
// 	if (c == T_ENV) return "env";
// 	if (c == T_EXIT) return "exit";
// 	if (c == CMD_NONE) return "(none)";
// 	return "not_builtin";
// }

// static void print_command_structure(t_command *cmd_list)
// {
// 	for (t_command *cmd = cmd_list; cmd; cmd = cmd->next)
// 	{
// 		printf("(cmd)[%s]\n", cmd_name(cmd->cmd));
// 		for (t_element *e = cmd->element; e; e = e->next)
// 		{
// 			if (e->kind == ARG && e->u_.arg)
// 			{
// 				printf("    (arg)[%s] -> %s\n",
// 					type_name(e->u_.arg->type),
// 					e->u_.arg->str ? e->u_.arg->str : "(null)");
// 			}
// 			else if (e->kind == REDIR && e->u_.redirs)
// 			{
// 				for (t_redir *r = e->u_.redirs; r; r = r->next)
// 				{
// 					printf("    (redir)[%s]\n", type_name(r->type));
// 					printf("        (target)[%s] -> %s\n",
// 						type_name(r->target_type),
// 						r->target ? r->target : "(null)");
// 				}
// 			}
// 		}
// 		if (cmd->has_pipe_out && cmd->next)
// 			printf("    [PIPE] -> |\n");
// 		puts("");
// 	}
// }

static int one_shot(char *line, t_shell *sh, int verbose)
{
	t_character *chars = NULL;
	t_token     *toks  = NULL;
	t_command   *cmds  = NULL;

	chars = build_char_list(line);
	if (!chars)
		return 1;

	toks = build_token_list(chars);
	if (!toks)
	{
		free_character_list(chars);
		return 1;
	}

	cmds = init_struct_globale(toks, &line);
	if (!cmds)
	{
		free_token_list(toks);
		free_character_list(chars);
		return 2;
	}

	expander(&cmds, sh);

	if (verbose)
	{
		// puts("----- EXPANDED COMMAND STRUCTURE -----");
		// print_command_structure(cmds);
		// puts("--------------------------------------");
	}

	int code = run_pipeline(cmds, sh);

	cleanup(cmds);
	free_token_list(toks);
	free_character_list(chars);
	return code;
}

int main(int ac, char **av, char **envp)
{
	t_shell shell;

	// ENV
	if (!envp || !*envp) 
        shell.env = init_local_env();
	else                 
        shell.env = copy_env(envp);
	if (!shell.env) 
        return 1;
    init_shell_shlvl(&shell, envp);
	shell.last_exit = 0;

	// Mode non-interactif : ./test_expander "commande ici"
	if (ac > 1)
	{
		int code = one_shot(av[1], &shell, 1 /*verbose*/);
		shell.last_exit = code;
		return code;
	}

	setup_signals();
	// Mode interactif
	while (1)
	{
		check_signals();
		char *line = readline("minishell$ ");
		if (!line) 
		{ 
			printf("exit\n"); 
			exit(0); 
		}
		if (*line == '\0') { free(line); continue; }

		add_history(line);
		int code = one_shot(line, &shell, 1 /*verbose*/);
		free(line);
		shell.last_exit = code; // pour tester $?
	}
	return shell.last_exit;
}
