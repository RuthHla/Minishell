#include "../../minishell.h"

int ft_len_without_spaces(const char *str)
{
    int i = 0;
    int c = 0;

    while (str[i])
    {
        if (!ft_isspace(str[i]))
            c++;
        i++;
    }
    return c;
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}
