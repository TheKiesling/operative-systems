/***********************************************************

Validates a 9x9 Sudoku board.

On each row, column, and 3x3 subgrid, the digits 1-9 
must appear exactly once.

This program is a parrallel implementation of the Sudoku

***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/syscall.h>

// Global array to store the Sudoku board
#define SIZE 9
int board[SIZE][SIZE];
bool valid = false;

void* checkColumns(void *arg) {
    long thread_id = syscall(SYS_gettid);
    printf("El thread que ejecuta el método para ejecutar el metodo de revision de Columnas es: %lu\n", (unsigned long)thread_id);
    for (int i = 0; i < SIZE; i++) {
        int check[SIZE] = {0};
        printf("En la revision de la columna %d el siguiente es un thread en ejecucion: %lu\n", i, (unsigned long)thread_id);
        for (int j = 0; j < SIZE; j++) {
            if (board[j][i] < 1 || board[j][i] > 9 || check[board[j][i] - 1] == 1) {
                valid = false;
                pthread_exit(NULL);
            }
            check[board[j][i] - 1] = 1;
        }
    }
    valid = true;
    pthread_exit(NULL);
}

void* checkRows(void *arg) {
    long thread_id = syscall(SYS_gettid);
    printf("El thread que ejecuta el método para ejecutar el metodo de revision de Filas es: %lu\n", (unsigned long)thread_id);
    for (int i = 0; i < 9; i++) {
        int check[9] = {0};
        printf("En la revision de la fila %d el siguiente es un thread en ejecucion: %lu\n", i, (unsigned long)thread_id);
        for (int j = 0; j < 9; j++) {
            if (board[i][j] < 1 || board[i][j] > 9 || check[board[i][j] - 1] == 1) {
                valid = false;
                pthread_exit(NULL);
            }
            check[board[i][j] - 1] = 1;
        }
    }
    valid = true;
    pthread_exit(NULL);
}

void *checkSubGrids(void *arg) {
    long thread_id = syscall(SYS_gettid);
    printf("El thread que ejecuta el método para ejecutar el metodo de revision de SubGrids es: %lu\n", (unsigned long)thread_id);
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            int check[9] = {0};
            printf("En la revision de la grid %dx%d el siguiente es un thread en ejecucion: %lu\n", i, j, (unsigned long)thread_id);
            for (int k = i; k < i + 3; k++) {
                for (int l = j; l < j + 3; l++) {
                    if (board[k][l] < 1 || board[k][l] > 9 || check[board[k][l] - 1] == 1) {
                        valid = false;
                        pthread_exit(NULL);
                    }
                    check[board[k][l] - 1] = 1;
                }
            }
        }
    }
    valid = true;
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

    // Check if the input file is provided
    if (argc != 2) {
        printf("Usage: ./SudokuValidator <input file>\n");
        return 1;
    }

    // Open the file
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Get the file size
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }

    // Map the file to memory
    char *file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return 1;
    }

    // Copy each symbol from the solution file to the board array
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int index = i * 9 + j; // Calculate the correct index in the string
            if (index < sb.st_size) {
                // Convert char to int and store in board
                board[i][j] = file[index] - '0';
            }
        }
    }

    pthread_t threadColumns, threadRows, threadSubGrids;
    pid_t pid, wpid;
    int status = 0;

    pid_t parent_pid = getpid();

    if ((pid = fork()) == 0) {
        char proc_id[10];
        sprintf(proc_id, "%d", parent_pid);
        printf("El thread en el que se ejecuta main es : %d\n", parent_pid);
        execlp("ps", "ps", "-p", proc_id, "-lLf", NULL);
        exit(EXIT_FAILURE);
    } else if (pid > 0) { 
        pthread_create(&threadColumns, NULL, checkColumns, NULL);
        pthread_join(threadColumns, NULL);

        pthread_create(&threadRows, NULL, checkRows, NULL);
        pthread_join(threadRows, NULL);

        pthread_create(&threadSubGrids, NULL, checkSubGrids, NULL);
        pthread_join(threadSubGrids, NULL);

        while ((wpid = wait(&status)) > 0);

        if (valid) printf("Sudoku resuelto!.\n");

        printf("Antes de terminar el estado de este proceso y sus threads es: \n");

        if ((pid = fork()) == 0) {
            char proc_id[10];
            sprintf(proc_id, "%d", parent_pid);
            execlp("ps", "ps", "-p", proc_id, "-lLf", NULL);
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            while ((wpid = wait(&status)) > 0);
        }
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    // Clean up
    munmap(file, sb.st_size);
    close(fd);

    return 0;
}
