#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

int lock = 0;
int shared_data = 0;

/* Hardware synchronization construct: Test-and-Set */
int TestAndSet(int *target)
{
    int old = *target;
    *target = 1;
    return old;
}

/* Function executed by each process */
void *process(void *arg)
{
    int id = *(int *)arg;

    /* Entry Section */
    while (TestAndSet(&lock))
    {
        /* Busy waiting */
    }

    /* Critical Section */
    printf("Process %d entered the critical section.\n", id);

    shared_data++;
    printf("Process %d: Shared data = %d\n", id, shared_data);

    sleep(1);

    printf("Process %d leaving the critical section.\n", id);

    /* Exit Section */
    lock = 0;

    return NULL;
}

int main()
{
    pthread_t threads[N];
    int process_id[N];

    printf("Hardware Synchronization using Test-and-Set\n");
    printf("Number of processes: %d\n\n", N);

    /* Create N processes (threads) */
    for (int i = 0; i < N; i++)
    {
        process_id[i] = i + 1;
        pthread_create(&threads[i], NULL, process, &process_id[i]);
    }

    /* Wait for all processes to finish */
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nFinal value of shared data = %d\n", shared_data);

    return 0;
}
