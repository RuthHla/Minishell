/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:43:12 by adenny            #+#    #+#             */
/*   Updated: 2025/09/26 13:29:02 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_last_cmd(t_command *cmd)
{
	return (cmd->next == NULL);
}

size_t	count_args_nonempty(const t_command *cmd)
{
	const t_element	*e;
	size_t			count;

	count = 0;
	e = cmd->element;
	while (e)
	{
		if (e->kind == ARG && (count > 0 || e->u_.arg->str[0] != '\0'))
			count++;
		else if (e->kind == ARG && count == 0)
			count++;
		e = e->next;
	}
	return (count);
}

void	free_argv_dup(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int	child_prepare_fds(t_command *cmd, int prev_rd, int out_wr, t_shell *sh)
{
	t_ios	ios;

	if (prev_rd >= 0 && dup2(prev_rd, STDIN_FILENO) < 0)
		return (perror("dup2"), 0);
	if (!collect_redirs_fds(cmd->element, &ios, sh))
		return (0);
	if (out_wr >= 0 && dup2(out_wr, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close_redirs(&ios);
		return (0);
	}
	if (!apply_redirs(&ios))
	{
		close_redirs(&ios);
		return (0);
	}
	if (prev_rd >= 0)
		close(prev_rd);
	if (out_wr >= 0)
		close(out_wr);
	close_redirs(&ios);
	return (1);
}
