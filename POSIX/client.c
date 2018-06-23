#include "common.h"

char* barber_semaphore_name = "barber";
char* chairs_semaphore_name = "chair";
char* clients_semaphore_name = "client";

sem_t* barber_semaphore;
sem_t* chair_semaphore;
sem_t* client_semaphore;


int main(int argc, char** argv)
{
	barber_semaphore = openSemaphore(barber_semaphore_name);
	chair_semaphore = openSemaphore(chairs_semaphore_name);
	client_semaphore = openSemaphore(clients_semaphore_name);
	return 0;
}