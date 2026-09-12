#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PROCESSES 10

sem_t mutex;                 // Binary semaphore
int shared_data = 0;
int n;

/* Function executed by each process/thread */
void *process(void *arg)
{
    int id = *(int *)arg;

    printf("Process %d is waiting to enter the critical section.\n", id);

    // Wait (P operation)
    sem_wait(&mutex);

    // Critical Section
    printf("Process %d entered the critical section.\n", id);

    shared_data++;
    printf("Process %d: Shared data = %d\n", id, shared_data);

    sleep(1);

    printf("Process %d leaving the critical section.\n", id);

    // Signal (V operation)
    sem_post(&mutex);

    free(arg);
    return NULL;
}

int main()
{
    pthread_t threads[MAX_PROCESSES];

    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
    scanf("%d", &n);

    if (n < 1 || n > MAX_PROCESSES)
    {
        printf("Invalid number of processes.\n");
        return 1;
    }

    /*
     * Initialize binary semaphore.
     * Initial value = 1 means the critical section is free.
     */
    sem_init(&mutex, 0, 1);

    // Create N processes (implemented using threads)
    for (int i = 0; i < n; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;

        if (pthread_create(&threads[i], NULL, process, id) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }

    // Wait for all processes to finish
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);

    printf("\nFinal value of shared data = %d\n", shared_data);

    return 0;
}
