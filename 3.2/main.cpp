#include <iostream>
#include "funcs.h"
#include <vector>

int main(int argc, char* argv[]){
    pthread_t* threads = new pthread_t[argc - 1];
    thread_args* args = new thread_args[argc - 1];
    for(int i = 0; i < argc - 1; i++){
        args[i].filename = argv[i + 1];
        args[i].m = i + 1;
        args[i].p = argc - 1;
        args[i].first = 0;
        args[i].last = 0;
        args[i].size = 0;
        args[i].prelast = 0;
        args[i].second = 0;
        args[i].tid = threads[i];
        args[i].status = 0;
        args[i].count = 0;
        args[i].sum = 0;
        if(pthread_create(&threads[i], NULL, read, (void*)&args[i])){
            std::cerr << "THREAD CREATION ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
        };
    }
    int flag = 1;
    int error = 0;
    int count = 0;
    double sum = 0;
    std::vector<double> v;
    for(int i = 0; i < argc - 1; i++){
        pthread_join(threads[i], 0);
        if(args[i].status == FILE_OPEN_ERROR || args[i].status == DATA_TYPE_ERROR){
            error = args[i].status;
            flag = -1;
        }
        else{
            sum += args[i].sum;
            count += args[i].count;
        }
    }
    if(flag == -1){
        std::cerr << (error == -1 ? "FILE OPEN ERROR" : "DATA TYPE ERROR") << std::endl;
        delete[] threads;
        delete[] args;
        return -1;
    }
    for(int i = 0; i < argc - 2; i++){
        if(args[i].size == 0) continue;
        for(int j = i + 1; j < argc - 1; j++){
            if(args[j].size == 0) continue;
            if(args[i].size == 1){
                if(args[j].size == 1){
                    for(int k = j + 1; k < argc - 1; k++){
                        if(args[k].size == 0) continue;
                        else if(args[i].last <= args[j].last && args[j].last >= args[k].first){
                            sum += args[j].last;
                            count++;
                        }
                        break;
                    }
                }
                else{
                    if(args[i].last <= args[j].first && args[j].first >= args[j].second){
                        sum += args[j].first;
                        count++;
                    }
                }
                
            }
            else if(args[i].size > 1){
                if(args[j].size == 1){
                    if(args[i].prelast <= args[i].last && args[i].last >= args[j].first){
                        sum += args[i].last;
                        count++;
                    }
                }
                else if(args[j].size > 1){
                    if(args[i].prelast <= args[i].last && args[i].last >= args[j].first){
                        sum += args[i].last;
                        count++;
                    }
                    if(args[i].last <= args[j].first && args[j].first >= args[j].second){
                        sum += args[j].first;
                        count++;
                    }
                }
            }
            break;
        }
    }
    if(count == 0){
        std::cout << 0 << std::endl;
        delete[] threads;
        delete[] args;
        return 0;
    }
    double av = sum / count;
    for(int i = 0; i < argc - 1; i++){
        args[i].sum = av;
        args[i].count = 0;
        if(pthread_create(&threads[i], NULL, recount, (void*)&args[i])){
            std::cerr << "THREAD CREATION ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
        };
    }
    flag = 1;
    error = 0;
    count = 0;
    for(int i = 0; i < argc - 1; i++){
        pthread_join(threads[i], 0);
        if(args[i].status == FILE_OPEN_ERROR || args[i].status == DATA_TYPE_ERROR){
            error = args[i].status;
            flag = -1;
        }
        else{
            count += args[i].count;
        }
    }
    if(flag == -1){
        std::cerr << (error == -1 ? "FILE OPEN ERROR" : "DATA TYPE ERROR") << std::endl;
        delete[] threads;
        delete[] args;
        return -1;
    }
    std::cout << count << std::endl;
    delete[] threads;
    delete[] args;
    return 0;
}