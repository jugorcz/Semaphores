#include "common.h"
#include <time.h>
int clients_numebr;
int visits;
int visited = 0;
int semaphore_list_id;

int shared_memory_id;
void* shared_memory_ptr;
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


void getAccessToSemaphoreList()
{
	key_t key = createKey();
	semaphore_list_id = semget(key, 0, 0);
	if(semaphore_list_id == -1)
	{
		printf("Error: cannot get access to semaphore list.\n");
		exit(1);
	}
}

void getAccessToSharedMemory()
{
	key_t key = createKey();
	shared_memory_id = shmget(key, 0, 0);
	if(shared_memory_id == -1)
	{
		printf("Error: cannot get access to shared memory segmant\n");
		exit(1);
	}
	shared_memory_ptr = shmat(shared_memory_id, NULL, 0);
	if(shared_memory_ptr == (void*)-1)
	{
		printf("Error: cannot add shared memory segment to program memory.\n");
		exit(1);
	}
}


void handleSignal(int sig)
{
	printf("\nClient leaves barber shop.\n");
	shmdt(shared_memory_ptr);
	exit(0);
}

void wakeUpBarber(pid_t pid)
{
	printf("Client %d wakes up barber.\n", pid);
	clients_queue -> barber_is_sleeping = false;
	clients_queue -> barber_current_client = pid;
}

bool findFreeChair(pid_t pid)
{
	 int result = decreaseSemaphore(CHAIR_SEMAPHORE, semaphore_list_id, IPC_NOWAIT);
	 if(result == -1)
	 {
	 	printf("There is no free chair, client %d leaves barber shop.\n", pid);
	 	return false;
	 }
	 printf("Client %d takes a seat.\n",pid);

	 return (takeASeat(pid, clients_queue));

}

void clientIsCut()
{
	increaseSemaphore(CHAIR_SEMAPHORE, semaphore_list_id, 0);
	visited++;
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
		increaseSemaphore(CLIENT_SEMAPHORE, semaphore_list_id, 0);

	return true;
}

long getTime(){
	long pom = 2;	
	//struct timespec t;
	//clock_gettime(CLOCK_MONOTONIC, &t);
	//pom = t.tv_nsec / 1000;
	return pom;
}

int main(int argc, char** argv)
{
	printf("time: %ld\n",getTime() );
	signal(SIGINT, handleSignal);
	signal(SIGTSTP, handleSignal);
	signal(SIGRTMIN, clientIsCut);

	checkArguments(argc, argv);
	getAccessToSemaphoreList();
	initializeSemaphor(CLIENT_SEMAPHORE, 0 , semaphore_list_id);
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

	return 0;
}