#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define SIZE 1024

int main()
{
    int fd1, fd2;
    char input[SIZE], result[SIZE];
    int chars = 0, words = 0, lines = 0;
    fd1 = open("fifo1", O_RDONLY);
    fd2 = open("fifo2", O_WRONLY);

    read(fd1, input, SIZE);

    for (int i = 0; input[i] != '\0'; i++)
    {
        chars++;
        if (input[i] == ' ')
            words++;
        if (input[i] == '\n')
            lines++;
    }

    words++; // Last word

    FILE *fp = fopen("output.txt", "w");
    fprintf(fp, "Characters: %d\nWords: %d\nLines: %d\n", chars, words, lines);
    fclose(fp);

    fp = fopen("output.txt", "r");
    fread(result, sizeof(char), SIZE, fp);
    fclose(fp);

    write(fd2, result, strlen(result) + 1);

    close(fd1);
    close(fd2);

    return 0;
}