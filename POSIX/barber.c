#include "common.h"

char* barber_semaphore_name = "barber";
char* chairs_semaphore_name = "chair";
char* clients_semaphore_name = "client";

sem_t* barber_semaphore;
sem_t* chair_semaphore;
sem_t* client_semaphore;

char* shared_memory_name = "shared memory";
int shared_memory_descryptor;
void* shared_memory_ptr;

queue* clients_queue;

int ftruncate(int fd, off_t length);

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


void createSharedMemory()
{
	shared_memory_descryptor = shm_open(shared_memory_name, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if(shared_memory_descryptor == -1)
	{
		printf("Error: cannot create shared mamory segment.\n");
		closeSemaphores();
		exit(1);
	}

	int set_size = ftruncate(shared_memory_descryptor, sizeof(queue));
	if(set_size == -1)
	{
		printf("Error: cannot set size of shared memory.\n");
		shm_unlink(shared_memory_name);
		closeSemaphores();
		exit(1);
	}

	shared_memory_ptr = mmap(NULL, sizeof(queue), PROT_WRITE | PROT_READ, MAP_SHARED, shared_memory_descryptor, 0);
	if(shared_memory_ptr == (void*)-1)
	{
		printf("Error: cannot add shared memory segment to process memory.\n");
		shm_unlink(shared_memory_name);
		closeSemaphores();
		exit(1);
	}

}

void finishBarberWork()
{
	printf("\nBarber finishes his work\n");
	closeSemaphores();
	munmap(shared_memory_ptr, sizeof(queue));
	shm_unlink(shared_memory_name);
	exit(0);
}



int main(int argc, char** argv)
{
	signal(SIGINT, finishBarberWork);
	signal(SIGTSTP, finishBarberWork);
	
	int chairs = getChairsNumber(argc, argv);
	
	barber_semaphore = createSemaphore(barber_semaphore_name, 1);
	chair_semaphore = createSemaphore(chairs_semaphore_name, chairs);
	client_semaphore = createSemaphore(clients_semaphore_name, 0);
	createSharedMemory();

	clients_queue = (queue*)shared_memory_ptr;
	initializeQueue(clients_queue);

	while(1)
	{

	}

	finishBarberWork();


	//clients_queue = (queue*)shared_memory_ptr;
	//initializeQueue(clients_queue);
	return 0;
}