#ifndef FUNCTIONS
#define FUNCTIONS

#define FILE_OPEN_ERROR -1
#define DATA_TYPE_ERROR -2

#include <pthread.h>

struct thread_args{
    char* filename;
    int m;
    int p;
    int min;
    int count;
    int status;
    pthread_t tid;
};

void* read(void* args);

void* find(void* args);

#endif