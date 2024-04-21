#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

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

    // p = get a number from left neighbor
    // print p
    // loop:
    //     n = get a number from left neighbor
    //     if (p does not divide n)
    //         send n to right neighbor
    
    int p, n;
    while (read(pipeline[0], &p, sizeof(int)) > 0)
    {
        printf("prime: %d\n", p);
        while (read(pipeline[0], &n, sizeof(int)) > 0)
        {
            if (n % p != 0)
            {
                write(pipeline[1], &n, sizeof(int));
            }
        }
    }
    exit(0);
}