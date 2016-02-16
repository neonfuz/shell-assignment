#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

static inline
int is_whitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\r';
}

int parse_line(char *line, char *args[])
{
    int args_len = 0;
    for (int line_i = 0, in_arg = 0; line[line_i] != '\0'; ++line_i) {
        if (!in_arg) {
            if (!is_whitespace(line[line_i])) {
                args[args_len++] = &line[line_i];
                in_arg = 1;
            }
        } else {
            if (is_whitespace(line[line_i])) {
                line[line_i] = '\0';
                in_arg = 0;
            }
        }
    }
    args[args_len] = NULL;

    return args_len;
}

int main(int argc, char **argv)
{
    char line[256];
    char *args[256];

    while (1) {
        fputs("$ ", stdout);
        fgets(line, 256, stdin);

        int args_len = parse_line(line, args);

        if (args_len > 0 && strcmp("exit", args[0]) == 0)
            break;

        pid_t childPid = fork();

        if(childPid == 0) {
            int fd = STDOUT_FILENO;

            if (args_len >= 2 && strcmp(">", args[args_len-2]) == 0 ) {
                close(fd);
                fd = open(args[args_len-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }

            if(args_len > 0)
                execvp(args[0], args);

            break;
        } else {
            waitpid(childPid, NULL, 0);
        }
    }

    return 0;
}
