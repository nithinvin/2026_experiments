#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int flag[2] = {0, 0};
int turn;

// Peterson's algorithm
void enter_critical_section(int process)
{
    int other = 1 - process;

    flag[process] = 1;
    turn = other;

    while (flag[other] && turn == other)
        ;   // Busy wait
}

void exit_critical_section(int process)
{
    flag[process] = 0;
}

void *process(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++)
    {
        // Entry section
        enter_critical_section(id);

        // Critical section
        printf("Process %d is in the critical section\n", id);
        sleep(1);

        // Exit section
        exit_critical_section(id);

        // Remainder section
        printf("Process %d is in the remainder section\n", id);
        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t p0, p1;
    int id0 = 0, id1 = 1;

    pthread_create(&p0, NULL, process, &id0);
    pthread_create(&p1, NULL, process, &id1);

    pthread_join(p0, NULL);
    pthread_join(p1, NULL);

    return 0;
}
