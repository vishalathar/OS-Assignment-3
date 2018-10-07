#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int global_sum = 0;
int main(int argc, char *argv[]) {
	int level = 0;
	int fd[2];
	int sum = 0;
	
	// Just a dummy Array
	int array[1000];
	// Initializing Array
	for (int i = 0; i < 1000; ++i)
	{
		array[i] = i;
	}

	for (int j = 0; j < 1000; ++j)
	{	
		if(j%100 == 0)
		{
			// create pipe descriptors
			pipe(fd);
			level++;
			sum = 0;
			
			// fork() returns 0 for child process, child-pid for parent process.
			if (fork() == 0) {
				// Child
				close(fd[0]);
				for (int i = j; i < j+100; ++i)
				{
					sum += array[i];
				}	
					
				write(fd[1], &sum, sizeof(sum));
				
				printf("Chile send %d Sum at %d Level\n", sum, level);
				
				// close the write descriptor
				close(fd[1]);
				kill(getpid(), SIGKILL);
			} 
			else 
			{
				// Parent: reading only, so close the write-descriptor
				close(fd[1]);
				wait(NULL);

				// now read the data (will block until it succeeds)
				read(fd[0], &sum, sizeof(sum));
				global_sum = global_sum + sum;
				
				printf("Parent received %d Sum at %d Level\n\n", sum, level);

				// close the read-descriptor
				close(fd[0]);
			}
		}
	}
	printf("\nGlobal Sum = %d\n", global_sum);

	return 0;
}