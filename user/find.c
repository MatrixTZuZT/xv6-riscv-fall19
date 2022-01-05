#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char path[], char file_name[])
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        close(fd); // close file after open to release memory
        return;
    }
    // printf("test0\n");
    switch (st.type)
    {
    case T_FILE:

        break;

    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(path);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0; // equals to '\0'
            // printf("%s\n", p);
            if (stat(buf, &st) < 0)
            {
                continue;
            }
            else if (st.type == T_DIR)
            {
                find(buf, file_name);
            }
            else
            {
                if (strcmp(de.name, file_name) == 0)
                {
                    printf("%s\n", buf);
                }
            }
        }
        break;
    default:
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}