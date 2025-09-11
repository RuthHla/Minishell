#include "../../minishell.h"

static int is_echo_n(const char *s)
{
    if (!s || s[0] != '-') return 0;
    for (int i = 1; s[i]; ++i) if (s[i] != 'n') return 0;
    return (strlen(s) > 1);
}

int builtin_echo(t_command *cmd, t_shell *sh)
{
    (void)sh;
    int no_newline = 0;
    int first = 1;

    t_element *e = cmd->element;
    if (e && e->kind == ARG) e = e->next;

    while (e && e->kind == ARG && e->u_.arg && is_echo_n(e->u_.arg->str))
        e = e->next, no_newline = 1;

    for (; e; e = e->next) {
        if (e->kind != ARG || !e->u_.arg) continue;
        if (!first) write(STDOUT_FILENO, " ", 1);
        first = 0;
        if (e->u_.arg->str)
            write(STDOUT_FILENO, e->u_.arg->str, strlen(e->u_.arg->str));
    }
    if (!no_newline) write(STDOUT_FILENO, "\n", 1);
    return 0;
}
