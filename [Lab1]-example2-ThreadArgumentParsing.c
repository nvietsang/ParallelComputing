#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 10

void *user_def_func(void *pthreadID){
	long TID;
	TID = (long) pthreadID;
	printf("Hello World from thread #%ld\n", TID);
	pthread_exit(NULL);
}

long taskids[NUM_THREADS];

int main(int argc, char *argv[]){
	pthread_t threads[NUM_THREADS];
	int create_flag;
	long i;

	for(i = 0; i < NUM_THREADS; i++){
		//pass argument
		taskids[i] = i;
		printf("In main: creating thread %ld\n", i);
		create_flag = pthread_create(&threads[i], NULL, user_def_func, (void *)taskids[i]);

		if (create_flag){
			printf("ERROR: return code from pthread_create() is %d\n", create_flag);
			//exit(-1);
		}
	}

	pthread_exit(NULL);
	return(0);
}