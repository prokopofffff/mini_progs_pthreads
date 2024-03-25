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
    int n;
    int m;
    int p;
    pthread_t tid;
    pthread_mutex_t* mutex;
    int status;
    int count;
    int begin;
    int end;
    double left;
    double right;
    double left_q;
    double right_q;
    int left_count;
    int right_count;
    double left_sum;
    double right_sum;
    int size;
};
void* change(void* args);

#endif