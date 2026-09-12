#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

// Semaphores
sem_t empty;     // Counts empty slots
sem_t full;      // Counts filled slots
sem_t mutex;     // Provides mutual exclusion

// Producer function
void *producer(void *arg)
{
    int item;

    for (item = 1; item <= 10; item++)
    {
        // Wait for an empty slot
        sem_wait(&empty);

        // Enter critical section
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Producer produced: %d\n", item);

        in = (in + 1) % BUFFER_SIZE;

        // Leave critical section
        sem_post(&mutex);

        // Increase count of full slots
        sem_post(&full);

        sleep(1);
    }

    return NULL;
}

// Consumer function
void *consumer(void *arg)
{
    int item;

    for (int i = 1; i <= 10; i++)
    {
        // Wait for a filled slot
        sem_wait(&full);

        // Enter critical section
        sem_wait(&mutex);

        item = buffer[out];
        printf("Consumer consumed: %d\n", item);

        out = (out + 1) % BUFFER_SIZE;

        // Leave critical section
        sem_post(&mutex);

        // Increase count of empty slots
        sem_post(&empty);

        sleep(2);
    }

    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores
    // Initially, all BUFFER_SIZE slots are empty
    sem_init(&empty, 0, BUFFER_SIZE);

    // Initially, no items are available
    sem_init(&full, 0, 0);

    // Binary semaphore for mutual exclusion
    sem_init(&mutex, 0, 1);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
