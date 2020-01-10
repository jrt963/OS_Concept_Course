/***********************************************************************
 Pthreads library example
 
 compile using: gcc -pthread semaphore.c -o semaphore
 
 Each thread will increment a counter UPPER times.  The
 counter is global, i.e., shared, and so the access to it is protected
 by a binary semaphore.
 ***********************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

/* Global constants */

const int UPPER = 1000; /* # times counter is incremented by each thread */

/* Global variables */

int counter = 0;        // variable to be incremented by each thread 
sem_t semaphore;	      // global semaphore, used for mutual exclusion
pthread_t tid[10];   // array of thread identifiers

// ***********************************************************************
// Each thread will execute the function and do, counter += Upper. 
// The semaphore will control access to counter and if the semaphore
// works properly than counter will display the same amount as the number of iterations.
// 6 threads and 1000 iterations per thread is the default.
// If we change the value of the number of threads or iterations we can
// observe the results.
// ***********************************************************************

void *
increment( void *arg )
{
	/* Thread number is the parameter passed.  Since the parameter could be 
	 anything, it is passed as void * and must be "converted". */
	/* following are local variables of the thread; other threads can't acccess these variables */
	
	int i,j,temp;
	// Changed (int) arg; to (long) arg; due to error and stackexchange suggestion.
	int id = (long) arg;
	int spin; /* to slow down threads */

	printf("Thread %d is startng\n",id);

	// Iterates UPPER times, increments the counter per iteration.
	// Must protext counter as is a global (shared) variable.
	// Semaphores are used to control access and thus erroneous results.
	// See more info on semaphore and their use for further explanation.

	for(j = 0; j < UPPER; j++ )
	{
		spin=0;
		for (i=0;i<1000000;i++) spin++;
		sem_wait( &semaphore ); // semaphore WAIT operation
		 counter++;
		sem_post( &semaphore ); // semaphore SIGNAL operation
	}

	printf("Thread %d is ending\n",id); 
	return NULL;
}

/***********************************************************************
 The main program:
    1) Creates n threads, each one identical, and running the
       increment() function.
    3) Wait for each thread to complete execution.  Since each thread
       simply increments the counter UPPER times, the value of the
       counter when each thread terminates should be n*UPPER.
 ***********************************************************************/

int 
main()
{
	int i, no_threads;
	no_threads=6;

	/* Initialize the mutual exclusion semaphore. */

	sem_init( &semaphore, 0, 1 );

	/* Create no_threads identical threads each one running the increment()
	 function, saving the thread-id in the tid array, and using the default 
	 attributes.  Each thread calls the function increment(), with one
	 argument "i", used as the thread number. */

	for( i = 0; i < no_threads; i++ ) 
		pthread_create( &tid[ i ], NULL, increment, & i );

	// Wait for all the threads to exit.  This suspends the calling thread,
	// i.e., the main program, until all target threads complete.

	for( i = 0; i < no_threads; i++ ) 
		pthread_join( tid[ i ], NULL );
  
	// Display final contents of counter.  We know what the value should
	// be if the mutual exclusion semaphores worked properly.

	printf("Final value of counter is %d and this should be %d\n",counter,no_threads*UPPER);

	return 0;
}
