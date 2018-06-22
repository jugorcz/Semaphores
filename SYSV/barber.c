#include "common.h"

int semaphore_list_id;
int shared_memory_id;
void* shared_memory_ptr;
queue* clients_queue;

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

void createSemaphoreList()
{
	key_t key = createKey();
	semaphore_list_id = semget(key, SEMAPHORS_QUANTITY, IPC_CREAT | 0666);

	if(semaphore_list_id == -1)
	{
		printf("Error: cannot create semaphore list.\n");
		exit(1);
	}
}

bool inviteClient()
{
	if(decreaseSemaphore(CLIENT_SEMAPHORE,semaphore_list_id, IPC_NOWAIT) == -1)
		return false;

	clients_queue -> barber_current_client = leaveASeat(clients_queue);
	if(clients_queue -> barber_current_client == -1)
		return false;

	printf("Barber invites client %d.\n",clients_queue -> barber_current_client);
	return true;
}

void deleteSemaphoreList()
{
	semctl(semaphore_list_id, 0, IPC_RMID);
}

void deleteSharedMemory()
{
	shmdt(shared_memory_ptr);
	shmctl(shared_memory_id, IPC_RMID, NULL);
}


void createSharedMemory()
{
	key_t key = createKey();
	shared_memory_id = shmget(key, sizeof(queue), IPC_CREAT | 0666);
	if(shared_memory_id == -1)
	{
		printf("Error: cannot create shared memory segmant\n");
		deleteSemaphoreList();
		exit(1);
	}
	shared_memory_ptr = shmat(shared_memory_id, NULL, 0);
	if(shared_memory_ptr == (void*)-1)
	{
		printf("Error: cannot add shared memory segment to program memory.\n");
		deleteSharedMemory();
		deleteSemaphoreList();
		exit(1);
	}
}

void handleSignal(int sig)
{
	printf("\nBarber ends his work.\n");
	deleteSemaphoreList();
	deleteSharedMemory();
	exit(0);
}

int main(int argc, char** argv)
{
	signal(SIGINT, handleSignal);
	signal(SIGTSTP, handleSignal);

	int chairs = getChairsNumber(argc, argv);
	
	createSemaphoreList();
	initializeSemaphor(CHAIR_SEMAPHORE, chairs,semaphore_list_id);
	initializeSemaphor(BARBER_SEMAPHORE, GO_TO_SLEEP, semaphore_list_id);
	createSharedMemory();

	clients_queue = (queue*)shared_memory_ptr;
	initializeQueue(clients_queue);

	while(1)
	{
		if(clients_queue -> barber_is_sleeping == false)
		{
			if(decreaseSemaphore(BARBER_SEMAPHORE, semaphore_list_id, IPC_NOWAIT) == -1)
				continue;

			printf("Barber wakes up.\n");
			inviteClient();
			pid_t client_pid = clients_queue -> barber_current_client;
			printf("Barber cuts %d.\n", client_pid);
			sleep(5);
			printf("Barber finisches with %d.\n",client_pid);
			kill(client_pid, SIGRTMIN);

			while(1)
			{
				if(inviteClient())
				{
					client_pid = clients_queue -> barber_current_client;
					printf("Barber cuts %d.\n", client_pid);
					sleep(5);
					printf("Barber finisches with %d.\n",client_pid);
					kill(client_pid, SIGRTMIN);
					
				} else
				{
					printf("Barber go to sleep.\n");
					clients_queue -> barber_is_sleeping = true;
					increaseSemaphore(BARBER_SEMAPHORE, semaphore_list_id, 1);
					break;

				}
			}
		}
	}


	deleteSemaphoreList();
	return 0;
}