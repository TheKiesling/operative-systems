/*
hello-world.c

This program prints "Hello World" and the process id of the program.
*/

// Includes the libraries to have access to the functions of
#include <stdio.h> // printf
#include <unistd.h> // getpid

int main() {
    printf("Hello World\n"); // print hello world
    printf("%d\n", (int)getpid()); // print process id
    return 0; // return 0 to indicate that the program has finished successfully
}