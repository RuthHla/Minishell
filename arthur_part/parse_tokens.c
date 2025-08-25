/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:35:52 by alandel           #+#    #+#             */
/*   Updated: 2025/06/26 13:11:37 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_first_and_last_place(t_token *token_list)
{
	t_token	*last_node;

	last_node = NULL;
	if (token_list->str[0] == 39 || token_list->str[0] == 34
		|| token_list->type == PIPE)
	{
		printf("bash: syntax error near unexpected token\n");
		return (1);
	}
	last_node = lst_last_node(token_list);
	if (last_node && last_node->type == PIPE)
	{
		printf("bash: syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}

// attente nouvelle version amaury pour que str soit bien rempli
static int	check_orphan_quote(t_token *token_list)
{
	t_token	*current;
	int		len;

	current = token_list;
	len = 0;
	while (current)
	{
        if (current->str[0] == 34 || current->str[0] == 39)
        {
		    len = strlen(current->str);
            if (len < 2 || current->str[len-1] != current->str[0])
            {
                printf("bash: syntax error -> find an orphan quote\n");
                return 1;
            }
        }
		current = current->next;
	}
	return (0);
}

// si token commencant et terminant par une quote -> modifier la chaine de caractere pour supprimer [0] et [len-1]
// static void remove_extern_quotes(t_token *token_list)
// {
//     t_token *current = token_list;
//     int len = 0;
//     char first;
//     char last;
//     while(current)
//     {
//         len = strlen(current->str);
//         if (len >= 2) // >2 car [', ', \0]
//         {
//             first = current->str[0];
//             last = current->str[len-1];
//             if ((first == 39 && last == 39) || (first == 34 && last == 34))
//             {
//                 memmove(current->str, current->str + 1, len - 2);
//                 current->str[len-2] = '\0';
//             }
//         }
//         current = current->next;
//     }
// }

int	parse_token(t_command *cmd, t_token *token_list, char **line)
{
	// checker la validite du premier et dernier caractere de la commande entiere
	if (check_first_and_last_place(token_list))
    {
		cleanall_exit(cmd, token_list, line);
        return 1;
    }
	// check quote orpheline dans la ligne -> quitter
	if (check_orphan_quote(token_list))
    {
		cleanall_exit(cmd, token_list, line);
        return 1;
    }
	// 3 noeuds 1 seul token
	// quote double str[0]- si $qqchose alors -> enrg arg type literal/ puis enrg arg type variable / puis enrg arg type literal
	// check_variable(token_list);
	// si token commencant et terminant par une quote -> modifier la chaine de caractere pour supprimer [0] et [len-1]
	// remove_extern_quotes(token_list);
    return 0;
}
