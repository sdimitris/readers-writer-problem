#include <stdio.h>
#include"header.h"

int ShmCreate(key_t key,int entries){

	
	return shmget(key,sizeof(entries*sizeof(struct Entry)),0644|IPC_CREAT);
}

Entry* ShmAttach(int shmid){

	return  shmat(shmid,NULL,0); //memory attach
}

int ShmDetach( Entry* shared){
	return shmdt(shared);
}

int ShmDelete(int shmid){

	return shmctl(shmid,IPC_RMID,0);
}

Entry* SemInit(Entry* shared,int entries){
	for( int i = 0;  i < entries; i++){
		int w = sem_init(&(shared[i].wrt),1,1);
		int m = sem_init(&(shared[i].rd),1,1);
		if( w == -1 || m == -1){
			perror("Sem init error\n");
			exit(1);
		}
	}
	return shared;
}
void SemDestroy(Entry* shared,int entries){
	for( int i = 0; i < entries; i++){
		if(sem_destroy(&shared[i].wrt) == -1 || sem_destroy(&shared[i].rd) == -1){
			perror("Destroy semaphores error\n");
			exit(1);
		}
	}

}