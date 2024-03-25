#include <iostream>
#include <fstream>
#include "funcs.h"

int main(int argc, char* argv[]){
    if(argc != 4){
        std::cerr << "Incorrect input" << std::endl;
        return 1;
    }

    int p = atoi(argv[1]), n = atoi(argv[2]);
    if(p < 1){
        std::cerr << "Wrong count of threads" << std::endl;
        return COUNT_THREADS_ERROR;
    }
    char *filename = argv[3];
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "File open error" << std::endl;
        return FILE_OPEN_ERROR;
    }
    double *a= new double[n];
    try{
        std::string num;
        int i = 0;
        int c = 0;
        while(file >> num){
            a[i] = std::stod(num);
            i++;
            c++;
            if(c == n) break;
        }
        if(c < n){
            delete[] a;
            std::cerr << "Array size error" << std::endl;
            return ARRAY_SIZE_ERROR;
        }
    }
    catch(...){
        delete[] a;
        std::cerr << "Data type error" << std::endl;
        return DATA_TYPE_ERROR;
    }
    pthread_t* threads = new pthread_t[p];
    thread_args* args = new thread_args[p];
    int l = n / p;
    int m = p == 1 ? 0 : n % p;
    int count = 0;
    double start = clock();
    if(n > 2){
        // pthread_mutex_t mutex;
        // pthread_mutex_init(&mutex, 0);
        for(int i = 0; i < p; i++){
            args[i].a = a;
            args[i].n = n;
            args[i].m = i;
            args[i].p = p;
            args[i].tid = threads[i];
            args[i].status = 0;
            args[i].count = 0;
            args[i].size = l + (i < m ? 1 : 0);
            args[i].begin = (i < m ? i * (l + 1) : m * (l + 1) + (i - m) * l);
            args[i].end = args[i].begin + l + (i < m ? 1 : 0) - 1;
            args[i].left = args[i].begin > 0 ? a[args[i].begin - 1] : 0;
            args[i].right = args[i].end < (n - 1) ? a[args[i].end + 1] : 0;
            args[i].left_q = args[i].begin > 0 ? (a[args[i].begin] - a[args[i].begin - 1]) : 0;
            args[i].right_q = args[i].end < (n - 1) ? (a[args[i].end + 1] - a[args[i].end]) : 0;
            args[i].right_sum = 0;
            args[i].left_sum = 0;
            args[i].right_count = 0;
            args[i].left_count = 0;
            //args[i].mutex = &mutex;
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
            count += args[i].count;
        }
        //pthread_mutex_destroy(&mutex);
        double sum = 0;
        int c = 0;
        for(int i = 1; i < p; i++){
            if(args[i].size == 0) break;
            else if(args[i].size == args[i].left_count){
                if(args[i - 1].left_count != args[i - 1].right_count){
                    sum = args[i - 1].right_sum;
                    c = args[i - 1].right_count;
                }
                sum += args[i].left_sum;
                c += args[i].left_count;
                if(args[i].end == n - 1 || args[i + 1].left_count == 0){
                    for(int j = args[i].begin + args[i].left_count - c ; j < args[i].begin + args[i].left_count; j++){
                        a[j] = sum / c;
                        count++;
                    }
                }
            }
            else if(args[i].left_count != 0){
                sum += (c == 0 ? args[i - 1].right_sum : 0) + args[i].left_sum;
                c += (c == 0 ? args[i - 1].right_count : 0) + args[i].left_count;
                for(int j = args[i].begin + args[i].left_count - c ; j < args[i].begin + args[i].left_count; j++){
                    a[j] = sum / c;
                    count++;
                }
                sum = 0;
                c = 0;
            }
        }
    }
    double end = clock();
    std::cout << "RESULT "<< p << ": ";
    for(int i = 0; i < n; i++){
        std::cout << a[i] << " ";
    }
    std::cout << "     changed: " << count << "    time: " << (end - start) / CLOCKS_PER_SEC << std::endl;
    delete[] threads;
    delete[] args;
    delete[] a;
    return 0;
}