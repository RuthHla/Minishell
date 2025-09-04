/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:09:02 by alandel           #+#    #+#             */
/*   Updated: 2025/09/03 16:52:47 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// je parcours ma liste de commande, si jai un type dollar ou speciale_variable
// je modifie la variable en question par les data dans t_shell.
// si pas de variable trouvee alors pas d'erreur mais juste une ligne vide (donc enregistrer du vide)

char *itoa(int value, char *str, int base)
{
    char *rc = str;
    char *ptr;
    char *low;
    // Handle negative numbers
    if (value < 0 && base == 10) {
        *str++ = '-';
        value = -value;
    }
    ptr = str;
    do {
        *ptr++ = "0123456789abcdef"[value % base];
        value /= base;
    } while (value);
    *ptr-- = '\0';
    // Reverse string
    for (low = str; low < ptr; ++low, --ptr) {
        char tmp = *low;
        *low = *ptr;
        *ptr = tmp;
    }
    return rc;
}

static char	*find_variable(char **env, char *variable)
{
	int	i;
	int	len;

	len = strlen(variable);
	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], variable, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*expand_arg_variable(t_shell shell, t_arg variable, t_type type)
{
	char	*str = NULL;
	char	buffer[12];

	if(type == SPECIAL_VARIABLE)
	{
		itoa(shell.last_exit, buffer, 10); // attention : allouer str
		str = strdup(buffer);
	}
	else
		str = find_variable(shell.env, variable.str);

	if (!str)
		str = strdup(""); // verifeir
	else
		strdup(str);
	return (str);
}

static char	*expand_redir_variable(t_shell shell, t_redir variable, t_type type)
{
	char	*str = NULL;
	char	buffer[12];

	if(type == SPECIAL_VARIABLE)
	{
		itoa(shell.last_exit, buffer, 10);
		str = strdup(buffer);
	}
	else
		str = find_variable(shell.env, variable.target);

	if (!str)
		str = strdup(""); // verifeir
	else
		strdup(str);
	return (str);
}

void	expander(t_command **cmd_list, t_shell *shell)
{
	t_command	*cmd;
	t_element	*element;
	char		*old_str;

	cmd = (*cmd_list);
	while (cmd)
	{
		element = cmd->element;
		while (element)
		{
			if (element->kind == ARG)
			{
				if (element->u_.arg->type == DOLLAR
					|| element->u_.arg->type == SPECIAL_VARIABLE)
				{
					old_str = element->u_.arg->str;
					element->u_.arg->str = expand_arg_variable(*shell,
							*element->u_.arg, element->u_.arg->type);
					free(old_str);
				}
			}
			else if (element->kind == REDIR)
			{
				if (element->u_.redirs->target_type == DOLLAR
					|| element->u_.redirs->target_type == SPECIAL_VARIABLE)
				{
					old_str = element->u_.redirs->target;
					element->u_.redirs->target = expand_redir_variable(*shell,
							*element->u_.redirs, element->u_.redirs->target_type);
					free(old_str);
				}
			 }
			element = element->next;
		}
		cmd = cmd->next;
	}
}
