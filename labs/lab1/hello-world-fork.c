/*
hello-world-fork.c

This program demonstrates the fork() system call to create a new process.
*/

// Includes the libraries to have access to the functions of
#include <stdio.h> // printf
#include <unistd.h> // getpid and fork

int main() {
    // Call fork
    int f = fork();

    if (f == 0) {
        // Child process
        execl("./helloWorld", "./helloWorld", (char*)NULL);
        // The child process replaces its image with the "helloWorld" program.
    }
    else {
        // Parent process
        printf("%d\n", (int)getpid()); // Print process id
        execl("./helloWorld", "./helloWorld", (char*)NULL, (char*)NULL);
        // The parent process also replaces its image with the "helloWorld" program.
    }
    return 0; // Return 0 to indicate that the program has finished successfully
}
