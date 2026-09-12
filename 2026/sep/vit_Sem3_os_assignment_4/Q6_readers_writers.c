#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int shared_data = 0;
int read_count = 0;

sem_t mutex;   // Protects read_count
sem_t wrt;     // Controls access to shared_data

// Reader function
void *reader(void *arg)
{
    int id = *(int *)arg;

    // Entry section
    sem_wait(&mutex);
    read_count++;

    if (read_count == 1)
        sem_wait(&wrt);     // First reader blocks writers

    sem_post(&mutex);

    // Critical section
    printf("Reader %d is reading shared data = %d\n",
           id, shared_data);
    sleep(1);

    // Exit section
    sem_wait(&mutex);
    read_count--;

    if (read_count == 0)
        sem_post(&wrt);     // Last reader allows writers

    sem_post(&mutex);

    return NULL;
}

// Writer function
void *writer(void *arg)
{
    int id = *(int *)arg;

    // Entry section
    sem_wait(&wrt);

    // Critical section
    shared_data++;
    printf("Writer %d is writing shared data = %d\n",
           id, shared_data);
    sleep(1);

    // Exit section
    sem_post(&wrt);

    return NULL;
}

int main()
{
    int n_readers, n_writers;

    printf("Enter number of readers: ");
    scanf("%d", &n_readers);

    printf("Enter number of writers: ");
    scanf("%d", &n_writers);

    pthread_t readers[n_readers];
    pthread_t writers[n_writers];

    int reader_id[n_readers];
    int writer_id[n_writers];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create writer threads
    for (int i = 0; i < n_writers; i++)
    {
        writer_id[i] = i + 1;
        pthread_create(&writers[i], NULL, writer,
                       &writer_id[i]);
    }

    // Create reader threads
    for (int i = 0; i < n_readers; i++)
    {
        reader_id[i] = i + 1;
        pthread_create(&readers[i], NULL, reader,
                       &reader_id[i]);
    }

    // Wait for reader threads
    for (int i = 0; i < n_readers; i++)
        pthread_join(readers[i], NULL);

    // Wait for writer threads
    for (int i = 0; i < n_writers; i++)
        pthread_join(writers[i], NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
