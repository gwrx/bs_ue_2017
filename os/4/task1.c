#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void)
{
	for(int i = 0; i < 9; ++i)
	{
		pid_t ret = fork();
		if(ret == -1)
		{
			perror("fork");
			return 1;
		}

		if(ret == 0)
		{
			// child exits immediately
			return 0;
		}
	}

	for(int i = 0; i < 9; ++i)
	{
		wait(NULL);
	}

	return 0;
}
