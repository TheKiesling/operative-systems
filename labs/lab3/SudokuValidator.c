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

// Global array to store the Sudoku board
int board[9][9];

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

    // Print the Sudoku board
    printf("Sudoku board:\n");
    for (int i = 0; i < 9; i++) {
        printf(" ");
        for (int j = 0; j < 9; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    // Clean up
    munmap(file, sb.st_size);
    close(fd);

    return 0;
}