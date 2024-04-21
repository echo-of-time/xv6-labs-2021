#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int fd){
    // p = get a number from left neighbor
    // print p
    // loop:
    //     n = get a number from left neighbor
    //     if (p does not divide n)
    //         send n to right neighbor

    int pipeline[2];
    pipe(pipeline);

    int left_num;
    read(fd, &left_num, sizeof(int));
    close(fd);
    printf("primes: %d\n", left_num);
    int pid = fork();
    if (pid == 0) {
        primes(pipeline[1]);
    } else if (pid > 0){
        int n;
        read(pipeline[0], &n, sizeof(int));
        close(pipeline[0]);
        if (n % left_num != 0) {
            write(pipeline[1], &n, sizeof(int));
        }
        close(pipeline[1]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(2, "Invalid argument(s) for primes!\nprimes: no need for any argument.\n");
        exit(1);
    }

    // The first process feeds the numbers 2 through 35 into the pipeline.
    int pipeline[2];
    pipe(pipeline);
    for (int i = 2; i < 36; i++)
    {
        write(pipeline[1], &i, sizeof(int));
    }
    close(pipeline[1]);
    primes(pipeline[0]);
    close(pipeline[0]);

    int p, n;
    read(pipeline[0], &p, sizeof(int));
    printf("prime: %d\n", p);
    while (read(pipeline[0], &n, sizeof(int)) > 0)
    {
        if (n % p != 0)
        {
            write(pipeline[1], &n, sizeof(int));
            int pid = fork();
            if (pid == 0)
            {
                // child
                read(pipeline[0], &p, sizeof(int));
                printf("prime: %d\n", p);
            }
            else
            {
                // parent
                wait(0);
            }
        }
    }

    exit(0);
}