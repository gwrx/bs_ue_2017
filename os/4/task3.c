#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int alarmCount = 0;

pid_t childPid;

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

void blockSignal(int sig, int how)
{
	sigset_t set;
	if(sigemptyset(&set) == -1)
	{
		perror("sigemptyset");
		exit(1);
	}
	if(sigaddset(&set, sig) == -1)
	{
		perror("sigaddset");
		exit(1);
	}
	if(sigprocmask(how, &set, NULL) == -1)
	{
		perror("sigprocmask");
		exit(1);
	}
}

void alarmHandler(int sig)
{
	if(++alarmCount == 4)
	{
		kill(childPid, SIGUSR2);
	}
	else
	{
		kill(childPid, SIGUSR1);
	}
	alarm(5);
}

void childHandler(int sig)
{
	wait(NULL);
	exit(0);
}

void parent(void)
{
	signalHandler(SIGALRM, alarmHandler);
	signalHandler(SIGCHLD, childHandler);

	alarm(5);

	for(;;)
	{
		pause();
	}
}

void usr2Handler(int sig)
{
	exit(0);
}

void usr1Handler(int sig)
{
	printf("SIGUSR1 received for the %d. time\n", ++alarmCount);
}

sig_atomic_t unblockUsr2 = 0;

void childAlarmHandler(int sig)
{
	unblockUsr2 = 1;
}

void child(void)
{
	signalHandler(SIGUSR1, usr1Handler);
	signalHandler(SIGUSR2, usr2Handler);
	signalHandler(SIGALRM, childAlarmHandler);

	blockSignal(SIGUSR2, SIG_BLOCK);

	alarm(15);

	for(;;)
	{
		pause();
		if(unblockUsr2 == 1)
		{
			blockSignal(SIGUSR2, SIG_UNBLOCK);
			unblockUsr2 = 0;
		}
	}
}

int main(void)
{
	pid_t ret = fork();

	if(ret == -1)
	{
		perror("fork");
		return 1;
	}

	if(ret == 0)
	{
		child();
		return 0;
	}
	else
	{
		childPid = ret;
		parent();
	}
	return 0;
}
