#  Coordinator

## Utility of program
 
  Processes are write and read to/from  shared memory concurrently. So, we need to synchronize  the processes.
  This is a reader/writer problem based on Inter Process Communication ( IPC). At the end of the execution the program prints how many times an entry has been written/red,how many times a peer has writen/red, and the average time that peer waited to entry  in the critical section.


## Coordinate processes
  Coordinator's job , is to  synchronize the processes.
  Coordinator, (in this case the father process) coordinates a bunch of processes( child processes). The number of child processes is
  an argument given by user. Coordinator create the processes  using  fork(). 
  So, assume that we have N peers( that means n child processes) and the ratio of readers/writers = ratio( ratio is also an argument).
  Each peer is enabled "iteration" times, and the operation that  process makes each time is read or write(random operation).
  So,if iteration is 6 and ratio = 1,  3 reads and 3 writes are occured.

##  Entries

  Entry is a struct that contains 2 counters and 2 semaphores. Writed counter stands for  times that entry has been writen and readed     stands for 
  times that entry has been red.
  Each child process can read or write in an Entry,one time at each iteration.
  The number of entries is  an argument also given by user.
  Each entry is guarded by two semaphores(POSIX IMPLEMENTATION).


##  Shared memory
  Shared memory is  a set of Entries that allows peers to communicate. 
  
##  COMPILING AND EXECUTE   
   This is the instruction for copmilation  
   **make coordinator**   
   This is the instruction for execution    
   **./build/coordinator  #peers #entries #ratio #iterations**
   









