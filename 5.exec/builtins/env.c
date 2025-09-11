#include "../../minishell.h"

int builtin_env(t_command *cmd, t_shell *sh)
{
    size_t argc = 0;
    for (t_element *e = cmd->element; e; e = e->next) if (e->kind == ARG) argc++;
    char **argv = calloc(argc + 1, sizeof(char*));
    if (!argv) return 1;
    size_t i = 0; for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG) argv[i++] = e->u_.arg->str;
    argv[i] = NULL;

    int idx = 0;
    if (argv[0] && strcmp(argv[0], "env") == 0) idx = 1;

    if (argv[idx]) {
        dprintf(STDERR_FILENO, "minishell: env: too many arguments\n");
        free(argv);
        return 1;
    }

    for (int k = 0; sh->env && sh->env[k]; ++k){
        if (strchr(sh->env[k], '=')) // n'afficher que NAME=VALUE
            dprintf(STDOUT_FILENO, "%s\n", sh->env[k]);
    }
    free(argv);
    return 0;
}
