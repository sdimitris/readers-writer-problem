#include<stdio.h>
#include"header.h"

int main(int argc, char* argv[]){
	if( argc  > 0 && argc < 5 ){
		printf("Wrong arguments\n");
		return 0;
	}
	if(!atoi(argv[4]) || !atoi(argv[1]) || !atoi(argv[2])){
		printf("Wrong arguments\n");
		return 0;
	}

	int peers = atoi(argv[1]); // keeps the number of processes
	int entries = atoi(argv[2]); // keeps the number of entries
	float ratio = atof(argv[3]);   // readers/writers
	int iterations = atoi(argv[4]); //iterations

	if(!peers ||  !entries || !iterations){
		printf("Wrong arguments\n");
		return -1;

	}

	int operation;
	printf("Peers = %d, Entries = %d, ratio = %f , iterations %d\n",peers,entries,ratio,iterations);

	

    int shmid = ShmCreate(IPC_PRIVATE,entries);
	if(shmid < 0 ){
		perror("Shared Memory error\n");
		exit(1);
	}

	Entry* shared = ShmAttach(shmid);
	if(shared == (Entry*)(-1)){
		perror("Attach Error\n");
		exit(1);
	}
	shared = SemInit(shared,entries);

	pid_t pid;  // creating childs
	for( int i = 0; i < peers; i++){
		pid = fork();

		if(pid < 0){
			perror("Fork failed\n");
			exit(1);
		}

		if(pid == 0){

			int writers = iterations/(ratio + 1);
			int readers =  iterations - writers;
			srand(time(NULL) * getpid()); // initialize rand

		 	// counters gia ta kolpa
		 	int counter_writer = 0;
		 	int counter_reader = 0;
			int read = 0; // process has been red "read" times
			int write = 0; // process wrote  "write" times
			double waiting = 0.0;
			double time_taken = 0.0;
			struct timeval start, end;
			for(int i = 0; i < iterations; i++){
				int entry = Uniform(0,entries -1);

				operation = rand() % 2; //generate randomly 

				if(operation && counter_writer < writers || !operation && counter_reader == readers){  // then write

					write++;
					counter_writer++;
					gettimeofday(&start, NULL); // start clock


					sem_wait(&(shared[entry].wrt));

					gettimeofday(&end, NULL);   // stop clock

				    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    			    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6; 
    			    waiting += time_taken;  // waiting time 

					shared[entry].writed++;
					sleep(Exponential(0.5));

					sem_post(&(shared[entry].wrt));

				}
				else{
					counter_reader++;
					read++;
					
					gettimeofday(&start, NULL); // start clock,reader waits

					sem_wait(&shared[entry].rd);

					gettimeofday(&end, NULL); // stop clock 

					time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    			    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6; 
    			    waiting += time_taken;  // waiting time 



					shared[entry].current_readers++;
					if( shared[entry].current_readers == 1)
						sem_wait(&shared[entry].wrt);
					

					sem_post(&shared[entry].rd);

					gettimeofday(&end, NULL); // stop clock  

					shared[entry].readed++;  // read operation

					sleep(Exponential(0.5)); //sleeping
	

					gettimeofday(&start, NULL); // start clock,reader waits

					sem_wait(&shared[entry].rd);

					gettimeofday(&end, NULL); // stop clock 

					time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    			    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6; 
    			    waiting += time_taken;  // waiting time 


					shared[entry].current_readers--;

					if(shared[entry].current_readers == 0) //reader leaves
					   sem_post(&shared[entry].wrt);      

					sem_post(&shared[entry].rd);        

				}	
			} // end iterations.

			printf("PID %d   writes %d reads %d  Avg waiting =  %0.4f sec\n",getpid(),write,read,waiting/iterations);
			exit(0); // after n iterations exit
		}	
	}

	int status = 0;
	while(wait(&status) > 0);  // waiting

	printf("\n\n");
	for( int i = 0; i < entries; i++)
		printf("Entry %d has red %d times and has been written %d times\n",i + 1, shared[i].readed,shared[i].writed);

	SemDestroy(shared,entries); //destroying semaphores

	if (ShmDetach(shared) == -1){  //detatching
    	perror("Detatch Error\n");
    	return 1;
    }

    if (ShmDelete(shmid) == -1) { //deleting shared memory
       perror("Delete Error\n");
       return 1;
   }
	


return 0;
}