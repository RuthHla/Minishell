#include "../minishell.h"

// t_character *rien(const char *str)
// {
//     t_character *head = NULL;
//     t_character *curr = NULL;
//     int current_word = 0;
//     int in_word = 0;
//     char last_op = 0;

//     while (*str)
//     {
//         if (*str == 34 || *str == 39)
//         {
//             char quote_type = *str;
//             current_word++;
//             t_character *quote_open = malloc(sizeof(t_character));
//             if (!quote_open)
//             {
//                 free_character_list(head);
//                 return NULL;
//             }
//             quote_open->c = quote_type;
//             quote_open->type = get_character_type(quote_type);
//             quote_open->word = current_word;
//             quote_open->next = NULL;
//             if (!head)
//                 head = quote_open;
//             else
//                 curr->next = quote_open;
//             curr = quote_open;
//             str++;
//             current_word++;
//             while (*str && *str != quote_type)
//             {
//                 t_character *quoted = malloc(sizeof(t_character));
//                 if (!quoted)
//                 {
//                     free_character_list(head);
//                     return NULL;
//                 }
//                 quoted->c = *str;
//                 quoted->type = LITERAL;
//                 quoted->word = current_word;
//                 quoted->next = NULL;
//                 curr->next = quoted;
//                 curr = quoted;
//                 str++;
//             }
//             if (*str == quote_type)
//             {
//                 current_word++;
//                 t_character *quote_close = malloc(sizeof(t_character));
//                 if (!quote_close)
//                 {
//                     free_character_list(head);
//                     return NULL;
//                 }
//                 quote_close->c = *str;
//                 quote_close->type = get_character_type(*str);
//                 quote_close->word = current_word;
//                 quote_close->next = NULL;
//                 curr->next = quote_close;
//                 curr = quote_close;
//                 str++;
//             }
//             in_word = 0;
//             last_op = 0;
//             continue;
//         }
//         if (!ft_isspace(*str))
//         {
//             t_character *new = malloc(sizeof(t_character));
//             if (!new)
//             {
//                 free_character_list(head);
//                 return NULL;
//             }
//             new->c = *str;
//             new->type = get_character_type(*str);
//             if (new->type == LITERAL)
//             {
//                 if (!in_word)
//                 {
//                     current_word++;
//                     in_word = 1;
//                 }
//                 last_op = 0;
//             }
//             else
//             {
//                 if (!in_word || last_op != *str)
//                     current_word++;
//                 in_word = 1;
//                 last_op = *str;
//             }
//             new->word = current_word;
//             new->next = NULL;
//             if (!head)
//                 head = new;
//             else
//                 curr->next = new;
//             curr = new;
//         }
//         else
//         {
//             in_word = 0;
//             last_op = 0;
//         }
//         str++;
//     }
//     return head;
// }

// t_token *o_k_ou(t_character *chars)
// {
// 	t_token *token_head = NULL;
// 	t_token *current_token = NULL;
// 	t_character *original = chars;

// 	while (chars)
// 	{
// 		t_token *new_token = malloc(sizeof(t_token));
// 		if (!new_token)
// 		{
// 			free_head(token_head);
// 			free_character_list(original);
// 			return NULL;
// 		}
// 		int word_id = chars->word;
// 		int len = get_word_len(chars, word_id);
// 		char *str = build_token_string(chars, len);
// 		if (!str)
// 		{
// 			free(new_token);
// 			free_head(token_head);
// 			free_character_list(original);
// 			return NULL;
// 		}
// 		new_token->str = str;
// 		if (chars->type == LITERAL)
// 			new_token->type = LITERAL;
// 		else if (len == 2)
// 			new_token->type = get_operator_token_type(chars);
// 		else if (len > 2)
// 			new_token->type = UNKNOWN;
// 		else
// 			new_token->type = chars->type;
// 		new_token->next = NULL;
// 		if (!token_head)
// 			token_head = new_token;
// 		else
// 			current_token->next = new_token;
// 		current_token = new_token;
// 		while (chars && chars->word == word_id)
// 			chars = chars->next;
// 	}
// 	free_character_list(original);
// 	return token_head;
// }

void malloc_str(t_character *char_list, char **str, t_type current_type, int current_word)
{
    int len = get_token_len(char_list, current_type, current_word);
    *str = malloc(sizeof **str * (len + 1));
}

void init_node(t_token **head, t_token **tail, t_character *char_list)
{
    t_token *new_node = NULL;
    if(!new_node)
        return NULL;

    new_node->str = NULL;
    new_node->next = NULL;
    node->type = char_list->type;// which_type(char_list);

    malloc_str(char_list ,&node ->str, current_type, current_word);
    if(!new_node)
    {
        free(node); 
        return NULL;
    }

    if(!head)
    {
        *head = new_node;
        *tail = new_node;
    }
    else    
    {
        *tail->next = new_node;
        *tail = new_node;
    }

    return new_node;
}

t_token *build_token_list(t_character *char_list)
{

    t_token *head = NULL;
    t_token *tail = NULL;
    t_token *node = NULL;
    int         current_word;
    t_type      current_type;
    int i;

    while(char_list)
    {
        current_word = char_list->word_id;
        current_type = char_list->type;
        node = init_node(&head, &tail, char_list, current_type, current_word);
        if(!node->str) // ajt free
            return NULL;

        i = 0;
        while(char_list && char_list->type == current_type && char_list->word_id == current_word)
        {
            node->str[i++] = char_list->c;
            char_list = char_list->next;
        }
        node->str[i] = '\0';
        char_list = char_list->next;
    }

    return head;
}

// 1, parcourir toute la liste de caractere
// 2, regle de base -> tant que mon type de mot est le meme et que id_word parei pareil je rempli sinon nouveau token
// 3, redir -> voir comment les assembler