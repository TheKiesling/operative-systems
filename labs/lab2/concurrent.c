/*************************************************
concurrent.c

Prints a count from 1 to 1,000,000 three times 
with for loops concurrently
*************************************************/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t id1, id2, id3;
    clock_t start, end;

    start = clock();

    id1 = fork(); // Create first child
    if (id1 == 0) {
        int id2 = fork(); // Create second child
        if (id2 == 0) {
            id3 = fork(); // Create third child
            if (id3 == 0) {
                for (int i = 0; i < 1000000; i++) {
                    printf("%d\n", i);
                }
            } 
            else {
                for (int i = 0; i < 1000000; i++) {
                    printf("%d\n", i);
                }
                wait(NULL);
            }
        }
        else {
            for (int i = 0; i < 1000000; i++) {
                printf("%d\n", i);
            }
            wait(NULL);
        }
    }
    else {
        wait(NULL);
        end = clock();
        double time_delta = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Time elapsed: %f\n", time_delta);
    }



    return 0;
}