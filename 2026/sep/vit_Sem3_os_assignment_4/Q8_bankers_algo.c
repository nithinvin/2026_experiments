#include <stdio.h>

#define P 3       // Number of processes
#define R 3       // Number of resources

// Function to check whether the system is in a safe state
int isSafe(int allocation[P][R], int need[P][R], int available[R])
{
    int work[R];
    int finish[P] = {0};
    int safeSequence[P];
    int count = 0;

    // Work = Available
    for (int i = 0; i < R; i++)
        work[i] = available[i];

    while (count < P)
    {
        int found = 0;

        for (int i = 0; i < P; i++)
        {
            if (finish[i] == 0)
            {
                int j;

                // Check if Need[i] <= Work
                for (j = 0; j < R; j++)
                {
                    if (need[i][j] > work[j])
                        break;
                }

                // If process can finish
                if (j == R)
                {
                    // Assume process finishes and releases resources
                    for (j = 0; j < R; j++)
                        work[j] += allocation[i][j];

                    safeSequence[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        // No process can proceed
        if (found == 0)
        {
            printf("\nSystem is NOT in a safe state.\n");
            return 0;
        }
    }

    printf("\nSystem is in a SAFE state.\n");

    printf("Safe sequence: ");
    for (int i = 0; i < P; i++)
    {
        printf("P%d", safeSequence[i] + 1);

        if (i != P - 1)
            printf(" -> ");
    }

    printf("\n");

    return 1;
}

int main()
{
    // Total resources:
    // Database connections = 10
    // Printers = 3
    // CPU cycles = 4

    int total[R] = {10, 3, 4};

    // Current allocation
    int allocation[P][R] = {
        {2, 0, 0},   // P1
        {0, 1, 0},   // P2
        {3, 0, 0}    // P3
    };

    // Current requests / Need
    int need[P][R] = {
        {0, 1, 0},   // P1 requests 1 Printer
        {3, 0, 0},   // P2 requests 3 Database connections
        {0, 1, 0}    // P3 requests 1 Printer
    };

    int available[R];

    // Calculate Available = Total - Allocation
    for (int j = 0; j < R; j++)
    {
        int allocated = 0;

        for (int i = 0; i < P; i++)
            allocated += allocation[i][j];

        available[j] = total[j] - allocated;
    }

    printf("BANKER'S ALGORITHM\n");
    printf("------------------\n");

    printf("\nTotal Resources:\n");
    printf("Database Connections = %d\n", total[0]);
    printf("Printers             = %d\n", total[1]);
    printf("CPU Cycles           = %d\n", total[2]);

    printf("\nCurrently Available:\n");
    printf("Database Connections = %d\n", available[0]);
    printf("Printers             = %d\n", available[1]);
    printf("CPU Cycles           = %d\n", available[2]);

    printf("\nChecking whether granting the requests is safe...\n");

    isSafe(allocation, need, available);

    return 0;
}

