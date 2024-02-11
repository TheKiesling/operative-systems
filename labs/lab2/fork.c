/***********************************************
fork.c

This program create 4 fork consecutively
************************************************/

#include <stdio.h>
#include <unistd.h>

int main() {
    fork();
    fork();
    fork();
    fork();
    printf("Proceso %d \n", (int)getpid());
    return 0;
}

