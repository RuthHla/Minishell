/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:44:37 by adenny            #+#    #+#             */
/*   Updated: 2025/09/26 14:51:31 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipeinfo(t_command *cmd, int prev_rd, t_pipeinfo *pi)
{
	pi->need_pipe = !is_last_cmd(cmd);
	pi->out_wr = -1;
	if (!pi->need_pipe)
		return (1);
	if (pipe(pi->pfd) < 0)
	{
		perror("pipe");
		if (prev_rd >= 0)
			close(prev_rd);
		return (0);
	}
	pi->out_wr = pi->pfd[1];
	return (1);
}

static int	spawn_and_record(t_command *cmd, int *prev_rd, t_pipeinfo *pi,
		t_launch_ctx *ctx)
{
	pid_t	pid;

	pid = spawn_one(cmd, *prev_rd, pi->out_wr, ctx->sh);
	if (pid < 0)
	{
		cleanup_on_fail(prev_rd, pi);
		return (0);
	}
	ctx->pids[*ctx->out_n] = pid;
	*ctx->out_n = *ctx->out_n + 1;
	advance_pipe_state(prev_rd, pi);
	return (1);
}

static int	launch_one_cmd(t_command *cmd, int *prev_rd, t_launch_ctx *ctx)
{
	t_pipeinfo	pi;

	if (!setup_pipeinfo(cmd, *prev_rd, &pi))
		return (1);
	if (!spawn_and_record(cmd, prev_rd, &pi, ctx))
		return (1);
	return (0);
}

static int	launch_all(t_command *cmds, t_shell *sh, pid_t *pids, int *out_n)
{
	int				prev_rd;
	t_command		*cmd;
	t_launch_ctx	ctx;

	prev_rd = -1;
	*out_n = 0;
	ctx.pids = pids;
	ctx.out_n = out_n;
	ctx.sh = sh;
	cmd = cmds;
	while (cmd)
	{
		if (launch_one_cmd(cmd, &prev_rd, &ctx))
		{
			if (prev_rd >= 0)
				close(prev_rd);
			return (1);
		}
		cmd = cmd->next;
	}
	if (prev_rd >= 0)
		close(prev_rd);
	return (0);
}

int	run_pipeline(t_all *all, t_command *cmds, t_shell *sh)
{
	pid_t		pids[256];
	int			n;
	int			last;
	t_command	*cmd_list;

	cmd_list = cmds;
	if (cmd_list && !cmd_list->next && is_builtin_cmd(cmd_list->cmd))
	{
		if (cmd_list->cmd == T_EXIT && cmd_list->nb_args > 2)
			return (exit_too_many_args());
		return (run_single_builtin(cmd_list, sh, all));
	}
	if (launch_all(cmds, sh, pids, &n) != 0)
		return (1);
	last = wait_all(pids, n);
	if (WIFSIGNALED(last))
		sh->last_exit = 128 + WTERMSIG(last);
	else if (WIFEXITED(last))
		sh->last_exit = WEXITSTATUS(last);
	else
		sh->last_exit = 1;
	if (check_signals())
		sh->last_exit = 130;
	setup_signals();
	return (sh->last_exit);
}
