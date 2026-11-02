/* Program 5: Page Replacement - FIFO, Optimal, LRU

   HOW TO WRITE ONLY ONE ALGORITHM:
   Every part is labelled "NEEDED FOR: ...". Keep only the parts
   that list your algorithm and delete everything else.
     FIFO    -> globals, find(), fifo(),    main (with fifo() call only)
     OPTIMAL -> globals, find(), optimal(), main (with optimal() call only)
     LRU     -> globals, find(), lru(),     main (with lru() call only)

   All three follow the same steps:
     for each page:
       if it is already in a frame -> hit
       else -> page fault, choose a victim frame, put the page there
   Only the way the victim is chosen is different.
*/
#include <stdio.h>

/* ---------- NEEDED FOR: ALL ---------- */
int n;        /* number of pages in reference string */
int f;        /* number of frames */
int p[50];    /* reference string */


/* ============================================================
   find()  -  NEEDED FOR: ALL
   Returns the frame index holding page x, or -1 if not present.
   ============================================================ */
int find(int fr[], int x) {
    for (int j = 0; j < f; j++)
        if (fr[j] == x) return j;
    return -1;
}


/* ============================================================
   fifo()  -  NEEDED FOR: FIFO only
   Replaces the page that entered memory first.
   Pointer k moves round the frames in a circle.
   ============================================================ */
void fifo() {
    int fr[10], faults = 0, k = 0;
    for (int j = 0; j < f; j++) fr[j] = -1;         /* -1 means empty frame */

    for (int i = 0; i < n; i++) {
        if (find(fr, p[i]) == -1) {                 /* page fault */
            fr[k] = p[i];                           /* replace the oldest page */
            k = (k + 1) % f;                        /* move to next oldest */
            faults++;
        }
    }
    printf("FIFO faults = %d\n", faults);
}


/* ============================================================
   optimal()  -  NEEDED FOR: OPTIMAL only
   Replaces the page that will be used farthest in the future
   (or never used again).
   ============================================================ */
void optimal() {
    int fr[10], faults = 0;
    for (int j = 0; j < f; j++) fr[j] = -1;         /* -1 means empty frame */

    for (int i = 0; i < n; i++) {
        if (find(fr, p[i]) == -1) {                 /* page fault */
            int pos = 0, far = -1;
            for (int j = 0; j < f; j++) {
                int k = i + 1;                      /* look ahead for next use of fr[j] */
                while (k < n && p[k] != fr[j]) k++;
                if (fr[j] == -1) k = n + 1;         /* empty frame gets filled first */
                if (k > far) { far = k; pos = j; }
            }
            fr[pos] = p[i];                         /* replace farthest-used page */
            faults++;
        }
    }
    printf("Optimal faults = %d\n", faults);
}


/* ============================================================
   lru()  -  NEEDED FOR: LRU only
   Replaces the page that was used longest ago.
   used[j] stores the time (index) when frame j was last used.
   ============================================================ */
void lru() {
    int fr[10], used[10], faults = 0;
    for (int j = 0; j < f; j++) { fr[j] = -1; used[j] = -1; }

    for (int i = 0; i < n; i++) {
        int pos = find(fr, p[i]);
        if (pos == -1) {                            /* page fault */
            pos = 0;
            for (int j = 1; j < f; j++)             /* find least recently used frame */
                if (used[j] < used[pos]) pos = j;
            fr[pos] = p[i];
            faults++;
        }
        used[pos] = i;                              /* update last-used time (hit or fault) */
    }
    printf("LRU faults = %d\n", faults);
}


/* ============================================================
   main()  -  NEEDED FOR: ALL (keep only the call you need)
   ============================================================ */
int main() {
    /* input - NEEDED FOR: ALL */
    printf("Number of pages: ");
    scanf("%d", &n);
    printf("Reference string: ");
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);
    printf("Number of frames: ");
    scanf("%d", &f);

    /* run - keep only the line for your algorithm */
    fifo();                                         /* FIFO */
    optimal();                                      /* OPTIMAL */
    lru();                                          /* LRU */
    return 0;
}
