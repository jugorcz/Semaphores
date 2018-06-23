
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define QUEUE_SIZE 20

typedef struct queue
{
	bool barber_is_sleeping;
	int next_free_chair;
	int the_longest_waiting_client;
	pid_t chairs[QUEUE_SIZE];
	int barber_current_client;
} queue;

sem_t* createSemaphore(char* name, int value);
sem_t* openSemaphore(char* name);
void closeSemaphore(sem_t* semaphore);

void initializeQueue(queue* queue);
void displayQueue(queue* queue);
bool takeASeat(pid_t pid, queue* queue);
pid_t leaveASeat(queue* queue);

int tryDecreaseSemaphore(sem_t* semaphore);
int increaseSemaphore(sem_t* semaphore);
int decreaseSemaphore(sem_t* semaphore);