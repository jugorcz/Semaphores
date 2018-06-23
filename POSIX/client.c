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

void wakeUpBarber(pid_t pid)
{
	printf("Client %d wakes up barber.\n", pid);
	clients_queue -> barber_is_sleeping = false;
	clients_queue -> barber_current_client = pid;
}

bool findFreeChair(pid_t pid)
{
	 int result = tryDecreaseSemaphore(chair_semaphore);
	 if(result == -1)
	 {
	 	printf("There is no free chair, client %d leaves barber shop.\n", pid);
	 	return false;
	 }
	 printf("Client %d takes a seat.\n",pid);

	 return (takeASeat(pid, clients_queue));
}

bool goToBarber()
{
	pid_t client_pid = getpid();
	if(clients_queue -> barber_is_sleeping == true)
		wakeUpBarber(client_pid);
	
	if(!findFreeChair(client_pid))
	{
		visited++;
		return false;
	} else
		increaseSemaphore(client_semaphore);

	return true;
}

void clientIsCut()
{
	increaseSemaphore(chair_semaphore);
	visited++;
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
	signal(SIGRTMIN, clientIsCut);

	checkArguments(argc, argv);

	barber_semaphore = openSemaphore(barber_semaphore_name);
	chair_semaphore = openSemaphore(chairs_semaphore_name);
	client_semaphore = openSemaphore(clients_semaphore_name);

	getAccessToSharedMemory();

	clients_queue = (queue*)shared_memory_ptr;

	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIGRTMIN);
	sigdelset(&mask, SIGINT);
	sigdelset(&mask, SIGTSTP);

	sigset_t mask1;
	sigemptyset(&mask1);
	sigaddset(&mask1, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &mask1, NULL);

	for(int i = 0; i < clients_numebr; i++)
	{
		pid_t pid = fork();
		if(pid < 0)
		{
			printf("Error: cannot create new process.\n");
			exit(1);
		} else if(pid == 0)
		{	
			while(visited < visits)
			{
				if(goToBarber())
				{
					sigsuspend(&mask);
					printf("Client %d has new haircut and leaves barber shop.\n", getpid());
				}
			}
			exit(0);
		}
	}
	
	while(1)
	{

	}

	finishVisit();
	return 0;
}