/***********************************************
fork.c

This program create 4 fork in a for loop
************************************************/

#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 4; i++) {
        fork();
    }
    printf("Proceso %d \n", (int)getpid());
    return 0;
}