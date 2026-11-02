/* Program 1: Copy the content of one file to another using system calls
   Run: ./a.out source.txt destination.txt */
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char buf[1024];
    int n;
    int src = open(argv[1], O_RDONLY);                          /* open source for reading */
    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); /* create/overwrite destination */

    while ((n = read(src, buf, 1024)) > 0)   /* read a block until end of file */
        write(dst, buf, n);                  /* write the same block to destination */

    close(src);
    close(dst);
    return 0;
}
