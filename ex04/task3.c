#define _POSIX_SOURCE
#include <sys/types.h> // used for pid_t
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>    // used for fork() & getpid()
#include <sys/wait.h>
#include <signal.h>

static int received = 0;

void sigHandler(int sig) {
    if (sig == SIGUSR1) {
		received = 1;
	}
	if (sig == SIGUSR2) {
		received = 2;
	}
}

int main() {
	

	pid_t pid = fork();

	if (pid == 0) {

		signal(SIGUSR1, sigHandler);
		signal(SIGUSR2, sigHandler);

		printf("Child running..., waiting for signal\n");

		while (!received) {
			received = pause();
		}

		printf("%d received\n", received);

		switch (received) {
			case 1:
				printf("SIGUSR1 received, Child pausing...\n");
				received = 0;
				break;
			case 2:
				printf("SIGUSR2 received, Child exiting...\n");
				exit(254);
				break;
		}


	} else if (pid > 0) {

		for (int i = 0; i < 3; i++) {
			sleep(5);
			printf("Parent sending SIGUSR1...\n");
			kill(pid,SIGUSR1); /*send SIGUSR1 signal to child*/
		}

		kill(pid,SIGUSR2); /*send SIGUSR2 signal to child*/

	} else {
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
