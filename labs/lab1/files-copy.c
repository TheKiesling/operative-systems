/*
files-copy.c

open a file in read mode and copy its contents to another file
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fileOriginal = fopen(argv[1], "r");
    FILE *fileCopy = fopen(argv[2], "w");
    
    // Read the original file and copy it to the new file
    char c;
    while ((c = fgetc(fileOriginal)) != EOF){
        fputc(c, fileCopy);
    }

    // Close the files
    fclose(fileOriginal);
    fclose(fileCopy);

    return 0;
}