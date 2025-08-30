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
	if (token_list->type == PIPE)
	{
		fprintf(stderr, "bash: syntax error near unexpected token '%s'\n",token_list->str);
		return (1);
	}
	last_node = lst_last_node(token_list);
	if (last_node && last_node->type == PIPE)
	{
		fprintf(stderr, "bash: syntax error near unexpected token '%s'\n", last_node->str);
		return (1);
	}
	return (0);
}

int check_redir(t_token *token_list)
{
    t_token *tkn;
    t_token *target;

    tkn = token_list;
    while (tkn)
    {
        if (!is_redir(tkn->type)) 
		{ 
			tkn = tkn->next; 
			continue; 
		}
        target = tkn->next;
        if (!target) 
		{ 
			fprintf(stderr,
            "bash: syntax error near unexpected token `newline'\n"); return (0); 
		}
        if (!(target->type == LITERAL || target->type == DOLLAR))
        {
            if (target->str && target->str[0])
                fprintf(stderr, "bash: syntax error near unexpected token `%s'\n", target->str);
            else
                fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
            return (0);
        }
        tkn = target->next;
    }
    return (1);
}


int	parse_token(t_token *token_list)
{
	if (check_first_and_last_place(token_list))
        return 0;

	if(!check_redir(token_list))
		return 0;
    
    return 1;
}
