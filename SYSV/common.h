#ifndef COMMON_H
#define COMMON_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>



#define PROJECT_ID 10

#define BARBER_SEMAPHORE 0
#define CLIENT_SEMAPHORE 1
#define CHAIR_SEMAPHORE 2

#define GO_TO_SLEEP 1
#define WAKE_UP 0

#define SEMAPHORS_QUANTITY 3

#define QUEUE_SIZE 20

typedef struct queue
{
	bool barber_is_sleeping;
	int next_free_chair;
	int the_longest_waiting_client;
	pid_t chairs[QUEUE_SIZE];
	int barber_current_client;
} queue;

int semaphore_list_id;

key_t createKey();
void initializeSemaphor(int semaphore_number, int value, int semaphore_list_id);
void displaySemaphoreListContent(int semaphore_list_id);

int increaseSemaphore(int semaphore_number,int semaphore_list_id, int flag);
int decreaseSemaphore(int semaphore_number,int semaphore_list_id, int flag);

void initializeQueue(queue* queue);
void displayQueue(queue* queue);
bool takeASeat(pid_t pid, queue* queue);
pid_t leaveASeat(queue* queue);

#endif