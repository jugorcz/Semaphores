#include "common.h"

sem_t* createSemaphore(char* name, int value)
{
	sem_t* semaphore = sem_open(name, O_RDWR | O_CREAT | O_EXCL, 0666, value);
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