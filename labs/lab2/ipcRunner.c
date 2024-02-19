/*************************************************
ipcRunner.c

Runs the process with a and b as name. 
*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        execl("./ipc", "ipc", "11", "a", NULL);
        perror("execl");
    } else {
        usleep(500000);
        execl("./ipc", "ipc", "9", "b", NULL);
        perror("execl");
    }
    return 0;
}
