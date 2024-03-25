#ifndef FUNCTIONS
#define FUNCTIONS

#define INCORRECT_INPUT 1
#define FILE_OPEN_ERROR 2
#define DATA_TYPE_ERROR 3
#define ARRAY_SIZE_ERROR 4
#define COUNT_THREADS_ERROR 5

#include <pthread.h>
#include <iostream>

struct thread_args{
    double* a;
    int n1;
    int n2;
    int m;
    int p;
    pthread_t tid;
    pthread_mutex_t* mutex;
    int status;
    int count;
    double sum;
    int begin;
    int end;
    int size;
};

void* count(void* args);

void* change(void* args);

#endif