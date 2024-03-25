#include <iostream>
#include <pthread.h>
#include <fstream>
#include <math.h>
#include <typeinfo>
#include <string>

#define FILE_OPEN_ERROR -1
#define DATA_TYPE_ERROR -2

struct thread_args{
    char* filename;
    int m;
    int p;
    int status;
    pthread_t tid;
    int res;
};

void* algorithm(void* args){
    thread_args *a = (thread_args*)args;
    std::ifstream file(a->filename);
    std::string num;
    double x_1, x_2, x_3, mult;
    if(!file.is_open()){
        a->status = FILE_OPEN_ERROR;
        return 0;
    }
    if(!file){
        file.close();
        return 0;
    }
    try{
        if(!(file >> num)){
            file.close();
            return 0;
        }
        x_1 = std::stod(num);
        if(!(file >> num)){
            file.close();
            return 0;
        }
        x_2 = std::stod(num);
        x_3 = x_2;
        while(file >> num){
            x_3 = std::stod(num);
            mult = x_1 * x_3;
            if(mult >= 0) 
                if(x_2 < sqrt(x_1 * x_3)) a->res++;
            x_1 = x_2;
            x_2 = x_3;
        }
        file.close();
        return 0;
    }
    catch(...){
        file.close();
        a->status = DATA_TYPE_ERROR;
        return 0;
    }
}

int main(int argc, char* argv[]){
    pthread_t* threads = new pthread_t[argc - 1];
    thread_args* args = new thread_args[argc - 1];
    for(int i = 0; i < argc - 1; i++){
        args[i].filename = argv[i + 1];
        args[i].m = i + 1;
        args[i].p = argc - 1;
        args[i].tid = threads[i];
        args[i].status = 0;
        args[i].res = 0;
        if(pthread_create(&threads[i], NULL, algorithm, (void*)&args[i])){
            std::cerr << "THREAD CREATION ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
        };
    }
    int res = 0;
    int flag = 1;
    int error = 0;
    for(int i = 0; i < argc - 1; i++){
        pthread_join(threads[i], 0);
        if(args[i].status == FILE_OPEN_ERROR || args[i].status == DATA_TYPE_ERROR){
            error = args[i].status;
            flag = -1;
        }
        res += args[i].res;
    }
    if(flag == -1){
        switch(error){
            case -1:
                std::cerr << "FILE OPEN ERROR" << std::endl;
                break;
            case -2:
                std::cerr << "DATA TYPE ERROR" << std::endl;
                break;
            default:
                break;
        }
        delete[] threads;
        delete[] args;
        return -1;
    }
    std::cout << res << std::endl;
    delete[] args;
    delete[] threads;
    return 1;
}