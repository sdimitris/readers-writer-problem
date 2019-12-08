#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<unistd.h>
#include<semaphore.h>
#include<math.h>
#include<sys/time.h>

typedef struct Entry Entry;

struct Entry{
	
	int writed;
	int readed;
	int current_readers;
	sem_t wrt;
	sem_t rd;

};

int ShmCreate(key_t,int);
//create memory

//retuns pointer to memory
Entry* ShmAttach(int);

//detatch
int ShmDetach(Entry*);

//delete shared memory
int ShmDelete(int);

Entry* SemInit(Entry*,int);

int Uniform(int, int);

double Exponential(double);

void SemDestroy(Entry* , int);
