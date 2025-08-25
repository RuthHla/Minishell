#include "minishell.h"
#include <stddef.h>  // pour size_t

const char *get_cmd_name(t_type_cmd cmd)
{
    switch (cmd)
    {
        case T_ECHO:   return "echo";
        case T_CD:     return "cd";
        case T_PWD:    return "pwd";
        case T_EXPORT: return "export";
        case T_UNSET:  return "unset";
        case T_ENV:    return "env";
        case T_EXIT:   return "exit";
        default:       return "not_builtin";
    }
}

const char *get_type_name(t_type type)
{
    switch (type)
    {
        case LITERAL:      return "LITERAL";
        case PIPE:         return "PIPE";
        case OR:           return "or";
        case AMPERSAND:    return "ampersand";
        case AND:          return "and";
        case REDIR_IN:     return "<";
        case HEREDOC:      return "<<";
        case REDIR_OUT:    return ">";
        case APPEND:       return ">>";
        case VARIABLE:        return "variable";
        default:           return "UNKNOWN";
    }
}

/* Affiche la liste de tokens (utile pour debug). */
void print_tokens(t_token *token_list)
{
    for (; token_list; token_list = token_list->next)
    {
        printf("Token: '%s' [Type: %s]\n",
               token_list->str,
               get_type_name(token_list->type));
    }
}

/* Affiche la structure de commandes, args et redirections. */
void print_command_structure(t_command *cmd_list)
{
    while (cmd_list)
    {
        /* 1) La commande (builtin ou not_builtin) */
        printf("(cmd)[%s]\n", get_cmd_name(cmd_list->cmd));

        /* 2) Les arguments */
        for (size_t i = 0; i < cmd_list->nb_args; i++)
        {
            printf("    (args)[%s] -> %s\n",
                   get_type_name(cmd_list->args[i].type),
                   cmd_list->args[i].str);
        }

        /* 3) Les redirections */
        for (t_redir *r = cmd_list->redirs; r; r = r->next)
        {
            /* opérateur */
            printf("    (redir)[%s] -> %s\n",
                   get_type_name(r->type),
                   get_type_name(r->type));
            /* cible */
            if (r->target)
                printf("        (target)[%s] -> %s\n",
                       get_type_name(LITERAL),
                       r->target);
            else
                printf("        (target)[UNKNOWN] -> UNKNOWN\n");
        }

        /* 4) Le pipe sortant, si présent */
        if (cmd_list->has_pipe_out)
            printf("    [PIPE] -> |\n");

        printf("\n");
        cmd_list = cmd_list->next;
    }
}

int main(void)
{
    char *line;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            printf("exit\n");
            break;
        }

        /* ignore les lignes vides */
        if (*line == '\0')
        {
            free(line);
            continue;
        }

        add_history(line);

        /* 1) lexing char-by-char */
        t_character *char_list = build_token_list(line);
        if (!char_list)
        {
            fprintf(stderr, "Error building character list.\n");
            free(line);
            continue;
        }

        /* 2) création des tokens */
        t_token *token_list = convert_to_tokens(char_list);
        if (!token_list)
        {
            fprintf(stderr, "Error converting to tokens.\n");
            free_character_list(char_list);
            free(line);
            continue;
        }

        /* 3) parsing & construction des t_command */
        t_command *cmd_list = init_struct_globale(token_list, &line);
        if (!cmd_list)
            continue;
        

        /* 4) affichage de la structure interne */
        print_command_structure(cmd_list);
            

        /* 5) cleanup */
        cleanup(cmd_list);
        free_token_list(token_list);
        // free_character_list(char_list);
        free(line);
    }

    return 0;
}
