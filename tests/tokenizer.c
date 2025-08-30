#include "../minishell.h"

static const char	*get_type_name(t_type type)
{
	switch (type)
	{
	case LITERAL:
		return ("LITERAL");
	case PIPE:
		return ("PIPE");
	case AMPERSAND:
		return ("ampersand");
	case REDIR_IN:
		return ("<");
	case HEREDOC:
		return ("<<");
	case REDIR_OUT:
		return (">");
	case APPEND:
		return (">>");
	case DOLLAR:
		return ("$");
	case SPECIAL_VARIABLE:
		return ("SPECIAL_VARIABLE");
	default:
		return ("UNKNOWN");
	}
}

int	main(void)
{
	char		*line;
	t_character	*char_list;
    t_token     *token_list;

	line = NULL;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		char_list = build_char_list(line);
		if (!char_list)
		{
			free(line);
			continue ;
		}
        token_list = build_token_list(char_list);
		if(!token_list)
		{
			free(line);
			continue ;
		}

		int i = 0;
		while(token_list)
		{
			printf("token[%d] str[%s], type[%s]\n", i, token_list->str, get_type_name(token_list->type));
			i++;
			token_list = token_list->next;
		}
	}
	return (0);
}
// penser a free les listes char et token
