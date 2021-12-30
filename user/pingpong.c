#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    if(fork() == 0) { // child process
        close(p1[0]);
        close(p2[1]);
        int c_pid = getpid();
        char c_buf[10];
        int n = read(p2[0], c_buf, 4);
        if(n > 0) {
            fprintf(1, "%d: received ping\n", c_pid);
            write(p1[1], "pong", 4);
        }
        close(p1[1]);
        close(p2[0]);

    } else { // parent process
        close(p2[0]);
        close(p1[1]);
        int p_pid = getpid();
        char p_buf[10];
        write(p2[1], "ping", 4);
        int n = read(p1[0], p_buf, 4);
        if(n > 0)
            fprintf(1, "%d: received pong\n", p_pid);
        close(p1[0]);
        close(p2[1]);
    }
    exit(0);
}