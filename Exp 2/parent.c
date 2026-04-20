#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void sort(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int n, i;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter elements:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    sort(arr, n);

    pid_t pid = fork();

    if (pid == 0)
    {
        char *args[n + 3];

        args[0] = "./child";
        for (i = 0; i < n; i++)
        {
            args[i + 1] = malloc(10);
            sprintf(args[i + 1], "%d", arr[i]);
        }

        args[n + 1] = NULL;

        execve("./child", args, NULL);
        perror("execve failed");
    }
    else
    {
        wait(NULL);
    }

    return 0;
}