#include "../minishell.h"

static const char	*get_type_name(t_type type)
{
	switch (type)
	{
	case LITERAL:
		return ("LITERAL");
	case PIPE:
		return ("PIPE");
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
	default:
		return ("UNKNOWN");
	}
}

static const char	*get_ctx_name(t_ctx c)
{
	switch (c)
	{
	case NONE:
		return ("NONE");
	case S_QUOTE:
		return ("S_QUOTE");
	case D_QUOTE:
		return ("D_QUOTE");
	default:
		return ("?");
	}
}

int	main(void)
{
	char		*line;
	t_character	*char_list;

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
		while (char_list)
		{
			printf("word[%d] char[%c] type[%s] context[%s]\n", char_list->word_id, char_list->c,
				get_type_name(char_list->type),
				get_ctx_name(char_list->context));
			char_list = char_list->next;
		}
	}
	return (0);
}
// penser a free les listes char et token
