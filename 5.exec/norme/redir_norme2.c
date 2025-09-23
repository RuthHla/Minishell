/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_norme2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:33:22 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 13:10:11 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	replace_fd(int *dst, int newfd)
{
	if (*dst >= 0)
		close(*dst);
	*dst = newfd;
	if (newfd < 0)
		return (0);
	return (1);
}

static int	set_in_redir(t_ios *ios, t_redir *r)
{
	int	fd;

	fd = -1;
	if (r->type == REDIR_IN)
		fd = open_in(r->target);
	else if (r->type == HEREDOC)
		fd = create_heredoc_fd(r->target);
	return (replace_fd(&ios->in_fd, fd));
}

static int	set_out_redir(t_ios *ios, t_redir *r)
{
	int	fd;

	fd = -1;
	if (r->type == REDIR_OUT)
		fd = open_out_trunc(r->target);
	else if (r->type == APPEND)
		fd = open_out_append(r->target);
	return (replace_fd(&ios->out_fd, fd));
}

int	apply_redir(t_ios *ios, t_redir *r)
{
	if (r->type == REDIR_IN || r->type == HEREDOC)
		return (set_in_redir(ios, r));
	if (r->type == REDIR_OUT || r->type == APPEND)
		return (set_out_redir(ios, r));
	return (1);
}

int	wait_all(pid_t *pids, int n)
{
	int	i;
	int	st;
	int	last;

	i = 0;
	last = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &st, 0) > 0)
			last = st;
		i++;
	}
	return (last);
}
