/* Program 3: Interprocess communication using pipe
   Parent writes a message into the pipe, child reads it */
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];               /* fd[0] = read end, fd[1] = write end */
    char buf[50];

    pipe(fd);                /* create the pipe before fork so both share it */

    if (fork() == 0) {       /* child: read from pipe */
        read(fd[0], buf, 50);
        printf("Child received: %s\n", buf);
    } else {                 /* parent: write into pipe */
        write(fd[1], "Hello from parent", 18);
    }
    return 0;
}
