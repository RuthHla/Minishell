/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:49:21 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 11:51:13 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*itoa(int value, char *str, int base)
{
	char	*rc;
	char	*ptr;
	char	*low;
	char	tmp;

	rc = str;
	if (value < 0 && base == 10)
	{
		*str++ = '-';
		value = -value;
	}
	ptr = str;
	if (value == 0)
	{
		*ptr++ = '0';
	}
	else
	{
		while (value)
		{
			*ptr++ = "0123456789abcdef"[value % base];
			value /= base;
		}
	}
	*ptr-- = '\0';
	for (low = str; low < ptr; ++low, --ptr)
	{
		tmp = *low;
		*low = *ptr;
		*ptr = tmp;
	}
	return (rc);
}

char	*find_variable_in_env(char **env, char *variable)
{
	int	i;
	int	len;

	len = strlen(variable);
	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], variable, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	how_many_variable(char *str)
{
	int i = 0;
	int j = 0;
	int len = strlen(str);
	int n_var = 0;

	while (i < len)
	{
		if (str[i] == '$') 
		{
			j = i + 1;
			if (j < len && (valid_variable_char(str[j]) || str[j] == '?'))
			{
				n_var++;
				while (j < len && (valid_variable_char(str[j]) || str[j] == '?'))
					j++;
				i = j - 1;
			}
		}
		i++;
	}
	return (n_var);
}

int strlen_variable(char *str, int j)
{
    int count;
    
    count = 0;
    if (str[j] == '?')
        return 1;
    
    while (str[j] && valid_variable_char(str[j]))
    {
        count++;
        j++;
    }
    return (count);
}

char **find_variable_in_str(char *str)
{
    char **variable;
    int str_len;
    int var_count;
    int i = 0;
    int j = 0;
    int index = 0;
    int k = 0;
    int var_len;
    
    var_count = how_many_variable(str);
    if (var_count == 0)
        return (NULL);
    
    variable = malloc(sizeof(char *) * (var_count + 1));
    if (!variable)
        return (NULL);
    
    str_len = strlen(str);
    while (i < str_len)
    {
        if (str[i] == '$' && i + 1 < str_len && (valid_variable_char(str[i + 1]) || str[i + 1] == '?'))
        {
            j = i + 1;
            k = 0;
            
            if (str[j] == '?')
            {
                variable[index] = malloc(sizeof(char) * 2);
                if (!variable[index])
                {
                    while (--index >= 0)
                        free(variable[index]);
                    free(variable);
                    return (NULL);
                }
                variable[index][0] = '?';
                variable[index][1] = '\0';
                index++;
                i = j + 1;
            }
            else
            {
                var_len = strlen_variable(str, j);
                variable[index] = malloc(sizeof(char) * (var_len + 1));
                if (!variable[index])
                {
                    while (--index >= 0)
                        free(variable[index]);
                    free(variable);
                    return (NULL);
                }
                while (valid_variable_char(str[j]))
                    variable[index][k++] = str[j++];
                variable[index][k] = '\0';
                index++;
                i = j - 1;
            }
        }
        else
        {
            i++;
        }
    }
    variable[index] = NULL;
    return (variable);
}
