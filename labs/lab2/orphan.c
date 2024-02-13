/*************************************************
orphan.c

Creates a child that will die first than his
parent
*************************************************/
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int main() {
    pid_t id1;
    id1 = fork();
    
    if (id1 == 0) {
        for (int i = 0; i < 4000000; i++) {
            printf("%d\n", i);
        }
    }
    else {
        while(true);
    }

    return 0;
}