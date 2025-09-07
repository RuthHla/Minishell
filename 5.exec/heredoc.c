#include "../minishell.h"
#include <fcntl.h>

int create_heredoc_fd(const char *delim)
{
    int pfd[2];
    if (pipe(pfd) < 0) { perror("pipe"); return -1; }

    while (1) {
        char *line = readline("> "); // pas d'historique
        if (!line) { 
            break;
        }
        if (strcmp(line, delim) == 0) {
            free(line);
            break;
        }
        write(pfd[1], line, strlen(line));
        write(pfd[1], "\n", 1);
        free(line);
    }
    close(pfd[1]);     // lecture
    return pfd[0];
}
