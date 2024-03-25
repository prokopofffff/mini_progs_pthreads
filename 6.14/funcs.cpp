#include "funcs.h"

#define EPSILON 1.0e-15


void* count(void* args){
    thread_args* a = (thread_args*) args;
    if(a->size > 0){
        //pthread_mutex_lock(a->mutex);
        int i = 0, j = 0;
        for(int k = a->begin; k <= a->end; k++){
            i = k / a->n2;
            j = k % a->n2;
            if(i == 0 || i == (a->n1 - 1) || j == (a->n2 - 1)) continue;
            if(a->a[i * a->n2 + j] >= (a->a[(i + 1) * a->n2 + j] + a->a[(i - 1) * a->n2 + j] + a->a[(i + 1) * a->n2 + (j + 1)] + a->a[(i - 1) * a->n2 + (j + 1)] + a->a[i * a->n2 + (j + 1)] - 5 * a->a[i * a->n2 + j])){
                a->count++;
                a->sum += a->a[i * a->n2 + j];
            }
        }
        //pthread_mutex_unlock(a->mutex);
    }
    return 0;
}

void* change(void* args){
    thread_args* a = (thread_args*) args;
    if(a->size > 0){
        int i = 0, j = 0;
        for(int k = a->begin; k <= a->end; k++){
            i = k / a->n2;
            j = k % a->n2;
            if(i == 0 || i == (a->n1 - 1) || j == (a->n2 - 1)) continue;
            if(a->a[i * a->n2 + j] >= (a->a[(i + 1) * a->n2 + j] + a->a[(i - 1) * a->n2 + j] + a->a[(i + 1) * a->n2 + (j + 1)] + a->a[(i - 1) * a->n2 + (j + 1)] + a->a[i * a->n2 + (j + 1)] - 5 * a->a[i * a->n2 + j])){
                a->a[i * a->n2 + j] = a->sum;
            }
        }
    }
    return 0;
}