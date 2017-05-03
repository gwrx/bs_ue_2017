#define _POSIX_SOURCE
#include <sys/types.h> // used for pid_t
#include <unistd.h>    // used for fork() & getpid()
#include <wait.h>      // used for wait()
#include <stdlib.h>
#include <stdio.h>

int main() {
	
	pid_t pid = fork();


	if (pid == 0) {
		printf("iama child with pid %d\n", pid);
		return EXIT_SUCCESS;
	} else if (pid > 0) {
		wait(NULL);
	} else {
		printf("spawning a new child failed\n");
	}

	return EXIT_SUCCESS;
}
