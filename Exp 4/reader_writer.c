#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt;
pthread_mutex_t mutex;

int readcount = 0;
int shared_data = 0;

void *reader(void *arg)
{
    int id = *((int *)arg);

    pthread_mutex_lock(&mutex);
    readcount++;
    if (readcount == 1)
    {
        sem_wait(&wrt); // First reader locks writer
    }
    pthread_mutex_unlock(&mutex);

    printf("Reader %d is reading data = %d\n", id, shared_data);
    sleep(1);

    pthread_mutex_lock(&mutex);
    readcount--;
    if (readcount == 0)
    {
        sem_post(&wrt); // Last reader releases writer
    }
    pthread_mutex_unlock(&mutex);

    printf("Reader %d finished reading\n", id);
    pthread_exit(NULL);
}
void *writer(void *arg)
{
    int id = *((int *)arg);

    sem_wait(&wrt); // Writer gets exclusive access

    shared_data++;
    printf("Writer %d is writing data = %d\n", id, shared_data);
    sleep(2);

    printf("Writer %d finished writing\n", id);

    sem_post(&wrt);

    pthread_exit(NULL);
}

int main()
{
    pthread_t r[5], w[2];
    int rid[5], wid[2];

    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

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

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}