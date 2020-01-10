#include <unistd.h>
#include <stdio.h>

int main(){

int pid;

pid=fork();

if (pid==0) {
/*	here is where the parent (shell) can instruct the child to execute the program needed by the user
	How?
	using an exec() system call;
	*/	
	printf("Inside child, value of pid variable =%d\n",pid);
	printf("Inside child; my process id is %d\n",getpid());
	printf("Child: my parent's process id = %d\n",getppid());
	sleep(5);	
	}
else {
	printf("From Parent; value of variable pid = %d\n",pid);
	printf("From Parent; my process id =%d\n",getpid());
	printf("Parent: my parent's process id = %d\n",getppid());
	sleep(5);
	}
}
