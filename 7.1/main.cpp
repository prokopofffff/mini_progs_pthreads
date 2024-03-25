#include <iostream>
#include <stdio.h>
#include <ctime>
#include "funcs.h"
#include <valarray>

#define STEP 1000;

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Incorrect input" << std::endl;
        return 1;
    }
    int p = atoi(argv[1]);
    unsigned long long int n = atoi(argv[2]);
    unsigned long long int result = 0;
    pthread_t *threads = new pthread_t[p];
    thread_args *args = new thread_args[p];
    // pthread_mutex_t mutex;
    // pthread_mutex_init(&mutex, 0);
    double start = clock();
    for(int i = 0; i < p; i++){
        args[i].count = 0;
        args[i].index = i;
        args[i].locCount = 0;
        args[i].n = n;
        args[i].p = p;
        args[i].res = 0;
        args[i].start = 2 + i * STEP;
        args[i].status = 0;
        args[i].step = STEP;
        args[i].stepCount = 0;
        // args[i].mutex = &mutex;
        if(pthread_create(&threads[i], NULL, find, (void*)&args[i])){
			std::cerr << "THREAD CREATION ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
		}
    }
    for(int i = 0; i < p; i++){
        if(pthread_join(threads[i], 0)){
            std::cerr << "THREAD JOIN ERROR" << std::endl;
            delete[] threads;
            delete[] args;
            return -1;
        }
    }
    //  std::cout << args[0].status << std::endl;
    if(args[0].status == 1){
        result = args[0].res;
    }
    else if(args[0].status == 2){
        for(int i = p - 1; i >= 0; i--){
            if(args[i].res != 0){
                result = args[i].res;
                break;
            }
        }
    }
    else if(args[0].status == -1){
        // std::cout << "start: " << args[0].start << "   end: " << args[0].start + p *p * p<<std::endl;
        for(unsigned long long int i = args[0].start; i <= (args[0].start + p * p * p); i++){
            if(isPrime(i)) args[0].count += 1;
            if(args[0].count == n) {
                result = i;
                break;
            }
        }
    }
    double end = clock();

    printf("RESULT = %llu\n", result);

    printf("Time of threads:\n");
	for(int i = 0; i < p; i++){
		printf("%d: %10.3e, ", i + 1, args[i].time / CLOCKS_PER_SEC);
	}
	printf("\nTotal time:\n%10.3e\n", (end - start) / CLOCKS_PER_SEC);

    delete[] threads;
    delete[] args;

    return 0;
}