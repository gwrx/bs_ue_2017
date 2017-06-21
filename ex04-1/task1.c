#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>




int main(void) {

	pid_t cpid, w;
	int wstatus;

	int childs = 9;

	for (int i = 0; i < childs; i++) {
		cpid = fork();
		if (cpid == 0) {
			break;
		}
	}

	if (cpid >= 0) {
		printf("fork successful");
	 
		if (cpid == 0) {
			printf("iama child, exiting... - pid %d\n", cpid);
			exit(3);
		} else {
			printf("iama parent, waiting for my childs to finish... - pid %d\n", cpid);
			w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
			if (w == -1) {
				perror("waidpid");
				exit(EXIT_FAILURE);
			}
			if (WIFEXITED(wstatus)) {
				printf("childs exited\n");
			}
			exit(EXIT_SUCCESS);
		}

	} else {
		printf("fork failed, exiting...\n");
		exit(EXIT_FAILURE);
	}


	return EXIT_SUCCESS;
}
