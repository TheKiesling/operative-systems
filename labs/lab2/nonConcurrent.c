/*************************************************
nonConcurrent.c

Prints a count from 1 to 1,000,000 three times 
with for loops secuently
*************************************************/

#include <stdio.h>
#include <time.h>

int main() {
    clock_t start, end;

    start = clock();
    for (int i = 0; i < 1000000; i++) {
        printf("%d\n", i);
    }
    for (int i = 0; i < 1000000; i++) {
        printf("%d\n", i);
    }
    for (int i = 0; i < 1000000; i++) {
        printf("%d\n", i);
    }
    end = clock();

    double time_delta = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time elapsed: %f\n", time_delta);
    
    return 0;
}