# OS Lab Programs

## 1. Copy content of one file to another using system calls

`1_file_copy.c`

```c
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char buf[1024];
    int n;
    int src = open(argv[1], O_RDONLY);
    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    while ((n = read(src, buf, 1024)) > 0)
        write(dst, buf, n);

    close(src);
    close(dst);
    return 0;
}
```

## 2. Create a process and display its PID and PPID

`2_process_pid.c`

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    if (fork() == 0)
        printf("Child:  PID=%d PPID=%d\n", getpid(), getppid());
    else {
        wait(NULL);
        printf("Parent: PID=%d PPID=%d\n", getpid(), getppid());
    }
    return 0;
}
```

## 3. Interprocess communication using pipe

`3_pipe_ipc.c`

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    char buf[50];

    pipe(fd);

    if (fork() == 0) {
        read(fd[0], buf, 50);
        printf("Child received: %s\n", buf);
    } else {
        write(fd[1], "Hello from parent", 18);
    }
    return 0;
}
```

## 4. CPU scheduling algorithms (FCFS, SJF, Priority, Round Robin)

`4_cpu_scheduling.c`

```c
#include <stdio.h>

int n;
int bt[20];

int id[20];

int pr[20];

void sort(int *key) {
    for (int i = 0; i < n; i++) id[i] = i;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (key && key[id[j]] < key[id[i]]) {
                int temp = id[i]; id[i] = id[j]; id[j] = temp;
            }
}

void show(char *name) {
    int w = 0, t, totalW = 0, totalT = 0;
    printf("\n%s\nP\tBT\tWT\tTAT\n", name);
    for (int i = 0; i < n; i++) {
        t = w + bt[id[i]];
        printf("P%d\t%d\t%d\t%d\n", id[i] + 1, bt[id[i]], w, t);
        totalW += w;
        totalT += t;
        w = t;
    }
    printf("Avg WT = %.2f  Avg TAT = %.2f\n", (float)totalW / n, (float)totalT / n);
}

void rr(int q) {
    int rem[20];
    int time = 0, done = 0, totalW = 0, totalT = 0;
    for (int i = 0; i < n; i++) rem[i] = bt[i];

    printf("\nRound Robin (q = %d)\nP\tWT\tTAT\n", q);
    while (done < n) {
        for (int i = 0; i < n; i++) {
            if (rem[i] > 0) {
                int run = rem[i] < q ? rem[i] : q;
                time += run;
                rem[i] -= run;
                if (rem[i] == 0) {
                    done++;
                    printf("P%d\t%d\t%d\n", i + 1, time - bt[i], time);
                    totalW += time - bt[i];
                    totalT += time;
                }
            }
        }
    }
    printf("Avg WT = %.2f  Avg TAT = %.2f\n", (float)totalW / n, (float)totalT / n);
}

int main() {
    int q;

    printf("Number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("P%d burst time: ", i + 1);
        scanf("%d", &bt[i]);
        printf("P%d priority: ", i + 1);
        scanf("%d", &pr[i]);
    }
    printf("Time quantum: ");
    scanf("%d", &q);

    sort(NULL); show("FCFS");
    sort(bt);   show("SJF");
    sort(pr);   show("Priority");
    rr(q);
    return 0;
}
```

## 5. Page replacement algorithms (FIFO, Optimal, LRU)

`5_page_replacement.c`

```c
#include <stdio.h>

int n;
int f;
int p[50];

int find(int fr[], int x) {
    for (int j = 0; j < f; j++)
        if (fr[j] == x) return j;
    return -1;
}

void fifo() {
    int fr[10], faults = 0, k = 0;
    for (int j = 0; j < f; j++) fr[j] = -1;

    for (int i = 0; i < n; i++) {
        if (find(fr, p[i]) == -1) {
            fr[k] = p[i];
            k = (k + 1) % f;
            faults++;
        }
    }
    printf("FIFO faults = %d\n", faults);
}

void optimal() {
    int fr[10], faults = 0;
    for (int j = 0; j < f; j++) fr[j] = -1;

    for (int i = 0; i < n; i++) {
        if (find(fr, p[i]) == -1) {
            int pos = 0, far = -1;
            for (int j = 0; j < f; j++) {
                int k = i + 1;
                while (k < n && p[k] != fr[j]) k++;
                if (fr[j] == -1) k = n + 1;
                if (k > far) { far = k; pos = j; }
            }
            fr[pos] = p[i];
            faults++;
        }
    }
    printf("Optimal faults = %d\n", faults);
}

void lru() {
    int fr[10], used[10], faults = 0;
    for (int j = 0; j < f; j++) { fr[j] = -1; used[j] = -1; }

    for (int i = 0; i < n; i++) {
        int pos = find(fr, p[i]);
        if (pos == -1) {
            pos = 0;
            for (int j = 1; j < f; j++)
                if (used[j] < used[pos]) pos = j;
            fr[pos] = p[i];
            faults++;
        }
        used[pos] = i;
    }
    printf("LRU faults = %d\n", faults);
}

int main() {

    printf("Number of pages: ");
    scanf("%d", &n);
    printf("Reference string: ");
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);
    printf("Number of frames: ");
    scanf("%d", &f);

    fifo();
    optimal();
    lru();
    return 0;
}
```
