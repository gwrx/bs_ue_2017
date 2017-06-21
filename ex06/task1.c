#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

#include <sys/ipc.h> /* System V ipc */
#include <sys/shm.h> /* System V ipc */

#define FIFO "tmp/RESULT_FIFO"
#define SHMSZ 27


int main(void) {

	pid_t pid;
	key_t key = 42;
	int* shm;

	int shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
	if (shmid == -1) {
		printf("fail shmget");
		exit(EXIT_FAILURE);
	}
	shm = shmat(shmid, NULL, 0);

	int counter = 100;

	for (int i; i < counter; i++) {

		pid = fork();
		if (pid < 0) {
			printf("error forking");
		} else if (pid == 0) {
			
			printf("child (%d): %d, shm: %d\n", i + 1, getpid(), *(shm));
			exit(0);
		} else {
			int wstatus = wait(&wstatus);

			char * fifo = FIFO;
			int buffer = 80;
			mkfifo(fifo, 0666);
			int fd = open(fifo, O_WRONLY);
			write(fd, shm, buffer);
			close(fd);

		}

	}

	return EXIT_SUCCESS;
}
