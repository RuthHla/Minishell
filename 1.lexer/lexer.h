/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:16:04 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 11:51:26 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "../minishell.h"

typedef struct s_character	t_character;
typedef enum s_ctx			t_ctx;

typedef struct s_build_state
{
	t_character				*head;
	t_character				*tail;
	int						word;
}							t_build_state;

t_character					*init_node(char ch, t_ctx context,
								t_character *tail);
int							check_empty_string(t_ctx ctx, char next_c,
								char next_next_c);
int							check_oprhan_quote(t_character *head,
								t_ctx current_context);

#endif