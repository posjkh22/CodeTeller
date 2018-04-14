#include <stdlib.h>
#include <pthread.h>

char charGlobal1;
int IntegerGlobalVariable = 3;
double DoubleGlobalVariable= 3.5;
int *IntegerPointerGV;
char charGlobal2;

void t1_fnc1(int *ptr1){

	free(ptr1);
}


void t2_fnc1(int *ptr1, int *ptr2){

	 free(ptr1);
	 free(ptr2);
}

int *shared_func2(int *ptr1, int *ptr2){

	return ptr1;
}


void task1(void *ptr){

	int *ptr1 = (int *)malloc(sizeof(int));
	int *ptr2 = (int *)malloc(sizeof(int));
	int *ret;

	IntegerGlobalVariable = 1;

	int a = 0;
	if(a == 0){
		/* ptr2 not free */
		t1_fnc1(ptr1);

	}else{

		/* no error */
		ret = shared_func2(ptr1, ptr2);
		free(ret);
		free(ptr2);
	}
}

void task2(void *ptr){

	int *ptr1 = (int *)malloc(sizeof(int));
	int *ptr2 = (int *)malloc(sizeof(int));
	int *ret;

	int a = 0;
	if( a == 0){

		/* double free ptr2 */
		ret = ptr2;
		free(ret);
		ret = shared_func2(ptr1, ptr2);
		t2_fnc1(ptr1, ptr2);

	}else{
	
		/* double free ptr1 */
		free(ptr1);
		free(ptr2);
		free(ptr2);
	}	

}


int main(){

	int status;
	int thread_id1;
	int thread_id2;	

	pthread_t thread[2];
	
	thread_id1 = pthread_create(&thread[0], NULL, task1, NULL);
	thread_id2 = pthread_create(&thread[1], NULL, task2, NULL);

	pthread_join(thread[0], &status);
	pthread_join(thread[1], &status);


	return 0;
}

