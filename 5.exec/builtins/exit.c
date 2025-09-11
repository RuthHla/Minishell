#include "../minishell.h"

int builtin_exit(t_command *cmd, t_shell *sh)
{
    size_t argc = 0;
    for (t_element *e = cmd->element; e; e = e->next) if (e->kind == ARG) argc++;
    char **argv = calloc(argc + 1, sizeof(char*));
    if (!argv) return 1;
    size_t i = 0; for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG) argv[i++] = e->u_.arg->str;
    argv[i] = NULL;

    int idx = 0;
    if (argv[0] && strcmp(argv[0], "exit") == 0) idx = 1;

    if (argv[idx]) {
        dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
        free(argv);
        return 1;
    }

    dprintf(STDOUT_FILENO, "exit\n");
    free(argv);
    exit(sh->last_exit);
}
