#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void signalHandler(int sig, void(*func)(int))
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = func;

	if(sigaction(sig, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}

void childHandler(int sig)
{
	wait(NULL);
	exit(0);
}

int main(void)
{
	signalHandler(SIGCHLD, childHandler);

	pid_t ret = fork();
	if(ret == -1)
	{
		perror("fork");
		return 1;
	}

	if(ret)
	{
		for(;;)
		{
			pause();
		}
		return 0;
	}
	else
	{
		printf("Child pid: %d\n", getpid());
		return 0;
	}
}
