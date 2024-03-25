#include <iostream>
#include <fstream>
#include <sstream>
#include "funcs.h"

int main(int argc, char* argv[]){
    if(argc != 5){
        std::cerr << "Incorrect input" << std::endl;
        return 1;
    }

    int p = atoi(argv[1]), n1 = atoi(argv[2]), n2 = atoi(argv[3]);
    if(p < 1){
        std::cerr << "Wrong count of threads" << std::endl;
        return COUNT_THREADS_ERROR;
    }
    char *filename = argv[4];
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "File open error" << std::endl;
        return FILE_OPEN_ERROR;
    }
    double *a= new double[n1 * n2];
    // try{
    //     int i = 0;
    //     std::string line;
    //     while (std::getline(file, line)){
    //         std::stringstream ss(line);
    //         std::string num;
    //         while(ss >> num){
    //             a[i] = std::stod(num);
    //             i++;
    //             if(i == n1 * n2) break;
    //         }
    //         if(i == n1 * n2) break;
    //     }
    //     if(i < n1 * n2){
    //         delete[] a;
    //         std::cerr << "Array size error" << std::endl;
    //         return ARRAY_SIZE_ERROR;
    //     }
    // }
    // catch(...){
    //     delete[] a;
    //     std::cerr << "Data type error" << std::endl;
    //     return DATA_TYPE_ERROR;
    // }
    try{
        std::string num;
        int i = 0;
        while(file >> num){
            a[i] = std::stod(num);
            i++;
            if(i == n1 * n2) break;
        }
        if(i < n1 * n2){
            delete[] a;
            file.close();
            std::cerr << "Array size error" << std::endl;
            return ARRAY_SIZE_ERROR;
        }
    }
    catch(...){
        delete[] a;
        file.close();
        std::cerr << "Data type error" << std::endl;
        return DATA_TYPE_ERROR;
    }
    file.close();
    // for(int i = 0; i < n1; i++){
    //     for(int j = 0; j < n2; j++){
    //         std::cout << a[i * n2 + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    pthread_t* threads = new pthread_t[p];
    thread_args* args = new thread_args[p];
    int c = 0;
    double sum = 0;
    double start = clock();
    int l = (n1 * n2) / p;
    int m = p == 1 ? 0 : (n1 * n2) % p;
    if(n1 > 2 && n2 > 1){
        // pthread_mutex_t mutex;
        // pthread_mutex_init(&mutex, 0);
        for(int i = 0; i < p; i++){
            args[i].a = a;
            args[i].n1 = n1;
            args[i].n2 = n2;
            args[i].m = i;
            args[i].p = p;
            args[i].tid = threads[i];
            args[i].status = 0;
            args[i].count = 0;
            args[i].size = l + (i < m ? 1 : 0);
            args[i].begin = (i < m ? i * (l + 1) : m * (l + 1) + (i - m) * l);
            args[i].end = args[i].begin + l + (i < m ? 1 : 0) - 1;
            args[i].sum = 0;
            //args[i].mutex = &mutex;
        }
        for(int i = 0; i < p; i++){
            if(pthread_create(&threads[i], NULL, count, (void*)&args[i])){
                std::cerr << "THREAD CREATION ERROR" << std::endl;
                delete[] a;
                delete[] threads;
                delete[] args;
                return -1;
            };
        }
        for(int i = 0; i < p; i++){
            pthread_join(threads[i], 0);
            c += args[i].count;
            sum += args[i].sum;
        }
        //pthread_mutex_destroy(&mutex);
        if(c == 0){
            double end = clock();
            std::cout << "RESULT " << p << ":" << std::endl;
            for(int i = 0; i < n1; i++){
                for(int j = 0; j < n2; j++){
                    std::cout << a[i * n2 + j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << "changed: " << c << "   time: " << (end - start) / CLOCKS_PER_SEC << std::endl;
            delete[] threads;
            delete[] a;
            delete[] args;
            return 0;
        }
        double av = sum / c;
        for(int i = 0; i < p; i++){
            args[i].sum = av;
        }
        for(int i = 0; i < p; i++){
            if(pthread_create(&threads[i], NULL, change, (void*)&args[i])){
                std::cerr << "THREAD CREATION ERROR" << std::endl;
                delete[] a;
                delete[] threads;
                delete[] args;
                return -1;
            };
        }
        for(int i = 0; i < p; i++){
            pthread_join(threads[i], 0);
        }
        double end = clock();
        std::cout << "RESULT " << p << ":" << std::endl;
        for(int i = 0; i < n1; i++){
            for(int j = 0; j < n2; j++){
                std::cout << a[i * n2 + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "changed: " << c << "   time: " << (end - start) / CLOCKS_PER_SEC << std::endl;
        delete[] threads;
        delete[] a;
        delete[] args;
    }
    delete[] threads;
    delete[] a;
    delete[] args;
    return 0;
}