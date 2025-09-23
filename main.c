/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:14:34 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 17:13:44 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	free_and_return(t_character *chars, t_token *toks, t_command *cmds,
		int code)
{
	if (cmds)
		cleanup(cmds);
	if (toks)
		free_token_list(toks);
	if (chars)
		free_character_list(chars);
	return (code);
}

static int	one_shot(char *line, t_shell *sh)
{
	t_character	*chars;
	t_token		*toks;
	t_command	*cmds;
	t_all		*all_struct;
	int			code;

	all_struct = get_all();

	chars = build_char_list(line);
	if (!chars)
		return (1);
	all_struct->char_list = chars;

	toks = build_token_list(chars);
	if (!toks)
		return (free_and_return(chars, NULL, NULL, 1));
	all_struct->token_list = toks;

	cmds = init_struct_globale(toks);
	if (!cmds)
		return (free_and_return(chars, toks, NULL, 1));
	all_struct->command_list = cmds;

	expander(&cmds, sh);
	code = run_pipeline(cmds, sh, all_struct);
	return (free_and_return(chars, toks, cmds, code));
}


static int	init_env(char **env, t_shell *shell)
{
	if (!env || !*env)
		shell->env = init_local_env();
	else
		shell->env = copy_env(env);
	if (!shell->env)
		return (0);
	init_shell_shlvl(shell, env);
	shell->last_exit = 0;
	return (1);
}

static void	lauch_minishell(t_shell *shell)
{
	char	*line;
	int		code;

	setup_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (check_signals())
			shell->last_exit = 130;
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		code = one_shot(line, shell);
		free(line);
		shell->last_exit = code;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)av;
	if (ac != 1)
		return (1);
	if (!init_env(envp, &shell))
		return (1);
	lauch_minishell(&shell);
	free_env(&shell);
	return (shell.last_exit);
}
