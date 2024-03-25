#include "funcs.h"
#include <fstream>
#include <string>
#include <iostream>

void* read(void* args){
    thread_args* a = (thread_args*) args;
    std::ifstream file(a->filename);
    double x1, x2, x3;
    std::string num;
    if(!file.is_open()){
        a->status = FILE_OPEN_ERROR;
        return 0;
    }
    try{
        if(!(file >> num)){
            file.close();
            return 0;
        }
        x1 = std::stod(num);
        a->first = x1;
        a->second = x1;
        a->prelast = x1;
        a->last = x1;
        a->size++;
        if(!(file >> num)){
            file.close();
            return 0;
        }
        x2 = std::stod(num);
        a->second = x2;
        a->prelast = x2;
        a->last = x2;
        a->size++;
        x3 = x2;
        std::cout << x1 << " " << x2 << " ";
        while (file >> num){
            x3 = std::stod(num);
            a->size++;
            if(x1 <= x2 && x2 >= x3){
                a->sum += x2;
                a->count++;
            }
            x1 = x2;
            x2 = x3;
        }
        a->prelast = x1;
        a->last = x3;
        file.close();
        return 0;
    }
    catch(...){
        file.close();
        a->status = DATA_TYPE_ERROR;
        return 0;
    }
}

void* recount(void* args){
    thread_args* a = (thread_args*) args;
    std::ifstream file(a->filename);
    double x;
    std::string num;
    if(!file.is_open()){
        a->status = FILE_OPEN_ERROR;
        return 0;
    }
    try{
        while(file >> num){
            x = std::stod(num);
            if(x > a->sum) a->count++;
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