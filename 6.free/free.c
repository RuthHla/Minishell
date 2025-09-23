/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:47:13 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 15:08:07 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_all	*get_all(void)
{
	static t_all all = {0};  // tout à NULL
	return (&all);
}


void	free_env(t_shell *shell)
{
	size_t	i;

	if (!shell || !shell->env)
		return ;
	i = 0;
	while (shell->env[i] != NULL)
		free(shell->env[i++]);
	free(shell->env);
	shell->env = NULL;
}

void	free_character_list(t_character *head)
{
	t_character	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->str);
		free(head);
		head = tmp;
	}
}
