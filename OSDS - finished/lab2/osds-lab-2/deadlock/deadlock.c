/// INITIAL CODE
/*

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mtx[2];



void * ThrFunc(void * p)
{
	int * param = (int *) p;
	pthread_mutex_lock(&mtx[*param]);
	pthread_mutex_lock(&mtx[1-*param]);
	return 0;
}

int main()
{
	pthread_t thr1;
	pthread_t thr2;
	int i1 = 0, i2 = 1;
	pthread_mutex_init(&mtx[0], NULL);
	pthread_mutex_init(&mtx[1], NULL);
	pthread_create(&thr1, NULL, ThrFunc, &i1);
	pthread_create(&thr2, NULL, ThrFunc, &i2);

	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);
	pthread_mutex_destroy(&mtx[0]);
	pthread_mutex_destroy(&mtx[1]);
	return 0;
}

*/

#define _XOPEN_SOURCE 600 

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mtx[2];
pthread_barrier_t barrier;

void *ThrFunc(void *p)
{
	int *param = (int *)p;
	pthread_mutex_lock(&mtx[*param]); // here the thread takes the first resource
	printf("Thread %d before lock\n", *param);
	pthread_barrier_wait(&barrier); // here the thread waits for the other thread to take the second resource
	pthread_mutex_lock(&mtx[1 - *param]); // here the thread takes the second resource	
	return 0;
}

int main()
{
	pthread_t thr1;
	pthread_t thr2;
	int i1 = 0, i2 = 1;
	pthread_barrier_init(&barrier, NULL, 2);

	pthread_mutex_init(&mtx[0], NULL);
	pthread_mutex_init(&mtx[1], NULL);

	pthread_create(&thr1, NULL, ThrFunc, &i1);
	pthread_create(&thr2, NULL, ThrFunc, &i2);

	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);

	pthread_mutex_destroy(&mtx[0]);
	pthread_mutex_destroy(&mtx[1]);
	
	pthread_barrier_destroy(&barrier);
	return 0;
}