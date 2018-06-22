#include "common.h"




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

void createSemaphore(char* name, int value)
{

}

int main(int argc, char** argv)
{
	//signal(SIGINT, handleSignal);
	//signal(SIGTSTP, handleSignal);

	int chairs = getChairsNumber(argc, argv);
	
	createSemaphoreList();
	//initializeSemaphor(CHAIR_SEMAPHORE, chairs,semaphore_list_id);
	//initializeSemaphor(BARBER_SEMAPHORE, GO_TO_SLEEP, semaphore_list_id);
	//createSharedMemory();

	//clients_queue = (queue*)shared_memory_ptr;
	//initializeQueue(clients_queue);
	return 0;
}