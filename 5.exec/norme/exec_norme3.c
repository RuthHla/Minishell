/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:32:09 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:05:07 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	exit_status_from_errno(int e)
{
	if (e == ENOENT)
		return (127);
	if (e == EACCES)
		return (126);
	if (e == EISDIR)
		return (126);
	if (e == ENOTDIR)
		return (126);
	if (e == ENOEXEC)
		return (126);
	return (126);
}

int	is_directory(const char *p)
{
	struct stat	st;

	if (!p)
		return (0);
	if (stat(p, &st) != 0)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

char	*join_path(const char *dir, const char *cmd)
{
	size_t	ld;
	size_t	lc;
	char	*p;

	ld = ft_strlen(dir);
	lc = ft_strlen(cmd);
	p = (char *)malloc(ld + 1 + lc + 1);
	if (!p)
		return (NULL);
	memcpy(p, dir, ld);
	p[ld] = '/';
	memcpy(p + ld + 1, cmd, lc);
	p[ld + 1 + lc] = '\0';
	return (p);
}

void	putstr_err(const char *s)
{
	if (s)
		write(STDERR_FILENO, s, ft_strlen(s));
}

void	print2_err(const char *a, const char *b)
{
	putstr_err(a);
	putstr_err(b);
}
