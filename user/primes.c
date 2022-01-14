#include "kernel/types.h"
#include "user/user.h"

void primes(int from) {
    int right_pipe[2];
    int pivot;
    int num;
    int pid;

    pipe(right_pipe);
    // read one int from right pipe
    read(from, &num, sizeof(int));
    printf("prime %d\n", num);
    pivot = num;
    if(pivot == 35) return;
    if((pid = fork()) == 0) {
        close(right_pipe[1]);
        primes(right_pipe[0]);
        close(right_pipe[0]);
    } else {
        while(read(from, &num, sizeof(int)) != 0) {
            // printf("test %d\n", num);
            if(num % pivot == 0) continue;
            else write(right_pipe[1], &num, sizeof(int));
        }
        //close the write end
        close(right_pipe[1]);
        wait(0);
    }
    close(from);
    return;
}

int main(int argc, char *argv[]) {
    int r = 35;
    int left_pipe[2];
    pipe(left_pipe);
    for(int i = 2; i <= r; i ++) write(left_pipe[1], &i, sizeof(int));
    close(left_pipe[1]);
    primes(left_pipe[0]);
    

    exit(0);
}