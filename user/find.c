#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../user/user.h"
#include "../kernel/fs.h"

void 
find(char *path, char *filename)
{
    char buf[512], *p;
    int fd; // file description
    struct dirent de; // include "inum" and file name
    struct stat st; // include disk device, inode, type, link counts and size of the file
    
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
    case T_FILE:
        for (p = path + strlen(path); p >= path && *p != '/'; p--)
            ;
        p++;
        if (strcmp(filename, p)==0) {
            printf("%s\n", path);
        }
        close(fd);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path is too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, filename);
        }
        close(fd);
        break;
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "Missing filename to be found!\nUse ``find [filename]`` to find the specific file in current directory.\n");
        exit(1);
    } else if (argc == 2) {
        find((char*)".", argv[1]);
        exit(0);
    } else {
        for (int i = 2; i < argc; i++)
            {find(argv[1], argv[i]);}
        exit(0);
    }

}
