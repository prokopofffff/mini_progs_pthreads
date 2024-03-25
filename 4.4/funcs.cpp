#include "funcs.h"
#include <ctime>

void* change(void* args){
    thread_args* a = (thread_args*) args;
    // double start = clock();
    if(a->size > 0){
        double* b = new double[a->size];
        for(int i = 0; i < a->size; i++){
            b[i] = 0;
        }
        //pthread_mutex_lock(a->mutex);
        for(int i = a->begin, j = 0; i <= a->end; i++, j++){
            if(i == 0 || i == 1 || i == (a->n - 1) || i == (a->n - 2)) b[j] = a->a[i];
            else if(j == 0){
                b[j] = (a->left1 + a->left2 + (a->size < 2 ? a->right2 : a->a[i + 1]) + (a->size < 3 ? a->right1 : a->a[i + 2])) / 4;
                a->count++;    
            }
            else if(j == 1){
                b[j] = (a->left2 + a->a[i - 1] + (a->size < 3 ? a->right2 : a->a[i + 1]) + (a->size < 4 ? a->right1 : a->a[i + 2])) / 4;
                a->count++;    
            }
            else if(j == a->end - 1){
                b[j] = ((a->size < 3 ? a->left1 : a->a[i - 1]) + (a->size < 4 ? a->left2 : a->a[i - 2]) + a->a[i + 1] + a->right1) / 4;
                a->count++;    
            }
            else if(j == a->end){
                b[j] = ((a->size < 3 ? a->left2 : a->a[i - 2]) + (a->size < 2 ? a->left1 : a->a[i - 1]) + a->right1 + a->right2) / 4;
                a->count++;    
            }
            else{
                b[j] = (a->a[i - 2] + a->a[i - 1] + a->a[i + 1] + a->a[i + 2]) / 4;
                a->count++;
            }
        }
        // std::cout << a->right1 << " " << a->right2 << " " << std::endl;
        // double end = clock();
        // pthread_mutex_lock(a->mutex);
        // std::cout << "RESULT " << a->m << ": ";
        for(int i = a->begin, j = 0; i <= a->end; i++, j++){
            a->a[i] = b[j];
            // std::cout << b[j] << " ";
        }
        // std::cout << "     changed: " << a->count << "    time: " << (end - start) / CLOCKS_PER_SEC << std::endl;
        // pthread_mutex_unlock(a->mutex);
        delete[] b;
    }
    // else{
    //     double end = clock();
    //     pthread_mutex_lock(a->mutex);
    //     std::cout << "RESULT " << a->m << ": ";
    //     for(int i = a->begin; i <= a->end; i++){
    //         std::cout << a->a[i] << " ";
    //     }
    //     std::cout << "     changed: " << a->count << "    time: " << (end - start) / CLOCKS_PER_SEC << std::endl;
    //     pthread_mutex_unlock(a->mutex);
    // }
    return 0;
}