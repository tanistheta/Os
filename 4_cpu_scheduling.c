/* Program 4: CPU Scheduling - FCFS, SJF, Priority, Round Robin
   Assumption: all processes arrive at time 0.
   Lower priority number = higher priority.

   HOW TO WRITE ONLY ONE ALGORITHM:
   Every part is labelled "NEEDED FOR: ...". Keep only the parts
   that list your algorithm and delete everything else.
     FCFS     -> n, bt[], id[], sort(), show(), input, FCFS line
     SJF      -> n, bt[], id[], sort(), show(), input, SJF line
     PRIORITY -> n, bt[], id[], pr[], sort(), show(), input + priority input, Priority line
     RR       -> n, bt[], rr(), input + quantum input, RR line
*/
#include <stdio.h>

/* ---------- NEEDED FOR: ALL ---------- */
int n;            /* number of processes */
int bt[20];       /* burst time of each process */

/* ---------- NEEDED FOR: FCFS, SJF, PRIORITY ---------- */
int id[20];       /* order in which processes will run */

/* ---------- NEEDED FOR: PRIORITY ---------- */
int pr[20];       /* priority of each process */


/* ============================================================
   sort()  -  NEEDED FOR: FCFS, SJF, PRIORITY
   Decides the order in which processes run.
     sort(NULL) -> input order          (FCFS)
     sort(bt)   -> shortest burst first (SJF)
     sort(pr)   -> highest priority first (PRIORITY)
   For FCFS only, you can keep just the first for-loop.
   ============================================================ */
void sort(int *key) {
    for (int i = 0; i < n; i++) id[i] = i;          /* start with input order */
    for (int i = 0; i < n; i++)                     /* bubble sort on id[] */
        for (int j = i + 1; j < n; j++)
            if (key && key[id[j]] < key[id[i]]) {
                int temp = id[i]; id[i] = id[j]; id[j] = temp;
            }
}


/* ============================================================
   show()  -  NEEDED FOR: FCFS, SJF, PRIORITY
   Runs processes in the order stored in id[], calculates
   waiting time (WT) and turnaround time (TAT), prints averages.
   Rule: each process waits for all processes before it.
   ============================================================ */
void show(char *name) {
    int w = 0, t, totalW = 0, totalT = 0;
    printf("\n%s\nP\tBT\tWT\tTAT\n", name);
    for (int i = 0; i < n; i++) {
        t = w + bt[id[i]];                          /* TAT = WT + BT */
        printf("P%d\t%d\t%d\t%d\n", id[i] + 1, bt[id[i]], w, t);
        totalW += w;
        totalT += t;
        w = t;                                      /* next one waits till this ends */
    }
    printf("Avg WT = %.2f  Avg TAT = %.2f\n", (float)totalW / n, (float)totalT / n);
}


/* ============================================================
   rr()  -  NEEDED FOR: RR only
   Goes round the processes again and again, giving each one
   at most q units of time per turn, until all are finished.
   ============================================================ */
void rr(int q) {
    int rem[20];                                    /* remaining burst time */
    int time = 0, done = 0, totalW = 0, totalT = 0;
    for (int i = 0; i < n; i++) rem[i] = bt[i];

    printf("\nRound Robin (q = %d)\nP\tWT\tTAT\n", q);
    while (done < n) {
        for (int i = 0; i < n; i++) {
            if (rem[i] > 0) {
                int run = rem[i] < q ? rem[i] : q;  /* run for q or what is left */
                time += run;
                rem[i] -= run;
                if (rem[i] == 0) {                  /* process finished */
                    done++;
                    printf("P%d\t%d\t%d\n", i + 1, time - bt[i], time); /* WT = TAT - BT */
                    totalW += time - bt[i];
                    totalT += time;
                }
            }
        }
    }
    printf("Avg WT = %.2f  Avg TAT = %.2f\n", (float)totalW / n, (float)totalT / n);
}


/* ============================================================
   main()  -  NEEDED FOR: ALL (delete the lines you don't need)
   ============================================================ */
int main() {
    int q;                                          /* NEEDED FOR: RR */

    /* input - NEEDED FOR: ALL */
    printf("Number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("P%d burst time: ", i + 1);
        scanf("%d", &bt[i]);
        printf("P%d priority: ", i + 1);            /* NEEDED FOR: PRIORITY */
        scanf("%d", &pr[i]);                        /* NEEDED FOR: PRIORITY */
    }
    printf("Time quantum: ");                       /* NEEDED FOR: RR */
    scanf("%d", &q);                                /* NEEDED FOR: RR */

    /* run - keep only the line for your algorithm */
    sort(NULL); show("FCFS");                       /* FCFS */
    sort(bt);   show("SJF");                        /* SJF */
    sort(pr);   show("Priority");                   /* PRIORITY */
    rr(q);                                          /* RR */
    return 0;
}
