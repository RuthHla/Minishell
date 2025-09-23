/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:43:12 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 13:02:36 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	free_argv(char **argv)
// {
// 	size_t	i;

// 	if (!argv)
// 		return ;
// 	i = 0;
// 	while (argv[i])
// 	{
// 		free(argv[i]);
// 		i++;
// 	}
// 	free(argv);
// }

int	is_last_cmd(t_command *cmd)
{
	return (cmd->next == NULL);
}

size_t	count_args_nonempty(const t_command *cmd)
{
	size_t		n;
	t_element	*e;

	n = 0;
	e = cmd->element;
	while (e)
	{
		if (e->kind == ARG && e->u_.arg && e->u_.arg->str
			&& *e->u_.arg->str)
			n++;
		e = e->next;
	}
	return (n);
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

int	child_prepare_fds(t_command *cmd, int prev_rd, int out_wr)
{
	t_ios	ios;

	if (prev_rd >= 0 && dup2(prev_rd, STDIN_FILENO) < 0)
		return (perror("dup2"), 0);
	if (!collect_redirs_fds(cmd->element, &ios))
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
