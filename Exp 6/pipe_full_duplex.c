#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define SIZE 1024

int main()
{
    int p1[2], p2[2];
    pid_t pid;
    char filename[100], buffer[SIZE];

    pipe(p1); // Parent → Child
    pipe(p2); // Child → Parent

    pid = fork();

    if (pid > 0)
    { // Parent
        close(p1[0]);
        close(p2[1]);

        printf("Enter file name: ");
        scanf("%s", filename);

        write(p1[1], filename, strlen(filename) + 1);

        read(p2[0], buffer, SIZE);
        printf("\nFile Contents:\n%s\n", buffer);

        close(p1[1]);
        close(p2[0]);
    }

    else
    { // Child
        close(p1[1]);
        close(p2[0]);

        read(p1[0], filename, 100);

        FILE *fp = fopen(filename, "r");
        if (fp == NULL)
        {
            strcpy(buffer, "File not found!");
        }
        else
        {
            fread(buffer, sizeof(char), SIZE, fp);
            fclose(fp);
        }

        write(p2[1], buffer, strlen(buffer) + 1);

        close(p1[0]);
        close(p2[1]);
    }

    return 0;
}