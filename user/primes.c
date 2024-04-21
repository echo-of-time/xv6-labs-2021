#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXSIZE 35

/*pipe left or right is coresponding to this parent process*/
void primes(int pipe_left){
    int pipe_right[2];
    pipe(pipe_right);
    
    // print first prime
    int first_num_from_left;
    int readFLAG = read(pipe_left, &first_num_from_left, sizeof(int)); 
    // if readFLAG = 0, then no data ****KEY FOR END RECURSION****
    if (!readFLAG) {
        return;
    } else {
        printf("prime %d\n", first_num_from_left);
    }

    int forkPID = fork();
    if (forkPID == 0) {
        close(pipe_right[1]);
        primes(pipe_right[0]);
        close(pipe_right[0]);
    } else if (forkPID > 0) {
        // p = get a number from left neighbor
        // print p
        // loop:
        //     n = get a number from left neighbor
        //     if (p does not divide n)
        //         send n to right neighbor
        close(pipe_right[0]);

        // deal with other num
        int other_num_from_left;
        while (read(pipe_left, &other_num_from_left, sizeof(int)) > 0) {
            if (other_num_from_left % first_num_from_left != 0) {
                write(pipe_right[1], &other_num_from_left, sizeof(int));
            }
        }
        close(pipe_right[1]);
        wait(0);
    }
    
}

int main(int argc, char *argv[])
{
    if (argc != 1) {
        fprintf(2, "Invalid argument(s) for primes!\nprimes: no need for any argument.\n");
        exit(1);
    }

    // The first process feeds the numbers 2 through 35 into the pipe_left.
    int pipe_left[2];
    pipe(pipe_left);
    for (int i = 2; i < MAXSIZE+1; i++) {
        write(pipe_left[1], &i, sizeof(int));
    }
    int forkPID = fork();
    if (forkPID == 0) {
        close(pipe_left[1]);
        primes(pipe_left[0]);
        close(pipe_left[0]);
    } else if (forkPID > 0) {
        close(pipe_left[0]);
        close(pipe_left[1]); 
        wait(0);
    } else {
        fprintf(2, "fork : error!\n");
        exit(1);
    }
    exit(0);
}
