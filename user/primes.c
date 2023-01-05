#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define START 2
#define END 35

void primes(int *pipefd1)
{
    int prime, num, pid;
    int pipefd2[2]; /* Another pipe for child */

    /* Child don't need to write */
    close(pipefd1[1]);

    if (read(pipefd1[0], &prime, sizeof(int)) != sizeof(int))
    {
        fprintf(2, "fail to read!\n");
        exit(1);
    }

    printf("prime %d\n", prime);

    /* Make sure what we read is not the last one */
    if (read(pipefd1[0], &num, sizeof(int)) > 0)
    {
        if (pipe(pipefd2) < 0)
        {
            fprintf(2, "pipe error!\n");
            exit(1);
        }

        if ((pid = fork()) < 0)
        {
            fprintf(2, "fork error!\n");
            exit(1);
        }
        else if (pid == 0)
        { /* Child */
            primes(pipefd2);
        }
        else
        { /* Parent */
            /* Parent don't need to read */
            close(pipefd2[0]);

            do
            {
                if (num % prime != 0)
                    write(pipefd2[1], &num, sizeof(int));
            } while (read(pipefd1[0], &num, sizeof(int)) > 0);

            close(pipefd1[0]);
            close(pipefd2[1]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char const *argv[])
{
    int pid;
    int pipefd[2];

    if (pipe(pipefd) < 0)
    {
        fprintf(2, "pipe error!\n");
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        fprintf(2, "fork error!\n");
        exit(1);
    }
    else if (pid == 0)
    { /* Child */
        primes(pipefd);
    }
    else
    { /* Parent */
        /* Parent don't need to read */
        close(pipefd[0]);

        for (int i = START; i <= END; i++)
        {
            if (write(pipefd[1], &i, sizeof(int)) != sizeof(int))
            {
                fprintf(2, "fail to write!\n");
                exit(1);
            }
        }

        close(pipefd[1]);
        wait(0);
    }

    exit(0);
}
