#include "common.h"

char* barber_semaphore_name = "barber";
char* chairs_semaphore_name = "chair";
char* clients_semaphore_name = "client";

sem_t* barber_semaphore;
sem_t* chair_semaphore;
sem_t* client_semaphore;


int getChairsNumber(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Error: you have to enter chairs number as program argument!\n");
		exit(1);
	}

	int chairs = strtol(argv[1], NULL, 10);

	if(chairs == 0 || chairs > 20)
	{
		printf("Error: Argument should be integer greater than 0 and smaller than 20.\n");
		exit(1);
	}

	return chairs;
}

void closeSemaphores()
{
	closeSemaphore(barber_semaphore);
	closeSemaphore(chair_semaphore);
	closeSemaphore(client_semaphore);
}


int main(int argc, char** argv)
{
	//signal(SIGINT, handleSignal);
	//signal(SIGTSTP, handleSignal);

	int chairs = getChairsNumber(argc, argv);
	
	barber_semaphore = createSemaphore(barber_semaphore_name, 1);
	chair_semaphore = createSemaphore(chairs_semaphore_name, chairs);
	client_semaphore = createSemaphore(clients_semaphore_name, 0);

	closeSemaphores();

	//initializeSemaphor(CHAIR_SEMAPHORE, chairs,semaphore_list_id);
	//initializeSemaphor(BARBER_SEMAPHORE, GO_TO_SLEEP, semaphore_list_id);
	//createSharedMemory();

	//clients_queue = (queue*)shared_memory_ptr;
	//initializeQueue(clients_queue);
	return 0;
}