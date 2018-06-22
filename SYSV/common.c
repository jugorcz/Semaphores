#include "common.h"

union semun {
    	int val;                /* value for SETVAL */
		struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
		short *array;          /* array for GETALL & SETALL */
		struct seminfo *__buf;  /* buffer for IPC_INFO */
		void *__pad;
};

key_t createKey()
{
	char* path = getenv("HOME");
	key_t key = ftok(path, PROJECT_ID);
	return key;
}

void initializeSemaphor(int semaphore_number, int value, int semaphore_list_id)
{
    union semun semopts;
    semopts.val = value; 
    semctl(semaphore_list_id, semaphore_number, SETVAL, semopts);
}


int increaseSemaphore(int semaphore_number, int semaphore_list_id, int flag)
{
	struct sembuf sembuffer;
	sembuffer.sem_num = semaphore_number;
	sembuffer.sem_op = 1;
	sembuffer.sem_flg = flag;
	int result = semop(semaphore_list_id, &sembuffer, 1);
	//displaySemaphoreListContent(semaphore_list_id);
	return result;
}

int decreaseSemaphore(int semaphore_number, int semaphore_list_id, int flag)
{
	struct sembuf sembuffer;
	sembuffer.sem_num = semaphore_number;
	sembuffer.sem_op = -1;
	sembuffer.sem_flg = flag;
	int result = semop(semaphore_list_id, &sembuffer, 1);
	//displaySemaphoreListContent(semaphore_list_id);
	return result;
}

void displaySemaphoreListContent(int semaphore_list_id)
{
	for(int i = 0; i < SEMAPHORS_QUANTITY; i++)
	{
		if(i == BARBER_SEMAPHORE)
			printf("BARBER ");
		else if(i == CLIENT_SEMAPHORE)
			printf("CLIENT ");
		else if(i == CHAIR_SEMAPHORE)
			printf("CHAIR ");

		int value = semctl(semaphore_list_id, i, GETVAL, NULL);
		printf("sem %d: %d\n",i,value);
	}
	printf("\n");
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

bool takeASeat(pid_t pid, queue* queue)
{
	int index = queue -> next_free_chair;
	bool result;
	if(queue -> chairs[index] != -1)
	{
		printf("There is no free chair.\n");
		result = false;
	}
	else
	{
		queue -> chairs[index] = pid;
		index = (index + 1) % QUEUE_SIZE;
		queue -> next_free_chair = index;
		result = true;
	}
	return result;
}

pid_t leaveASeat(queue* queue)
{
	int index = queue -> the_longest_waiting_client;
	pid_t client = queue -> chairs[index];
	if(client == -1)
	{
		printf("There is no waiting client.\n");
	}
	else
	{
		queue -> chairs[index] = -1;
		index = (index + 1) % QUEUE_SIZE;
		queue -> the_longest_waiting_client = index;
	}
	return client;
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