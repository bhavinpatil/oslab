#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int readerCount = 0;

void *writer(void *arg)
{
    sem_wait(&wrt);
    cnt = cnt * 2;
    printf("Writer %d modified count to %d \n", (*((int *)arg)), cnt);
    sem_post(&wrt);
}

void *reader(void *arg)

{
    pthread_mutex_lock(&mutex);

    readerCount++;

    if (readerCount == 1)
    {
        sem_wait(&wrt);
    }

    pthread_mutex_unlock(&mutex);

    printf("Reader %d read count as %d \n", *((int *)arg), cnt);

    pthread_mutex_lock(&mutex);
    readerCount--;
    if (readerCount == 0)
    {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_t read[10], write[5];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);

    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(read[i], NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
