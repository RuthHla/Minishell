/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:26:50 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 09:39:30 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_in(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	open_out_trunc(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	open_out_append(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	collect_redirs_fds(t_element *elem, t_ios *ios)
{
	t_redir	*r;

	ios->in_fd = -1;
	ios->out_fd = -1;
	while (elem)
	{
		if (elem->kind == REDIR)
		{
			r = elem->u_.redirs;
			while (r)
			{
				if (!apply_redir(ios, r))
					return (0);
				r = r->next;
			}
		}
		elem = elem->next;
	}
	return (1);
}
