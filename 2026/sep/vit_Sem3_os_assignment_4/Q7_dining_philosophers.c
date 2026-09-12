#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

// Philosopher states
#define THINKING 0
#define HUNGRY   1
#define EATING   2

// Monitor structure
typedef struct {
    int state[N];
    pthread_mutex_t mutex;
    pthread_cond_t condition[N];
} DiningMonitor;

DiningMonitor monitor;

// Get left and right philosopher
#define LEFT(i)  ((i + N - 1) % N)
#define RIGHT(i) ((i + 1) % N)

// Check whether philosopher can eat
void test(int i)
{
    if (monitor.state[i] == HUNGRY &&
        monitor.state[LEFT(i)] != EATING &&
        monitor.state[RIGHT(i)] != EATING)
    {
        monitor.state[i] = EATING;
        pthread_cond_signal(&monitor.condition[i]);
    }
}

// Philosopher tries to pick up forks
void pickup_forks(int i)
{
    pthread_mutex_lock(&monitor.mutex);

    monitor.state[i] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", i + 1);

    test(i);

    while (monitor.state[i] != EATING)
        pthread_cond_wait(&monitor.condition[i], &monitor.mutex);

    printf("Philosopher %d picked up forks and is EATING\n", i + 1);

    pthread_mutex_unlock(&monitor.mutex);
}

// Philosopher puts down forks
void putdown_forks(int i)
{
    pthread_mutex_lock(&monitor.mutex);

    monitor.state[i] = THINKING;

    printf("Philosopher %d put down forks and is THINKING\n", i + 1);

    // Check whether neighbors can now eat
    test(LEFT(i));
    test(RIGHT(i));

    pthread_mutex_unlock(&monitor.mutex);
}

// Philosopher thread
void *philosopher(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 3; i++)
    {
        // Thinking
        printf("Philosopher %d is THINKING\n", id + 1);
        sleep(1);

        // Pick up forks
        pickup_forks(id);

        // Eating
        sleep(1);

        // Put down forks
        putdown_forks(id);
    }

    return NULL;
}

int main()
{
    pthread_t philosophers[N];
    int id[N];

    // Initialize monitor
    pthread_mutex_init(&monitor.mutex, NULL);

    for (int i = 0; i < N; i++)
    {
        monitor.state[i] = THINKING;
        pthread_cond_init(&monitor.condition[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++)
    {
        id[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &id[i]);
    }

    // Wait for all philosophers
    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    // Destroy monitor
    pthread_mutex_destroy(&monitor.mutex);

    for (int i = 0; i < N; i++)
        pthread_cond_destroy(&monitor.condition[i]);

    printf("\nAll philosophers have finished eating.\n");

    return 0;
}
