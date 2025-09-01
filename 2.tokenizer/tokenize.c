#include "minishell.h"
//IMPORTANT -> securiser les fonctions

typedef enum e_action 
{
    ACT_NORMAL = 0,
    ACT_VAR_NAME,
    ACT_VAR_SPECIAL,
    ACT_DOLLAR_LITERAL,
    ACT_DOLLAR_SOLO,
    ACT_OPERATOR,
    ACT_SKIP_DOLLAR_QUOTE
}   t_action;

static t_action decide_action(t_character *char_list)
{
    t_character *n;

    if (!char_list)
        return (ACT_NORMAL);
    if (is_operator_char(char_list->c))
        return (ACT_OPERATOR);
    if (char_list->c == '$' && char_list->context != S_QUOTE)
    {
        n = char_list->next;
		if (!n)
            return (ACT_DOLLAR_SOLO);
		if (n->context != NONE && char_list->context == NONE)
            return (ACT_SKIP_DOLLAR_QUOTE);
        if (!same_word(char_list, n))
            return (ACT_DOLLAR_SOLO);
        if (n->c == '?')
            return (ACT_VAR_SPECIAL);
        if (valid_variable_char(n->c))
            return (ACT_VAR_NAME);
        return (ACT_DOLLAR_LITERAL);
    }
    return (ACT_NORMAL);
}

int create_normal_token(t_token **h, t_token **t, t_character **p)
{
    t_character *s = *p;
    t_character *c = s;
    size_t      len = 0;

    while (c && same_word(s, c))
    {
        if (is_operator_char(c->c))
            break;

        if (c->c == '$' && c->context != S_QUOTE)
        {
            t_character *n = c->next;
            /* <-- clé : si '$' est le dernier char du mot, on l'absorbe */
            if (!n || !same_word(c, n))
            {
                len++;
                c = c->next;   /* on consomme le '$' final */
            }
            break;              /* sinon on s'arrête avant '$' */
        }

        len++;
        c = c->next;
    }

    if (len == 0) 
		return 1;

    t_token *tok = new_token(LITERAL, len);
    if (!tok) 
		return 0;

    c = s;
    for (size_t i = 0; i < len && c; i++, c = c->next)
        tok->str[i] = c->c;
    tok->str[len] = '\0';

    append_token(h, t, tok);
    *p = c;   /* on avance */

    return 1;
}

int create_operator_token(t_token **h, t_token **t, t_character **p)
{
    t_character *c;
    t_type      type;
    size_t      len;
    t_token     *tok;
    t_character *n;

    c = *p;
    n = (c ? c->next : NULL);
    type = UNKNOWN;
    len = 1;
    if (c->c == '|') type = PIPE;
    else if (c->c == '<')
    {
        if (n && same_word(c, n) && n->c == '<') { type = HEREDOC; len = 2; }
        else type = REDIR_IN;
    }
    else if (c->c == '>')
    {
        if (n && same_word(c, n) && n->c == '>') { type = APPEND; len = 2; }
        else type = REDIR_OUT;
    }
    tok = new_token(type, len);
    if (!tok)
        return (0);
    tok->str[0] = c->c;
    if (len == 2 && n)
        tok->str[1] = n->c;
    tok->str[len] = '\0';
    append_token(h, t, tok);
    if (len == 2 && n)
        *p = n->next;
    else
        *p = c->next;
    return (1);
}

t_token *build_token_list(t_character *char_list)
{
    t_token *head;
    t_token *tail;
    t_action a;

    head = NULL;
    tail = NULL;
    while (char_list)
    {
        a = decide_action(char_list);
        if (a == ACT_SKIP_DOLLAR_QUOTE)
        {
            if (!create_dollar_quoted_token(&head, &tail, &char_list))
                return (free_token_list(head), NULL);
        }
        else if (a == ACT_OPERATOR && !create_operator_token(&head, &tail, &char_list))
            return (free_token_list(head), NULL);
        else if (a == ACT_VAR_NAME && !create_variable_token(&head, &tail, &char_list))
            return (free_token_list(head), NULL);
        else if (a == ACT_VAR_SPECIAL && !create_special_variable_token(&head, &tail, &char_list))
            return (free_token_list(head), NULL);
        else if (a == ACT_DOLLAR_LITERAL && !create_dollar_literal(&head, &tail, &char_list))
            return (free_token_list(head), NULL);
        else if (a == ACT_DOLLAR_SOLO && !create_single_dollar_literal(&head, &tail, &char_list))
            return (free_token_list(head), NULL);
        else if (a == ACT_NORMAL && !create_normal_token(&head, &tail, &char_list))
            return (free_token_list(head), NULL);
        if (a == ACT_SKIP_DOLLAR_QUOTE)
            continue ;
        if (a == ACT_NORMAL && char_list == NULL)
            break ;
    }
    return (head);
}
