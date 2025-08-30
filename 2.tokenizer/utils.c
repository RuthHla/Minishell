#include "../minishell.h"
//IMPORTANT -> securiser les fonctions

int	valid_variable_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	return (0);
}

void	free_token_list(t_token *head) // verfier si assez securise
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

int is_operator_char(char c)
{
    if (c == '|' || c == '&' || c == '<' || c == '>')
        return (1);
    return (0);
}

int same_word(t_character *a, t_character *b)
{
    if (!a || !b)
        return (0);
    if (a->word_id == b->word_id)
        return (1);
    return (0);
}

t_token *new_token(t_type type, size_t len)
{
    t_token *tkn;

    tkn = (t_token *)malloc(sizeof(*tkn));
    if (!tkn)
        return (NULL);
    tkn->str = (char *)malloc(len + 1);
    if (!tkn->str)
    {
        free(tkn);
        return (NULL);
    }
    tkn->type = type;
    tkn->next = NULL;
    tkn->str[0] = '\0';
    return (tkn);
}

void append_token(t_token **head, t_token **tail, t_token *node)
{
    if (!node)
        return ;
    if (!*head)
    {
        *head = node;
        *tail = node;
    }
    else
    {
        (*tail)->next = node;
        *tail = node;
    }
}