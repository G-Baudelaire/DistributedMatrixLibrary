//
// Created by Germain Jones on 03/11/2023.
//

#include <pthread.h>
#include <stdio.h>

const int NUM_THREADS = 5;

struct number
{
    int uniqueNumber;
};

void* showUniqueNumber(void* numberStruct);

int main()
{
    pthread_t threadID[NUM_THREADS];
    struct number numbers[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        numbers[i].uniqueNumber = i;
        pthread_create(&threadID[i], NULL, showUniqueNumber, numbers + i);
    }

    for(int i=0; i < NUM_THREADS; i++)
    {
        pthread_join(threadID[i], NULL);
    }

    return 0;
}

void* showUniqueNumber(void* numberStruct)
{
    struct number* threadPtrN = numberStruct;

    printf("%i\n", threadPtrN->uniqueNumber);

    return (void*)0;
}
