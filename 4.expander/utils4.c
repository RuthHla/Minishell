/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:34:22 by alandel           #+#    #+#             */
/*   Updated: 2025/09/26 14:47:29 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_element	*remove_empty_var_arg(t_command *cmd, t_element *e)
{
	t_element	*next;

	next = e->next;
	cmd->element = next;
	free(e->u_.arg->str);
	free(e->u_.arg);
	free(e);
	return (next);
}
