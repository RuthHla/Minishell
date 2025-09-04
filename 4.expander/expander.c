/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:09:02 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 11:48:13 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// je parcours ma liste de commande, si jai un type dollar ou speciale_variable
// je modifie la variable en question par les data dans t_shell.
// si pas de variable trouvee alors pas d'erreur mais juste une ligne vide (donc enregistrer du vide)

// static char	*expand_arg_variable(t_shell shell, t_arg arg, char **tab_variable, t_type type)
// {
// 	char	*str;
// 	char	buffer[12];

// 	str = NULL;
// 	if (type == SPECIAL_VARIABLE) // remplacer special variable par si je trouve une variabele $? dans mon tableau alors
// 	{
// 		itoa(shell.last_exit, buffer, 10);
// 		str = strdup(buffer);
// 	}
// 	else
// 		str = find_variable_in_env(shell.env, arg.str);
// 	if (!str)
// 		str = strdup(""); // verifier
// 	else
// 		strdup(str);
// 	return (str);
// }

// static char	*expand_redir_variable(t_shell shell, t_redir redir, char **tab_variable, t_type type)
// {
// 	char	*str;
// 	char	buffer[12];

// 	str = NULL;
// 	if (type == SPECIAL_VARIABLE) // remplacer special variable par si je trouve une variabele $? dans mon tableau alors
// 	{
// 		itoa(shell.last_exit, buffer, 10);
// 		str = strdup(buffer);
// 	}
// 	else
// 		str = find_variable_in_env(shell.env, variable.target);
// 	if (!str)
// 		str = strdup(""); // verifier
// 	else
// 		strdup(str);
// 	return (str);
// }



// void	expander(t_command **cmd_list, t_shell *shell)
// {
// 	t_command	*cmd;
// 	t_element	*element;
// 	char		*old_str;
// 	char 		**tab_variable;

// 	cmd = (*cmd_list);
// 	while (cmd)
// 	{
// 		element = cmd->element;
// 		while (element)
// 		{
// 			if (element->kind == ARG)
// 			{
// 				if (element->u_.arg->type == DOLLAR || element->u_.arg->type == SPECIAL_VARIABLE)
// 				{
// 					tab_variable = find_variable_in_str(element->u_.arg->str);
// 					old_str = element->u_.arg->str;
// 					element->u_.arg->str = expand_arg_variable(*shell,
// 							*element->u_.arg, tab_variable, element->u_.arg->type);
// 					free(old_str);
// 				}
// 			}
// 			else if (element->kind == REDIR)
// 			{
// 				if (element->u_.redirs->target_type == DOLLAR || element->u_.redirs->target_type == SPECIAL_VARIABLE)
// 				{
// 					old_str = element->u_.redirs->target;
// 					element->u_.redirs->target = expand_redir_variable(*shell,
// 							*element->u_.redirs,
// 							element->u_.redirs->target_type);
// 					free(old_str);
// 				}
// 			}
// 			element = element->next;
// 		}
// 		cmd = cmd->next;
// 	}
// }


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:09:02 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 11:38:29 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Structure pour stocker info sur chaque variable trouvée
typedef struct s_var_pos {
    char    *name;      // Nom de la variable (sans le $)
    int     start;      // Position du $ dans la string
    int     end;        // Position après la variable
} t_var_pos;

// Trouve toutes les variables avec leurs positions
static t_var_pos *find_variables_with_positions(char *str, int *count)
{
    t_var_pos   *vars;
    int         i = 0;
    int         j;
    int         index = 0;
    
    *count = how_many_variable(str);
    if (*count == 0)
        return (NULL);
    
    vars = malloc(sizeof(t_var_pos) * (*count));
    if (!vars)
        return (NULL);
    
    while (str[i])
    {
        if (str[i] == '$' && valid_variable_char(str[i + 1]))
        {
            vars[index].start = i;
            j = i + 1;
            
            // Calculer la longueur du nom de la variable
            int var_len = strlen_variable(str, j);
            
            // Extraire le nom de la variable
            vars[index].name = malloc(sizeof(char) * (var_len + 1));
            if (!vars[index].name)
            {
                // Libérer la mémoire déjà allouée
                while (--index >= 0)
                    free(vars[index].name);
                free(vars);
                return (NULL);
            }
            
            int k = 0;
            while (valid_variable_char(str[j]))
                vars[index].name[k++] = str[j++];
            vars[index].name[k] = '\0';
            
            vars[index].end = j;
            index++;
            i = j - 1;
        }
        i++;
    }
    return (vars);
}

