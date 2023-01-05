#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int time;

    if (argc != 2)
    {
        fprintf(2, "Usage: %s [TIME]\n", argv[0]);
        exit(1);
    }

    time = atoi(argv[1]);
    sleep(time);

    exit(0);
}
