#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t chopstick[N];
sem_t room;

void *philosopher(void *num)
{
    int id = *(int *)num;

    while (1)
    {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        sem_wait(&room); // Allow only 4 philosophers

        sem_wait(&chopstick[id]);           // Pick left
        sem_wait(&chopstick[(id + 1) % N]); // Pick right

        printf("Philosopher %d is eating\n", id);
        sleep(2);

        sem_post(&chopstick[id]);           // Release left
        sem_post(&chopstick[(id + 1) % N]); // Release right

        sem_post(&room); // Leave room

        printf("Philosopher %d finished eating\n", id);
    }
}

int main()
{
    pthread_t ph[N];
    int id[N];

    sem_init(&room, 0, N - 1); // Only 4 allowed at a time

    for (int i = 0; i < N; i++)
        sem_init(&chopstick[i], 0, 1);

    for (int i = 0; i < N; i++)
    {
        id[i] = i;
        pthread_create(&ph[i], NULL, philosopher, &id[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(ph[i], NULL);

    for (int i = 0; i < N; i++)
        sem_destroy(&chopstick[i]);

    sem_destroy(&room);

    return 0;
}
