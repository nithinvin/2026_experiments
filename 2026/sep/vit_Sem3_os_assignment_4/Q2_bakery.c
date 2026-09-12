#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define N 5

bool choosing[N];
int number[N];

void lock(int process)
{
    int max_number = 0;

    // Process chooses a number
    choosing[process] = true;

    for (int i = 0; i < N; i++) {
        if (number[i] > max_number)
            max_number = number[i];
    }

    number[process] = max_number + 1;
    choosing[process] = false;

    // Wait until it is this process's turn
    for (int i = 0; i < N; i++) {

        if (i == process)
            continue;

        // Wait while process i is choosing a number
        while (choosing[i])
            ;

        // Wait if process i has a smaller ticket number
        // or the same number but smaller process ID
        while (number[i] != 0 &&
              (number[i] < number[process] ||
              (number[i] == number[process] && i < process)))
            ;
    }
}

void unlock(int process)
{
    number[process] = 0;
}

void *process(void *arg)
{
    int id = *(int *)arg;

    printf("Process %d is trying to enter the critical section.\n", id);

    lock(id);

    // Critical Section
    printf("Process %d entered the critical section.\n", id);

    printf("Process %d is executing in the critical section.\n", id);

    printf("Process %d is leaving the critical section.\n", id);

    unlock(id);

    return NULL;
}

int main()
{
    pthread_t threads[N];
    int process_id[N];

    // Initialize arrays
    for (int i = 0; i < N; i++) {
        choosing[i] = false;
        number[i] = 0;
    }

    // Create N processes (threads)
    for (int i = 0; i < N; i++) {
        process_id[i] = i;
        pthread_create(&threads[i], NULL, process, &process_id[i]);
    }

    // Wait for all processes
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
