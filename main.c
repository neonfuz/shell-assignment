#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline
int is_whitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\r';
}

int main(int argc, char **argv)
{
    char line[256];
    char *args[256];

    int loop = 1;
    while (loop) {
        fputs("$ ", stdout);
        fgets(line, 256, stdin);

        int args_len = 0, in_arg = 0;
        for (int line_i = 0; line[line_i] != '\0'; ++line_i) {
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

        for(int i=0; i<args_len; ++i)
            if (strcmp("quit", args[i]) == 0)
                loop = 0;
    }

    return 0;
}
