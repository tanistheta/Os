/* Program 2: Create a process and display its PID and PPID */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    if (fork() == 0)                     /* fork() returns 0 in the child */
        printf("Child:  PID=%d PPID=%d\n", getpid(), getppid());
    else {
        wait(NULL);                      /* parent waits so child's PPID stays correct */
        printf("Parent: PID=%d PPID=%d\n", getpid(), getppid());
    }
    return 0;
}
