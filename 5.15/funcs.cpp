#include "funcs.h"

#define EPSILON 1.0e-15

void* change(void* args){
    thread_args* a = (thread_args*) args;
    if(a->size > 0){
        if(a->size == 1 && a->begin == 0) {
            a->right_count = 1;
            a->right_sum = a->a[0];
            return 0;
        }
        //pthread_mutex_lock(a->mutex);
        double* b = new double[a->size];
        double sum = 0;
        int count = 2;
        for(int j = 0; j < a->size; j++){
            b[j] = a->a[a->begin + j];
        }
        if(a->size == 1){
            int j = a->begin;
            if(a->begin > 0 && a->end < (a->n - 1) && (a->a[j] - a->a[j - 1]) > (a->a[j + 1] - a->a[j] - EPSILON) && (a->n - 1) && (a->a[j] - a->a[j - 1]) < (a->a[j + 1] - a->a[j] + EPSILON)){
                a->left_count++;
                a->left_sum = a->a[a->begin];
                a->right_count++;
                a->right_sum = a->a[a->begin];
            }
            else if(a->begin > 1 && (a->a[j] - a->a[j - 1]) > (a->a[j - 1] - a->a[j - 2] - EPSILON) && (a->a[j] - a->a[j - 1]) < (a->a[j - 1] - a->a[j - 2] + EPSILON)){
                a->left_count++;
                a->left_sum = a->a[a->begin];
            }
            else if(a->end < a->n - 2 && (a->a[j + 1] - a->a[j]) > (a->a[j + 2] - a->a[j + 1] - EPSILON) && (a->a[j + 1] - a->a[j]) < (a->a[j + 2] - a->a[j + 1] + EPSILON)){
                a->right_count++;
                a->right_sum = a->a[a->begin];
            }
            //pthread_mutex_unlock(a->mutex);
            delete[] b;
            return 0;
        }
        int j = a->begin;
        if(a->begin > 0 && a->size > 1 && (a->a[j] - a->a[j - 1]) > (a->a[j + 1] - a->a[j] - EPSILON) && (a->a[j] - a->a[j - 1]) < (a->a[j + 1] - a->a[j] + EPSILON)){
            a->left_count++;
            a->left_sum += a->a[j];
        }
        else if(a->begin > 1 && (a->a[j - 1] - a->a[j - 2]) > (a->a[j] - a->a[j - 1] - EPSILON) && (a->a[j - 1] - a->a[j - 2]) < (a->a[j] - a->a[j - 1] + EPSILON)){
            a->left_count++;
            a->left_sum += a->a[j];
        }
        j++;
        while(j <= a->end){
            if(a->begin != 0 && (a->a[j] - a->a[j - 1]) > (a->left_q - EPSILON) && (a->a[j] - a->a[j - 1]) < (a->left_q + EPSILON)){
                a->left_count++;
                a->left_sum += a->a[j];
                j++;
            }
            else{
                break;
            }
        }
        if(j > a->end){
            a->right_count = a->left_count;
            a->right_sum = a->left_sum;
            //pthread_mutex_unlock(a->mutex);
            delete[] b;
            return 0;
        }
        
        sum = a->a[j - 1] + a->a[j];
        for(int i = j; i <= (a->end - 1); i++){
            if(((a->a[i + 1] - a->a[i]) > (a->a[i] - a->a[i - 1] - EPSILON)) && ((a->a[i + 1] - a->a[i]) < (a->a[i] - a->a[i - 1] + EPSILON))){
                sum += a->a[i + 1];
                count++;
            }
            else{
                if(count == 2){
                    b[i - a->begin - 1] = a->a[i - 1];
                    b[i - a->begin] = a->a[i];
                    sum = a->a[i] + a->a[i + 1];
                }
                else{
                    for(int k = i - count + 1; k <= i; k++){
                        b[k] = sum / count;
                    }
                    a->count += count;
                    i++;
                    sum = a->a[i] + a->a[i + 1];
                    count = 2;
                }
            }
        }
        if(a->end < (a->n - 2) && (a->a[a->end] - a->a[a->end - 1]) > (a->a[a->end + 1] - a->a[a->end] - EPSILON) && (a->a[a->end] - a->a[a->end - 1]) < (a->a[a->end + 1] - a->a[a->end] + EPSILON)){
            a->right_count = count;
            a->right_sum = sum;
        }
        else if(a->end < (a->n - 3) && (a->a[a->end + 1] - a->a[a->end]) > (a->a[a->end + 2] - a->a[a->end + 1] - EPSILON) && (a->a[a->end + 1] - a->a[a->end]) < (a->a[a->end + 2] - a->a[a->end + 1] + EPSILON)){
            a->right_count = 1;
            a->right_sum = a->a[a->end];
        }
        else if(a->size > 2 && count > 2){
            for(int i = a->size - count; i < a->size; i++){
                b[i] = sum / count;
            }
            a->count += count;
        }
        for(int i = a->begin; i <= a->end; i++){
            a->a[i] = b[i - a->begin];
        }
        delete[] b;
        //pthread_mutex_unlock(a->mutex);
    }
    return 0;
}