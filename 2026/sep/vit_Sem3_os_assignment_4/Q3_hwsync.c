#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

volatile int lock = 0;
int shared_data = 0;

/* Process function */
void *process(void *arg)
{
    int id = *(int *)arg;

    /* Entry Section */
    while (__sync_lock_test_and_set(&lock, 1))
    {
        /* Busy waiting */
    }

    /* Critical Section */
    printf("Process %d entered the critical section.\n", id);

    shared_data++;
    printf("Process %d: Shared data = %d\n", id, shared_data);

    sleep(1);

    printf("Process %d leaving the critical section.\n\n", id);

    /* Exit Section */
    __sync_lock_release(&lock);

    return NULL;
}

int main()
{
    pthread_t threads[N];
    int process_id[N];

    printf("Hardware Synchronization using Test-and-Set\n");
    printf("Number of processes: %d\n\n", N);

    /* Create N processes */
    for (int i = 0; i < N; i++)
    {
        process_id[i] = i + 1;

        if (pthread_create(&threads[i], NULL, process,
                           &process_id[i]) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }

    /* Wait for all processes */
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Final value of shared data = %d\n", shared_data);

    return 0;
}
