#include "../minishell.h"
#include <fcntl.h>

static int open_in(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) perror(path);
    return fd;
}

static int open_out_trunc(const char *path) {
    int fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fd < 0) perror(path);
    return fd;
}

static int open_out_append(const char *path) {
    int fd = open(path, O_WRONLY|O_CREAT|O_APPEND, 0644);
    if (fd < 0) perror(path);
    return fd;
}

int collect_redirs_fds(t_element *elem, t_ios *ios)
{
    ios->in_fd = -1; ios->out_fd = -1;
    while (elem) {
        if (elem->kind == REDIR) {
            t_redir *r = elem->u_.redirs;
            for (; r; r = r->next) {
                if (r->type == REDIR_IN) {
                    if (ios->in_fd >= 0) close(ios->in_fd);
                    ios->in_fd = open_in(r->target);
                    if (ios->in_fd < 0) return 0;
                } else if (r->type == HEREDOC) {
                    if (ios->in_fd >= 0) close(ios->in_fd);
                    ios->in_fd = create_heredoc_fd(r->target);
                    if (ios->in_fd < 0) return 0;
                } else if (r->type == REDIR_OUT) {
                    if (ios->out_fd >= 0) close(ios->out_fd);
                    ios->out_fd = open_out_trunc(r->target);
                    if (ios->out_fd < 0) return 0;
                } else if (r->type == APPEND) {
                    if (ios->out_fd >= 0) close(ios->out_fd);
                    ios->out_fd = open_out_append(r->target);
                    if (ios->out_fd < 0) return 0;
                }
            }
        }
        elem = elem->next;
    }
    return 1;
}

int apply_redirs(const t_ios *ios)
{
    if (ios->in_fd >= 0) {
        if (dup2(ios->in_fd, STDIN_FILENO) < 0) { perror("dup2"); return 0; }
    }
    if (ios->out_fd >= 0) {
        if (dup2(ios->out_fd, STDOUT_FILENO) < 0) { perror("dup2"); return 0; }
    }
    return 1;
}

void close_redirs(t_ios *ios)
{
    if (ios->in_fd >= 0) { close(ios->in_fd); ios->in_fd = -1; }
    if (ios->out_fd >= 0) { close(ios->out_fd); ios->out_fd = -1; }
}
