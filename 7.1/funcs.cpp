#include "funcs.h"

void ReduceSum(int p, unsigned long long int* a, int n) {
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static unsigned long long int* r = nullptr;
    int i;
    if (p <= 1) return;
    pthread_mutex_lock(&m);
    if (r == nullptr) r = a;
    else for(i = 0; i < n; ++i) r[i] += a[i];
    ++t_in;
    if (t_in >= p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (r != a) {
        for (i = 0; i < n; ++i) a[i] = r[i];
    }
    ++t_out;
    if (t_out >= p) {
        t_in = 0;
        r = nullptr;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

void ReduceSum(int p, int* a, int n) {
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static int* r = nullptr;
    int i;
    if (p <= 1) return;
    pthread_mutex_lock(&m);
    if (r == nullptr) r = a;
    else for(i = 0; i < n; ++i) r[i] += a[i];
    ++t_in;
    if (t_in >= p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (r != a) {
        for (i = 0; i < n; ++i) a[i] = r[i];
    }
    ++t_out;
    if (t_out >= p) {
        t_in = 0;
        r = nullptr;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

int isPrime(unsigned long long int n){
	unsigned long long int i;
	if(n == 2 || n == 3) return 1;
	if(n == 1 || n % 2 == 0 || n % 3 == 0) return 0;
	double m = sqrt(n);
	for(i = 5; i <= m; i += 2){
		if(i % 3 == 0) continue;
		if(n % i == 0){
			return 0;
		}
	}
	return 1;
}

void* find(void* args){
    thread_args* a = (thread_args*) args;

	double time = clock();

	while(true){
		if(a->step == 1 || a->step == 0){
			a->status = -1;
			a->time = clock() - time;
			return 0;
		}
		a->stepCount = 0;
		a->locCount = 0;
        a->res = 0;
		for(unsigned long long int i = a->start; i < a->start + a->step; i++){
			if(isPrime(i)){
				a->res = i;
				a->locCount++;
			}
			if((a->locCount == a->n || a->count + a->locCount == a->n) && a->index == 0){
				a->status = 1;
				break;
			}
		}

		ReduceSum(a->p, &a->status, 1);

		if(a->status != 0){
			a->time = clock() - time;
			return 0;
		}

		a->stepCount = a->locCount;

		ReduceSum(a->p, &a->stepCount, 1);

		a->count += a->stepCount;

        // if(a->index == a->p - 1) {
        //     pthread_mutex_lock(a->mutex);

        //     std::cout << a->index << "  " << a->start << " - " << a->start + a->step << " : " << a->res << std::endl;

        //     pthread_mutex_unlock(a->mutex);
        // }

		if(a->count < a->n){
			a->start = a->start + a->step * a->p;
			continue;
		}
		else if(a->count == a->n){
			a->status = 2;
			a->time = clock() - time;
			return 0;
		}
		else{
			a->count -= a->stepCount;
			a->start = a->start - a->index * a->step;
			a->step /= a->p;
			a->start = a->start + a->index * a->step;
		}
	}
}