#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

/* Maxinum character a line could have */
#define MAXLINE 1024

char *readline(void)
{
    int i, cc;
    char c;
    char *buf;

    buf = malloc(MAXLINE);
    if (buf == 0)
    {
        return 0;
    }

    for (i = 0; i + 1 < MAXLINE;)
    {
        cc = read(0, &c, 1);
        if (cc < 0)
        {
            fprintf(2, "read error!\n");
            exit(1);
        }
        if (cc == 0)
        { /* Nothing read in */
            free(buf);
            return 0;
        }

        if (c == '\n' || c == '\r')
            break;

        buf[i++] = c;
    }

    buf[i] = '\0';
    return buf;
}

int main(int argc, char const *argv[])
{
    char *new_argv[MAXARG], *buf;
    int i, pid;

    if (argc < 2)
    {
        fprintf(2, "Usage: %s [command [initial-arguments]]\n", argv[0]);
        exit(1);
    }

    for (i = 0; i < argc; i++)
    {
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
    }

    while ((buf = readline()) != 0)
    {
        new_argv[i - 1] = buf;
        new_argv[i] = 0; /* Null termination */

        if ((pid = fork()) < 0)
        {
            fprintf(2, "fork error!\n");
            exit(1);
        }
        else if (pid == 0)
        { /* Child */
            exec(new_argv[0], new_argv);
            fprintf(2, "exec error!\n");
            exit(1);
        }
        else
        { /* Parent */
            wait(0);
        }
    }

    exit(0);
}
