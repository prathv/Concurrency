#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>

//pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t mutex;
void check(int *data)
{	
	while(1 == 1)
{
	sem_wait(&mutex);
	int num = *data;	
	printf(" This is thread %d",num); 
	sem_post(&mutex);
	
	}
}

int main()
{

sem_init(&mutex,0,1);

pthread_t pid[1];
int i;
int data1 = 3;
int data = 1;
	//	pthread_create(&pid1,NULL,(void *) &check,(void*) &data);          
	//	pthread_create(&pid2,NULL,(void *) &check, (void*) &data1);
		for(i = 0; i <= 1; i++ )
			{
			
			pthread_create(&pid[i],NULL,(void*) &check,(void*) &i);

			}
		pthread_join(pid[0],NULL);
		pthread_join(pid[1],NULL);
	

return 0; 
}