// Obtient la valeur d'une variable
static char *get_variable_value(t_shell *shell, char *var_name)
{
    char    buffer[12];
    char    *value;
    
    // Vérifier si c'est une variable spéciale
    if (strcmp(var_name, "?") == 0)
    {
        itoa(shell->last_exit, buffer, 10);
        return (strdup(buffer));
    }
    // Ajouter d'autres variables spéciales si nécessaire
    // $$ pour le PID, $# pour le nombre d'arguments, etc.
    
    // Chercher dans l'environnement
    value = find_variable_in_env(shell->env, var_name);
    if (value)
        return (strdup(value));
    
    // Si la variable n'existe pas, retourner une chaîne vide
    return (strdup(""));
}

// Calcule la taille nécessaire pour la nouvelle string
static int calculate_expanded_length(char *original, t_var_pos *vars, 
                                    char **values, int var_count)
{
    int new_len = strlen(original);
    int i = 0;
    
    while (i < var_count)
    {
        // Soustraire la longueur de $VAR
        new_len -= (vars[i].end - vars[i].start);
        // Ajouter la longueur de la valeur
        new_len += strlen(values[i]);
        i++;
    }
    return (new_len);
}

// Construit la nouvelle string avec les variables remplacées
static char *build_expanded_string(char *original, t_var_pos *vars, 
                                  char **values, int var_count)
{
    char    *expanded;
    int     new_len;
    int     orig_idx = 0;
    int     exp_idx = 0;
    int     var_idx = 0;
    
    // Calculer la taille nécessaire
    new_len = calculate_expanded_length(original, vars, values, var_count);
    expanded = malloc(sizeof(char) * (new_len + 1));
    if (!expanded)
        return (NULL);
    
    // Construire la nouvelle string
    while (original[orig_idx])
    {
        // Si on arrive à une variable
        if (var_idx < var_count && orig_idx == vars[var_idx].start)
        {
            // Copier la valeur de la variable
            int val_len = strlen(values[var_idx]);
            int i = 0;
            while (i < val_len)
                expanded[exp_idx++] = values[var_idx][i++];
            
            // Sauter la variable dans l'original
            orig_idx = vars[var_idx].end;
            var_idx++;
        }
        else
        {
            // Copier le caractère normal
            expanded[exp_idx++] = original[orig_idx++];
        }
    }
    expanded[exp_idx] = '\0';
    return (expanded);
}

// Fonction principale d'expansion d'une string
static char *expand_string(t_shell *shell, char *str)
{
    t_var_pos   *vars;
    char        **values;
    char        *expanded;
    int         var_count;
    int         i;
    
    // Trouver toutes les variables avec leurs positions
    vars = find_variables_with_positions(str, &var_count);
    if (!vars || var_count == 0)
        return (strdup(str)); // Pas de variables, retourner une copie
    
    // Obtenir les valeurs de toutes les variables
    values = malloc(sizeof(char *) * var_count);
    if (!values)
    {
        i = 0;
        while (i < var_count)
            free(vars[i++].name);
        free(vars);
        return (NULL);
    }
    
    i = 0;
    while (i < var_count)
    {
        values[i] = get_variable_value(shell, vars[i].name);
        if (!values[i])
        {
            // En cas d'erreur, nettoyer et retourner NULL
            int j = 0;
            while (j < i)
                free(values[j++]);
            j = 0;
            while (j < var_count)
                free(vars[j++].name);
            free(vars);
            free(values);
            return (NULL);
        }
        i++;
    }
    
    // Construire la string expandée
    expanded = build_expanded_string(str, vars, values, var_count);
    
    // Libérer la mémoire
    i = 0;
    while (i < var_count)
    {
        free(vars[i].name);
        free(values[i]);
        i++;
    }
    free(vars);
    free(values);
    
    return (expanded);
}

// Fonction principale d'expansion
void expander(t_command **cmd_list, t_shell *shell)
{
    t_command   *cmd;
    t_element   *element;
    char        *old_str;
    char        *new_str;
    
    cmd = *cmd_list;
    while (cmd)
    {
        element = cmd->element;
        while (element)
        {
            if (element->kind == ARG)
            {
                // Vérifier si la string contient des variables
                if (strchr(element->u_.arg->str, '$'))
                {
                    old_str = element->u_.arg->str;
                    new_str = expand_string(shell, old_str);
                    if (new_str)
                    {
                        element->u_.arg->str = new_str;
                        free(old_str);
                    }
                    // Si expansion échoue, garder l'ancienne string
                }
            }
            else if (element->kind == REDIR)
            {
                // Vérifier si la target contient des variables
                if (strchr(element->u_.redirs->target, '$'))
                {
                    old_str = element->u_.redirs->target;
                    new_str = expand_string(shell, old_str);
                    if (new_str)
                    {
                        element->u_.redirs->target = new_str;
                        free(old_str);
                    }
                }
            }
            element = element->next;
        }
        cmd = cmd->next;
    }
}
