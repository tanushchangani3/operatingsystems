#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/shm.h>		// This is necessary for using shared memory constructs
#include <semaphore.h>		// This is necessary for using semaphore
#include <fcntl.h>			// This is necessary for using semaphore
#include <pthread.h>        // This is necessary for Pthread          
#include <string.h>

#define VAR_ACCESS_SEMAPHORE "/var_access_semaphore"

long int global_var = 0;

/**
* This function should be implemented by yourself. It must be invoked
* in the child process after the input parameter has been obtained.
* @parms: The input parameter from the terminal.
*/
void multi_threads_run(long int input_param);

int main(int argc, char **argv)
{
	int shmid, status;
	long int local_var = 0;
	long int *shared_var_p, *shared_var_c;

	if (argc < 2) {
		printf("Please enter an nine-digit decimal number as the input parameter.\nUsage: ./main <input_param>\n");
		exit(-1);
	}

   	/*
		Creating semaphores. Mutex semaphore is used to acheive mutual
		exclusion while processes access (and read or modify) the global
		variable, local variable, and the shared memory.
	*/ 

	// Checks if the semaphore exists, if it exists we unlink him from the process.
	sem_unlink(VAR_ACCESS_SEMAPHORE);
	
	// Create the semaphore. sem_init() also creates a semaphore. Learn the difference on your own.
	sem_t *var_access_semaphore = sem_open(VAR_ACCESS_SEMAPHORE, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1);

	// Check for error while opening the semaphore
	if (var_access_semaphore != SEM_FAILED){
		printf("Successfully created new semaphore!\n");
	}	
	else if (errno == EEXIST) {   // Semaphore already exists
		printf("Semaphore appears to exist already!\n");
		var_access_semaphore = sem_open(VAR_ACCESS_SEMAPHORE, 0);
	}
	else {  // An other error occured
		assert(var_access_semaphore != SEM_FAILED);
		exit(-1);
	}

	/*  
	    Creating shared memory. 
        The operating system keeps track of the set of shared memory
	    segments. In order to acquire shared memory, we must first
	    request the shared memory from the OS using the shmget()
      	system call. The second parameter specifies the number of
	    bytes of memory requested. shmget() returns a shared memory
	    identifier (SHMID) which is an integer. Refer to the online
	    man pages for details on the other two parameters of shmget()
	*/
	shmid = shmget(IPC_PRIVATE, sizeof(long int), 0666|IPC_CREAT); // We request an array of one long integer

	/* 
	    After forking, the parent and child must "attach" the shared
	    memory to its local data segment. This is done by the shmat()
	    system call. shmat() takes the SHMID of the shared memory
	    segment as input parameter and returns the address at which
	    the segment has been attached. Thus shmat() returns a char
	    pointer.
	*/

	if (fork() == 0) { // Child Process
        
		printf("Child Process: Child PID is %jd\n", (intmax_t) getpid());
		
		/*  shmat() returns a long int pointer which is typecast here
		    to long int and the address is stored in the long int pointer shared_var_c. */
        shared_var_c = (long int *) shmat(shmid, 0, 0);

		while (1) // Loop to check if the variables have been updated.
		{
			// Get the semaphore
			sem_wait(var_access_semaphore);
			printf("Child Process: Got the variable access semaphore.\n");

			if ( (global_var != 0) || (local_var != 0) || (shared_var_c[0] != 0) )
			{
				printf("Child Process: Read the global variable with value of %ld.\n", global_var);
				printf("Child Process: Read the local variable with value of %ld.\n", local_var);
				printf("Child Process: Read the shared variable with value of %ld.\n", shared_var_c[0]);

                // Release the semaphore
                sem_post(var_access_semaphore);
                printf("Child Process: Released the variable access semaphore.\n");
                
				break;
			}

			// Release the semaphore
			sem_post(var_access_semaphore);
			printf("Child Process: Released the variable access semaphore.\n");
		}

        /**
         * After you have fixed the issue in Problem 1-Q1, 
         * uncomment the following multi_threads_run function 
         * for Problem 1-Q2. Please note that you should also
         * add an input parameter for invoking this function, 
         * which can be obtained from one of the three variables,
         * i.e., global_var, local_var, shared_var_c[0].
         */
		// multi_threads_run(xxx);

		/* each process should "detach" itself from the 
		   shared memory after it is used */

		shmdt(shared_var_c);

		exit(0);
	}
	else { // Parent Process

		printf("Parent Process: Parent PID is %jd\n", (intmax_t) getpid());

		/*  shmat() returns a long int pointer which is typecast here
		    to long int and the address is stored in the long int pointer shared_var_p.
		    Thus the memory location shared_var_p[0] of the parent
		    is the same as the memory locations shared_var_c[0] of
		    the child, since the memory is shared.
		*/
		shared_var_p = (long int *) shmat(shmid, 0, 0);

		// Get the semaphore first
		sem_wait(var_access_semaphore);
		printf("Parent Process: Got the variable access semaphore.\n");

		global_var = strtol(argv[1], NULL, 10);
		local_var = strtol(argv[1], NULL, 10);
		shared_var_p[0] = strtol(argv[1], NULL, 10);

		// Release the semaphore
		sem_post(var_access_semaphore);
		printf("Parent Process: Released the variable access semaphore.\n");
        
		wait(&status);

		/* each process should "detach" itself from the 
		   shared memory after it is used */

		shmdt(shared_var_p);

		/* Child has exited, so parent process should delete
		   the cretaed shared memory. Unlike attach and detach,
		   which is to be done for each process separately,
		   deleting the shared memory has to be done by only
		   one process after making sure that noone else
		   will be using it 
		 */

		shmctl(shmid, IPC_RMID, 0);

        // Close and delete semaphore. 
        sem_close(var_access_semaphore);
        sem_unlink(VAR_ACCESS_SEMAPHORE);

		exit(0);
	}

	exit(0);
}

/**
* This function should be implemented by yourself. It must be invoked
* in the child process after the input parameter has been obtained.
* @parms: The input parameter from terminal.
*/
void multi_threads_run(long int input_param)
{
	// Add your code here
}