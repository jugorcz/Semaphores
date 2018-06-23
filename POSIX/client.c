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

int clients_numebr;
int visits;
int visited = 0;

queue* clients_queue;


void checkArguments(int argc, char** argv)
{
	if(argc < 3)
	{
		printf("Error: you have to enter cilents number and how many times they visit barber shop.\n");
		printf("example: ./client 10 2\n");	
		exit(1);
	}

	clients_numebr = strtol(argv[1], NULL, 10);
	visits = strtol(argv[2], NULL, 10);
	if(clients_numebr < 1 || visits < 1)
	{
		printf("Error: Argument should be integer greater than 0.\n");
		exit(1);
	}
}

void getAccessToSharedMemory()
{
	shared_memory_descryptor = shm_open(shared_memory_name,  O_RDWR, 0666);
	if(shared_memory_descryptor == -1)
	{
		printf("Error: cannot get acces to shared memory segment.\n");
		exit(1);
	}

	shared_memory_ptr = mmap(NULL, sizeof(queue), PROT_WRITE | PROT_READ, MAP_SHARED, shared_memory_descryptor, 0);
	if(shared_memory_ptr == (void*)-1)
	{
		printf("Error: cannot add shared memory segment to process memory.\n");
		exit(1);
	}
}

void finishVisit()
{
	printf("\nClients leave barber shop.\n");
	munmap(shared_memory_ptr, sizeof(queue));
	exit(0);
}

int main(int argc, char** argv)
{
	signal(SIGINT, finishVisit);
	signal(SIGTSTP, finishVisit);

	checkArguments(argc, argv);

	barber_semaphore = openSemaphore(barber_semaphore_name);
	chair_semaphore = openSemaphore(chairs_semaphore_name);
	client_semaphore = openSemaphore(clients_semaphore_name);

	getAccessToSharedMemory();

	clients_queue = (queue*)shared_memory_ptr;

	finishVisit();
	return 0;
}