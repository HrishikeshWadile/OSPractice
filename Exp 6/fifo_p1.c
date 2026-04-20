#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define SIZE 1024

int main()
{
    int fd1, fd2;
    char input[SIZE], output[SIZE];

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    fd1 = open("fifo1", O_WRONLY);
    fd2 = open("fifo2", O_RDONLY);

    printf("Enter sentences (Ctrl+D to stop):\n");
    fgets(input, SIZE, stdin);

    write(fd1, input, strlen(input) + 1);

    read(fd2, output, SIZE);
    printf("\nResult from Process 2:\n%s\n", output);

    close(fd1);
    close(fd2);

    return 0;
}