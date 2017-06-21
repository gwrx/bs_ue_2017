#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


void signal_handler(int signum) {
	if (signum == SIGUSR1) {
		printf("SIGUSR1 caused me to message out...\n");
	}
	if (signum == SIGUSR2) {
		exit(EXIT_SUCCESS);
	}
}

int ntimes = 0;

int main(void) {

	pid_t cpid, w;
	int wstatus;
	static struct sigaction usr1act, usr2act;
	void usr1action(int), usr2action(int);

	cpid = fork();

	if (cpid > 0) { //parenting
		

		sleep(1);
		kill(cpid, SIGUSR1);
		sleep(1);
		kill(cpid, SIGUSR1);
		sleep(1);
		kill(cpid, SIGUSR1);
		sleep(1);
		kill(cpid, SIGUSR2);


		w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
		if (w == -1) {
			perror("waidpid");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(wstatus)) {
			printf("child exited\n");
		}
		exit(EXIT_SUCCESS);

	} else { //kidding

		usr1act.sa_handler = usr1action;
		usr2act.sa_handler = usr2action;
		sigaction(SIGUSR1, &usr1act, NULL);
		sigaction(SIGUSR2, &usr2act, NULL);

		printf("iama child\n");

		for (;;) {
			pause();
			if (ntimes > 3) {
				exit(3);
			}
		}
	}



	return EXIT_SUCCESS;
}
void usr1action(int sig) {
    printf("child caught usr1, %d, %d\n", sig, ++ntimes);
}
void usr2action(int sig) {
    printf("child caught use2, %d, %d\n", sig, ++ntimes);
}
