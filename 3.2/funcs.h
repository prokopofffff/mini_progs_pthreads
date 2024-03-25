#ifndef FUNCTIONS
#define FUNCTIONS

#define FILE_OPEN_ERROR -1
#define DATA_TYPE_ERROR -2

#include <pthread.h>

struct thread_args{
    char* filename;
    int m;
    int p;
    double first;
    double last;
    double prelast;
    double second;
    int size;
    int count;
    double sum;
    int status;
    pthread_t tid;
};

void* read(void* args);

void* recount(void* args);

#endif