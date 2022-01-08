#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void xargs(char *cmd, char **argv)
{
    int pid;
    // child process
    if ((pid = fork()) == 0)
    {
        exec(cmd, argv);
    }
    else
    { // parent process
        wait(&pid);
        return;
    }
}

int main(int argc, char *argv[])
{
    char cmd[] = "echo";
    char stdin_args[512];
    char *p, *h;
    if (argc > 1)
        strcpy(cmd, argv[1]);
    memcpy(argv, argv + 1, sizeof(char *) * (argc - 1));
    argv[argc] = 0;

    char buf[512];
    read(0, buf, sizeof(buf)); // read params from stdin

    p = h = buf;
    // feed xargs by line('/n')
    while ((p = strchr(h, '\n')) != 0)
    {
        *p = '\0';
        strcpy(stdin_args, h);
        argv[argc - 1] = stdin_args;
        xargs(cmd, argv);
        h = p + 1;
    }
    exit(0);
}