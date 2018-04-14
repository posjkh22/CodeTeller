
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex_lock;

int var = 1;

void *t_function()
{

	pthread_mutex_lock(&mutex_lock);

	var++;

	pthread_mutex_unlock(&mutex_lock);

};


int main()
{
	
	int* ptr = malloc(sizeof(int));
		
	pthread_t p_thread;

	int state;

	pthread_mutex_init(&mutex_lock, NULL);
	
	pthread_create(&p_thread, NULL, t_function, NULL);

	pthread_join(&p_thread, (void** )&state);

	return 0;
}
