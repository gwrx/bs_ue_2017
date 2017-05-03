#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>    // used for fork() & getpid()
#include <sys/types.h> // used for pid_t

int main() {
	
	int children = 16;
	pid_t pid;
	int procNum;

	for (procNum = 0; procNum < children; procNum++) {
		pid = fork();
		if (pid == 0) {
			break;
		}
	}


	if (pid == 0) {
		printf("%d\n", getpid());
		return EXIT_SUCCESS;
	} else {
		printf("16 childprocesses have been created\n");
		// parenting stuff
	}

	return EXIT_SUCCESS;
}
