/*************************************************
ipc.c

Write (and) read the content of a shared memory
and pass a message into it 
*************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define SHM_NAME "ipc_shm"
#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <x>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // Cast string to int
    char* x = (char *) argv[2]; // Pass to string
    int shm_fd;
    char *shm_ptr;

    printf("I'm %s\n", x);

    mkfifo("pipe", 0666); // Make a named pipe

    char buffer[80];

    // Open the shared memory object
    shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, 0666);

    if (shm_fd < 0) {

        // If the shared memory object already exists, open it
        if (errno == EEXIST) {
            printf("%s: Shared mem obj already created\n", x);
        }

        shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);

        // Open the pipe
        int pipefd = open("pipe", O_RDONLY);
        if (pipefd < 0) {
            perror("open");
            return 1;
        }

        // Read all the data from the pipe
        int nread = read(pipefd, buffer, 80);
        if (nread < 0) {
            perror("read");
            return 1;
        }

        close(pipefd);
        int fd_receive = atoi(buffer);
        printf("%s: Received shm fd: %d\n", x, fd_receive);

    } else {
        printf("%s: Created new shared mem obj %d\n", x, shm_fd);

        // Write the data to the pipe
        int pipefd = open("pipe", O_WRONLY);
        if (pipefd < 0) {
            perror("open");
            return 1;
        }
        char buffer[80];
        write(pipefd, buffer, 80);
        close(pipefd);
    }

    // Configure the size of the shared memory object
    ftruncate(shm_fd, SHM_SIZE);

    // Memory map the shared memory object
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    printf("%s: Ptr created with value %p\n", x, (void *)shm_ptr);

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        close(fd[1]);
        char buff[2];
        bool finished = false;
        int nbytes = 1;
        while(!finished && nbytes > 0) {
            nbytes = read(fd[0], buff, 2);
	    // If the char f is in the file, means that it's the end of it
            if (strcmp(buff, "f") == 0) { 
                finished = true;
            }
            else {
                int i = 0;
                while (*shm_ptr!= '\0') shm_ptr++;
                sprintf(shm_ptr, "%s", buff);
            }
            
        }
    } else {
	usleep(500000);
        close(fd[0]);
            for (int i = 0; i < 100; i++) {
                if(i % n == 0) {
                    write(fd[1], x, strlen(x) + 1);
                }
            }   
	    // Writes a f to indicates the end of the file
            write(fd[1], "f", strlen("f") + 1); 
            wait(NULL);
            printf("\n%s: Content in shared memory: %s\n", x, shm_ptr);
        }
        shm_unlink(SHM_NAME);
        munmap(shm_ptr, SHM_SIZE);

    return 0;
}
