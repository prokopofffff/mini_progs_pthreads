#include <iostream>
#include "funcs.h"
#include <limits>

int main(int argc, char* argv[]){
    pthread_t* threads = new pthread_t[argc - 1];
    thread_args* args = new thread_args[argc - 1];
    for(int i = 0; i < argc - 1; i++){
        args[i].filename = argv[i + 1];
        args[i].m = i + 1;
        args[i].p = argc - 1;
        args[i].tid = threads[i];
        args[i].status = 0;
        args[i].min = 0;
        args[i].count = 0;
        if(pthread_create(&threads[i], NULL, read, (void*)&args[i])){
            std::cerr << "THREAD CREATION ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
        };
    }
    double min = std::numeric_limits<double>::max();
    int max_count = 0;
    int flag = 1;
    int error = 0;
    int ans = -1;
    for(int i = 0; i < argc - 1; i++){
        pthread_join(threads[i], 0);
        if(args[i].status == FILE_OPEN_ERROR || args[i].status == DATA_TYPE_ERROR){
            error = args[i].status;
            flag = -1;
        }
        if(max_count == args[i].count && min > args[i].min){
            min = args[i].min;
            ans = i;
        }
        else if(max_count < args[i].count){
            ans = i;
            min = args[i].min;
            max_count = args[i].count;
        }
    }
    if(flag == -1){
        std::cerr << (error == -1 ? "FILE OPEN ERROR" : "DATA TYPE ERROR") << std::endl;
        delete[] threads;
        delete[] args;
        return -1;
    }
    for(int i = 0; i < argc - 1; i++){
        args[i].status = 0;
        args[i].min = args[ans].min;
        args[i].count = 0;
        if(pthread_create(&threads[i], NULL, find, (void*)&args[i])){
            std::cerr << "THREAD CREATION ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
        };
    }
    int res = 0;
    for(int i = 0; i < argc - 1; i++){
        pthread_join(threads[i], 0);
        if(args[i].status == FILE_OPEN_ERROR || args[i].status == DATA_TYPE_ERROR){
            error = args[i].status;
            flag = -1;
        }
        res += args[i].count;
    }
    if(flag == -1){
        std::cerr << (error == -1 ? "FILE OPEN ERROR" : "DATA TYPE ERROR") << std::endl;
        delete[] threads;
        delete[] args;
        return -1;
    }
    std::cout << res << std::endl;
    delete[] args;
    delete[] threads;
    return 1;
}