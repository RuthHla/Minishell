#include "../../minishell.h"

int builtin_pwd(t_command *cmd, t_shell *sh)
{
    (void)cmd; (void)sh;
    char buf[4096];

    if (getcwd(buf, sizeof buf))
    {
        write(STDOUT_FILENO, buf, strlen(buf));
        write(STDOUT_FILENO, "\n", 1);
        return 0;
    }
    perror("pwd");
    return 1;
}
