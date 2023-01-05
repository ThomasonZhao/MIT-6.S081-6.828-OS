#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pid, cpid;
    int pipefd[2];
    char buf[2];
    char *msg = " ";

    if (pipe(pipefd) < 0)
    {
        fprintf(2, "pipe error!\n");
        exit(1);
    }

    if ((cpid = fork()) < 0)
    {
        fprintf(2, "fork error!\n");
        exit(1);
    }
    else if (cpid == 0)
    { /* Child */
        pid = getpid();

        if (read(pipefd[0], buf, 1) != 1)
        {
            fprintf(2, "fail to read in child!\n");
            exit(1);
        }
        close(pipefd[0]);

        printf("%d: received ping\n", pid);

        if (write(pipefd[1], buf, 1) != 1)
        {
            fprintf(2, "fail to write in child!\n");
            exit(1);
        }
        close(pipefd[1]);

        exit(0);
    }
    else
    { /* Parent */
        pid = getpid();

        if (write(pipefd[1], msg, 1) != 1)
        {
            fprintf(2, "fail to write in parent!\n");
            exit(1);
        }
        close(pipefd[1]);

        /* Wait for child to terminate */
        wait(0);

        if (read(pipefd[0], buf, 1) != 1)
        {
            fprintf(2, "fail to read in parent!\n");
            exit(1);
        }
        close(pipefd[0]);

        printf("%d: received pong\n", pid);

        exit(0);
    }
}
