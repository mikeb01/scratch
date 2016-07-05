#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>

typedef unsigned long long uint64;

const uint64 COUNT = 500LL * 1000LL * 1000LL;

volatile uint64 counter = 0;

void* run_xadd(void*)
{
    register uint64 value = counter;
    while (value < COUNT)
    {
        value = __sync_add_and_fetch(&counter, 1);
    }
}

void* run_cas(void*)
{
    register uint64 value = 0;

    while (value < COUNT)
    {
        do
        {
            value = counter;
        }
        while (!__sync_bool_compare_and_swap(&counter, value, value + 1));
    }
}

int main (int argc, char *argv[])
{
    const int NUM_THREADS = atoi(argv[1]);

    pthread_t threads[NUM_THREADS];
    void* status;

    timespec ts_start;
    timespec ts_finish;


    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, run_xadd, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], &status);
    }


    uint64 start = (ts_start.tv_sec * 1000000000LL) + ts_start.tv_nsec;
    uint64 finish = (ts_finish.tv_sec * 1000000000LL) + ts_finish.tv_nsec;
    uint64 duration = finish - start;

    std::cout << "threads = " << NUM_THREADS << std::endl;
    std::cout << "duration = " <<  duration << std::endl;
    std::cout << "ns per op = " << (duration / (COUNT * 2)) << std::endl;
    std::cout << "op/sec = " << ((COUNT * 2L * 1000L * 1000L * 1000L) / duration) << std::endl;
    std::cout << "counter = " <<  counter << std::endl;

    return 0;
}
