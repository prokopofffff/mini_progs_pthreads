#ifndef FUNCTIONS
#define FUNCTIONS

#include <pthread.h>
#include <iostream>
#include <ctime>
#include <cmath>

struct thread_args{
    int p;
    int index;
    unsigned long long int n;
    unsigned long long int start;
    unsigned long long int locCount;
    unsigned long long int stepCount;
    unsigned long long int count;
    unsigned long long int res;
    int step;
    int status;
    double time;
    // pthread_mutex_t* mutex;
};

int isPrime(unsigned long long int n);

void* find(void* args);

void ReduceSum(int p, unsigned long long int* a = nullptr, int n = 0);

void ReduceSum(int p, int* a = nullptr, int n = 0);

#endif