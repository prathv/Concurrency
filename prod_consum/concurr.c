#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "mt19937ar.c"

/* data type definition for the items stored in the buffer */

typedef struct {
int printnum;
int waitime;
}item;

int counter = 0;
item buffer[32];

/* Initialization of mutex locks and conditional locks */

pthread_mutex_t bufferlock = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t buf_full = PTHREAD_COND_INITIALIZER; 
pthread_cond_t buf_empty = PTHREAD_COND_INITIALIZER;

int _rdrand(unsigned int *random_number)
{	unsigned char err;
	asm volatile("rdrand %0 ; setc %1"
				: "=r" (random_number), "=qm" (err));
	return (int)err;
}


/* producer threads function */
void* producer()
{	
	while(1 == 1)

	{
	/* lock the buffer */
	pthread_mutex_lock(&bufferlock);
	
	item entity;
	printf("Producer is  working \n\n");

	entity.printnum = randgen() % 100;
	entity.waitime = 3 + (randgen() % 7) ; 

	/* Adding into the buffer */ 

	if(counter < 32)
	{

	buffer[counter] = entity;
	counter++;
	sleep(entity.waitime);

	
	}
	
	/* wait on consumer, since buffer is full */
	else {

	printf("buffer full, going to wait for item to be consumed\n");
	pthread_cond_wait(&buf_empty,&bufferlock);
	}

	pthread_cond_signal(&buf_full);
	/* unlock buffer for other threads to use the buffer */
	pthread_mutex_unlock(&bufferlock); 
	sleep(randgen()%2 + 3);

	}

}

/* Consumer Thread Function */

void* consumer(void* consnum)
{
	//int connum = (int *) consnum;
	item entity;
	int time;
	int num;	
	while(1 == 1)
	{
	/* Locking the buffer resource */

	pthread_mutex_lock(&bufferlock);
	printf("Consumer is working\n");

	/* Consumer takes item from buffer */
	
	if(counter > 0)
	{
	num = buffer[counter-1].printnum;
	time = buffer[counter-1].waitime; 
	buffer[counter].printnum = -1 ;
	buffer[counter].waitime = -1;
	counter--;
	sleep(time);
	printf("CONSUMED!!, The consumed number is %d and waitime is %d\n\n",num,time);
	}
	
	/* Consumer waits on Producer, since the buffer is empty */

	else
	{
	printf("Buffer is empty, waiting for producer\n");
	pthread_cond_wait(&buf_full,&bufferlock);
	}

	pthread_cond_signal(&buf_empty);
	pthread_mutex_unlock(&bufferlock);
	sleep(time);
	}

}		


/* Random Number Generator function, and checking support for MT and RDRAND */

int randgen(){
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	int random_num;
	char vendor[13];
	int n;

	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	
	if(ecx & 0x40000000)
	{
		
		n=_rdrand(&random_num);
	}

else
	{
		//use mt19937
		
		n = (int)genrand_int32();
		n = abs(n);
	}

	return n;
	
}	



int main()
{

	

	pthread_t consum,consum1;
	pthread_t prod,prod1;
	int data = 1;
	
	/* Creation of producer and consumer threads */

	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&prod1, NULL, producer, NULL);
	pthread_create(&consum1, NULL, consumer, NULL);
	pthread_create(&consum, NULL, consumer, &data);



	pthread_join(prod,NULL);
	pthread_join(prod1,NULL);

	pthread_join(consum, NULL);
	pthread_join(consum1, NULL);

//exit(0);


return 0;
}


