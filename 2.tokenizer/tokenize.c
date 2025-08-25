#include "../minishell.h"

t_character *build_token_list(const char *str)
{
    t_character *head = NULL;
    t_character *curr = NULL;
    int current_word = 0;
    int in_word = 0;
    char last_op = 0;

    while (*str)
    {
        if (*str == 34 || *str == 39)
        {
            char quote_type = *str;
            current_word++;
            t_character *quote_open = malloc(sizeof(t_character));
            if (!quote_open)
            {
                free_character_list(head);
                return NULL;
            }
            quote_open->c = quote_type;
            quote_open->type = get_character_type(quote_type);
            quote_open->word = current_word;
            quote_open->next = NULL;
            if (!head)
                head = quote_open;
            else
                curr->next = quote_open;
            curr = quote_open;
            str++;
            current_word++;
            while (*str && *str != quote_type)
            {
                t_character *quoted = malloc(sizeof(t_character));
                if (!quoted)
                {
                    free_character_list(head);
                    return NULL;
                }
                quoted->c = *str;
                quoted->type = LITERAL;
                quoted->word = current_word;
                quoted->next = NULL;
                curr->next = quoted;
                curr = quoted;
                str++;
            }
            if (*str == quote_type)
            {
                current_word++;
                t_character *quote_close = malloc(sizeof(t_character));
                if (!quote_close)
                {
                    free_character_list(head);
                    return NULL;
                }
                quote_close->c = *str;
                quote_close->type = get_character_type(*str);
                quote_close->word = current_word;
                quote_close->next = NULL;
                curr->next = quote_close;
                curr = quote_close;
                str++;
            }
            in_word = 0;
            last_op = 0;
            continue;
        }
        if (!ft_isspace(*str))
        {
            t_character *new = malloc(sizeof(t_character));
            if (!new)
            {
                free_character_list(head);
                return NULL;
            }
            new->c = *str;
            new->type = get_character_type(*str);
            if (new->type == LITERAL)
            {
                if (!in_word)
                {
                    current_word++;
                    in_word = 1;
                }
                last_op = 0;
            }
            else
            {
                if (!in_word || last_op != *str)
                    current_word++;
                in_word = 1;
                last_op = *str;
            }
            new->word = current_word;
            new->next = NULL;
            if (!head)
                head = new;
            else
                curr->next = new;
            curr = new;
        }
        else
        {
            in_word = 0;
            last_op = 0;
        }
        str++;
    }
    return head;
}

t_token *convert_to_tokens(t_character *chars)
{
	t_token *token_head = NULL;
	t_token *current_token = NULL;
	t_character *original = chars;

	while (chars)
	{
		t_token *new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			free_token_list(token_head);
			free_character_list(original);
			return NULL;
		}
		int word_id = chars->word;
		int len = get_word_len(chars, word_id);
		char *str = build_token_string(chars, len);
		if (!str)
		{
			free(new_token);
			free_token_list(token_head);
			free_character_list(original);
			return NULL;
		}
		new_token->str = str;
		if (chars->type == LITERAL)
			new_token->type = LITERAL;
		else if (len == 2)
			new_token->type = get_operator_token_type(chars);
		else if (len > 2)
			new_token->type = UNKNOWN;
		else
			new_token->type = chars->type;
		new_token->next = NULL;
		if (!token_head)
			token_head = new_token;
		else
			current_token->next = new_token;
		current_token = new_token;
		while (chars && chars->word == word_id)
			chars = chars->next;
	}
	free_character_list(original);
	return token_head;
}
