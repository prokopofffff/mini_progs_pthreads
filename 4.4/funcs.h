#ifndef FUNCTIONS
#define FUNCTIONS

#define INCORRECT_INPUT 1
#define FILE_OPEN_ERROR 2
#define DATA_TYPE_ERROR 3
#define ARRAY_SIZE_ERROR 4

#include <pthread.h>
#include <iostream>

struct thread_args{
    double* a;
    double* result;
    int n;
    int m;
    int p;
    pthread_t tid;
    //pthread_mutex_t* mutex;
    int status;
    int count;
    int begin;
    int end;
    double left1;
    double left2;
    double right1;
    double right2;
    int size;
};
void* change(void* args);

#endif