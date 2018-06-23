#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

sem_t* createSemaphore(char* name, int value);
sem_t* openSemaphore(char* name);
void closeSemaphore(sem_t* semaphore);