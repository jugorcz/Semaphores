#include "common.h"

sem_t* createSemaphore(char* name, int value)
{
	sem_t* semaphore = sem_open(name, O_RDWR | O_CREAT , 0666, value);
	if(semaphore == SEM_FAILED)
	{
		printf("Error: cannot create semaphore\n");
		exit(1);
	}
	return semaphore;
}

sem_t* openSemaphore(char* name)
{
	sem_t * semaphore = sem_open(name, O_RDWR);
	if(semaphore == SEM_FAILED)
	{
		printf("Error: cannot get access semaphore\n");
		exit(1);
	}
	return semaphore;
}


void closeSemaphore(sem_t* semaphore)
{
	int closed = sem_close(semaphore);
	if(closed == -1)
	{
		printf("Warning: cannot close semaphore.\n");
	}
}


void initializeQueue(queue* queue)
{
	queue -> barber_is_sleeping = true;
	queue -> next_free_chair = 0;
	queue -> the_longest_waiting_client = 0;
	queue -> barber_current_client = -1;
	for(int i = 0; i < QUEUE_SIZE; i++)
	{
		queue -> chairs[i] = -1;
	}
}


void displayQueue(queue* queue)
{
	printf("Clients in queue: ");
	int current_index = queue -> the_longest_waiting_client;
	int end_index = queue -> next_free_chair;

	if(current_index == end_index && queue -> chairs[current_index] == -1)
	{
		printf("none.\n");
	}
	else do
	{
		printf("%d ",queue -> chairs[current_index]);
		current_index = (current_index + 1) % QUEUE_SIZE;
	} while (current_index != end_index);
			
	printf("\n");
}