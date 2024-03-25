#include "funcs.h"
#include <fstream>
#include <string>

void* read(void* args){
    thread_args *a = (thread_args*) args;
    std::ifstream file(a->filename);
    std::string num;
    double x_1, x_2;
    int count = 1, max_count = 1;
    if(!file.is_open()){
        a->status = FILE_OPEN_ERROR;
        return 0;
    }
    try{
        if(!(file >> num)){
            file.close();
            return 0;
        }
        x_2 = std::stod(num);
        double min = x_2;
        while (file >> num){
            x_1 = x_2;
            x_2 = std::stod(num);
            if(x_2 < x_1){
                count++;
                if(max_count == count && min > x_2) min = x_2;
                else if(max_count < count) {
                    max_count = count;
                    min = x_2;
                }
            }
            else{
                count = 0;
            }

        }
        file.close();
        a->count = max_count;
        a->min = min;
        return 0;
    }
    catch(...){
        file.close();
        a->status = DATA_TYPE_ERROR;
        return 0;
    }
}

void* find(void* args){
    thread_args *a = (thread_args*) args;
    std::ifstream file(a->filename);
    double x_1;
    std::string num;
    if(!file.is_open()){
        a->status = FILE_OPEN_ERROR;
        return 0;
    }
    try{
        while(file >> num){
            x_1 = std::stod(num);
            if(x_1 < a->min) a->count++;
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