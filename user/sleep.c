#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
    if (argc == 2) {
        int time = atoi(argv[1]);
        sleep(time);
        exit(0);
    } else {
        fprintf(2,"Invalid argument for sleep!\nUse ``sleep [n]`` to pause for n clock ticks.\n");
        exit(1);
    }
}
