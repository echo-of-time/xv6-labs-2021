#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 1) {
        fprintf(2, "Invalid argument(s) for pingpong!\npingpong: no need for any argument.\n");
        exit(1);
    }

    int pp[2];
    int pc[2];
    pipe(pp);
    pipe(pc);
    char buf[] = {' '};
    int pid = fork();

    if (pid == 0) {
        close(pc[1]);
        close(pp[0]);
        read(pc[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(pp[1], buf, 1);
        exit(0);
    } else {
        close(pp[1]);
        close(pc[0]);
        write(pc[1], buf, 1);
        wait((int*)0);
        read(pp[0], buf, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}
