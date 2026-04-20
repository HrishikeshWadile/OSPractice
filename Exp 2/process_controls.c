#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    printf("Enter number of integers: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter integers:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    pid_t pid = fork();

    if (pid > 0)
    { // Parent
        printf("\nParent Process (PID: %d)\n", getpid());

        sort(arr, n);

        printf("Sorted Array:\n");
        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);

        printf("\nWaiting for child (Zombie demonstration)...\n");
        sleep(10);  // Child exits before wait -> Zombie state
        wait(NULL); // Removes zombie

        printf("Child cleaned using wait().\n");
    }

    else if (pid == 0)
    { // Child
        printf("\nChild Process (PID: %d)\n", getpid());
        printf("Parent PID: %d\n", getppid());

        printf("Child exiting...\n");
        exit(0);
    }

    else
    {
        printf("Fork failed!\n");
    }

    return 0;
}