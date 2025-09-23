/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:08:15 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 13:15:54 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	fill_argv_dup(const t_command *cmd, char **argv)
{
	size_t		i;
	t_element	*e;

	i = 0;
	e = cmd->element;
	while (e)
	{
		if (e->kind == ARG && e->u_.arg && e->u_.arg->str && *e->u_.arg->str)
		{
			argv[i] = ft_strdup(e->u_.arg->str);
			if (!argv[i])
			{
				while (i > 0)
				{
					i--;
					free(argv[i]);
				}
				return (0);
			}
			i++;
		}
		e = e->next;
	}
	argv[i] = NULL;
	return (1);
}

static char	**build_argv(const t_command *cmd)
{
	size_t	argc;
	char	**argv;

	argc = count_args_nonempty(cmd);
	if (argc == 0)
		return (NULL);
	argv = (char **)ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	if (!fill_argv_dup(cmd, argv))
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}

static void	handle_exec_error(char **argv)
{
	int	e;
	int	code;

	e = errno;
	print_exec_error(NULL, argv[0], e);
	code = exit_status_from_errno(e);
	free_argv_dup(argv);
	_exit(code);
}

static void	child_do_exec(t_command *cmd, t_shell *sh, t_all *all)
{
	char	**argv;

	if (is_builtin_cmd(cmd->cmd))
		_exit(exec_builtin(cmd, sh, all));
	argv = build_argv(cmd);
	if (!argv || !argv[0])
		_exit(0);
	if (exec_with_path(argv, sh) < 0)
		handle_exec_error(argv);
}

pid_t	spawn_one(t_command *cmd, int prev_rd, int out_wr, t_shell *sh, t_all *all)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ((pid_t)(-1));
	}
	if (pid == 0)
	{
		if (!child_prepare_fds(cmd, prev_rd, out_wr))
			_exit(1);
		child_do_exec(cmd, sh, all);
		_exit(127);
	}
	return (pid);
}
