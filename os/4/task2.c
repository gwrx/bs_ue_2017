#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void)
{
	for(int i = 0; i < 16; ++i)
	{
		pid_t ret = fork();
		if(ret == -1)
		{
			perror("fork");
			return 1;
		}

		if(ret == 0)
		{
			printf("Child: %d\n", getpid());
			return 0;
		}
	}

	puts("16 child processes have been created");

	return 0;
}
