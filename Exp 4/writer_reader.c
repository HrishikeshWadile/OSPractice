#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource; // controls access to shared data
sem_t readTry;  // allows writers to block readers
pthread_mutex_t rmutex, wmutex;

int readcount = 0;
int writecount = 0;
int shared_data = 0;

void *reader(void *arg)
{
    int id = *((int *)arg);

    sem_wait(&readTry); // check if writer is waiting
    pthread_mutex_lock(&rmutex);

    readcount++;
    if (readcount == 1)
        sem_wait(&resource); // first reader locks resource

    pthread_mutex_unlock(&rmutex);
    sem_post(&readTry);

    // 🔹 Critical section (reading)
    printf("Reader %d is reading data = %d\n", id, shared_data);
    sleep(1);

    pthread_mutex_lock(&rmutex);
    readcount--;
    if (readcount == 0)
        sem_post(&resource); // last reader releases resource
    pthread_mutex_unlock(&rmutex);

    printf("Reader %d finished reading\n", id);
    pthread_exit(NULL);
}

void *writer(void *arg)
{
    int id = *((int *)arg);

    pthread_mutex_lock(&wmutex);
    writecount++;
    if (writecount == 1)
        sem_wait(&readTry); // block new readers
    pthread_mutex_unlock(&wmutex);

    sem_wait(&resource); // exclusive access

    // 🔹 Critical section (writing)
    shared_data++;
    printf("Writer %d is writing data = %d\n", id, shared_data);
    sleep(2);

    printf("Writer %d finished writing\n", id);

    sem_post(&resource);

    pthread_mutex_lock(&wmutex);
    writecount--;
    if (writecount == 0)
        sem_post(&readTry); // allow readers again
    pthread_mutex_unlock(&wmutex);

    pthread_exit(NULL);
}

int main()
{
    pthread_t r[5], w[2];
    int rid[5], wid[2];

    sem_init(&resource, 0, 1);
    sem_init(&readTry, 0, 1);

    pthread_mutex_init(&rmutex, NULL);
    pthread_mutex_init(&wmutex, NULL);

    for (int i = 0; i < 5; i++)
    {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(r[i], NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    pthread_mutex_destroy(&rmutex);
    pthread_mutex_destroy(&wmutex);
    sem_destroy(&resource);
    sem_destroy(&readTry);

    return 0;
}